#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <string>

enum class BlockType {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Wood,
    Leaf,
    Count
};

struct BlockData {
    std::string name;
    float hardness;
    glm::vec3 color;
};

inline BlockData getBlockData(BlockType type) {
    switch (type) {
        case BlockType::Grass: return {"Grass", 0.6f, {0.1f, 0.8f, 0.1f}};
        case BlockType::Dirt:  return {"Dirt", 0.5f, {0.5f, 0.35f, 0.05f}};
        case BlockType::Stone: return {"Stone", 1.5f, {0.5f, 0.5f, 0.5f}};
        case BlockType::Wood:  return {"Wood", 2.0f, {0.4f, 0.2f, 0.0f}};
        case BlockType::Leaf:  return {"Leaf", 0.2f, {0.0f, 0.5f, 0.0f}};
        default:               return {"Air", 0.0f, {0.0f, 0.0f, 0.0f}};
    }
}

#endif
