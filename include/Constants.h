#ifndef FLYINGSIMULATOR_CONSTANTS_H
#define FLYINGSIMULATOR_CONSTANTS_H

#include <string>
#include <filesystem>

namespace UtilConstants {
    extern const int WIDTH;
    extern const int HEIGHT;

    extern const std::string PARENT_DIRECTORY;
    extern const std::string CUBEMAPS_PATH[6];
    extern const std::string SHADER_DEFAULT_PATH[2];
    extern const std::string SHADER_SKYBOX_PATH[2];
    extern const std::string MODEL_PATH;

    extern  const int SAMPLES;
}
#endif //FLYINGSIMULATOR_CONSTANTS_H