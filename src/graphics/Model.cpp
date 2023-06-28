#include "Model.h"
#include "Constants.h"

Model::Model(const char* file) {
    std::string text = get_file_contents(file);
    JSON = json::parse(text);

    Model::file = file;
    data = getData();

    traverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Mesh::Draw(shader, camera, Position, matricesMeshes[i], rotation, scale);
}

void Model::loadMesh(unsigned int indMesh) {
    unsigned int posAccInd =
            JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccInd =
            JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccInd =
            JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indAccInd =
            JSON["meshes"][indMesh]["primitives"][0]["indices"];

    std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
    std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
    std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
    std::vector<Texture> textures = getTextures();

    meshes.emplace_back(vertices, indices, textures);
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix) {
    json node = JSON["nodes"][nextNode];

    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end()) {
        float transValues[3];
        for (unsigned int i = 0; i < node["translation"].size(); i++)
            transValues[i] = (node["translation"][i]);
        translation = glm::make_vec3(transValues);
    }
    glm::quat rotation_t = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    if (node.find("rotation") != node.end()) {
        float rotValues[4] =
                {
                        node["rotation"][3],
                        node["rotation"][0],
                        node["rotation"][1],
                        node["rotation"][2]
                };
        rotation_t = glm::make_quat(rotValues);
    }

    glm::vec3 scale_t = glm::vec3(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end()) {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++)
            scaleValues[i] = (node["scale"][i]);
        scale_t = glm::make_vec3(scaleValues);
    }

    glm::mat4 matNode = glm::mat4(1.0f);
    if (node.find("matrix") != node.end()) {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++)
            matValues[i] = (node["matrix"][i]);
        matNode = glm::make_mat4(matValues);
    }

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);


    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation_t);
    sca = glm::scale(sca, scale_t);

    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

    if (node.find("mesh") != node.end()) {
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation_t);
        scalesMeshes.push_back(scale_t);
        matricesMeshes.push_back(matNextNode);

        loadMesh(node["mesh"]);
    }

    if (node.find("children") != node.end())
        for(const auto & i : node["children"])
            traverseNode(i, matNextNode);
}

std::vector<unsigned char> Model::getData() {
    // Create a place to store the raw text, and get the uri of the .bin file
    std::string bytesText;
    std::string uri = JSON["buffers"][0]["uri"];

    // Store raw text data into bytesText
    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    bytesText = get_file_contents((fileDirectory + uri).c_str());

    // Transform the raw text data into bytes and put them in a vector
    return {bytesText.begin(), bytesText.end()};
}

std::vector<float> Model::getFloats(json accessor) {
    std::vector<float> floatVec;

    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int numPerVert;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;

    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i+= 4) {
        unsigned char bytes[] = { data[i], data[i+1], data[i+2], data[i+3] };
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }
    return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor) {
    std::vector<GLuint> indices;

    // Get properties from the accessor
    unsigned int buffViewInd = accessor.value("bufferView", 0);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];


    // Get properties from the bufferView
    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView.value("byteOffset", 0);

    // Get indices in regard to their type: unsigned int, unsigned short, or short
    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5126) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i+= 4) {
            unsigned char bytes[] = { data[i], data[i+1], data[i+2], data[i+3] };
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5125) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 4) {
            unsigned char bytes[] = { data[i], data[i+1], data[i+2], data[i+3] };
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5123) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2) {
            unsigned char bytes[] = { data[i], data[i+1] };
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5122) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i += 2) {
            unsigned char bytes[] = { data[i], data[i+1] };
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }

    return indices;
}

