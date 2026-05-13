#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "chunk.h"
#include "shader.h"

struct ivec3_compare {
    bool operator()(const glm::ivec3& a, const glm::ivec3& b) const {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

class World {
public:
    World();
    ~World();

    void render(Shader& shader);
    void setBlock(int x, int y, int z, BlockType type);
    BlockType getBlock(int x, int y, int z) const;

private:
    std::map<glm::ivec3, Chunk*, ivec3_compare> chunks;
};

#endif
