#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera camera(800,600, glm::vec3(-783.0f, 76.0f, -559.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;
bool sequentialStarted = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float chessScale = 0.3f;
float chessRotate = 0.0f;
float chessSpeed = 36.0f;
float chessPosX = -783.0f;
float chessPosY = 71.0f;
float chessPosZ = -559.0f;


struct PawnAnim {
    float offsetZ = 0.0f;
    float offsetX = 0.0f;
    float speed = 10.0f;
    float maxZ = 150.0f;
    float maxX = 150.0f;
    int dirZ = 1;
    int dirX = 0;
    bool active = false;
    bool done = false;
    bool done2 = false;

    void update(float dt) {
        if (active) {
            if (abs(offsetZ) < maxZ)
                offsetZ += speed * dt * dirZ;
            else
                offsetZ = dirZ * maxZ;

            if (abs(offsetX) < maxX)
                offsetX += speed * dt * dirX;
            else
                offsetX = dirX * maxX;

            if (abs(offsetZ) >= maxZ && abs(offsetX) >= maxX) {
                active = false;
                if (done == true)
                    done2 = true;
                else
                    done = true;
            }
        }
    }
};



PawnAnim whitePawnAnim = { 0.0f, 0.0f, chessSpeed * chessScale, 72.0f* chessScale, 0.0f* chessScale, 1, 0, false, false };
PawnAnim whiteQueenAnim = { 0.0f, 0.0f, chessSpeed * chessScale, 72.0f* chessScale, 72.0f* chessScale, 1, -1, false, false };
PawnAnim whiteBishopAnim = { 0.0f, 0.0f, chessSpeed * chessScale, 108.0f* chessScale, 108.0f* chessScale, 1, 1, false, false };
PawnAnim blackPawnAnim = { 0.0f, 0.0f, chessSpeed * chessScale, 72.0f* chessScale, 0.0f* chessScale, -1, 0, false, false };
PawnAnim blackPawn2Anim = { 0.0f, 0.0f, chessSpeed * chessScale, 72.0f* chessScale, 0.0f* chessScale, -1, 0, false, false };
PawnAnim blackPawn3Anim = { 0.0f, 0.0f, 200.0f * chessScale, 72.0f* chessScale, 108.0f* chessScale, -1, -1, false, false };
PawnAnim blackKnightAnim = { 0.0f, 0.0f, chessSpeed * chessScale, 72.0f* chessScale, 36.0f* chessScale, -1, 1, false, false };


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Fare hareketi ile kamera kontrolü
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.ProcessMouse(window);
}

