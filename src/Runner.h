#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>
#include <vector>

// Предварительное объявление класса Player
class Player;

/**
 * @class Runner
 * @brief Быстрый враг, преследующий игрока.
 */
class Runner : public Enemy {
public:
    // Конструктор
    Runner(float x, float y, const sf::Texture& walkTexture, const sf::Texture& attackTexture,
           const sf::Texture& deathTexture, Player& player);

    // Переопределённые методы из Enemy
    void update(float dt, const sf::Vector2f& playerPosition,
                const std::vector<std::unique_ptr<Enemy>>& enemies, Player& player,
                sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage(float amount, sf::RenderWindow& window) override;
    bool isDead() const override;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const override;

private:
    // Перечисление состояний бегуна
    enum class State {
        Walking,
        Attacking,
        Dying
    };

    // Ссылка на игрока
    Player& player;

    // Спрайт бегуна
    sf::Sprite sprite;

    // Текстуры для различных анимаций
    const sf::Texture& walkTexture;
    const sf::Texture& attackTexture;
    const sf::Texture& deathTexture;

    // Параметры движения
    sf::Vector2f velocity;
    float speed = RUNNER_SPEED;

    // Параметры состояния
    float health = RUNNER_HEALTH;
    bool dead = false;
    State currentState = State::Walking;

    // Управление анимацией
    int currentFrame = 0;
    float currentFrameTime = 0.0f;
    const float frameDuration = FRAME_DURATION;
    const int walkFrameCount = RUNNER_WALK_FRAME_COUNT;
    const int attackFrameCount = RUNNER_ATTACK_FRAME_COUNT;
    const int deathFrameCount = RUNNER_DEATH_FRAME_COUNT;
    void updateAnimation(float dt);

    // Задержка удаления после смерти
    float deathAnimationTime = 0.0f;
    float removeDelayTime = RUNNER_REMOVE_DELAY;

    // Направление спрайта
    bool facingLeft = false;
    float targetRotation = 0.0f;
    float currentRotation = 0.0f;
    const float rotationSpeed = 360.0f; // градусов в секунду
};



