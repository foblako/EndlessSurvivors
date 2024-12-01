#include "Jumper.h"
#include "Const.h"
#include <iostream>

// Конструктор класса Jumper
Jumper::Jumper(float x, float y, const sf::Texture& walkTexture, const sf::Texture& attackTexture,
               const sf::Texture& deathTexture, const sf::Texture& jumpTexture, Player& player)
    : walkTexture(walkTexture), attackTexture(attackTexture), deathTexture(deathTexture),
      jumpTexture(jumpTexture), player(player) {
    // Инициализация спрайта
    sprite.setTexture(walkTexture);
    sprite.setPosition(x, y);
    sprite.setScale(1.0f, 1.0f);
    // Установка начального прямоугольника текстуры для анимации
    sprite.setTextureRect(sf::IntRect(0, 0, JUMPER_WALK_WIDTH / walkFrameCount,
                                      SPRITESHEET_HEIGHT / ROW_COUNT));
    // Установка начальной задержки прыжка
    jumpCooldown = minJumpInterval + static_cast<float>(rand()) /
                   (static_cast<float>(RAND_MAX / (maxJumpInterval - minJumpInterval)));
}

// Обновляем сигнатуру метода
void Jumper::update(float dt, const sf::Vector2f& playerPosition, const std::vector<std::unique_ptr<Enemy>>& enemies, Player& player, sf::RenderWindow& window) {
    if (currentState == State::Dying) {
        // Обновление анимации смерти
        updateAnimation(dt);
        return;
    }

    // Обновление времени с момента последнего прыжка
    timeSinceLastJump += dt;

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

    // Логика прыжка
    if (currentState == State::Jumping) {
        // Обновляем прогресс прыжка
        jumpProgress += dt / jumpDuration;
        if (jumpProgress >= 1.0f) {
            jumpProgress = 1.0f;
            currentState = State::Walking;
        }
        // Обновляем позицию во время прыжка
        sf::Vector2f newPosition = calculateJumpPosition(jumpProgress);
        sprite.setPosition(newPosition);
        sprite.setTexture(jumpTexture);
    }
    else if (length <= JUMPER_ATTACK_RANGE) {
        // Логика атаки
        currentState = State::Attacking;
        player.takeDamage(JUMPER_DAMAGE * dt);
        sprite.setTexture(attackTexture);
    }
    else {
        if (timeSinceLastJump >= jumpCooldown) {
            // Начинаем прыжок
            currentState = State::Jumping;
            timeSinceLastJump = 0.0f;
            jumpProgress = 0.0f;
            jumpCooldown = minJumpInterval + static_cast<float>(rand()) /
                           (static_cast<float>(RAND_MAX / (maxJumpInterval - minJumpInterval)));

            jumpStartPosition = sprite.getPosition();
            jumpEndPosition = sprite.getPosition() + direction * jumpDistance;
            sprite.setTexture(jumpTexture);
        }
        else {
            // Ходьба к игроку
            currentState = State::Walking;
            velocity = direction * speed;
            sprite.move(velocity * dt);
            sprite.setTexture(walkTexture);
        }
    }

    // Обновляем анимацию
    updateAnimation(dt);
}

// Вычисление позиции во время прыжка
sf::Vector2f Jumper::calculateJumpPosition(float t) {
    // Линейная интерполяция между начальной и конечной позициями
    sf::Vector2f position = (1 - t) * jumpStartPosition + t * jumpEndPosition;

    // Параболическая высота прыжка
    float parabolicHeight = -4 * jumpHeight * (t - 0.5f) * (t - 0.5f) + jumpHeight;

    // Добавляем высоту к позиции по оси Y
    position.y -= parabolicHeight;

    return position;
}

void Jumper::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Jumper::takeDamage(float amount, sf::RenderWindow& window) {
    health -= amount;
    if (health <= 0 && currentState != State::Dying) {
        currentState = State::Dying;
        currentFrame = 0;
        sprite.setTexture(deathTexture);
        player.gainExperience(JUMPER_EXPERIENCE);
    }
}

bool Jumper::isDead() const {
    return dead;
}

sf::FloatRect Jumper::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Jumper::getPosition() const {
    return sprite.getPosition();
}

void Jumper::updateAnimation(float dt) {
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
        sprite.setTextureRect(sf::IntRect(currentFrame * (JUMPER_DEATH_WIDTH / deathFrameCount), 0,
                                          JUMPER_DEATH_WIDTH / deathFrameCount, SPRITESHEET_HEIGHT / ROW_COUNT));
    }
    else if (currentState == State::Attacking) {
        if (currentFrame >= attackFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (JUMPER_ATTACK_WIDTH / attackFrameCount), 0,
                                          JUMPER_ATTACK_WIDTH / attackFrameCount, SPRITESHEET_HEIGHT / ROW_COUNT));
    }
    else if (currentState == State::Jumping) {
        if (currentFrame >= jumpFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (JUMPER_JUMP_WIDTH / jumpFrameCount), 0,
                                          JUMPER_JUMP_WIDTH / jumpFrameCount, SPRITESHEET_HEIGHT / ROW_COUNT));
    }
    else if (currentState == State::Walking) {
        if (currentFrame >= walkFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (JUMPER_WALK_WIDTH / walkFrameCount), 0,
                                          JUMPER_WALK_WIDTH / walkFrameCount, SPRITESHEET_HEIGHT / ROW_COUNT));
    }
}