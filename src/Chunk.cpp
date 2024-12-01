#include "Chunk.h"

// Реализация методов класса Chunk

Chunk::Chunk(int chunkX, int chunkY, int blockSize, int chunkSize,
             const sf::Texture& grass_texture, const sf::Texture& tree_texture) {
    // Генерация блоков в чанке
    for (int x = 0; x < chunkSize; ++x) {
        for (int y = 0; y < chunkSize; ++y) {
            float posX = chunkX * chunkSize * blockSize + x * blockSize;
            float posY = chunkY * chunkSize * blockSize + y * blockSize;
            if (static_cast<float>(std::rand()) / RAND_MAX < TREE_SPAWN_CHANCE) {
                blocks.emplace_back(posX, posY, blockSize, tree_texture, false);
            }
            else {
                blocks.emplace_back(posX, posY, blockSize, grass_texture, true);
            }
        }
    }
}

void Chunk::draw(sf::RenderWindow& window) {
    // Отрисовка всех блоков в чанке
    for (Block& block : blocks) {
        block.draw(window);
    }
}

const std::vector<Block>& Chunk::getBlocks() const {
    return blocks;
}
