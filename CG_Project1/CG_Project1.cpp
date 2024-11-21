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
#include "OBJLoader.h"

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

/*
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};*/


	void InitObjModels();
	{
		std::vector<Vertex>temp;
		temp = loadOBJ("1.obj");
	}


	/*unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
*/

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo); // Generate 1 element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shaderProgram("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

	stbi_set_flip_vertically_on_load(true);

	Texture texture0("resources/container.jpg", GL_TEXTURE_2D, 0);

	Texture texture1("resources/awesomeface.png", GL_TEXTURE_2D, 1);

	shaderProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	//GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	//glEnableVertexAttribArray(colorAttrib);
	//glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	shaderProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	shaderProgram.use();
	shaderProgram.set3Float("triangleColor", 0.0f, 1.0f, 0.0f);
	shaderProgram.setInt("ourTexture", 0);
	shaderProgram.setInt("ourTexture2", 1);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	glm::mat4 view;
	view = camera.getViewMatrix();

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

	shaderProgram.setMat4("model", model);
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	SDL_Event event;

	int start = SDL_GetTicks();
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = start; // Time of last frame

	glEnable(GL_DEPTH_TEST);
	bool isRunning = true;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (isRunning) // Render Loop
	{
		int now = SDL_GetTicks();
		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;


		while (SDL_PollEvent(&event) != 0) // Event Loop
		{
			if (event.type == SDL_QUIT) isRunning = false;

			if (event.type == SDL_MOUSEMOTION) {
				camera.processMouseInput(event.motion.x, event.motion.y);
			}

			if (event.type == SDL_MOUSEWHEEL) {
				camera.processMouseScroll(event.wheel.y);
			}
		}

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		camera.processKeyboardInput(keyState, deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		texture0.bind();

		texture1.bind();

		glBindVertexArray(vao);

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		for (unsigned int i = 0; i < 10; ++i)
		{
			
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

