#include "Player.h"
#include <algorithm>
#include <random>

Player::Player(float x, float y, float size, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(size / 64.0f, size / 64.0f);
    sprite.setTextureRect(sf::IntRect(0, 0, SPRITESHEET_WIDTH / frameCount, SPRITESHEET_HEIGHT / ROW_COUNT));

    // Инициализация апгрейдов
    upgrades = {
        {"Increase Health", "Increases maximum health by 20", 
            [](Player& player) { player.maxHealth += 20; player.health += 20; }},
        {"Increase Speed", "Increases speed by 10%", 
            [](Player& player) { player.speed *= 1.1f; }},
        {"Increase Damage", "Increases damage by 10%", 
            [](Player& player) { player.damage *= 1.1f; }},
        {"Increase Attack Speed", "Decreases time between attacks by 50%", 
            [](Player& player) { player.attackCooldown *= 0.5f; }},
        {"Increase Health Regen", "Increases health regeneration by 10%", 
            [](Player& player) { player.healthRegen *= 1.1f; }},
        {"Increase Projectiles", "Adds one more projectile", 
            [](Player& player) { player.projectileCount += 1; }},
        {"Increase Projectile Size", "Increases projectile size by 20%", 
            [](Player& player) { player.projectileSize *= 1.2f; }},
        {"Increase Range", "Increases projectile range by 25%", 
            [](Player& player) { player.projectileLifetime *= 1.25f; }},
        {"Critical Hits", "10% chance to deal double damage", 
            [](Player& player) { 
                player.criticalHitChance += 0.1f;
                if (player.criticalHitChance > 0.5f) // Максимум 50% шанс
                    player.criticalHitChance = 0.5f;
            }},
        {"Piercing Shot", "Projectiles pierce through enemies", 
            [](Player& player) { 
                player.pierceCount += 1;
            }},
        {"Explosive Shot", "Increases chance and radius of explosive projectiles", 
            [](Player& player) { 
                player.explosionChance += 0.1f;
                if (player.explosionChance > 1.0f)
                    player.explosionChance = 1.0f;
                if (player.explosionRadius == 0.0f) 
                    player.explosionRadius = 50.0f;
                else 
                    player.explosionRadius *= 1.2f;
            }},
        {"Critical Power", "Increase critical hit damage by 50%", 
            [](Player& player) { 
                player.criticalHitMultiplier += 0.5f;
            }},
        {"Explosive Shots", "10% chance for explosive projectiles", 
            [](Player& player) { 
                player.explosionChance += 0.1f;
                if (player.explosionChance > 0.5f) // Maximum 50% chance
                    player.explosionChance = 0.5f;
            }}
    };

    // Генератор случайных чисел для апгрейдов
    randomEngine.seed(std::random_device{}());
}

void Player::handleInput() {
    velocity = sf::Vector2f(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += speed;
    }

    if (velocity.x != 0 || velocity.y != 0) {
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity.x /= length;
        velocity.y /= length;
        velocity.x *= speed;
        velocity.y *= speed;
    }
}

void Player::update(float dt, World& world) {
    if (isDead()) {
        return;
    }

    if (hasWon()) {
        return;
    }

    sf::Vector2f newPos = sprite.getPosition() + velocity * dt;

    sf::FloatRect bounds = sprite.getGlobalBounds();
    bounds.left = newPos.x;
    if (!world.checkCollision(bounds)) {
        sprite.setPosition(newPos.x, sprite.getPosition().y);
    } else {
        velocity.x = 0;
        takeDamage(0.1f);
    }

    bounds.left = sprite.getPosition().x;
    bounds.top = newPos.y;
    if (!world.checkCollision(bounds)) {
        sprite.setPosition(sprite.getPosition().x, newPos.y);
    } else {
        velocity.y = 0;
        takeDamage(0.1f);
    }

    health += healthRegen * dt;
    if (health > maxHealth) {
        health = maxHealth;
    }

    updateAnimation(dt);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    // UI теперь обрабатывается в UIManager
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::takeDamage(float amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
    }
    if (isDead()) {
        isDeathMenuVisible = true;
    }
}

void Player::gainExperience(float amount) {
    currentExperience += amount;
    if (currentExperience >= experienceForNextLevel) {
        levelUp();
    }
}

void Player::updateAnimation(float dt) {
    int row = 0;

    if (velocity.y < 0) {
        row = 1; // движение вверх
    } else if (velocity.y > 0) {
        row = 0; // движение вниз
    } else if (velocity.x < 0) {
        row = 2; // движение влево
    } else if (velocity.x > 0) {
        row = 3; // движение вправо
    }

    if (velocity != sf::Vector2f(0, 0)) {
        currentFrameTime += dt;
        if (currentFrameTime >= frameDuration) {
            currentFrameTime = 0.0f;
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
        }
    } else {
        currentFrame = 0;
    }

    sprite.setTextureRect(sf::IntRect(
        currentFrame * (SPRITESHEET_WIDTH / FRAME_COUNT),
        row * (SPRITESHEET_HEIGHT / ROW_COUNT),
        SPRITESHEET_WIDTH / FRAME_COUNT,
        SPRITESHEET_HEIGHT / ROW_COUNT
    ));
}

void Player::levelUp() {
    level++;
    currentExperience = 0;
    experienceForNextLevel *= 1.15f;
    isUpgradeMenuVisible = true;

    randomUpgrades.clear();
    std::sample(upgrades.begin(), upgrades.end(), std::back_inserter(randomUpgrades), 2, randomEngine);
}

void Player::applyUpgrade(const std::string& upgradeName) {
    for (const auto& upg : upgrades) {
        if (upg.name == upgradeName) {
            upg.apply(*this);
            break;
        }
    }
}

bool Player::isDead() const {
    return health <= 0;
}

bool Player::restartRequested() const {
    return restart;
}

void Player::resetRestartRequest() {
    restart = false;
}

void Player::winGame() {
    isWinMenuVisible = true;
}

bool Player::hasWon() const {
    return isWinMenuVisible;
}