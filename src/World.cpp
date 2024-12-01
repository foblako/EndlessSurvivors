#include "World.h"
#include <random>

// Реализация методов класса World

World::World(int chunkSize, int blockSize, int viewRadius,
             const sf::Texture& grass_texture, const sf::Texture& tree_texture,
             const sf::Texture& pickable_texture, const sf::Texture& gregStarTexture,
             const sf::Texture& cactus_texture)
    : chunkSize(chunkSize), blockSize(blockSize), viewRadius(viewRadius),
      grassTexture(grass_texture), treeTexture(tree_texture),
      pickableTexture(pickable_texture), gregStarTexture(gregStarTexture),
      cactusTexture(cactus_texture)
{
    // Инициализация генераторов шума случайными сидами
    std::random_device rd;
    biomeNoise.reseed(rd());
    detailNoise.reseed(rd());
}

void World::update(const sf::Vector2i& playerChunkPos, Player& player, sf::RenderWindow& window) {
    // Удаление чанков, вышедших за пределы радиуса обзора
    for (auto it = chunks.begin(); it != chunks.end();) {
        if (std::abs(it->first.x - playerChunkPos.x) > viewRadius ||
            std::abs(it->first.y - playerChunkPos.y) > viewRadius) {
            it = chunks.erase(it);
        }
        else {
            ++it;
        }
    }

    // Генерация новых чанков в радиусе обзора
    for (int dx = -viewRadius; dx <= viewRadius; ++dx) {
        for (int dy = -viewRadius; dy <= viewRadius; ++dy) {
            sf::Vector2i chunkPos = playerChunkPos + sf::Vector2i(dx, dy);
            if (chunks.find(chunkPos) == chunks.end()) {
                generateChunk(chunkPos);
                generatePickablesInChunk(chunkPos);
                generateGregStarInChunk(chunkPos);
            }
        }
    }

    // Обновление подбираемых предметов
    updatePickables(player, window);
}

void World::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
    // Установка вида камеры
    sf::View view = window.getView();
    view.setCenter(playerPos);
    window.setView(view);

    // Отрисовка чанков
    for (const auto& pair : chunks) {
        const Chunk& chunk = pair.second;
        const_cast<Chunk&>(chunk).draw(window);
    }

    // Отрисовка подбираемых предметов
    for (auto& pickable : pickables) {
        pickable.draw(window);
    }
}

void World::generateChunk(const sf::Vector2i& chunkPos) {
    std::vector<Block> blocks;
    blocks.reserve(chunkSize * chunkSize);

    // Генерация блоков в чанке
    for (int x = 0; x < chunkSize; ++x) {
        for (int y = 0; y < chunkSize; ++y) {
            float worldX = (chunkPos.x * chunkSize + x) * blockSize;
            float worldY = (chunkPos.y * chunkSize + y) * blockSize;

            // Получаем значение биома для каждого блока
            float biomeValue = getBiomeValue(worldX, worldY);
            Biome biome = getBiomeAt(worldX, worldY);

            // Получаем цвет земли для текущего биома
            sf::Color groundColor = getGroundColor(biomeValue);

            // Создаем блок земли с соответствующим цветом
            Block groundBlock(worldX, worldY, blockSize, grassTexture, true);
            groundBlock.setColor(groundColor);
            blocks.push_back(groundBlock);

            // Получаем значение для деталей (деревья, кактусы)
            float detailValue = detailNoise.noise2D_01(worldX * DETAIL_SCALE, worldY * DETAIL_SCALE);

            // Спавним объекты в зависимости от биома
            if (shouldSpawnObject(biome, detailValue)) {
                if (biome == Biome::Forest) {
                    blocks.emplace_back(worldX, worldY, blockSize, treeTexture, false);
                } else if (biome == Biome::Desert) {
                    blocks.emplace_back(worldX, worldY, blockSize, cactusTexture, false);
                }
            }
        }
    }

    // Добавляем сгенерированный чанк в карту
    chunks.emplace(chunkPos, Chunk());
    chunks[chunkPos].setBlocks(std::move(blocks));
}

