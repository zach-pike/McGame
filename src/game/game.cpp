#include "game.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader/shader.hpp"
#include "texture/texture.hpp"

#include "game/world/chunk/chunk.hpp"
#include "game/player/player.hpp"

#include <glm/gtc/matrix_transform.hpp>

// Game constructor
Game::Game() : world{5, 5} {}

// Game destructor
Game::~Game() {
    // delete this->world;
}

void Game::setFov(float fov) {
    this->fov = fov;

    recalculateMvpMatrix();
}

float Game::getFov() {
    return this->fov;
}

void Game::recalculateMvpMatrix() {
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}

void Game::setProjectionMatrix(glm::mat4 projectionMatrix) {
    this->projectionMatrix = projectionMatrix;

    recalculateMvpMatrix();
}

void Game::setViewMatrix(glm::mat4 viewMatrix) {
    this->viewMatrix = viewMatrix;

    recalculateMvpMatrix();
}

void Game::setModelMatrix(glm::mat4 modelMatrix) {
    this->modelMatrix = modelMatrix;

    recalculateMvpMatrix();
}

// Game run function
void Game::run() {
    // Initialise GLFW
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Set window hints
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Create the window with glfw
    window = glfwCreateWindow( windowWidth, windowHeight, "Tutorial 01", NULL, NULL);

    // If we failed to initialize glfw
    if(window == NULL) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        throw std::runtime_error("Failed to open GLFW window");
    }

    // Make the window object the current glfw contect
    glfwMakeContextCurrent(window); // Initialize GLEW

    glewExperimental=true; // Needed in core profile

    // If glew initialized correctly
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        throw std::runtime_error("Failed to initialize GLEW");
    }

    // Hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create our VBO's
    // I don't know why we do this but its needed
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Load our shaders
    programID = LoadShaders("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    Player player(*this);

    // Set up our buffers
    std::vector<GLfloat> verticies;
    std::vector<GLuint> indicies;
    std::vector<GLfloat> uvs;

    // Generate the world
    world.generateMesh(verticies, indicies, uvs);

    // Load the texture and set it up
    GLuint Texture = LoadBMP("src/textures/Chunk.bmp");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Vertex buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(GLfloat), verticies.data(), GL_STATIC_DRAW);

    // uv buffer
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);

    GLuint indiciesbuffer;
    glGenBuffers(1, &indiciesbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indiciesbuffer);
    glBufferData(GL_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    setProjectionMatrix(glm::perspective(glm::radians(fov), (float) windowWidth / (float)windowHeight, near, far));
    setViewMatrix(glm::lookAt(
        player.getPosition(),
        player.getPosition() - player.getCamera().getLookVector(),
        glm::vec3(0,1,0)
    ));
    setModelMatrix(glm::mat4(1.0f));

    // Use our shader program
    glUseProgram(programID);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    int frameCount = 0;

    do {
        // Every 5th frame, we will get the framebuffer size and update the projection matrix if it has changed
        if (frameCount == 0) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width != windowWidth || height != windowHeight) {
                windowWidth = width;
                windowHeight = height;
                setProjectionMatrix(glm::perspective(glm::radians(fov), (float) windowWidth / (float)windowHeight, near, far));
            
                glViewport(0, 0, windowWidth, windowHeight);
            }
        }

        frameCount = (frameCount + 1) % 60;


        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 0.3f);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

        // Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

        // 1st attribute buffer : vertices
        // Enable our vertex array
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiciesbuffer);
        glDrawElements(
            GL_TRIANGLES,      // mode
            indicies.size(),    // count
            GL_UNSIGNED_INT,   // type
            (void*)0           // element array buffer offset
        );

        // Do the thing
        // glDrawArrays(GL_TRIANGLES, 0, verticies.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update the player
        player.update();

        // Camera matrix
        setViewMatrix(glm::lookAt(
            player.getPosition(),
            player.getPosition() + player.getCamera().getLookVector(),
            glm::vec3(0,1,0)
        ));

        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
}

GLFWwindow* Game::getWindow() {
    return window;
}

int Game::getWindowWidth() {
    return windowWidth;
}

int Game::getWindowHeight() {
    return windowHeight;
}