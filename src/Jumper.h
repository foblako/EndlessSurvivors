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
 * @class Jumper
 * @brief Враг, способный прыгать к игроку.
 */
class Jumper : public Enemy {
public:
    // Конструктор
    Jumper(float x, float y, const sf::Texture& walkTexture, const sf::Texture& attackTexture,
           const sf::Texture& deathTexture, const sf::Texture& jumpTexture, Player& player);

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
    // Перечисление состояний джампера
    enum class State {
        Walking,
        Jumping,
        Attacking,
        Dying
    };

    // Переменная состояния
    State currentState = State::Walking;

    // Ссылка на игрока
    Player& player;

    // Обновление анимации
    void updateAnimation(float dt);

    // Расчёт позиции в прыжке
    sf::Vector2f calculateJumpPosition(float t);


    // Переменные для анимации и движения
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = JUMPER_SPEED;
    float health = JUMPER_HEALTH;
    int currentFrame = 0;
    const float frameDuration = FRAME_DURATION;
    float currentFrameTime = 0.0f;
    bool dead = false;

    // Текстуры и количество кадров для анимаций
    const sf::Texture& walkTexture;
    const sf::Texture& attackTexture;
    const sf::Texture& deathTexture;
    const sf::Texture& jumpTexture;
    const int walkFrameCount = JUMPER_WALK_FRAME_COUNT;
    const int attackFrameCount = JUMPER_ATTACK_FRAME_COUNT;
    const int deathFrameCount = JUMPER_DEATH_FRAME_COUNT;
    const int jumpFrameCount = JUMPER_JUMP_FRAME_COUNT;

    // Переменные для прыжка
    float timeSinceLastJump = 0.0f;
    float jumpCooldown = 0.0f;
    const float minJumpInterval = 3.0f;
    const float maxJumpInterval = 5.0f;
    const float jumpDistance = 200.0f;
    sf::Vector2f jumpStartPosition;
    sf::Vector2f jumpEndPosition;
    float jumpProgress = 0.0f;
    const float jumpDuration = 0.5f; // Длительность прыжка в секундах
    const float jumpHeight = 50.0f;  // Максимальная высота прыжка

    // Переменные для удаления после смерти
    float deathAnimationTime = 0.0f;
    const float removeDelayTime = JUMPER_REMOVE_DELAY;
};