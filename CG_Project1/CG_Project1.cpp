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

	Model* WoodenPier = new Model("resources/WoodenPier/woodenpier.obj");
	glm::mat4 pierModel;

	Model* Wall = new Model("resources/wall/wall.obj");
	Model* Tree = new Model("resources/Tree/Tree.obj");

	Model* Weapon = new Model("resources/Weapon/AK-74(HP).obj");
	glm::mat4 weaponModelMatrix;
	glm::vec3 weaponOffset(0.8f, -1.8f, -3.0f);

	Model* Cabin = new Model("resources/Cabin/cabin.obj");
	Material* cabinFloorMaterial = Cabin->getMaterial("02___Default");
	if (cabinFloorMaterial)
	{
		cabinFloorMaterial->setDiffuseMap("resources/Cabin/floortexture.jpg");
	}

	Model* Floor = new Model("resources/Floor/floor.obj");
	Material* floorMaterial = Floor->getMaterial("FloorMaterial");
	if (floorMaterial)
	{
		floorMaterial->setDiffuseMap("resources/Floor/floortexture.jpg");
	}
	
	
	// Floor
	std::vector<glm::vec3> FloorPositions = {
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, -4.0f, 0.0f),
	glm::vec3(-1.0f, -8.0f, 0.0f),
	glm::vec3(-1.0f, -12.0f, 0.0f),
	glm::vec3(-1.0f, -16.0f, 0.0f),
	glm::vec3(-1.0f, -20.0f, 0.0f),

	glm::vec3(-5.0f, -12.0f, 0.0f),
	glm::vec3(-5.0f, -16.0f, 0.0f),
	glm::vec3(-5.0f, -20.0f, 0.0f),

	glm::vec3(-9.0f, -12.0f, 0.0f),
	glm::vec3(-9.0f, -16.0f, 0.0f),
	glm::vec3(-9.0f, -20.0f, 0.0f),

	glm::vec3(-13.0f, -12.0f, 0.0f),
	glm::vec3(-13.0f, -16.0f, 0.0f),
	glm::vec3(-13.0f, -20.0f, 0.0f),

	
	glm::vec3(3.0f, -12.0f, 0.0f),
	glm::vec3(3.0f, -16.0f, 0.0f),
	glm::vec3(3.0f, -20.0f, 0.0f),

	glm::vec3(7.0f, -12.0f, 0.0f),
	glm::vec3(7.0f, -16.0f, 0.0f),
	glm::vec3(7.0f, -20.0f, 0.0f),

	glm::vec3(11.0f, -12.0f, 0.0f),
	glm::vec3(11.0f, -16.0f, 0.0f),
	glm::vec3(11.0f, -20.0f, 0.0f),
	};

	// Vertical Walls
	std::vector<glm::vec3> VerticalWallPositions = {
	glm::vec3(-5.0f, 0.0f, 0.0f),
	glm::vec3(-5.0f, 0.0f, -10.0f),
	glm::vec3(-5.0f, 0.0f, -20.0f),
	glm::vec3(5.0f, 0.0f, 0.0f),
	glm::vec3(5.0f, 0.0f, -10.0f),
	glm::vec3(5.0f, 0.0f, -20.0f),

	glm::vec3(35.0f, 0.0f, -30.0f),
	glm::vec3(35.0f, 0.0f, -40.0f),
	glm::vec3(35.0f, 0.0f, -50.0f),


	glm::vec3(-35.0f, 0.0f, -30.0f),
	glm::vec3(-35.0f, 0.0f, -40.0f),
	glm::vec3(-35.0f, 0.0f, -50.0f),

	};

	// Horizontal Walls
	std::vector<glm::vec3> HorizontalWallPositions = {
	glm::vec3(10.0f, 0.0f, -25.0f),
	glm::vec3(20.0f, 0.0f, -25.0f),
	glm::vec3(30.0f, 0.0f, -25.0f),

	glm::vec3(-10.0f, 0.0f, -25.0f),
	glm::vec3(-20.0f, 0.0f, -25.0f),
	glm::vec3(-30.0f, 0.0f, -25.0f),

	glm::vec3(-30.0f, 0.0f, -55.0f),
	glm::vec3(-20.0f, 0.0f, -55.0f),
	glm::vec3(-10.0f, 0.0f, -55.0f),
	glm::vec3(0.0f, 0.0f, -55.0f),
	glm::vec3(10.0f, 0.0f, -55.0f),
	glm::vec3(20.0f, 0.0f, -55.0f),
	glm::vec3(30.0f, 0.0f, -55.0f),
	
	};

	// Trees Position
	std::vector<glm::vec3> TreesPositions = {
	glm::vec3(10.0f, 0.0f, -20.0f),
	glm::vec3(-10.0f, 0.0f, -20.0f),
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

		for (const auto& position : VerticalWallPositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			Wall->draw(shaderProgram, model);
		}

		for (const auto& position : HorizontalWallPositions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			Wall->draw(shaderProgram, model);
		}

		for (const auto& position : TreesPositions) {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); 
			model = glm::translate(model, position);
			Tree->draw(shaderProgram, model);
		}

		for (const auto& position : FloorPositions) {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::translate(model, position);
			Floor->draw(shaderProgram, model);
		}

		weaponModelMatrix = camera.getViewMatrix();
		weaponModelMatrix = glm::inverse(weaponModelMatrix);

		weaponModelMatrix = glm::translate(weaponModelMatrix, weaponOffset);
		weaponModelMatrix = glm::rotate(weaponModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		pierModel = glm::mat4(1.0f);
		pierModel = glm::translate(pierModel, glm::vec3(0.0f, -4.3f, 15.0f));
		pierModel = glm::rotate(pierModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		pierModel = glm::scale(pierModel, glm::vec3(1.8f, 1.8f, 1.8f));
		WoodenPier->draw(shaderProgram, pierModel);
		
		Weapon->draw(shaderProgram, weaponModelMatrix);

		Floor->draw(shaderProgram, model);

		glm::mat4 cabinModel(1.0f);
		cabinModel = glm::scale(cabinModel, glm::vec3(2.5f, 2.5f, 2.5f));
		cabinModel = glm::translate(cabinModel,glm::vec3( 0.0f, -1.0f, -17.0f));
		cabinModel = glm::rotate(cabinModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Cabin->draw(shaderProgram, cabinModel);

		SDL_GL_SwapWindow(window);
	}

	
	delete Wall;
	delete Tree;
	delete Weapon;
	delete WoodenPier;

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