// Определение биома на основе значения шума
Biome World::getBiomeAt(float x, float y) const {
    float biomeValue = biomeNoise.noise2D_01(x * BIOME_SCALE, y * BIOME_SCALE);
    return (biomeValue < 0.5f) ? Biome::Forest : Biome::Desert;
}

// Решаем, нужно ли спавнить объект в текущем биоме
bool World::shouldSpawnObject(Biome biome, float detailValue) const {
    if (biome == Biome::Forest) {
        return detailValue > 0.75f; // Шанс спавна деревьев
    } else if (biome == Biome::Desert) {
        return detailValue > 0.85f; // Шанс спавна кактусов
    }
    return false;
}

// Получаем значение биома для текущей позиции
float World::getBiomeValue(float x, float y) const {
    return biomeNoise.noise2D_01(x * BIOME_SCALE, y * BIOME_SCALE);
}

// Определяем цвет земли с учётом переходов между биомами
sf::Color World::getGroundColor(float biomeValue) const {
    if (biomeValue < 0.5f - TRANSITION_WIDTH / 2) {
        return FOREST_COLOR;
    } else if (biomeValue > 0.5f + TRANSITION_WIDTH / 2) {
        return DESERT_COLOR;
    } else {
        // Интерполяция цветов в зоне перехода
        float t = (biomeValue - (0.5f - TRANSITION_WIDTH / 2)) / TRANSITION_WIDTH;
        return sf::Color(
            static_cast<sf::Uint8>(FOREST_COLOR.r + (DESERT_COLOR.r - FOREST_COLOR.r) * t),
            static_cast<sf::Uint8>(FOREST_COLOR.g + (DESERT_COLOR.g - FOREST_COLOR.g) * t),
            static_cast<sf::Uint8>(FOREST_COLOR.b + (DESERT_COLOR.b - FOREST_COLOR.b) * t)
        );
    }
}

void World::generatePickablesInChunk(const sf::Vector2i& chunkPos) {
    Chunk& chunk = chunks[chunkPos];
    for (const Block& block : chunk.getBlocks()) {
        if (static_cast<float>(std::rand()) / RAND_MAX < POTION_IN_CHUNK_CHANCE) {
            sf::FloatRect bounds = block.getBounds();
            pickables.emplace_back(bounds.left, bounds.top, 32, pickableTexture);
        }
    }
}


void World::generateGregStarInChunk(const sf::Vector2i& chunkPos) {
    Chunk& chunk = chunks[chunkPos];
    for (const Block& block : chunk.getBlocks()) {
        if (static_cast<float>(std::rand()) / RAND_MAX < GREG_STAR_SPAWN_CHANCE) {
            sf::FloatRect bounds = block.getBounds();
            pickables.emplace_back(bounds.left, bounds.top, 128, gregStarTexture);
        }
    }
}

bool World::checkCollision(const sf::FloatRect& bounds) const {
    const float margin = 20.0f; // Отступ для корректной проверки столкновений
    for (const auto& pair : chunks) {
        const Chunk& chunk = pair.second;
        for (const Block& block : chunk.getBlocks()) {
            if (!block.isWalkable()) {
                sf::FloatRect blockBounds = block.getBounds();
                blockBounds.left += margin;
                blockBounds.top += margin;
                blockBounds.width -= 2 * margin;
                blockBounds.height -= 2 * margin;
                if (blockBounds.intersects(bounds)) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<Pickable>& World::getPickables() {
    return pickables;
}

void World::updatePickables(Player& player, sf::RenderWindow& window) {
    for (auto it = pickables.begin(); it != pickables.end();) {
        if (it->getBounds().intersects(player.getBounds())) {
            if (it->getTexture() == &gregStarTexture) {
                // Победа игрока при сборе звезды Грега
                player.winGame();
            }
            else {
                player.gainExperience(EXPIRIENCE_AMOUNT); // Игрок получает опыт
            }
            it = pickables.erase(it);
        }
        else {
            ++it;
        }
    }
}
