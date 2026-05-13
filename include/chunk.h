#ifndef CHUNK_H
#define CHUNK_H

#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include "block.h"

const int CHUNK_SIZE = 16;

class Chunk {
public:
    Chunk(glm::ivec3 position);
    ~Chunk();

    void setBlock(int x, int y, int z, BlockType type);
    BlockType getBlock(int x, int y, int z) const;
    void updateMesh();
    void render();

    glm::ivec3 getPosition() const { return position; }

private:
    BlockType blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    glm::ivec3 position;
    GLuint vao, vbo;
    int vertexCount;

    void generateMesh();
};

#endif