// Klavye girdilerini kamera ile işleme
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard('w');
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard('s');
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard('a');
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard('d');
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.ProcessKeyboard(17);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(' ');
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) camera.ProcessKeyboard('1');
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) camera.ProcessKeyboard('3');
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        whitePawnAnim.active = true;
        sequentialStarted = true;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        whitePawnAnim.offsetX = 0;
        whitePawnAnim.offsetZ = 0;
        whitePawnAnim.active = false;
        whitePawnAnim.done = false;

        whiteQueenAnim.offsetX = 0;
        whiteQueenAnim.offsetZ = 0;
        whiteQueenAnim.maxZ = 72.0f * chessScale;
        whiteQueenAnim.active = false;
        whiteQueenAnim.done = false;

        whiteBishopAnim.offsetX = 0;
        whiteBishopAnim.offsetZ = 0;
        whiteBishopAnim.active = false;
        whiteBishopAnim.done = false;

        blackPawnAnim.offsetX = 0;
        blackPawnAnim.offsetZ = 0;
        blackPawnAnim.active = false;
        blackPawnAnim.done = false;

        blackPawn2Anim.offsetX = 0;
        blackPawn2Anim.offsetZ = 0;
        blackPawn2Anim.active = false;
        blackPawn2Anim.done = false;

        blackPawn3Anim.offsetX = 0;
        blackPawn3Anim.offsetZ = 0;
        blackPawn3Anim.active = false;
        blackPawn3Anim.done = false;

        blackKnightAnim.offsetX = 0;
        blackKnightAnim.offsetZ = 0;
        blackKnightAnim.active = false;
        blackKnightAnim.done = false;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Assimp Model Loading", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    unsigned int skyVAO, skyVBO, skyEBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glGenBuffers(1, &skyEBO);

    glBindVertexArray(skyVAO);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // texCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glBindVertexArray(0);

    Shader shader("shaders/shader_glsl.vert", "shaders/shader_glsl.frag");
    Model chessBoard("models/Chess/model.obj");                   // Board
    Model whiteBishop("models/Chess/bishop/model.obj");           // White Bishop
    Model whiteQueen("models/Chess/queen/model.obj");             // White Queen
    Model whitePawn("models/Chess/pawn/model.obj");               // White Pawn
    Model blackPawn("models/Chess/blackPawn/model.obj");          // Black Pawn 1
    Model blackPawn2("models/Chess/blackPawn2/model.obj");        // Black Pawn 2
    Model blackPawn3("models/Chess/blackPawn3/model.obj");        // Black Pawn 3
    Model blackKnight("models/Chess/knight/model.obj");           // Black Knight
    Model room("models/Chess/room/model.obj");                    // Room
    Model myLightModel("models/Chess/lamb.glb");                  // Lamb

    if (!whiteBishop.IsLoaded()) {
        std::cerr << "Model yüklenemedi veya geçerli mesh içermiyor!" << std::endl;
        return -1;
    }

    // Ana döngü
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        whitePawnAnim.update(deltaTime);
        whiteQueenAnim.update(deltaTime);
        whiteBishopAnim.update(deltaTime);
        blackPawnAnim.update(deltaTime);
        blackPawn2Anim.update(deltaTime);
        blackPawn3Anim.update(deltaTime);
        blackKnightAnim.update(deltaTime);

        if (sequentialStarted) {
            if (whitePawnAnim.done == true)
                blackPawnAnim.active = true;
            if (blackPawnAnim.done == true)
                whiteQueenAnim.active = true;
            if (whiteQueenAnim.done == true)
                blackKnightAnim.active = true;
            if (blackKnightAnim.done == true)
                whiteBishopAnim.active = true;
            if (whiteBishopAnim.done == true)
                blackPawn2Anim.active = true;
            if (blackPawn2Anim.done == true){
                whiteQueenAnim.maxZ = 216.0f * chessScale;
                whiteQueenAnim.maxX = -72.0f * chessScale;
                whiteQueenAnim.dirX = 1;
                whiteQueenAnim.speed = 72.0f * chessScale;
                whiteQueenAnim.active = true;
            }
            if (blackPawn2Anim.done == true)
                blackPawn3Anim.active = true;
            if (blackPawn3Anim.done == true)
                sequentialStarted = false;
        }

        glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        shader.setVec3("lightPos", glm::vec3(100.0f, 200.0f, 100.0f));  // Işık kaynağı pozisyonu
        shader.setVec3("viewPos", camera.Position);                     // Kameranın konumu
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));      // Beyaz ışık

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
        glm::mat4 view = camera.getViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 lightSphere = glm::mat4(1.0f);
        lightSphere = glm::translate(lightSphere, glm::vec3(200.0f, 250.0f, -100.0f));
        lightSphere = glm::scale(lightSphere, glm::vec3(1.0f));
        shader.setMat4("model", lightSphere);
        myLightModel.Draw(shader); 

        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(chessPosX, chessPosY, chessPosZ)); // ikinci model sağa kaydırıldı
        model2 = glm::rotate(model2, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f)); // y ekseni etrafında -40 derece
        model2 = glm::scale(model2, glm::vec3(chessScale));                 // aynı ölçek
        shader.setMat4("model", model2);
        chessBoard.Draw(shader);

        glm::mat4 model4 = glm::mat4(1.0f);
        model4 = glm::translate(model4, glm::vec3(chessPosX + whiteBishopAnim.offsetX, chessPosY, chessPosZ + whiteBishopAnim.offsetZ)); // ikinci model sağa kaydırıldı
        model4 = glm::rotate(model4, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model4 = glm::scale(model4, glm::vec3(chessScale));                    // aynı ölçek
        shader.setMat4("model", model4);
        whiteBishop.Draw(shader);

        glm::mat4 model5 = glm::mat4(1.0f);
        model5 = glm::translate(model5, glm::vec3(chessPosX + whiteQueenAnim.offsetX, chessPosY, chessPosZ + whiteQueenAnim.offsetZ)); // ikinci model sağa kaydırıldı
        model5 = glm::rotate(model5, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model5 = glm::scale(model5, glm::vec3(chessScale));                    // aynı ölçek
        shader.setMat4("model", model5);
        whiteQueen.Draw(shader);

        glm::mat4 model6 = glm::mat4(1.0f);
        model6 = glm::translate(model6, glm::vec3(chessPosX + whitePawnAnim.offsetX, chessPosY, chessPosZ + whitePawnAnim.offsetZ));
        model6 = glm::rotate(model6, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model6 = glm::scale(model6, glm::vec3(chessScale));
        shader.setMat4("model", model6);
        whitePawn.Draw(shader);

        glm::mat4 model7 = glm::mat4(1.0f);
        model7 = glm::translate(model7, glm::vec3(chessPosX + blackPawnAnim.offsetX, chessPosY, chessPosZ + blackPawnAnim.offsetZ));
        model7 = glm::rotate(model7, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model7 = glm::scale(model7, glm::vec3(chessScale));
        shader.setMat4("model", model7);
        blackPawn.Draw(shader);

        glm::mat4 model8 = glm::mat4(1.0f);
        model8 = glm::translate(model8, glm::vec3(chessPosX + blackPawn2Anim.offsetX, chessPosY, chessPosZ + blackPawn2Anim.offsetZ)); // ikinci model sağa kaydırıldı
        model8 = glm::rotate(model8, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model8 = glm::scale(model8, glm::vec3(chessScale));                    // aynı ölçek
        shader.setMat4("model", model8);
        blackPawn2.Draw(shader);
        
        glm::mat4 pawn3 = glm::mat4(1.0f);
        pawn3 = glm::translate(pawn3, glm::vec3(chessPosX + blackPawn3Anim.offsetX, chessPosY, chessPosZ + blackPawn3Anim.offsetZ)); // ikinci model sağa kaydırıldı
        pawn3 = glm::rotate(pawn3, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        pawn3 = glm::scale(pawn3, glm::vec3(chessScale));                    // aynı ölçek
        shader.setMat4("model", pawn3);
        blackPawn3.Draw(shader);

        glm::mat4 model9 = glm::mat4(1.0f);
        model9 = glm::translate(model9, glm::vec3(chessPosX + blackKnightAnim.offsetX, chessPosY, chessPosZ + blackKnightAnim.offsetZ)); // ikinci model sağa kaydırıldı
        model9 = glm::rotate(model9, glm::radians(chessRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model9 = glm::scale(model9, glm::vec3(chessScale));                    // aynı ölçek
        shader.setMat4("model", model9);
        blackKnight.Draw(shader);

        glm::mat4 model10 = glm::mat4(1.0f);
        model10 = glm::translate(model10, glm::vec3(0.0f, -200.0f, 0.0f)); // ikinci model sağa kaydırıldı
        model10 = glm::rotate(model10, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model10 = glm::scale(model10, glm::vec3(200.0f));                    // aynı ölçek
        shader.setMat4("model", model10);
        room.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
