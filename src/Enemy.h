#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"

// Предварительное объявление класса Player
class Player;

/**
 * @class Enemy
 * @brief Абстрактный базовый класс для всех типов врагов.
 */
class Enemy {
public:
    virtual ~Enemy() = default;

    // Чисто виртуальные методы, которые должны быть реализованы в производных классах
    virtual void update(float dt, const sf::Vector2f& playerPosition,
                        const std::vector<std::unique_ptr<Enemy>>& enemies, Player& player,
                        sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void takeDamage(float amount, sf::RenderWindow& window) = 0;
    virtual bool isDead() const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual sf::Vector2f getPosition() const = 0;
};