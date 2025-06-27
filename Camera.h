#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

enum CameraMode {
    FREE,
    FPS
};

class Camera
{
public:
    // Ekran boyutlar�
    int width, height;

    // Kamera vekt�rleri
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Ayarlar
    float MovementSpeed;
    float MouseSensitivity;
    bool firstClick;

    // G�r�n�m matrisi
    glm::mat4 cameraMatrix;

    // Mod
    CameraMode mode;

    // Kurucu
    Camera(int width, int height, glm::vec3 position);

    // Kamera matrisini g�ncelle
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

    // Kamera matrisini shader'a g�nder
    void Matrix(Shader& shader, const char* uniform);

    // G�r�n�m ve projeksiyon matrislerini al
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float FOVdeg, float nearPlane, float farPlane);

    // Klavye ve fare girdilerini i�le
    void ProcessKeyboard(unsigned char key);
    void ProcessMouse(GLFWwindow* window);

    // Kamera modunu ayarla
    void SetFPSMode(bool enable);

private:
    // Kamera y�n�n� g�ncelle
    void updateCameraVectors();
};

#endif
