#include "VAO.h"

// Constructor that generates a VAO ID
VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO& VBO,
                     GLuint layout,
                     GLuint numComponents,
                     GLenum type,
                     GLsizeiptr stride,
                     void* offset)
{
    VBO.Bind();
    glVertexAttribPointer(layout,
                          (int)numComponents,
                          type,
                          GL_FALSE,
                          (int)stride,
                          offset);

    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

void VAO::Bind() const {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}