#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW//glfw3.h>

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 3
#define OPENGL_PROFILE GLFW_OPENGL_CORE_PROFILE

// vvv Debug messages -----------------
#define DEBUG_HIGH_SEVERITY
#define DEBUG_MEDIUM_SEVERITY
#define DEBUG_LOW_SEVERITY
#define DEBUG_NOTIFICATION_SEVERITY
// ^^^ --------------------------------

int create_window(int rWidth, int rHeight, const char* rTitle);

GLFWwindow* get_window_ptr();

bool window_should_close();
void window_set_should_close(bool rVal);

void window_terminate();

static void framebuffer_size_callback(GLFWwindow* rpWindow, int rWidth, int rHeight);

double window_get_time_since_start();

int window_get_width();
int window_get_height();

double window_get_cursor_x();
double window_get_cursor_y();

// Callbacks
typedef void (*ViewportChangeFunc)(int rWidth, int rHeight);
void window_set_viewport_change_callback(ViewportChangeFunc rFunc);

#endif // !WINDOW_H

