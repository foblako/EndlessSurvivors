#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"
#include "Player.h"
#include <memory>
#include <vector>
#include "Enemy.h"

// Предварительное объявление класса Player
class Player;

/**
 * @class Boss
 * @brief Мощный враг с большим здоровьем и уроном.
 */
class Boss : public Enemy {
public:
    // Конструктор
    Boss(float x, float y, const sf::Texture& walkTexture, const sf::Texture& attackTexture,
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
    // Перечисление состояний босса
    enum class State {
        Walking,
        Attacking,
        Dying
    };

    // Ссылка на игрока
    Player& player;

    // Спрайт босса
    sf::Sprite sprite;

    // Текстуры для различных анимаций
    const sf::Texture& walkTexture;
    const sf::Texture& attackTexture;
    const sf::Texture& deathTexture;

    // Параметры движения
    sf::Vector2f velocity;
    float speed = BOSS_SPEED;

    // Параметры состояния
    float health = BOSS_HEALTH;
    bool dead = false;

    // Переменная состояния
    State currentState = State::Walking;

    // Управление анимацией
    int currentFrame = 0;
    float currentFrameTime = 0.0f;
    const float frameDuration = FRAME_DURATION;
    const int walkFrameCount = BOSS_WALK_FRAME_COUNT;
    const int attackFrameCount = BOSS_ATTACK_FRAME_COUNT;
    const int deathFrameCount = BOSS_DEATH_FRAME_COUNT;
    void updateAnimation(float dt);

    // Задержка удаления после смерти
    float deathAnimationTime = 0.0f;
    float removeDelayTime = BOSS_REMOVE_DELAY;
};
