#ifndef UTILS_H
#define UTILS_H

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 GetBillboardModelMatrix(const glm::vec3& center, const glm::vec3& eye) {
	glm::vec3 forward = glm::normalize(center - eye);  // Forward direction
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);                   // Up direction (adjust if needed)
	glm::vec3 right = glm::normalize(glm::cross(up, forward));    // Right vector
	up = glm::cross(forward, right);                              // Recalculate up to ensure orthogonality

	// Construct rotation matrix
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0] = glm::vec4(right, 0.0f);
	rotation[1] = glm::vec4(up, 0.0f);
	rotation[2] = glm::vec4(-forward, 0.0f);  // Negate forward for right-handed system

	// Apply translation to position the billboard correctly
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), eye);

	// Final model matrix
	glm::mat4 model = translation * rotation;

	return model;
}

glm::vec3 GetBillboardAngles(const glm::vec3& center, const glm::vec3& eye) {
    // Calculate forward, right, and up vectors
    glm::vec3 forward = glm::normalize(center - eye);  // Forward direction
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);  // World up direction
    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward)); // Right vector
    glm::vec3 up = glm::cross(forward, right);                     // Recalculate up for orthogonality

    // Construct rotation matrix
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0] = glm::vec4(right, 0.0f);
    rotation[1] = glm::vec4(up, 0.0f);
    rotation[2] = glm::vec4(-forward, 0.0f); // Negate forward for right-handed system

    // Extract Euler angles (yaw, pitch, roll) in radians
    glm::vec3 angles;

    // Pitch (rotation around x-axis)
    float pitch = glm::asin(-rotation[1][2]); // -m[1][2] = -sin(pitch)

    // Yaw (rotation around y-axis)
    float yaw = glm::atan(rotation[0][2], rotation[2][2]); // atan2(m[0][2], m[2][2])

    // Roll (rotation around z-axis)
    float roll = glm::atan(rotation[1][0], rotation[1][1]); // atan2(m[1][0], m[1][1])

    angles.x = pitch;
    angles.y = yaw;
    angles.z = roll;

    return angles;
}

#endif // !UTILS_H
