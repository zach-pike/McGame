#include "player.hpp"

#include "game/game.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Player constructor
Player::Player(Game& game): gameobj(game) {
    this->camera = Camera();
    this->position = glm::vec3(0.0f, 1.0f, 0.0f);
}

// Player destructor
Player::~Player() {

}

// getCamera function
Camera& Player::getCamera() {
    return this->camera;
}

// getPosition function
glm::vec3 Player::getPosition() {
    return this->position;
}

// setPosition function
void Player::setPosition(glm::vec3 position) {
    this->position = position;
}

void Player::update() {
    // Get the cursor position
    double xpos, ypos;
    glfwGetCursorPos(gameobj.getWindow(), &xpos, &ypos);

    int width = gameobj.getWindowWidth();
    int height = gameobj.getWindowHeight();

    // Reset the cursor position
    glfwSetCursorPos(gameobj.getWindow(), width/2, height/2);

    xpos -= width/2;
    ypos -= height/2;

    // Update the camera pitch and yaw
    // get the camera pitch and yaw
    glm::vec2 pitchYaw = this->camera.getPitchYaw();
    // update the pitch
    pitchYaw.x += ypos * 0.2f;
    // update the yaw
    pitchYaw.y += xpos * 0.2f;

    if (pitchYaw.x > -91.0f) {
        pitchYaw.x = -91.0f;
    } else if (pitchYaw.x < -269.0f) {
        pitchYaw.x = -269.0f;
    }

    // set the camera pitch and yaw
    this->camera.setPitchYaw(pitchYaw);

    // Now check for movement
    

    if (glfwGetKey(gameobj.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        this->position += this->camera.getLookVector() * 0.1f;
    } else if (glfwGetKey(gameobj.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        this->position -= this->camera.getLookVector() * 0.1f;
    } else if (glfwGetKey(gameobj.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        this->position -= this->camera.getRightVector() * 0.1f;
    } else if (glfwGetKey(gameobj.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        this->position += this->camera.getRightVector() * 0.1f;
    }

    // Print the player position
    // std::cout << "Player position: " << this->position.x << ", " << this->position.y << ", " << this->position.z << std::endl;

}