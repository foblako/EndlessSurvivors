#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <vector>
#include "Const.h"
#include "World.h"
#include <cmath>
#include <random>

// Forward declarations
class World;

class Player {
public:
    // Constructors and main methods
    Player(float x, float y, float size, const sf::Texture& texture);
    void handleInput();
    void update(float dt, World& world);
    void draw(sf::RenderWindow& window);

    // Getters
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    bool isDead() const;
    bool hasWon() const;
    bool restartRequested() const;

    // Getters for UIManager
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    float getCurrentExperience() const { return currentExperience; }
    float getExperienceForNextLevel() const { return experienceForNextLevel; }
    int getLevel() const { return level; }
    float getSpeed() const { return speed; }
    float getDamage() const { return damage; }
    // Add more getters as needed

    // State flags for menus
    bool isUpgradeMenuVisible = false;
    bool isDeathMenuVisible = false;
    bool isWinMenuVisible = false;

    // Game state methods
    void resetRestartRequest();
    void winGame();
    void showDeathMenu(sf::RenderWindow& window);
    void showWinMenu(sf::RenderWindow& window);
    
    // Combat methods
    void takeDamage(float amount);
    void gainExperience(float amount);

    // Combat stats - public for ProjectileManager access
    float damage = 10.0f;
    float attackCooldown = ATTACK_SPEED;
    int projectileCount = 1;
    
    // New projectile parameters
    float projectileSize = 5.0f;
    float projectileLifetime = PROJECTILE_LIFETIME;
    float projectileStretch = 1.0f;
    float criticalHitChance = 0.0f;     // Шанс крита
    float criticalHitMultiplier = 2.0f;  // Множитель крита
    int pierceCount = 1;                // Сквозное попадание
    float explosionRadius = 0.0f;       // Радиус взрыва при попадании
    float explosionChance = 0.0f;       // Шанс взрыва

    // Upgrade system
    struct Upgrade {
        std::string name;
        std::string description;
        std::function<void(Player&)> apply;
    };

    std::vector<Upgrade> randomUpgrades;
    void applyUpgrade(const std::string& upgradeName);

private:
    // Movement
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = PLAYER_SPEED;

    // Animation
    const int frameCount = 5;
    int currentFrame = 0;
    const float frameDuration = 0.1f;
    float currentFrameTime = 0.0f;

    // Stats
    float health = 100.0f;
    float maxHealth = 100.0f;
    float healthRegen = PLAYER_HEALTH_REGEN;
    float timeSinceLastAttack = 0.0f;


    // Experience system
    int level = 1;
    float currentExperience = 0.0f;
    float experienceForNextLevel = 100.0f;

    std::vector<Upgrade> upgrades;
    std::vector<sf::Text> upgradeTexts;

    // State flags
    bool restart = false;

    // Private methods
    void updateAnimation(float dt);
    void levelUp();

    // Add random number generator
    std::mt19937 randomEngine;

    // Add constants for animation
    static const int FRAME_COUNT = FRAME_COUNT;
    static const int ROW_COUNT = ROW_COUNT;
    static const int SPRITESHEET_WIDTH = SPRITESHEET_WIDTH; // replace with actual value
    static const int SPRITESHEET_HEIGHT = SPRITESHEET_HEIGHT; // replace with actual value
};