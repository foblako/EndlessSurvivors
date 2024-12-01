#include "Boss.h"
#include <iostream>

// Конструктор класса Boss
Boss::Boss(float x, float y, const sf::Texture& walkTexture, const sf::Texture& attackTexture,
           const sf::Texture& deathTexture, Player& player)
    : walkTexture(walkTexture), attackTexture(attackTexture), deathTexture(deathTexture),
      player(player) {
    // Инициализация спрайта
    sprite.setTexture(walkTexture);
    sprite.setPosition(x, y);
    sprite.setScale(1.5f, 1.5f); // Босс крупнее обычных врагов
    // Установка начального прямоугольника текстуры для анимации
    sprite.setTextureRect(sf::IntRect(0, 0, BOSS_WALK_WIDTH / walkFrameCount, BOSS_DEATH_HEIGHT));
}

void Boss::update(float dt, const sf::Vector2f& playerPosition,
                  const std::vector<std::unique_ptr<Enemy>>& enemies, Player& player,
                  sf::RenderWindow& window) {
    if (currentState == State::Dying) {
        // Обновление анимации смерти
        updateAnimation(dt);
        return;
    }

    // Вычисление направления к игроку
    sf::Vector2f direction = playerPosition - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    // Избегание столкновений с другими врагами
    for (const auto& other : enemies) {
        if (other.get() == this) continue;
        sf::Vector2f otherPos = other->getPosition();
        sf::Vector2f diff = sprite.getPosition() - otherPos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < MIN_DISTANCE_BETWEEN_ENEMIES) {
            direction += diff / dist;
        }
    }

    // Нормализация направления после избегания
    float dirLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (dirLength != 0) {
        direction /= dirLength;
    }

    // Логика атаки или движения
    if (length <= BOSS_ATTACK_RANGE) {
        currentState = State::Attacking;
        player.takeDamage(BOSS_DAMAGE * dt);
        sprite.setTexture(attackTexture);
    } else {
        currentState = State::Walking;
        velocity = direction * speed;
        sprite.move(velocity * dt);
        sprite.setTexture(walkTexture);
    }

    // Обновляем анимацию
    updateAnimation(dt);
}

// Остальные методы аналогично прокомментированы и организованы
void Boss::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Boss::takeDamage(float amount, sf::RenderWindow& window) {
    health -= amount;
    if (health <= 0 && currentState != State::Dying) {
        currentState = State::Dying;
        currentFrame = 0;
        sprite.setTexture(deathTexture);
        player.gainExperience(BOSS_EXPERIENCE);
    }
}

bool Boss::isDead() const {
    return currentState == State::Dying && currentFrame >= deathFrameCount - 1;
}

sf::FloatRect Boss::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Boss::getPosition() const {
    return sprite.getPosition();
}

void Boss::updateAnimation(float dt) {
    currentFrameTime += dt;
    if (currentFrameTime >= frameDuration) {
        currentFrameTime = 0.0f;
        currentFrame = (currentFrame + 1);
    }

    if (currentState == State::Dying) {
        if (currentFrame >= deathFrameCount) {
            currentFrame = deathFrameCount - 1;
            dead = true;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (BOSS_DEATH_WIDTH / deathFrameCount), 0,
                                        BOSS_DEATH_WIDTH / deathFrameCount, BOSS_DEATH_HEIGHT));
    } else if (currentState == State::Attacking) {
        if (currentFrame >= attackFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (BOSS_ATTACK_WIDTH / attackFrameCount), 0,
                                        BOSS_ATTACK_WIDTH / attackFrameCount, BOSS_DEATH_HEIGHT));
    } else if (currentState == State::Walking) {
        if (currentFrame >= walkFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (BOSS_WALK_WIDTH / walkFrameCount), 0,
                                        BOSS_WALK_WIDTH / walkFrameCount, BOSS_DEATH_HEIGHT));
    }
}