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

void render_opengl(int amount)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertex_array);
	glUseProgram(program);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, amount);
}

void update_buffer(int amount, const int *x, const int *y, const int *width, const int *height)
{
	float positions[MAX_DISPLAYS * 4];
	unsigned char colors[MAX_DISPLAYS * 4];
	int i, ic, min_x, max_x;
	float scale;

	/* Calculate the maximum span of the screens, so we can scale the OpenGL objects */
	min_x = INT_MAX;
	max_x = INT_MIN;
	for(i = 0; i < amount; i++){
		if(x[i] < min_x){
			min_x = x[i];
		}
		if(x[i] + width[i] > max_x){
			max_x = x[i] + width[i];
		}
	}

	scale = 2.0f / (max_x - min_x);

	printf("id\tx\ty\tw\th\n");
	for(i = 0; i < amount; i++){
		ic = i * 4;
		positions[ic + 0] = (((float)x[i] + (width[i] >> 1)) * scale) - 1.0f;
		positions[ic + 1] = -(((float)y[i] + (height[i] >> 1)) * scale);
		positions[ic + 2] = ((float)width[i]) * scale * 0.99f;
		positions[ic + 3] = ((float)height[i]) * scale * 0.99f;

		colors[ic + 0] = 255;
		colors[ic + 1] = 255;
		colors[ic + 2] = 255;
		colors[ic + 3] = 255;

		printf("%d\t%d\t%d\t%d\t%d\n", i, x[i], y[i], width[i], height[i]);
	}
	putchar('\n');

	glBindVertexArray(vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_DISPLAYS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, amount * 4 * sizeof(GLfloat), positions);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_DISPLAYS * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, amount * 4 * sizeof(GLubyte), colors);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
}

int main(int argc, char** argv)
{
	int x[MAX_DISPLAYS], y[MAX_DISPLAYS], width[MAX_DISPLAYS], height[MAX_DISPLAYS];
	struct cc_event event;

	cc_set_error_handler(error_handler);

	cc_set_window_size(400, 200);
	cc_new_window(0);
	cc_set_window_title("ccore example: display");

	cc_bind_opengl_context();

	glViewport(0, 0, cc_get_window_width(), cc_get_window_width());

	setup_opengl();

	memset(x, 0, MAX_DISPLAYS * sizeof(int));
	memset(y, 0, MAX_DISPLAYS * sizeof(int));
	memset(width, 0, MAX_DISPLAYS * sizeof(int));
	memset(height, 0, MAX_DISPLAYS * sizeof(int));

	width[0] = 1280;
	height[0] = 1024;

	x[1] = 1280;
	width[1] = 1980;
	height[1] = 1050;

	update_buffer(2, x, y, width, height);

	while(cc_poll_window()){
		event = cc_pop_event();
		if(event.type == CC_EVENT_DRAW){
			render_opengl(2);

			cc_swap_opengl_buffers();
		}else if(event.type == CC_EVENT_RESIZE){
			glViewport(0, 0, cc_get_window_width(), cc_get_window_width());
		}
	}

	cc_destroy_opengl_context();
	cc_destroy_window();

	return EXIT_SUCCESS;
}
