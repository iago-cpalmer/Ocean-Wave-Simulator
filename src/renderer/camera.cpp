#include "camera.h"
#include "../core/input_handler.h"
#include <algorithm>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include "../core/time_manager.h"
#include <iostream>

void print_vec3(glm::vec3 vec, const char* vecName)
{
	printf("%s: %f, %f, %f \n", vecName, vec.x, vec.y, vec.z);
}

void camera_update(CameraInfo* rpCamera)
{
	// First person flying camera
	// Rotation
	float deltaX = get_mouse_dx() * rpCamera->SensitivityX; // * get_delta_time();
	float deltaY = get_mouse_dy() * rpCamera->SensitivityY; // * get_delta_time();

	float yaw = rpCamera->Rotation.y + deltaX;
	if (yaw >= 360.0f)
	{
		yaw -= 360.0f;
	}
	else if (yaw <= -360.0f)
	{
		yaw += 360.f;
	}

	float pitch = std::clamp(rpCamera->Rotation.x + deltaY, -89.0f, 89.0f);
	rpCamera->Rotation.y = yaw;
	rpCamera->Rotation.x = pitch;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	glm::vec3 cameraRight = glm::normalize(direction);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraFront = glm::normalize(glm::cross(cameraUp, cameraRight));
	cameraFront.y = -cameraRight.y;
	cameraRight.y = 0.0f;

	// Movement
	glm::vec3 inputVec = glm::vec3(0,0,0);

	// Z Axis
	if (is_input_down(Input::MoveFront))
	{
		inputVec += cameraFront;
	}
	if (is_input_down(Input::MoveBack))
	{
		inputVec -= cameraFront;
	}

	// Y Axis
	if (is_input_down(Input::MoveUp))
	{
		inputVec += cameraUp;
	}

	if (is_input_down(Input::MoveDown))
	{
		inputVec -= cameraUp;
	}

	// X Axis
	if (is_input_down(Input::MoveRight))
	{
		inputVec += cameraRight;
	}

	if (is_input_down(Input::MoveLeft))
	{
		inputVec -= cameraRight;
	}

	if (inputVec != glm::vec3(0, 0, 0))
	{
		inputVec = glm::normalize(inputVec);
	}

	rpCamera->Position += inputVec * rpCamera->MovementSpeed * (float)get_delta_time();
}

void camera_add_render_target(CameraInfo* rpCamera, FrameBuffer& fb)
{
	rpCamera->FrameBuffer = &fb;
	rpCamera->RenderTarget = RenderTargetType::Texture;
}
