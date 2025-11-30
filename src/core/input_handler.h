#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "window.h"
#include <glm/vec2.hpp>

#define NUMBER_MOUSE_BUTTONS 3

enum class Input{
	MoveLeft,
	MoveRight,
	MoveFront,
	MoveBack,
	MoveDown,
	MoveUp,
	Escape,

	Count,
};

enum class MouseButton
{
	LeftClick,
	RightClick,
	MiddleClick
};

enum class InputState
{
	Up,
	Pressed,
	Down,
	Released
};

enum class CursorLockType
{
	Free,
	WindowLock,
	CenterLock
};

typedef struct
{
	glm::vec2 NormalizedCursorPosition;	// stores the actual current mouse position relative to the center
	glm::vec2 NormalizedCursorDeltaPosition; // stores the position relative to the center that the mouse moved before it got centered. It only makes sense when cursor is center locked.
	CursorLockType LockType;
	bool CursorVisible;

	InputState MouseButtonStates[NUMBER_MOUSE_BUTTONS];

} MouseState;

void init_input();
void update_input();

void cursor_update_position();

void cursor_set_state(CursorLockType rLockType, bool rCursorVisible);

bool is_input_pressed(Input rInput);
bool is_input_released(Input rInput);
bool is_input_up(Input rInput);
bool is_input_down(Input rInput);

bool is_mouse_button_pressed(MouseButton rInput);
bool is_mouse_button_released(MouseButton rInput);
bool is_mouse_button_up(MouseButton rInput);
bool is_mouse_button_down(MouseButton rInput);

float get_mouse_dx();
float get_mouse_dy();

#endif // !INPUTHANDLER_H
