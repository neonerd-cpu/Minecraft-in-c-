#include "selection_box.h"
#include <glm/gtc/matrix_transform.hpp>

SelectionBox::SelectionBox() {
    float vertices[] = {
        -0.01f, -0.01f, -0.01f,  1.01f, -0.01f, -0.01f,
         1.01f,  1.01f, -0.01f, -0.01f,  1.01f, -0.01f,
        -0.01f, -0.01f,  1.01f,  1.01f, -0.01f,  1.01f,
         1.01f,  1.01f,  1.01f, -0.01f,  1.01f,  1.01f
    };

    unsigned int indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

SelectionBox::~SelectionBox() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void SelectionBox::render(const Shader& shader, glm::ivec3 position) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position));
    shader.setMat4("model", model);
    shader.setBool("useUniformColor", true);
    shader.setVec3("uniformColor", glm::vec3(0.0f, 0.0f, 0.0f)); // Black outline
    
    glBindVertexArray(vao);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    shader.setBool("useUniformColor", false);
}
