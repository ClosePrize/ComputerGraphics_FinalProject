#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Global deðiþkenler
Camera camera(800,600, glm::vec3(0.0f, 1.5f, 0.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// PawnAnim sýnýfý: animasyon için ofset ve sýnýr bilgisi taþýr
struct PawnAnim {
    float offsetZ = 0.0f;
    float speed = 10.0f;
    float maxZ = 150.0f;
    bool active = false;

    void update(float dt) {
        if (active && offsetZ < maxZ)
            offsetZ += speed * dt * 3.0f;
    }
};

// Beyaz ve siyah piyonlar için animasyon durumlarý
PawnAnim whitePawnAnim;
PawnAnim blackPawnAnim;

// Callback: pencere boyutu deðiþince viewport güncelle
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Fare hareketi ile kamera kontrolü
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.ProcessMouse(window);
}


// Klavye girdilerini kamera ile iþleme
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard('w');
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard('s');
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard('a');
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard('d');
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.ProcessKeyboard(17);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(' ');
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) camera.ProcessKeyboard('1');
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) camera.ProcessKeyboard('3');
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) whitePawnAnim.active = true;
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) blackPawnAnim.active = true;
}

float skyBoxVertices[] = {
    // Front face (+Z)
    -1000.0f, -1000.0f,  1000.0f,   0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     1000.0f, -1000.0f,  1000.0f,   0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     1000.0f,  1000.0f,  1000.0f,   0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -1000.0f,  1000.0f,  1000.0f,   0.0f,  0.0f, 1.0f,   0.0f, 1.0f,

    // Back face (-Z)
    -1000.0f, -1000.0f, -1000.0f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    -1000.0f,  1000.0f, -1000.0f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1000.0f,  1000.0f, -1000.0f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
     1000.0f, -1000.0f, -1000.0f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

     // Left face (-X)
     -1000.0f, -1000.0f, -1000.0f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     -1000.0f, -1000.0f,  1000.0f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     -1000.0f,  1000.0f,  1000.0f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     -1000.0f,  1000.0f, -1000.0f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

     // Right face (+X)
      1000.0f, -1000.0f, -1000.0f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      1000.0f,  1000.0f, -1000.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
      1000.0f,  1000.0f,  1000.0f,   1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      1000.0f, -1000.0f,  1000.0f,   1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

      // Top face (+Y)
      -1000.0f,  1000.0f, -1000.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      -1000.0f,  1000.0f,  1000.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       1000.0f,  1000.0f,  1000.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       1000.0f,  1000.0f, -1000.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f,

       // Bottom face (-Y)
       -1000.0f, -1000.0f, -1000.0f,   0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        1000.0f, -1000.0f, -1000.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        1000.0f, -1000.0f,  1000.0f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
       -1000.0f, -1000.0f,  1000.0f,   0.0f, -1.0f,  0.0f,  0.0f, 1.0f
};





