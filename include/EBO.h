#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
    // ID reference of Element Buffer Object
    GLuint ID{};
    // Constructor that generates an Element Buffer Object and links it to indices
    explicit EBO(std::vector<GLuint>& indices);

    void Bind() const;
    static void Unbind();
    void Delete();
};

#endif