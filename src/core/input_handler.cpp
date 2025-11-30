#include "input_handler.h"
#include <iostream>

#include <algorithm>
#include <cstdint>

static int _KeyMap[(int) Input::Count] =
{
	GLFW_KEY_A,
	GLFW_KEY_D,
	GLFW_KEY_W,
	GLFW_KEY_S,
	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_SPACE,
	GLFW_KEY_ESCAPE
};

static InputState _KeyStates[(int)Input::Count];

static MouseState _MouseState;

void init_input()
{
	//glfwSetCursorPosCallback(get_window_ptr(), _cursor_position_callback);
}

void update_input()
{
	static_assert((sizeof(_KeyMap)/sizeof(int)) == (int)Input::Count, "Size of KeyMap does not match with the input count");
	cursor_update_position();
	for (int i = 0; i < (int)Input::Count; i++)
	{
		int newState = glfwGetKey(get_window_ptr(), _KeyMap[i]);

		switch (newState)
		{
			case GLFW_PRESS:
			{
				switch (_KeyStates[i])
				{
					case InputState::Up:
					case InputState::Released:
					{
						_KeyStates[i] = InputState::Pressed;
					} break;
					//
					case InputState::Pressed:
					{
						_KeyStates[i] = InputState::Down;
					} break;
				}
			} break;
			case GLFW_RELEASE:
			{
				switch (_KeyStates[i])
				{
					case InputState::Released:
					{
						_KeyStates[i] = InputState::Up;
					} break;
					//
					case InputState::Pressed:
					case InputState::Down:
					{
						_KeyStates[i] = InputState::Released;
					} break;
				}
			} break;
			default:
				std::cout << "update_input - Value in switch case not expected!" << std::endl;
				break;
		}
	}
	for (int i = 0; i < NUMBER_MOUSE_BUTTONS; i++)
	{
		int newState = glfwGetMouseButton(get_window_ptr(), GLFW_MOUSE_BUTTON_LEFT + i);

		switch (newState)
		{
		case GLFW_PRESS:
		{
			switch (_MouseState.MouseButtonStates[i])
			{
			case InputState::Up:
			case InputState::Released:
			{
				_MouseState.MouseButtonStates[i] = InputState::Pressed;
			} break;
			//
			case InputState::Pressed:
			{
				_MouseState.MouseButtonStates[i] = InputState::Down;
			} break;
			}
		} break;
		case GLFW_RELEASE:
		{
			switch (_MouseState.MouseButtonStates[i])
			{
			case InputState::Released:
			{
				_MouseState.MouseButtonStates[i] = InputState::Up;
			} break;
			//
			case InputState::Pressed:
			case InputState::Down:
			{
				_MouseState.MouseButtonStates[i] = InputState::Released;
			} break;
			}
		} break;
		default:
			std::cout << "update_input - Value in switch case not expected!" << std::endl;
			break;
		}
	}
}


void cursor_update_position()
{
	int width = window_get_width(), height = window_get_height();

	double newXPos = window_get_cursor_x();
	double newYPos = window_get_cursor_y();

	double initialXPos = newXPos, initialYPos = newYPos;

	switch (_MouseState.LockType)
	{
	case CursorLockType::CenterLock:
	{
		newXPos = width / 2.0f;
		newYPos = height / 2.0f;
	} break;
	case CursorLockType::WindowLock:
	{
		newXPos = std::clamp(newXPos, (double)0.0f, (double)width);
		newYPos = std::clamp(newYPos, (double)0.0f, (double)height);
	} break;
	case CursorLockType::Free:
	default:
	{
		// do nothing
	} break;
	}

	glfwSetCursorPos(get_window_ptr(), newXPos, newYPos);

	// Calculate normalized x and y position relative to window center
	double nX = newXPos / (float)width;
	double nY = newYPos / (float)height;
	nX = (nX * 2.0f) - 1.0f;
	nY = (nY * 2.0f) - 1.0f;

	_MouseState.NormalizedCursorPosition.x = nX;
	_MouseState.NormalizedCursorPosition.y = nY;

	// Calculate "desired" normalized x and y position relative to window center
	double nDX = initialXPos / (float)width;
	double nDY = initialYPos / (float)height;
	nDX = (nDX * 2.0f) - 1.0f;
	nDY = (nDY * 2.0f) - 1.0f;

	_MouseState.NormalizedCursorDeltaPosition.x = nDX;
	_MouseState.NormalizedCursorDeltaPosition.y = nDY;
}

void cursor_set_state(CursorLockType rLockType, bool rCursorVisible)
{
	_MouseState.LockType = rLockType;
	_MouseState.CursorVisible = rCursorVisible;
	switch (rLockType)
	{
	case CursorLockType::Free:
		if (_MouseState.CursorVisible)
		{
			glfwSetInputMode(get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		break;
	case CursorLockType::WindowLock:
		if (_MouseState.CursorVisible)
		{
			glfwSetInputMode(get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		}
		else
		{
			glfwSetInputMode(get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		break;
	case CursorLockType::CenterLock:
	{
		// Center cursor on screen to prevent weird behaviour on code that depends
		// on the cursor position relative to the mouse when locking it
		int width = window_get_width(), height = window_get_height();
		glfwSetCursorPos(get_window_ptr(), width / 2, height / 2);

		if (_MouseState.CursorVisible)
		{
			glfwSetInputMode(get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		}
		else
		{
			glfwSetInputMode(get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	} break;
	default:
		break;
	}
}


bool is_input_pressed(Input rInput)
{
	return _KeyStates[(int)rInput] == InputState::Pressed;
}

bool is_input_released(Input rInput)
{
	return _KeyStates[(int)rInput] == InputState::Released;
}

bool is_input_up(Input rInput)
{
	return _KeyStates[(int)rInput] == InputState::Up;
}

bool is_input_down(Input rInput)
{
	return _KeyStates[(int)rInput] == InputState::Down;
}

bool is_mouse_button_pressed(MouseButton rInput)
{
	return _MouseState.MouseButtonStates[(int)rInput] == InputState::Pressed;
}

bool is_mouse_button_released(MouseButton rInput)
{
	return _MouseState.MouseButtonStates[(int)rInput] == InputState::Released;
}

bool is_mouse_button_up(MouseButton rInput)
{
	return _MouseState.MouseButtonStates[(int)rInput] == InputState::Up;
}

bool is_mouse_button_down(MouseButton rInput)
{
	return _MouseState.MouseButtonStates[(int)rInput] == InputState::Down;
}

float get_mouse_dx()
{
	return _MouseState.NormalizedCursorDeltaPosition.x;
}

float get_mouse_dy()
{
	return _MouseState.NormalizedCursorDeltaPosition.y;
}
