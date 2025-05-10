#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Position(position), WorldUp(up), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(yaw), Pitch(pitch),
	MovementSpeed(SPEED), MovementSensitivity(SENSITIVITY), Zoom(ZOOM) {
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	Position(glm::vec3(posX, posY, posZ)), WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch),
	MovementSpeed(SPEED), MovementSensitivity(SENSITIVITY), Zoom(ZOOM) {
	updateCameraVectors();
}

glm::mat4 Camera::GetLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
	glm::vec3 zaxis = glm::normalize(target - position);
	glm::vec3 xaxis = glm::normalize(glm::cross(zaxis, glm::normalize(worldUp)));
	glm::vec3 yaxis = glm::cross(xaxis, zaxis);

	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix[0][0] = xaxis.x;
	rotationMatrix[1][0] = xaxis.y;
	rotationMatrix[2][0] = xaxis.z;
	rotationMatrix[0][1] = yaxis.x;
	rotationMatrix[1][1] = yaxis.y;
	rotationMatrix[2][1] = yaxis.z;
	rotationMatrix[0][2] = - zaxis.x;
	rotationMatrix[1][2] = - zaxis.y;
	rotationMatrix[2][2] = - zaxis.z;

	glm::mat4 translationMatrix = glm::mat4(1.0f);
	translationMatrix[3][0] = -position.x;
	translationMatrix[3][1] = -position.y;
	translationMatrix[3][2] = -position.z;

	return rotationMatrix * translationMatrix;
}

glm::mat4 Camera::GetViewMatrix() {
	//return glm::lookAt(Position, Position + Front, Up);
	return GetLookAtMatrix(Position, Position + Front, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		Position += Front * velocity;
	}
	else if (direction == BACKWARD) {
		Position -= Front * velocity;
	}
	else if (direction == LEFT) {
		Position -= Right * velocity;
	}
	else if (direction == RIGHT) {
		Position += Right * velocity;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
	xOffset *= MovementSensitivity;
	yOffset *= MovementSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (constrainPitch) {
		if (Pitch > 89.0f) {
			Pitch = 89.0f;
		}
		else if (Pitch < -89.0f) {
			Pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
	Zoom -= (float)yOffset;
	if (Zoom < 1.0f) {
		Zoom = 1.0f;
	}
	else if (Zoom > 45.0f) {
		Zoom = 45.0f;
	}
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	// calculate the right and up vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