std::vector<Texture> Model::getTextures() {
    std::vector<Texture> textures;

    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    // Go over all images
    for (unsigned int i = 0; i < JSON["images"].size(); ++i) {
        // uri of current texture
        std::string texPath = JSON["images"][i]["uri"];

        // Check if the texture has already been loaded
        bool skip = false;
        for (unsigned int j = 0; j < loadedTexName.size(); ++j) {
            if (loadedTexName[j] == texPath) {
                textures.push_back(loadedTex[j]);
                skip = true;
                break;
            }
        }

        // If the texture has been loaded, skip this
        if (!skip) {
            // Load diffuse texture
            if (texPath.find("baseColorFactor") != std::string::npos ||
            texPath.find("diffuse") != std::string::npos) {
                Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
                textures.push_back(diffuse);
                loadedTex.push_back(diffuse);
                loadedTexName.push_back(texPath);
            }
                // Load specular texture
            else if (texPath.find("pbrMetallicRoughness") != std::string::npos
            || texPath.find("specular") != std::string::npos) {
                Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
                textures.push_back(specular);
                loadedTex.push_back(specular);
                loadedTexName.push_back(texPath);
            }
        }
    }

    return textures;
}

std::vector<Vertex> Model::assembleVertices(
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texUVs)
{
    std::vector<Vertex> vertices;
    vertices.reserve(positions.size());

    for (int i = 0; i < positions.size(); i++)
            vertices.push_back({
                        positions[i],normals[i],
                        glm::vec3(1.0f, 1.0f, 1.0f),
                        texUVs[i]});
        return vertices;
    }

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec) {
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size(); i+=2)
        vectors.emplace_back(glm::vec2(floatVec[i], floatVec[i+1]));
    return vectors;
}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) {
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i+=3)
        vectors.emplace_back(glm::vec3(floatVec[i], floatVec[i+1], floatVec[i+2]));
    return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) {
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i+=4)
        vectors.emplace_back(glm::vec4(floatVec[i], floatVec[i+1], floatVec[i+2], floatVec[i+3]));
    return vectors;
}

void Model::Input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        rotated_angle -= 0.2;
        if (rotated_angle <= -30)
            rotated_angle = -30;
    }
    else
        if (rotated_angle < 0)
            rotated_angle += 0.1;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rotated_angle += 0.2;
        if (rotated_angle >= 30)
            rotated_angle = 30;
    }
    else
        if (rotated_angle > 0)
            rotated_angle -= 0.1;
//    std::cout << rotated_angle << std::endl;
}


float Model::T(float x) {
    return -28 * x + 6970;
}

float calculate_F_z(float angle) {
    double coefficient = 125;
    return (float)coefficient * angle;
}

void Model::calculateMovement(float dt) {
    using namespace UtilConstants;

    float m_y = -0.5f * phi;

    dx_dt += 0.5f * v.x * dt;
    d_Vx_dt += (T(glm::length(v)) - (C_X * S * (float)RHO * powf(v.x, 2)) / 2) * cosf(phi) / (float)M * dt;

    dz_dt += v.z * dt;

    if (d_Vz_dt > 0.5)
        d_Vz_dt = 0.5;
    if (d_Vz_dt < -0.5)
        d_Vz_dt = - 0.5;

    d_Vz_dt += (T(glm::length(v)) - (C_X * S * (float)RHO * powf(v.z, 2)) / 2) * sinf(phi) / (float)M * dt;

    d_phi_dt += omega.y * dt;

    d_omega_w_dt += ((float)L * calculate_F_z(rotated_angle) + m_y) / (float)I_Y * dt;

    Position.x = dz_dt;
    Position.z = -dx_dt;

    v.x = d_Vx_dt;
    v.z = d_Vz_dt;

    if (d_phi_dt > 0.8)
        d_phi_dt = 0.8;

    if (d_phi_dt < -0.8)
        d_phi_dt = -0.8;
//
//    if (d_omega_w_dt > 0.7)
//        d_omega_w_dt = 0.7;
//    if (d_omega_w_dt < -0.7)
//        d_omega_w_dt = -0.7;

    phi = d_phi_dt;
    omega.y = d_omega_w_dt;

    // std::cout << "Скорость: " << v.x << " " << v.z << std::endl;
}