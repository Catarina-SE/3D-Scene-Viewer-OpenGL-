#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"

int main(int argc, char** argv)
{
	float screenWidth = 800;
	float screenHeight = 600;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_CaptureMouse(SDL_TRUE);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

	glm::mat4 model(1.0f);
	glm::mat4 view;
	glm::mat4 projection;

	Shader shaderProgram("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
	Camera camera(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	//Model* Building = new Model("resources/Building/building.obj");
	Model* Wall = new Model("resources/wall/wall.obj");
	//Model* WoodenPier = new Model("resources/WoodenPier/woodenpier.obj");
	//Model* Tree = new Model("resources/Tree/Tree.obj");
	//Model* myModel2 = new Model("resources/2.obj");

	std::vector<glm::vec3> wallPositions = {
	glm::vec3(-5.0f, 0.0f, 0.0f),
	glm::vec3(-5.0f, 0.0f, -10.0f),
	glm::vec3(-5.0f, 0.0f, -20.0f),
	glm::vec3(5.0f, 0.0f, 0.0f),
	glm::vec3(5.0f, 0.0f, -10.0f),
	glm::vec3(5.0f, 0.0f, -20.0f)
	};

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	SDL_Event event;
	bool isRunning = true;
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	while (isRunning) // Render Loop
	{
		int now = SDL_GetTicks();
		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;

		while (SDL_PollEvent(&event) != 0) // Event Loop
		{
			if (event.type == SDL_QUIT) isRunning = false;
			if (event.type == SDL_MOUSEMOTION)
			{
				camera.processMouseInput(event.motion.xrel, -event.motion.yrel);
			}

			if (event.type == SDL_MOUSEWHEEL)
			{
				camera.processMouseScroll(event.wheel.y);
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_bool captured = SDL_GetRelativeMouseMode();
					SDL_SetRelativeMouseMode(captured ? SDL_FALSE : SDL_TRUE);
				}
			}
		}
		
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		camera.processKeyboardInput(keyState, deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -15.0f));
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		for (const auto& position : wallPositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Wall->draw(shaderProgram, model);
		}


		//Building->draw(shaderProgram);
		//WoodenPier->draw(shaderProgram);
		//Tree->draw(shaderProgram);
		//myModel2->draw(shaderProgram);
		SDL_GL_SwapWindow(window);
	}

	//delete Building;
	delete Wall;
	//delete WoodenPier;
	//delete Tree;
	//delete myModel2;

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