unsigned int skyBoxIndices[] = {
     // Front face
    0, 1, 2,  2, 3, 0,

    // Back face
    4, 5, 6,  6, 7, 4,

    // Left face
    8, 9,10, 10,11, 8,

    // Right face
   12,13,14, 14,15,12,

    // Top face
   16,17,18, 18,19,16,

    // Bottom face
   20,21,22, 22,23,20
};



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
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), skyBoxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyBoxIndices), skyBoxIndices, GL_STATIC_DRAW);

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

    unsigned int skyTexture = TextureFromFile("pexels-asphotograpy-96622.jpg", "models");


    Shader shader("shader_glsl.vert", "shader_glsl.frag");
    Model myModel("models/backpack/adventure/Adventure_Sonic.obj");
    Model myModel1("models/Chess/chess/model.obj");                     // Board
    Model myModel2("models/backpack/tennis/model.obj");
    Model myModel3("models/Chess/chess/bishop/model.obj");              // White Bishop
    Model myModel4("models/Chess/chess/vezir/model.obj");               // White Queen
    Model myModel5("models/Chess/chess/pawn/model.obj");                // White Pawn
    Model myModel6("models/Chess/chess/blackPawn/model.obj");           // Black Pawn 1
    Model myModel7("models/Chess/chess/blackPawn2/model.obj");          // Black Pawn 2
    Model myModel8("models/Chess/chess/knight/model.obj");              // Black Knight
    Model myModel9("models/Chess/table/model.obj");              // Black Knight
    Model myLightModel("models/Chess/lamb.glb");              // Black Knight

    if (!myModel.IsLoaded()) {
        std::cerr << "Model yüklenemedi veya geçerli mesh içermiyor!" << std::endl;
        return -1;
    }

    float pawnOffsetZ = 0.0f;
    float pawnSpeed = 10.0f; // birim/saniye


    // Ana döngü
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Animasyon: piyon ileri gidiyor
        //std::cout << currentFrame << "\n";

        whitePawnAnim.update(deltaTime);
        blackPawnAnim.update(deltaTime);

        glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        shader.setVec3("lightPos", glm::vec3(100.0f, 200.0f, 100.0f)); // Iþýk kaynaðý pozisyonu
        shader.setVec3("viewPos", camera.Position);                   // Kameranýn konumu
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));     // Beyaz ýþýk


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
        glm::mat4 view = camera.getViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Sky Plane
        glm::mat4 skyModel = glm::mat4(1.0f);
        skyModel = glm::translate(skyModel, glm::vec3(0.0f, -2.0f, 0.0f)); // Hafif aþaðýda
        shader.setMat4("model", skyModel);
        shader.setVec3("materialColor", glm::vec3(1.0f)); // kullanýlmaz ama zorunlu
        shader.setInt("useTexture", 1);
        shader.setInt("texture_diffuse1", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyTexture);
        glBindVertexArray(skyVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glm::mat4 lightSphere = glm::mat4(1.0f);
        lightSphere = glm::translate(lightSphere, glm::vec3(0.0f, 250.0f, -100.0f));
        lightSphere = glm::scale(lightSphere, glm::vec3(1.0f));
        shader.setMat4("model", lightSphere);
        myLightModel.Draw(shader); // Önceden yüklü ýþýk modeli

        // Model 1: Ortada
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(0.0f, 200.0f, 0.0f)); // ilk modelin pozisyonu
        model1 = glm::scale(model1, glm::vec3(3.0f));                // ilk modelin ölçeði
        shader.setMat4("model", model1);
        myModel.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model2 = glm::scale(model2, glm::vec3(2.0f));                 // ayný ölçek
        shader.setMat4("model", model2);
        myModel1.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, glm::vec3(-100.0f, 300.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model3 = glm::scale(model3, glm::vec3(4.0f));                 // ayný ölçek
        shader.setMat4("model", model3);
        myModel2.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model4 = glm::mat4(1.0f);
        model4 = glm::translate(model4, glm::vec3(0.0f, 0.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model4 = glm::scale(model4, glm::vec3(2.0f));                    // ayný ölçek
        shader.setMat4("model", model4);
        myModel3.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model5 = glm::mat4(1.0f);
        model5 = glm::translate(model5, glm::vec3(0.0f, 0.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model5 = glm::scale(model5, glm::vec3(2.0f));                    // ayný ölçek
        shader.setMat4("model", model5);
        myModel4.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model6 = glm::mat4(1.0f);
        model6 = glm::translate(model6, glm::vec3(0.0f, 0.0f, 0.0f + whitePawnAnim.offsetZ)); // animasyonlu Z kaydýrma
        model6 = glm::scale(model6, glm::vec3(2.0f));
        shader.setMat4("model", model6);
        myModel5.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model7 = glm::mat4(1.0f);
        model7 = glm::translate(model7, glm::vec3(0.0f, 0.0f, blackPawnAnim.offsetZ)); // ikinci model saða kaydýrýldý
        model7 = glm::scale(model7, glm::vec3(2.0f));                    // ayný ölçek
        shader.setMat4("model", model7);
        myModel6.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model8 = glm::mat4(1.0f);
        model8 = glm::translate(model8, glm::vec3(0.0f, 0.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model8 = glm::scale(model8, glm::vec3(2.0f));                    // ayný ölçek
        shader.setMat4("model", model8);
        myModel7.Draw(shader);

        // Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model9 = glm::mat4(1.0f);
        model9 = glm::translate(model9, glm::vec3(0.0f, 0.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model9 = glm::scale(model9, glm::vec3(2.0f));                    // ayný ölçek
        shader.setMat4("model", model9);
        myModel8.Draw(shader);

        /*// Model 2: Saðda veya yukarýda (örneðin X ekseninde kaydýr)
        glm::mat4 model10 = glm::mat4(1.0f);
        model10 = glm::translate(model10, glm::vec3(0.0f, -200.0f, 0.0f)); // ikinci model saða kaydýrýldý
        model10 = glm::scale(model10, glm::vec3(200.0f));                    // ayný ölçek
        shader.setMat4("model", model10);
        myModel9.Draw(shader);*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
