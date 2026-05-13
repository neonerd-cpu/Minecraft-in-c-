#ifndef SELECTION_BOX_H
#define SELECTION_BOX_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include "shader.h"

class SelectionBox {
public:
    SelectionBox();
    ~SelectionBox();

    void render(const Shader& shader, glm::ivec3 position);

private:
    GLuint vao, vbo, ebo;
};

#endif
