#include "Runner.h"
#include <iostream>

// Конструктор класса Runner
Runner::Runner(float x, float y, const sf::Texture& walkTexture, const sf::Texture& attackTexture,
               const sf::Texture& deathTexture, Player& player)
    : walkTexture(walkTexture), attackTexture(attackTexture), deathTexture(deathTexture),
      player(player) {
    // Инициализация спрайта
    sprite.setTexture(walkTexture);
    sprite.setPosition(x, y);
    sprite.setScale(1.0f, 1.0f);
    // Установка начального прямоугольника текстуры для анимации
    sprite.setTextureRect(sf::IntRect(0, 0, RUNNER_WALK_WIDTH / walkFrameCount,
                                      SPRITESHEET_HEIGHT / ROW_COUNT));
}

void Runner::update(float dt, const sf::Vector2f& playerPosition,
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

    // Определяем направление спрайта
    bool shouldFaceLeft = direction.x < 0;
    if (facingLeft != shouldFaceLeft) {
        facingLeft = shouldFaceLeft;
        sprite.setScale(facingLeft ? -1.0f : 1.0f, 1.0f);
        
        // Корректируем позицию при развороте для сохранения правильных границ
        if (facingLeft) {
            sprite.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width, 
                             sprite.getPosition().y);
        } else {
            sprite.setPosition(sprite.getPosition().x - sprite.getGlobalBounds().width, 
                             sprite.getPosition().y);
        }
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

    // Рассчитываем дистанцию атаки с учетом ширины спрайтов
    float attackDistance = RUNNER_ATTACK_RANGE;
    if (facingLeft) { // Если бежим справа
        attackDistance += sprite.getGlobalBounds().width;
    }

    // Логика атаки или движения
    if (length <= RUNNER_ATTACK_RANGE) {
        currentState = State::Attacking;
        player.takeDamage(RUNNER_DAMAGE * dt);
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

// Метод отрисовки бегуна
void Runner::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Метод получения урона бегуном
void Runner::takeDamage(float amount, sf::RenderWindow& window) {
    health -= amount;
    if (health <= 0 && currentState != State::Dying) {
        currentState = State::Dying;
        currentFrame = 0;
        sprite.setTexture(deathTexture);
        player.gainExperience(RUNNER_EXPERIENCE);
    }
}

// Проверка, мёртв ли бегун
bool Runner::isDead() const {
    return currentState == State::Dying && currentFrame >= deathFrameCount - 1;
}

// Получение границ спрайта бегуна
sf::FloatRect Runner::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // Корректируем границы в зависимости от направления
    if (facingLeft) {
        bounds.left += bounds.width;
        bounds.width = -bounds.width;
    }

    return bounds;
}

// Получение позиции бегуна
sf::Vector2f Runner::getPosition() const {
    return sprite.getPosition();
}

// Обновление анимации бегуна
void Runner::updateAnimation(float dt) {
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
        sprite.setTextureRect(sf::IntRect(currentFrame * (RUNNER_DEATH_WIDTH / deathFrameCount), 0,
                                          RUNNER_DEATH_WIDTH / deathFrameCount,
                                          SPRITESHEET_HEIGHT / ROW_COUNT));
    } else if (currentState == State::Attacking) {
        if (currentFrame >= attackFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (RUNNER_ATTACK_WIDTH / attackFrameCount), 0,
                                          RUNNER_ATTACK_WIDTH / attackFrameCount,
                                          SPRITESHEET_HEIGHT / ROW_COUNT));
    } else if (currentState == State::Walking) {
        if (currentFrame >= walkFrameCount) {
            currentFrame = 0;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * (RUNNER_WALK_WIDTH / walkFrameCount), 0,
                                          RUNNER_WALK_WIDTH / walkFrameCount,
                                          SPRITESHEET_HEIGHT / ROW_COUNT));
    }
}