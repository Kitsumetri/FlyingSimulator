#include "Model.h"
#include "Constants.h"

#pragma region SKYBOX_CONSTANTS
constinit float SKYBOX_VERTICES[] =
        {
                //   Coordinates
                -1.0f, -1.0f, 1.0f,     //        7--------6
                1.0f, -1.0f, 1.0f,     //       /|       /|
                1.0f, -1.0f, -1.0f,    //      4--------5 |
                -1.0f, -1.0f, -1.0f, //      | |      | |
                -1.0f, 1.0f, 1.0f,  //      | 3------|-2
                1.0f, 1.0f, 1.0f,   //      |/       |/
                1.0f, 1.0f, -1.0f, //      0--------1
                -1.0f, 1.0f, -1.0f
        };

constinit  unsigned int SKYBOX_INDICES[] = {
                // Right
                1, 2, 6,
                6, 5, 1,
                // Left
                0, 4, 7,
                7, 3, 0,
                // Top
                4, 5, 6,
                6, 7, 4,
                // Bottom
                0, 3, 2,
                2, 1, 0,
                // Back
                0, 1, 5,
                5, 4, 0,
                // Front
                3, 7, 6,
                6, 2, 3
        };
#pragma endregion

void init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, UtilConstants::SAMPLES);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

unsigned int load_skybox() {
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES),
                 &SKYBOX_VERTICES, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SKYBOX_INDICES),
                 &SKYBOX_INDICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE,
                          3 * sizeof(float),
                          (void*) nullptr);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return skyboxVAO;
}

void check_success_window(GLFWwindow* g_window) {
    if (!g_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }
}

unsigned int create_cubemaps() {
    unsigned int cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; ++i) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(UtilConstants::CUBEMAPS_PATH[i].c_str(),
                                        &width, &height,
                                        &nrChannels, 0);
        if (data) {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,GL_RGB,
                        width, height,
                        0,GL_RGB,
                        GL_UNSIGNED_BYTE,
                        data
                    );
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture: " << UtilConstants::CUBEMAPS_PATH[i] << std::endl;
            stbi_image_free(data);
        }
    }
    return cubemapTexture;
}

void get_light_shaders(const Shader& shaderProgram) {
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
                lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"),
                lightPos.x, lightPos.y, lightPos.z);
}

int main()
{
    init_glfw();
    GLFWwindow* window = glfwCreateWindow(UtilConstants::WIDTH,
                                          UtilConstants::HEIGHT,
                                          "FlyingSimulator",
                                          nullptr, nullptr);
    check_success_window(window);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0,
               UtilConstants::WIDTH,
               UtilConstants::HEIGHT);

    Shader shaderProgram(UtilConstants::SHADER_DEFAULT_PATH[0].c_str(),
                         UtilConstants::SHADER_DEFAULT_PATH[1].c_str());
    get_light_shaders(shaderProgram);

    Shader skyboxShader(UtilConstants::SHADER_SKYBOX_PATH[0].c_str(),
                        UtilConstants::SHADER_SKYBOX_PATH[1].c_str());
    skyboxShader.Activate();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    Camera camera(UtilConstants::WIDTH,
                  UtilConstants::HEIGHT,
                  glm::vec3(0.0f, 0.0f, 0.0f));


    Model model((UtilConstants::PARENT_DIRECTORY + UtilConstants::MODEL_PATH).c_str());
    Model model2((UtilConstants::PARENT_DIRECTORY +  "/Resources/models/bunny/scene.gltf").c_str());


    model.Position = glm::vec3(0.0f,  -1.0f, -6.0f);
    model2.Position = glm::vec3(0.0f,  -1.0f, -6.0f);

    // Create VAO, VBO, and EBO for the skybox
    unsigned int skybox_VAO = load_skybox();

    // Creates the cubemap texture object
    unsigned int cubemapTexture = create_cubemaps();

    // FPS
    double prevTime = 0.0;
    double crntTime, timeDiff;
    unsigned int counter = 0;

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
#pragma region FPS
        // Updates counter and times
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;

        if (timeDiff >= 1.0 / 60.0) {
            // Creates new title
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle =  FPS.append("FPS / ").append(ms).append("ms");
            glfwSetWindowTitle(window, newTitle.c_str());

            // Resets times and counter
            prevTime = crntTime;
            counter = 0;
        }
#pragma endregion
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window); // delete if Vsync disabled
        model.Input(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        glDepthFunc(GL_LEQUAL);
        model.Draw(shaderProgram, camera);
        model2.Draw(shaderProgram, camera);
        skyboxShader.Activate();

        camera.Position = model.Position;
        camera.Position.z += 7.0f;
        camera.Position.y += 2.0f;


        glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera.Position,
                                                    camera.Position + camera.Orientation, camera.Up)));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                      (float)UtilConstants::WIDTH / (float)UtilConstants::HEIGHT,
                                      0.1f,
                                      100.0f);

        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"),
                           1, GL_FALSE,
                           glm::value_ptr(view));


        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"),
                           1, GL_FALSE,
                           glm::value_ptr(projection));


        glBindVertexArray(skybox_VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.Delete();
    skyboxShader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}