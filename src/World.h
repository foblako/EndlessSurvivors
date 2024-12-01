#pragma once

// Заголовочный файл класса World, представляющего игровой мир.

#include <unordered_map>
#include <vector>
#include <ctime>
#include <cstdlib>

// Подключение SFML
#include <SFML/Graphics.hpp>

// Подключение собственных файлов
#include "Chunk.h"
#include "Const.h"
#include "Pickable.h"
#include "Player.h"
#include "TextureManager.h"
#include "PerlinNoise.hpp"

// Специализация std::hash для sf::Vector2i
namespace std {
    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const noexcept {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };
}

// Перечисление биомов
enum class Biome {
    Forest, // Лес
    Desert  // Пустыня
};

class Player;

// Класс World отвечает за создание и управление игровым миром
class World {
public:
    // Конструктор
    World(int chunkSize, int blockSize, int viewRadius,
          const sf::Texture& grass_texture, const sf::Texture& tree_texture,
          const sf::Texture& pickable_texture, const sf::Texture& gregStarTexture,
          const sf::Texture& cactus_texture);

    // Обновление мира
    void update(const sf::Vector2i& playerChunkPos, Player& player, sf::RenderWindow& window);

    // Отрисовка мира
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos);

    // Проверка столкновений
    bool checkCollision(const sf::FloatRect& bounds) const;

    // Получение списка подбираемых предметов
    std::vector<Pickable>& getPickables();

private:
    // Генераторы шума для биомов и деталей
    siv::PerlinNoise biomeNoise;
    siv::PerlinNoise detailNoise;

    // Константы для генерации мира
    const float BIOME_SCALE = 0.02f;
    const float DETAIL_SCALE = 0.05f;
    const float TRANSITION_WIDTH = 0.2f; // Ширина зоны перехода между биомами

    // Цвета для биомов
    const sf::Color FOREST_COLOR = sf::Color(76, 153, 0);     // Зеленый цвет для леса
    const sf::Color DESERT_COLOR = sf::Color(255, 220, 150);  // Песочный цвет для пустыни

    // Методы для определения биомов и спавна объектов
    Biome getBiomeAt(float x, float y) const;
    bool shouldSpawnObject(Biome biome, float detailValue) const;
    sf::Color getGroundColor(float biomeValue) const;
    float getBiomeValue(float x, float y) const;

    // Параметры мира
    int chunkSize;    // Размер чанка
    int blockSize;    // Размер блока
    int viewRadius;   // Радиус обзора

    // Ссылки на текстуры
    const sf::Texture& grassTexture;
    const sf::Texture& treeTexture;
    const sf::Texture& cactusTexture;
    const sf::Texture& pickableTexture;
    const sf::Texture& gregStarTexture;

    // Карта чанков и список подбираемых предметов
    std::unordered_map<sf::Vector2i, Chunk, std::hash<sf::Vector2i>> chunks;
    std::vector<Pickable> pickables;

    // Приватные методы для генерации и обновления
    void generateChunk(const sf::Vector2i& chunkPos);
    void generatePickablesInChunk(const sf::Vector2i& chunkPos);
    void generateGregStarInChunk(const sf::Vector2i& chunkPos);
    void updatePickables(Player& player, sf::RenderWindow& window);
};
