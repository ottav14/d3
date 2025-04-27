#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.141592f
#define CAMERA_SPEED 0.1f
#define MOUSE_SENSITIVITY 0.1f
#define WIREFRAME_ENABLED true

const char* vertexShaderSource = R"glsl(
	#version 330 core

	layout(location = 0) in vec3 aPosition;

	uniform mat4 uModel;
	uniform mat4 uView;
	uniform mat4 uProjection;

	void main()
	{
		gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
	}
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // orange color
    }
)glsl";

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  // 0
     0.5f, -0.5f, -0.5f,  // 1
     0.5f,  0.5f, -0.5f,  // 2
    -0.5f,  0.5f, -0.5f,  // 3
    -0.5f, -0.5f,  0.5f,  // 4
     0.5f, -0.5f,  0.5f,  // 5
     0.5f,  0.5f,  0.5f,  // 6
    -0.5f,  0.5f,  0.5f   // 7
};

unsigned int cubeIndices[] = {
    // back face
    0, 1, 2,
    2, 3, 0,
    // front face
    4, 5, 6,
    6, 7, 4,
    // left face
    4, 0, 3,
    3, 7, 4,
    // right face
    1, 5, 6,
    6, 2, 1,
    // bottom face
    4, 5, 1,
    1, 0, 4,
    // top face
    3, 2, 6,
    6, 7, 3
};

float vertices[] = {
	-0.5f,  0.5f, 0.0f,  // top left
	-0.5f, -0.5f, 0.0f,  // bottom left
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f,  0.5f, 0.0f,  // top left
	 0.5f, -0.5f, 0.0f,  // bottom right
	 0.5f,  0.5f, 0.0f   // top right
};

uint8_t keys_held = 0;
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, -3.0f);
float camera_yaw = 90.0f;
float camera_pitch = 0;

void init(SDL_Window** window, SDL_GLContext* glContext) {
    // Initialize SDL3 with OpenGL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    // Create an SDL window with OpenGL context
    *window = SDL_CreateWindow("d3",
	   						    800, 
							    600, 
							    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

    // Create OpenGL context
    *glContext = SDL_GL_CreateContext(*window);

    // Initialize GLAD to load OpenGL functions
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	// Capture mouse
	SDL_SetWindowRelativeMouseMode(*window, true);
}

GLuint initializeShaders() {
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	return shaderProgram;
}

void initializeVertexBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO) {

    // Setup Vertex Array and Buffer Objects (VAO, VBO)
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    // Bind VAO
    glBindVertexArray(*VAO);

    // Bind VBO and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// Element Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Define vertex attributes (position attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool handleKeyboardInput(SDL_Event event) {
	if(event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
		switch(event.key.key) {
			case SDLK_W:
				keys_held |= (1 << 0);
				break;
			case SDLK_A:
				keys_held |= (1 << 1);
				break;
			case SDLK_S:
				keys_held |= (1 << 2);
				break;
			case SDLK_D:
				keys_held |= (1 << 3);
				break;
			case SDLK_ESCAPE:
				return false; 
		}
	}

	if(event.type == SDL_EVENT_KEY_UP) {
		switch(event.key.key) {
			case SDLK_W:
				keys_held &= ~(1 << 0);
				break;
			case SDLK_A:
				keys_held &= ~(1 << 1);
				break;
			case SDLK_S:
				keys_held &= ~(1 << 2);
				break;
			case SDLK_D:
				keys_held &= ~(1 << 3);
				break;
		}
	}
	return true;

}

glm::vec3 getCameraFront() {
	glm::vec3 front;
	front.x = cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
	front.y = sin(glm::radians(camera_pitch));
	front.z = sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
	front = glm::normalize(front);
	return front;
}

void updateCameraPosition() {
	glm::vec3 camera_front = getCameraFront();
	camera_front.y = 0;
	camera_front = glm::normalize(camera_front); // Project front vector to xz plane
	glm::vec3 camera_right = glm::cross(camera_front, glm::vec3(0.0f, 1.0f, 0.0f));
	if((keys_held & (1 << 0))) camera_position += CAMERA_SPEED * camera_front; // Forward
	if((keys_held & (1 << 1))) camera_position -= CAMERA_SPEED * camera_right; // Left
	if((keys_held & (1 << 2))) camera_position -= CAMERA_SPEED * camera_front; // Back
	if((keys_held & (1 << 3))) camera_position += CAMERA_SPEED * camera_right; // Right
}

void handleMouseInput(float xrel, float yrel) {

	camera_yaw += MOUSE_SENSITIVITY * xrel;
	camera_pitch -= MOUSE_SENSITIVITY * yrel;

	if(camera_yaw > 360.0f) camera_yaw -= 360.0f;
	if(camera_yaw < 0.0f) camera_yaw += 360.0f;

	if(camera_pitch > 90.0f) camera_pitch = 90.0f;
	if(camera_pitch < -90.0f) camera_pitch = -90.0f;

}

int main() {

	SDL_Window* window;
	SDL_GLContext glContext;
	init(&window, &glContext);

	GLuint shaderProgram = initializeShaders();

	GLuint VAO, VBO, EBO;
	initializeVertexBuffer(&VAO, &VBO, &EBO);


    // Main loop
    bool running = true;
    SDL_Event event;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                running = false;
            }

			if(event.type == SDL_EVENT_MOUSE_MOTION) {
				handleMouseInput(event.motion.xrel, event.motion.yrel);
			}

			if(event.type == SDL_EVENT_KEY_UP || event.type == SDL_EVENT_KEY_DOWN) {
				running = handleKeyboardInput(event);
			}
		}
		updateCameraPosition();


        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
		if(WIREFRAME_ENABLED) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe

		// Get time
		float timeSeconds = (float)SDL_GetTicks() / 1000.0f;

		// Build vertex matrices
		glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, timeSeconds, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotate model
        model = glm::scale(model, glm::vec3(0.5f, sin(timeSeconds) + 1.0f, 1.0f)); // Scale model

		// Rotate camera
		float pitchAngle = glm::radians(PI/2);
		float cosYaw = cos(pitchAngle);
		float sinYaw = sin(pitchAngle);

		glm::vec3 camera_front = getCameraFront();
	
		glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_front, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// Pass matrices to shader
		GLint modelLoc = glGetUniformLocation(shaderProgram, "uModel");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "uView");
        GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw triangles
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap buffers
        SDL_GL_SwapWindow(window);

        // Check for OpenGL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Cleanup SDL
	SDL_CaptureMouse(false);
	SDL_ShowCursor();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

