#include "window.h"
#include <iostream>

static GLFWwindow* _pWindow;

static int _ViewportWidth, _ViewportHeight;

static ViewportChangeFunc _ViewportChangeCallback;

int create_window(int rWidth, int rHeight, const char* rTitle)
{
	_ViewportWidth = rWidth;
	_ViewportHeight = rHeight;
 	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);

	_pWindow = glfwCreateWindow(rWidth, rHeight, rTitle, NULL, NULL);
	if (_pWindow == NULL)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(_pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(_pWindow, framebuffer_size_callback);
}

GLFWwindow* get_window_ptr()
{
	return _pWindow;
}

bool window_should_close()
{
	return glfwWindowShouldClose(_pWindow);
}

void window_set_should_close(bool rVal)
{
	glfwSetWindowShouldClose(_pWindow, rVal);
}

void window_terminate()
{
	glfwTerminate();
}

static void framebuffer_size_callback(GLFWwindow* rpWindow, int rWidth, int rHeight)
{
	_ViewportChangeCallback(rWidth, rHeight);
	_ViewportWidth = rWidth;
	_ViewportHeight = rHeight;
}

double window_get_time_since_start()
{
	return glfwGetTime();
}

int window_get_width()
{
	return _ViewportWidth;
}

int window_get_height()
{
	return _ViewportHeight;
}

double window_get_cursor_x()
{
	double x, y;
	glfwGetCursorPos(_pWindow, &x, &y);
	return x;
}

double window_get_cursor_y()
{
	double x, y;
	glfwGetCursorPos(_pWindow, &x, &y);
	return y;
}

void window_set_viewport_change_callback(ViewportChangeFunc rFunc)
{
	_ViewportChangeCallback = rFunc;
	_ViewportChangeCallback(_ViewportWidth, _ViewportHeight);
}
