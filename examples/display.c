#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef USE_EPOXY
#include <epoxy/gl.h>
#elif defined USE_GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#define MAX_DISPLAYS 64

const char *vertex_shader_source =
"#version 330\n"
"layout (location = 0) in vec4 position;\n"
"layout (location = 1) in vec4 color;\n"
"out vec4 fcolor;\n"
"\n"
"const vec2 pos[] = vec2[4](\n"
"	vec2(-0.5f, -0.5f),\n"
"	vec2( 0.5f, -0.5f),\n"
"	vec2(-0.5f,  0.5f),\n"
"	vec2( 0.5f,  0.5f)\n"
");\n"
"\n"
"void main(){\n"
"	vec2 offset = pos[gl_VertexID];\n"
"	gl_Position = vec4(position.xy + (offset * position.zw), 0.0, 1.0);\n"
"	fcolor = color;\n"
"}";

const char *fragment_shader_source =
"#version 330\n"
"in vec4 fcolor;\n"
"out vec4 color;\n"
"void main(){\n"
"	color = fcolor;\n"
"}";

GLuint vertex_array, position_buffer, color_buffer, vertex_shader, fragment_shader, program;
int display_amount, display_x[MAX_DISPLAYS], display_y[MAX_DISPLAYS], display_width[MAX_DISPLAYS], display_height[MAX_DISPLAYS];
float display_pos_data[MAX_DISPLAYS * 4];
unsigned char display_color_data[MAX_DISPLAYS * 4];

void error_handler(const char *message)
{
	fprintf(stderr, "Error: \"%s\"\n", cc_get_error());
	exit(EXIT_FAILURE);
}

void compile_and_check_shader(GLuint shader, const char *shader_source)
{
	int shader_status, shader_log_length;
	char *shader_log;

	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
  if(!shader_status) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shader_log_length);
		shader_log = (char*)malloc(shader_log_length + 1);
		glGetShaderInfoLog(shader, shader_log_length, NULL, shader_log);

		fprintf(stderr, "Compilation error in OpenGL shader:\n%s\nSource:\n%s\n", shader_log, shader_source);

		exit(EXIT_FAILURE);
	}
}

void setup_opengl(void)
{
#ifdef USE_GLEW
	glewInit();
#endif

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	compile_and_check_shader(vertex_shader, vertex_shader_source);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	compile_and_check_shader(fragment_shader, fragment_shader_source);

	program = glCreateProgram();
	glAttachShader(program, fragment_shader);
	glAttachShader(program, vertex_shader);
	glLinkProgram(program);

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_DISPLAYS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_DISPLAYS * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void render_opengl()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertex_array);
	glUseProgram(program);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, display_amount);
}

void update_buffers(void)
{	glBindVertexArray(vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_DISPLAYS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, display_amount * 4 * sizeof(GLfloat), display_pos_data);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_DISPLAYS * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, display_amount * 4 * sizeof(GLubyte), display_color_data);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
}

void update_display_geom()
{
	int i, ic, min_x, max_x, min_y, max_y;
	float scale_x, scale_y, pos_x, pos_y, pos_width, pos_height;

	/* Calculate the maximum span of the screens, so we can scale the OpenGL objects */
	min_x = min_y = INT_MAX;
	max_x = max_y = INT_MIN;
	for(i = 0; i < display_amount; i++){
		if(display_x[i] < min_x){
			min_x = display_x[i];
		}
		if(display_x[i] + display_width[i] > max_x){
			max_x = display_x[i] + display_width[i];
		}
		if(display_y[i] < min_y){
			min_y = display_y[i];
		}
		if(display_y[i] + display_height[i] > max_y){
			max_y = display_y[i] + display_height[i];
		}
	}

	cc_set_window_size((max_x - min_x) / 5, (max_y - min_y) / 5);

	scale_x = 1.0f / (max_x - min_x);
	scale_y = 1.0f / (max_y - min_y);

	printf("id\tx\ty\tw\th\n");
	for(i = 0; i < display_amount; i++){
		ic = i * 4;
		pos_x = display_x[i] + (display_width[i] >> 1);
		pos_y = display_y[i] + (display_height[i] >> 1);
		pos_width = display_width[i];
		pos_height = display_height[i];

		display_pos_data[ic + 0] = (pos_x * scale_x) * 2.0f - 1.0f;
		display_pos_data[ic + 1] = 1.0f - (pos_y * scale_y) * 2.0f;
		display_pos_data[ic + 2] = (pos_width * scale_x) * 1.98f;
		display_pos_data[ic + 3] = (pos_height * scale_y) * 1.98f;

		display_color_data[ic + 0] = 255;
		display_color_data[ic + 1] = 255;
		display_color_data[ic + 2] = 255;
		display_color_data[ic + 3] = 255;

		printf("%d\t%d\t%d\t%d\t%d\n", i, display_x[i], display_y[i], display_width[i], display_height[i]);
	}
	putchar('\n');
}

void update_display_data()
{
	int i;
	struct cc_display_info display_info;
	struct cc_resolution_info resolution_info;

	memset(display_x, 0, MAX_DISPLAYS * sizeof(int));
	memset(display_y, 0, MAX_DISPLAYS * sizeof(int));
	memset(display_width, 0, MAX_DISPLAYS * sizeof(int));
	memset(display_height, 0, MAX_DISPLAYS * sizeof(int));

	display_amount = cc_get_display_count();
	for(i = 0; i < display_amount; i++){
		cc_get_display_info(i, &display_info);
		display_x[i] = display_info.x;
		display_y[i] = display_info.y;

		cc_get_resolution_info(i, cc_get_default_resolution_id(i), &resolution_info);
		display_width[i] = resolution_info.width;
		display_height[i] = resolution_info.height;
	}

	update_display_geom();
}

int main(int argc, char** argv)
{
	struct cc_event event;

	cc_set_error_handler(error_handler);

	cc_initialize_display();

	cc_set_window_size(400, 200);
	cc_new_window(0);
	cc_set_window_title("ccore example: display");

	cc_bind_opengl_context();

	setup_opengl();

	update_display_data();
	update_buffers();

	while(cc_poll_window()){
		event = cc_pop_event();
		if(event.type == CC_EVENT_DRAW){
			render_opengl();

			cc_swap_opengl_buffers();
		}else if(event.type == CC_EVENT_RESIZE){
			glViewport(0, 0, cc_get_window_width(), cc_get_window_height());
		}else if(event.type == CC_EVENT_DISPLAY_CHANGE){
			update_display_data();
			update_buffers();
		}
	}

	cc_destroy_opengl_context();
	cc_destroy_window();
	cc_destroy_display();

	return EXIT_SUCCESS;
}
