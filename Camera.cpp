#include "Camera.h"
#include <iostream>

Camera::Camera(int width, int height, glm::vec3 position)
    : width(width), height(height), Position(position),
    WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(-90.0f), Pitch(0.0f),
    MovementSpeed(5.0f), MouseSensitivity(0.1f), firstClick(true), mode(FREE)
{
    updateCameraVectors();
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::lookAt(Position, Position + Front, Up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjectionMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
}

void Camera::ProcessKeyboard(unsigned char key)
{
    // If in FPS mode, restrict keyboard movement
    if (mode == FPS) {
        // Only allow '1' and '3' to switch modes
        if (key == '1') {
            SetFPSMode(false);
            std::cout << "Free camera mode\n";
        }
        if (key == '3') {
            SetFPSMode(true);
            std::cout << "FPS camera mode (already in FPS mode)\n"; // Already in FPS mode
        }
        return; // Exit if not 1 or 3
    }

    // Free camera mode movement
    float velocity = MovementSpeed * 0.05f;

    if (key == 'w') {
        glm::vec3 forward = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
        Position += forward * velocity;
    }
    if (key == 's') {
        glm::vec3 backward = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
        Position -= backward * velocity;
    }

    if (key == 'a') Position -= Right * velocity;
    if (key == 'd') Position += Right * velocity;
    if (key == ' ') Position += WorldUp * velocity;
    if (key == 17) Position -= WorldUp * velocity; // CTRL

    if (key == '1') {
        Position = glm::vec3(-643.326f, 151.75f, -561.753f);
        SetFPSMode(false);
        std::cout << "Free camera mode\n";
    }
    if (key == '3') {
        SetFPSMode(true);
        std::cout << "FPS camera mode\n";
    }
}

void Camera::ProcessMouse(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float xoffset = (float)(mouseX - width / 2);
        float yoffset = (float)(height / 2 - mouseY);

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        updateCameraVectors();
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}

void Camera::SetFPSMode(bool enable)
{
    mode = enable ? FPS : FREE;
    if (mode == FPS) {
        // Set fixed position for FPS mode
        Position = glm::vec3(-788.444f, 92.0f, -595.458f);
    }
    // No need to reset position for FREE mode, as it will be updated by keyboard input
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}