#pragma once

#include <vector>
#include "Block.h"

// Класс Chunk представляет собой участок мира, состоящий из блоков
class Chunk {
public:
    Chunk() = default;
    Chunk(int chunkX, int chunkY, int blockSize, int chunkSize,
          const sf::Texture& grass_texture, const sf::Texture& tree_texture);

    // Отрисовка чанка
    void draw(sf::RenderWindow& window);

    // Получение списка блоков
    const std::vector<Block>& getBlocks() const;

    // Установка новых блоков
    void setBlocks(std::vector<Block>&& newBlocks) {
        blocks = std::move(newBlocks);
    }

private:
    std::vector<Block> blocks; // Список блоков в чанке
};


