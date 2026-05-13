#include "world.h"

World::World() {
    // Generate a simple flat world with different blocks
    for (int cx = -2; cx <= 2; cx++) {
        for (int cz = -2; cz <= 2; cz++) {
            Chunk* chunk = new Chunk({cx, 0, cz});
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    chunk->setBlock(x, 0, z, BlockType::Stone);
                    chunk->setBlock(x, 1, z, BlockType::Dirt);
                    chunk->setBlock(x, 2, z, BlockType::Grass);
                }
            }
            // Add some "trees" (wood and leaves)
            if (cx == 0 && cz == 0) {
                chunk->setBlock(8, 3, 8, BlockType::Wood);
                chunk->setBlock(8, 4, 8, BlockType::Wood);
                chunk->setBlock(8, 5, 8, BlockType::Wood);
                for(int lx = 6; lx <= 10; lx++) {
                    for(int lz = 6; lz <= 10; lz++) {
                        chunk->setBlock(lx, 6, lz, BlockType::Leaf);
                        chunk->setBlock(lx, 7, lz, BlockType::Leaf);
                    }
                }
            }
            chunk->updateMesh();
            chunks[{cx, 0, cz}] = chunk;
        }
    }
}

World::~World() {
    for (auto const& [pos, chunk] : chunks) {
        delete chunk;
    }
}

void World::render(Shader& shader) {
    for (auto const& [pos, chunk] : chunks) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x * CHUNK_SIZE, pos.y * CHUNK_SIZE, pos.z * CHUNK_SIZE));
        shader.setMat4("model", model);
        chunk->render();
    }
}

void World::setBlock(int x, int y, int z, BlockType type) {
    glm::ivec3 chunkPos(std::floor((float)x / CHUNK_SIZE), std::floor((float)y / CHUNK_SIZE), std::floor((float)z / CHUNK_SIZE));
    if (chunks.count(chunkPos)) {
        int bx = x - chunkPos.x * CHUNK_SIZE;
        int by = y - chunkPos.y * CHUNK_SIZE;
        int bz = z - chunkPos.z * CHUNK_SIZE;
        chunks[chunkPos]->setBlock(bx, by, bz, type);
        chunks[chunkPos]->updateMesh();
    } else if (type != BlockType::Air) {
        // Create chunk if it doesn't exist and we are placing a block
        Chunk* chunk = new Chunk(chunkPos);
        int bx = x - chunkPos.x * CHUNK_SIZE;
        int by = y - chunkPos.y * CHUNK_SIZE;
        int bz = z - chunkPos.z * CHUNK_SIZE;
        chunk->setBlock(bx, by, bz, type);
        chunk->updateMesh();
        chunks[chunkPos] = chunk;
    }
}

BlockType World::getBlock(int x, int y, int z) const {
    glm::ivec3 chunkPos(std::floor((float)x / CHUNK_SIZE), std::floor((float)y / CHUNK_SIZE), std::floor((float)z / CHUNK_SIZE));
    if (chunks.count(chunkPos)) {
        int bx = x - chunkPos.x * CHUNK_SIZE;
        int by = y - chunkPos.y * CHUNK_SIZE;
        int bz = z - chunkPos.z * CHUNK_SIZE;
        return chunks.at(chunkPos)->getBlock(bx, by, bz);
    }
    return BlockType::Air;
}
