#include "Constants.h"

namespace UtilConstants {

    const int WIDTH = 800; // 800;
    const int HEIGHT = 800; //800;

    const std::string PARENT_DIRECTORY = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
    const std::string CUBEMAPS_PATH[6] =
            {
                    PARENT_DIRECTORY + "/Resources/skybox/right.jpg",
                    PARENT_DIRECTORY + "/Resources/skybox/left.jpg",
                    PARENT_DIRECTORY + "/Resources/skybox/top.jpg",
                    PARENT_DIRECTORY + "/Resources/skybox/bottom.jpg",
                    PARENT_DIRECTORY + "/Resources/skybox/front.jpg",
                    PARENT_DIRECTORY + "/Resources/skybox/back.jpg"
            };

    const std::string SHADER_DEFAULT_PATH[2] = {
            PARENT_DIRECTORY + "/src/verts/default.vert",
            PARENT_DIRECTORY + "/src/frags/default.frag"
    };

    const std::string SHADER_SKYBOX_PATH[2] = {
            PARENT_DIRECTORY + "/src/verts/skybox.vert",
            PARENT_DIRECTORY + "/src/frags/skybox.frag"
    };

    const std::string MODEL_PATH = "/Resources/models/airplane/scene.gltf";

    const int SAMPLES = 8;

    constinit int M = 6000;
    constinit float C_X = 0.6f;
    constinit float S = 10.6f;
    constinit int L = 2;
    constinit int I_Y = 57500;
    constinit int RHO = 1000;
}