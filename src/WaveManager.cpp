#include "WaveManager.h"
#include <cmath>
#include <random>

EnemyManager::EnemyManager(TextureManager& textureManager, Player& player, sf::RenderWindow& window)
    : textureManager(textureManager), player(player), window(window) {
    initializeWaves();
}

void EnemyManager::initializeWaves() {
    // Начальные волны
    waves = {
        // Волна 1: Только раннеры
        {30.0f, {"Runner"}, 2.0f, 10, 1.0f, 1.0f},
        
        // Волна 2: Раннеры + джамперы
        {30.0f, {"Runner", "Jumper"}, 1.5f, 15, 1.2f, 1.1f},
        
        // Волна 3: Все типы врагов
        {30.0f, {"Runner", "Jumper", "Boss"}, 1.2f, 10, 1.4f, 1.2f},
        
        // Волна 4: Усиленные враги
        {30.0f, {"Runner", "Jumper", "Boss"}, 1.0f, 6, 1.6f, 1.3f},
        
        // Волна 5: Босс-волна
        {60.0f, {"Boss"}, 3.0f, 3, 2.0f, 1.5f}
    };
}

void EnemyManager::update(float dt, const sf::Vector2f& playerPos) {
    updateWave(dt);
    
    // Обновление существующих врагов
    for (auto& enemy : enemies) {
        enemy->update(dt, playerPos, enemies, player, window);
    }
    
    // Удаление мертвых врагов
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) { return enemy->isDead(); }),
        enemies.end());
}

void EnemyManager::updateWave(float dt) {
    waveTimer += dt;
    spawnTimer += dt;
    
    // Проверка окончания текущей волны
    if (waveTimer >= waves[currentWave].duration) {
        currentWave++;
        waveTimer = 0.0f;
        difficultyMultiplier += difficultyIncreaseRate;
        
        // Зацикливание волн с увеличением сложности
        if (currentWave >= waves.size()) {
            currentWave = 0;
        }
    }
    
    // Спавн врагов
    if (spawnTimer >= waves[currentWave].spawnInterval) {
        spawnTimer = 0.0f;
        spawnEnemies(player.getPosition());
    }
}

void EnemyManager::spawnEnemies(const sf::Vector2f& playerPos) {
    const auto& wave = waves[currentWave];
    
    for (int i = 0; i < wave.enemiesPerSpawn; ++i) {
        // Выбор случайного типа врага из доступных в волне
        const std::string& enemyType = wave.enemies[rand() % wave.enemies.size()];
        sf::Vector2f spawnPos = getSpawnPosition(playerPos);
        
        auto enemy = createEnemy(enemyType, spawnPos);
        if (enemy) {
            enemies.push_back(std::move(enemy));
        }
    }
}

std::unique_ptr<Enemy> EnemyManager::createEnemy(const std::string& type, const sf::Vector2f& position) {
    if (type == "Runner") {
        return std::make_unique<Runner>(
            position.x, position.y,
            textureManager.getTexture("runnerWalk"),
            textureManager.getTexture("runnerAttack"),
            textureManager.getTexture("runnerDeath"),
            player
        );
    }
    else if (type == "Jumper") {
        return std::make_unique<Jumper>(
            position.x, position.y,
            textureManager.getTexture("jumperWalk"),
            textureManager.getTexture("jumperAttack"),
            textureManager.getTexture("jumperDeath"),
            textureManager.getTexture("jumperJump"),
            player
        );
    }
    else if (type == "Boss") {
        return std::make_unique<Boss>(
            position.x, position.y,
            textureManager.getTexture("bossWalk"),
            textureManager.getTexture("bossAttack"),
            textureManager.getTexture("bossDeath"),
            player
        );
    }
    return nullptr;
}

sf::Vector2f EnemyManager::getSpawnPosition(const sf::Vector2f& playerPos) {
    float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f;
    float distance = spawnRadiusMin + 
                    static_cast<float>(rand()) / RAND_MAX * (spawnRadiusMax - spawnRadiusMin);
    
    return sf::Vector2f(
        playerPos.x + distance * std::cos(angle),
        playerPos.y + distance * std::sin(angle)
    );
}

void EnemyManager::draw() {
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies() const {
    return enemies;
}