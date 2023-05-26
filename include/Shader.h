#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string get_file_contents(const char* filename);

class Shader
{
public:
    // Reference ID of the Shader Program
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile);
    Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

    void Activate() const;
    void Delete() const;
private:
    static void compileErrors(unsigned int shader, const char* type);
};
#endif