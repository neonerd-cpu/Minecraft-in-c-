#ifndef UI_H
#define UI_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"
#include "inventory.h"

class UI {
public:
    UI(int width, int height);
    ~UI();

    void render(const Inventory& inventory, float health, bool showInventory);
    void updateSize(int width, int height);

private:
    int width, height;
    Shader* uiShader;
    GLuint vao, vbo;

    void drawRect(float x, float y, float w, float h, glm::vec3 color);
};

#endif
