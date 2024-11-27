#include "Camera.h"

void Camera::updateCameraVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::move(CameraMovement direction, float velocity)
{
	glm::vec3 flatForward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

	if (direction == FORWARD) position += flatForward * velocity;
	if (direction == BACKWARD) position -= flatForward * velocity;
	if (direction == LEFT) position -= flatRight * velocity;
	if (direction == RIGHT) position += flatRight * velocity;
	// if (direction == UP) position += up * velocity;
	// if (direction == DOWN) position -= up * velocity;
}

void Camera::processJump(float deltaTime)
{
	if (isJumping) {
		position.y += jumpSpeed * deltaTime;
		if (position.y >= normalHeight + maxJumpHeight) {
			isJumping = false;
			isFalling = true;
		}
	}
	else if (isFalling) {
		position.y -= fallSpeed * deltaTime;
		if (position.y <= normalHeight) {
			position.y = normalHeight;
			isFalling = false;
		}
	}
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 up, float cameraYaw, float cameraPitch)
	: front(glm::vec3(0.f, 0.f, -1.f)), movementSpeed(15.0f), mouseSensitivity(0.1f), fov(45.f), normalHeight(cameraPosition.y), maxJumpHeight(3.0f), jumpSpeed(10.0f), fallSpeed(8.0f), isJumping(false), isFalling(false)
{
	position = cameraPosition;
	worldUp = up;
	yaw = cameraYaw;
	pitch = cameraPitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(const Uint8* keyState, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;

	if (keyState[SDL_SCANCODE_W]) move(FORWARD, velocity);
	if (keyState[SDL_SCANCODE_S]) move(BACKWARD, velocity);
	if (keyState[SDL_SCANCODE_A]) move(LEFT, velocity);
	if (keyState[SDL_SCANCODE_D]) move(RIGHT, velocity);
	// if (keyState[SDL_SCANCODE_Q]) move(DOWN, velocity);
	// if (keyState[SDL_SCANCODE_E]) move(UP, velocity);
	if (keyState[SDL_SCANCODE_SPACE] && !isJumping && !isFalling) {
		isJumping = true;
	}
	processJump(deltaTime);
}

void Camera::processMouseInput(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();
}


void Camera::processMouseScroll(float yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f) fov -= yoffset;
	if (fov <= 1.0f) fov = 1.0f;
	if (fov >= 45.0f) fov = 45.0f;
}