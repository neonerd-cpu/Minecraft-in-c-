#include "chunk.h"
#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

Chunk::Chunk(glm::ivec3 position) : position(position), vao(0), vbo(0), vertexCount(0) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                blocks[x][y][z] = BlockType::Air;
            }
        }
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

Chunk::~Chunk() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
        blocks[x][y][z] = type;
    }
}

BlockType Chunk::getBlock(int x, int y, int z) const {
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
        return blocks[x][y][z];
    }
    return BlockType::Air;
}

void Chunk::updateMesh() {
    generateMesh();
}

void Chunk::generateMesh() {
    std::vector<Vertex> vertices;

    auto addFace = [&](glm::vec3 p, int face, glm::vec3 c) {
        glm::vec3 v[4];
        switch (face) {
            case 0: // -X
                v[0] = {0, 0, 0}; v[1] = {0, 0, 1}; v[2] = {0, 1, 1}; v[3] = {0, 1, 0};
                break;
            case 1: // +X
                v[0] = {1, 0, 1}; v[1] = {1, 0, 0}; v[2] = {1, 1, 0}; v[3] = {1, 1, 1};
                break;
            case 2: // -Y
                v[0] = {0, 0, 0}; v[1] = {1, 0, 0}; v[2] = {1, 0, 1}; v[3] = {0, 0, 1};
                break;
            case 3: // +Y
                v[0] = {0, 1, 1}; v[1] = {1, 1, 1}; v[2] = {1, 1, 0}; v[3] = {0, 1, 0};
                break;
            case 4: // -Z
                v[0] = {1, 0, 0}; v[1] = {0, 0, 0}; v[2] = {0, 1, 0}; v[3] = {1, 1, 0};
                break;
            case 5: // +Z
                v[0] = {0, 0, 1}; v[1] = {1, 0, 1}; v[2] = {1, 1, 1}; v[3] = {1, 1, 0}; // Wait, Z+ winding
                v[0] = {0, 0, 1}; v[1] = {1, 0, 1}; v[2] = {1, 1, 1}; v[3] = {0, 1, 1};
                break;
        }
        for (int i = 0; i < 4; i++) v[i] += p;

        vertices.push_back({v[0], c}); vertices.push_back({v[1], c}); vertices.push_back({v[2], c});
        vertices.push_back({v[0], c}); vertices.push_back({v[2], c}); vertices.push_back({v[3], c});
    };

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType type = blocks[x][y][z];
                if (type == BlockType::Air) continue;

                BlockData data = getBlockData(type);
                glm::vec3 p(x, y, z);
                glm::vec3 c = data.color;

                if (x == 0 || blocks[x-1][y][z] == BlockType::Air) addFace(p, 0, c);
                if (x == CHUNK_SIZE-1 || blocks[x+1][y][z] == BlockType::Air) addFace(p, 1, c);
                if (y == 0 || blocks[x][y-1][z] == BlockType::Air) addFace(p, 2, c);
                if (y == CHUNK_SIZE-1 || blocks[x][y+1][z] == BlockType::Air) addFace(p, 3, c);
                if (z == 0 || blocks[x][y][z-1] == BlockType::Air) addFace(p, 4, c);
                if (z == CHUNK_SIZE-1 || blocks[x][y][z+1] == BlockType::Air) addFace(p, 5, c);
            }
        }
    }

    vertexCount = vertices.size();
    if (vertexCount == 0) return;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
}

void Chunk::render() {
    if (vertexCount == 0) return;
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
