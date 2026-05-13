#include "ui.h"
#include <glm/gtc/matrix_transform.hpp>

UI::UI(int width, int height) : width(width), height(height) {
    uiShader = new Shader("shaders/ui_vertex.glsl", "shaders/ui_fragment.glsl");
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

UI::~UI() {
    delete uiShader;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void UI::updateSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void UI::drawRect(float x, float y, float w, float h, glm::vec3 color) {
    float vertices[] = {
        x,   y,   color.r, color.g, color.b,
        x+w, y,   color.r, color.g, color.b,
        x+w, y+h, color.r, color.g, color.b,
        x,   y,   color.r, color.g, color.b,
        x+w, y+h, color.r, color.g, color.b,
        x,   y+h, color.r, color.g, color.b
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void UI::render(const Inventory& inventory, float health, bool showInventory) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    uiShader->use();
    // Ensure the projection is correctly mapping screen coordinates
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    uiShader->setMat4("projection", projection);

    // 1. Crosshair
    float chSize = 20.0f;
    float chThickness = 2.0f;
    drawRect(width/2.0f - chSize/2.0f, height/2.0f - chThickness/2.0f, chSize, chThickness, {1,1,1});
    drawRect(width/2.0f - chThickness/2.0f, height/2.0f - chSize/2.0f, chThickness, chSize, {1,1,1});

    // 2. Health bar
    float hbWidth = 400.0f;
    float hbHeight = 40.0f;
    float hbX = (width - hbWidth) / 2.0f;
    float hbY = height - hbHeight - 20.0f;

    float healthBarWidth = 200.0f;
    float healthBarHeight = 15.0f;
    drawRect(hbX, hbY - 25.0f, healthBarWidth, healthBarHeight, {0.2f, 0, 0});
    drawRect(hbX, hbY - 25.0f, healthBarWidth * (health / 100.0f), healthBarHeight, {1.0f, 0, 0});

    // 3. Hotbar
    drawRect(hbX, hbY, hbWidth, hbHeight, {0.3f, 0.3f, 0.3f});

    std::vector<BlockType> types = {BlockType::Grass, BlockType::Dirt, BlockType::Stone, BlockType::Wood, BlockType::Leaf};
    float slotSize = hbWidth / types.size();
    
    for (size_t i = 0; i < types.size(); i++) {
        BlockData data = getBlockData(types[i]);
        float x = hbX + i * slotSize + 5;
        float y = hbY + 5;
        float s = slotSize - 10;

        if (inventory.getSelectedType() == types[i]) {
            drawRect(x-3, y-3, s+6, s+6, {1,1,0}); // Yellow highlight
        }
        drawRect(x, y, s, s, data.color);
        
        int count = inventory.getCount(types[i]);
        // Draw little squares for count
        for (int c = 0; c < std::min(count, 5); c++) {
            drawRect(x + c*8, y + s - 8, 6, 6, {1, 1, 1});
        }
    }

    // 4. Inventory
    if (showInventory) {
        float invW = 600.0f;
        float invH = 400.0f;
        float invX = (width - invW) / 2.0f;
        float invY = (height - invH) / 2.0f;
        drawRect(invX, invY, invW, invH, {0.1f, 0.1f, 0.1f});
        
        for(int i=0; i<5; i++) {
            BlockData data = getBlockData(types[i]);
            drawRect(invX + 50 + i*100, invY + 50, 80, 80, data.color);
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}
