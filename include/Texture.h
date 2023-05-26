#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include "stb_image.h"

#include "Shader.h"

class Texture
{
public:
    GLuint ID{};
    const char* type;
    GLuint unit;

    Texture(const char* image, const char* texType, GLuint slot);

    // Assigns a texture unit to a texture
    static void texUnit(Shader& shader, const char* uniform, GLuint unit);

    void Bind() const;
    [[maybe_unused]] static void Unbind();
    void Delete();
};
#endif