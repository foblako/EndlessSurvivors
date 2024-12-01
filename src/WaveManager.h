#pragma once
#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Runner.h"
#include "Jumper.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct Wave {
    float duration;                    // Длительность волны
    std::vector<std::string> enemies; // Типы врагов в волне
    float spawnInterval;              // Интервал спавна
    int enemiesPerSpawn;             // Количество врагов за раз
    float enemyHealthMultiplier;      // Множитель здоровья врагов
    float enemyDamageMultiplier;      // Множитель урона врагов
};

class EnemyManager {
public:
    EnemyManager(TextureManager& textureManager, Player& player, sf::RenderWindow& window);
    void update(float dt, const sf::Vector2f& playerPos);
    void draw();
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;

private:
    void initializeWaves();
    void updateWave(float dt);
    void spawnEnemies(const sf::Vector2f& playerPos);
    std::unique_ptr<Enemy> createEnemy(const std::string& type, const sf::Vector2f& position);
    sf::Vector2f getSpawnPosition(const sf::Vector2f& playerPos);

    TextureManager& textureManager;
    Player& player;
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Wave> waves;
    
    // Состояние волн
    int currentWave = 0;
    float waveTimer = 0.0f;
    float spawnTimer = 0.0f;
    
    // Параметры спавна
    const float spawnRadiusMin = 500.0f;
    const float spawnRadiusMax = 800.0f;
    
    // Прогрессия сложности
    float difficultyMultiplier = 1.0f;
    const float difficultyIncreaseRate = 0.1f;
};