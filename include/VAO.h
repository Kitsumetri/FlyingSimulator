#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    // ID reference for the Vertex Array Object
    GLuint ID{};
    VAO();

    // Links a VBO Attribute such as a position or color to the VAO
    static void LinkAttrib(VBO& VBO,
                    GLuint layout,
                    GLuint numComponents,
                    GLenum type,
                    GLsizeiptr stride,
                    void* offset);

    void Bind() const;
    static void Unbind();
    void Delete();
};
#endif