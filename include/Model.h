#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "json.h"
#include "Mesh.h"
#include "armadillo"

using json = nlohmann::json;

class Model
{
public:
    // Loads in a model from a file and stores the information in 'data', 'JSON', and 'file'
    explicit Model(const char* file);

    glm::vec3 Position{};
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    float speed = 1.0f;
    float phi = 0;
    float rotated_angle = 0.0f;
    glm::vec3 omega {0.0f};
    glm::vec3 v{0.0f};

    glm::quat rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    void Draw(Shader& shader, Camera& camera);
    void Input(GLFWwindow* window);
    void calculateMovement(float dt);

private:
    // Variables for easy access
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    // All the meshes and transformations
    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    // Prevents textures from being loaded twice
    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    // Loads a single mesh by its index
    void loadMesh(unsigned int indMesh);

    // Traverses a node recursively, so it essentially traverses all connected nodes
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    // Gets the binary data from a file
    std::vector<unsigned char> getData();
    // Interprets the binary data into floats, indices, and textures
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture> getTextures();

    // Assembles all the floats into vertices
    static std::vector<Vertex> assembleVertices
            (
                    std::vector<glm::vec3> positions,
                    std::vector<glm::vec3> normals,
                    std::vector<glm::vec2> texUVs
            );

    // Helps with the assembly from above by grouping floats
    static std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    static std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);

    [[maybe_unused]] static std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

    float dx_dt, d_Vx_dt, dz_dt, d_Vz_dt, d_phi_dt, d_omega_w_dt;

    static float T(float v);
};
#endif