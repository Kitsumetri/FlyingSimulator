#include "Constants.h"

namespace UtilConstants {

    const int WIDTH = 800; // 2560;
    const int HEIGHT = 800; //1600;

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
}