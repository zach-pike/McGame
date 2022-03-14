#include "camera.hpp"

// Camera constructor
Camera::Camera() {
    this->lookvector = glm::vec3(0.0f, 0.0f, 0.0f);
}

// Camera destructor
Camera::~Camera() {

}

glm::vec3 Camera::getLookVector() {
    return this->lookvector;
}

void Camera::setPitchYaw(glm::vec2 rotation) {
    this->pitch = rotation.x;
    this->yaw = rotation.y;

    // Calculate the look vector
    this->lookvector.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->lookvector.y = sin(glm::radians(this->pitch));
    this->lookvector.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->lookvector = glm::normalize(this->lookvector);
}

glm::vec2 Camera::getPitchYaw() {
    return glm::vec2(this->pitch, this->yaw);
}

glm::vec3 Camera::getRightVector() {
    return glm::normalize(glm::cross(this->lookvector, glm::vec3(0.0f, 1.0f, 0.0f)));
}