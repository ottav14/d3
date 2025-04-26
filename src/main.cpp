#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

float vertices[] = {
	-0.5f,  0.5f, 0.0f,  // top left
	-0.5f, -0.5f, 0.0f,  // bottom left
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f,  0.5f, 0.0f,  // top left
	 0.5f, -0.5f, 0.0f,  // bottom right
	 0.5f,  0.5f, 0.0f   // top right
};

void init(SDL_Window** window, SDL_GLContext* glContext) {
    // Initialize SDL3 with OpenGL
    SDL_Init(SDL_INIT_VIDEO);

    // Create an SDL window with OpenGL context
    *window = SDL_CreateWindow("d3",
	   						    800, 
							    600, 
							    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

    // Create OpenGL context
    *glContext = SDL_GL_CreateContext(*window);

    // Initialize GLAD to load OpenGL functions
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
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

void initializeVertexBuffer(GLuint* VAO, GLuint* VBO) {

    // Setup Vertex Array and Buffer Objects (VAO, VBO)
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // Bind VAO
    glBindVertexArray(*VAO);

    // Bind VBO and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define vertex attributes (position attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main() {

	SDL_Window* window;
	SDL_GLContext glContext;
	init(&window, &glContext);

	GLuint shaderProgram = initializeShaders();

	GLuint VAO, VBO;
	initializeVertexBuffer(&VAO, &VBO);


    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

		// Get time
		float timeSeconds = (float)SDL_GetTicks() / 1000.0f;

		// Build vertex matrices
		glm::vec3 initial_camera_position = glm::vec3(0.0f, 0.0f, -3.0f);

		glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, timeSeconds, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Z

        glm::mat4 view = glm::translate(glm::mat4(1.0f), initial_camera_position); // Move camera back

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
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);  

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
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

