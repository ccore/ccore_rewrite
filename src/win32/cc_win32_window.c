#include "cc_win32_window_c.h"
#include <cc_window.h>

#include <cc_error.h>
#include <cc_event.h>
#include <cc_display.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <windows.h>
#include <hidsdi.h>

#define CC_WINDOW_CLASS_NAME "cc_window"

static HWND _window;
static ATOM _window_class;
static int _x, _y, _width, _height, _mouse_x, _mouse_y, _need_redraw;

/* Private functions */

static LRESULT CALLBACK cc_window_process(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	LONG_PTR style;
	struct cc_event event;

	switch(message){
		case WM_CLOSE:
			cc_clear_event_queue();

			event.type = CC_EVENT_QUIT;
			cc_push_event(event);
			break;
		case WM_SYSCOMMAND:
			style = GetWindowLongPtr(_window, GWL_STYLE);
			if((w_param & 0xFFF0) == SC_MOVE && (style & WS_MAXIMIZE) && !(style & WS_MAXIMIZEBOX)){
				break;
			}
		default:
			return DefWindowProc(window_handle, message, w_param, l_param);
			break;
	}

	return FALSE;
}

static int cc_register_instance(HINSTANCE instance_handle)
{
	WNDCLASSEX window_class;

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_OWNDC;
	window_class.lpfnWndProc = cc_window_process;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance_handle;
	window_class.hIcon = NULL;
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = NULL;
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = CC_WINDOW_CLASS_NAME;
	window_class.hIconSm = NULL;

	if(!(_window_class = RegisterClassEx(&window_class))) {
		cc_set_error("Could not register window class");
		return 0;
	}

	return 1;
}

/* Local functions */

int cc_poll_window(void)
{	
	MSG message;

	if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 1;
}

/* Public functions */

int cc_new_window(enum cc_window_flag flags)
{
	HMODULE module_handle;

	module_handle = GetModuleHandle(NULL);
	if(!module_handle){
		cc_set_error("Could not get a module handle");
		return 0;
	}

	if(!cc_register_instance(module_handle)){
		return 0;
	}

	if(_width == 0){
		_width = 100;
	}
	if(_height == 0){
		_height = 100;
	}

	_x = 0;
	_y = 0;

	_window = CreateWindowEx(WS_EX_APPWINDOW, CC_WINDOW_CLASS_NAME, "ccore window", WS_OVERLAPPEDWINDOW, _x, _y, _width, _height, NULL, NULL, module_handle, NULL);

	if(ShowWindow(_window, SW_SHOW)){
		cc_set_error("Could not create and show a Win32 window");
		return 0;
	}

	return 1;
}

int cc_destroy_window(void)
{	
	return 1;
}

int cc_blink_window(void)
{
	return 1;
}

int cc_capture_mouse(void)
{	return 1;
}

int cc_release_mouse(void)
{
	return 1;
}

/* Setters */
int cc_set_window_windowed(void)
{
	return 1;
}

int cc_set_window_maximized(void)
{
	return 1;
}

int cc_set_window_centered(void)
{	
	return 1;
}

int cc_set_window_fullscreen_on_screen(int display_id)
{	
	return 1;
}

int cc_set_window_fullscreen_on_multiple_screens(int left_display_id, int right_display_id, int top_display_id, int bottom_display_id)
{	
	return 1;
}

int cc_set_window_title(const char *title)
{
	return 1;
}

int cc_set_window_icon(int width, int height, const uint32_t *data)
{	
	return 1;
}

int cc_set_window_position(int x, int y)
{	
	return 1;
}

int cc_set_window_size(int width, int height)
{	
	return 1;
}

int cc_set_mouse_position(int x, int y)
{	
	return 1;
}

int cc_set_mouse_cursor(enum cc_cursor cursor)
{	
	return 1;
}

/* Getters */

int cc_get_window_coordinates(int *x, int *y)
{	
	return 1;
}

int cc_get_window_size(int *width, int *height)
{	
	return 1;
}

int cc_get_mouse_coordinates(int *x, int *y)
{	
	return 1;
}
