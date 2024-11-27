#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT//,
	// UP,
	// DOWN
};

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float fov;

	float normalHeight;
	float maxJumpHeight;
	float jumpSpeed;
	float fallSpeed;
	bool isJumping;
	bool isFalling;

	float lastX, lastY;
	bool firstMouse;

	void updateCameraVectors();
	void move(CameraMovement direction, float velocity);
	void processJump(float deltaTime);

public:
	Camera(glm::vec3 cameraPosition, glm::vec3 up, float yaw, float pitch);

	glm::mat4 getViewMatrix() const;

	void processKeyboardInput(const Uint8* keyState, float deltaTime);
	void processMouseInput(float xpos, float ypos);
	void processMouseScroll(float yoffset);
	float getFov() const { return fov; }
};