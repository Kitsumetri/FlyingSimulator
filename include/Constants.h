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

    extern const int SAMPLES;

    extern constinit int M;
    extern constinit float C_X;
    extern constinit float S;
    extern constinit int L;
    extern constinit int I_Y;
    extern constinit int RHO;
}
#endif //FLYINGSIMULATOR_CONSTANTS_H