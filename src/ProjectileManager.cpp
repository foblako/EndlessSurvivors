#include "ProjectileManager.h"
#include <iostream>

ProjectileManager::ProjectileManager(sf::RenderWindow& window, Player& player) : shootCooldown(0.0f), window(window), player(player) {}

void ProjectileManager::update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // ��������� �������
    if (shootCooldown > 0.0f) {
        shootCooldown -= dt;
    }

    // ��������� ��� �������
    for (auto& projectile : projectiles) {
        projectile.update(dt);
    }

    // ��������� ������������ � �������
    checkCollisions(enemies);

    // Обновляем состояние взрывов
    for (auto& explosion : explosions) {
        explosion.lifetime -= dt;
        explosion.tickTimer -= dt;

        if (explosion.tickTimer <= 0.0f) {
            // Наносим урон врагам внутри области взрыва
            for (auto& enemy : enemies) {
                if (!enemy->isDead()) {
                    float dx = enemy->getPosition().x - explosion.position.x;
                    float dy = enemy->getPosition().y - explosion.position.y;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance <= explosion.shape.getRadius()) {
                        enemy->takeDamage(EXPLOSION_DAMAGE, window);
                    }
                }
            }
            explosion.tickTimer = EXPLOSION_TICK_INTERVAL;
        }
    }

    // Удаляем истекшие взрывы
    explosions.erase(std::remove_if(explosions.begin(), explosions.end(),
        [](const Explosion& explosion) {
            return explosion.lifetime <= 0.0f;
        }), explosions.end());

    // ������� �������, � ������� ������� ����� ����� ��� ������� �������� ��� ��������
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& projectile) {
            return projectile.isExpired() || projectile.toBeRemoved;
        }), projectiles.end());
}

void ProjectileManager::draw() {
    for (auto& projectile : projectiles) {
        projectile.draw(window);
    }

    // Draw explosions
    for (auto& explosion : explosions) {
        window.draw(explosion.shape);
    }
}

void ProjectileManager::shoot(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition) {
    if (shootCooldown <= 0.0f) {
        sf::Vector2f baseDirection = targetPosition - startPosition;
        float length = std::sqrt(baseDirection.x * baseDirection.x + baseDirection.y * baseDirection.y);
        if (length != 0) {
            baseDirection /= length;
        }

        float angleStep = 10.0f;
        float startAngle = -angleStep * (player.projectileCount - 1) / 2.0f;

        for (int i = 0; i < player.projectileCount; ++i) {
            float angle = startAngle + i * angleStep;
            sf::Vector2f direction = sf::Vector2f(
                baseDirection.x * std::cos(angle * 3.14159265 / 180.0f) - baseDirection.y * std::sin(angle * 3.14159265 / 180.0f),
                baseDirection.x * std::sin(angle * 3.14159265 / 180.0f) + baseDirection.y * std::cos(angle * 3.14159265 / 180.0f)
            );

            projectiles.emplace_back(startPosition, direction, 1000.0f, player.damage,
                                   player.projectileSize, player.projectileLifetime, 
                                   player.pierceCount,
                                   player.criticalHitChance, player.criticalHitMultiplier,
                                   player.explosionChance, player.explosionRadius);
        }

        shootCooldown = player.attackCooldown;
    }
}



// В ProjectileManager.cpp:

void ProjectileManager::checkCollisions(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& projectile : projectiles) {
        for (auto& enemy : enemies) {
            if (projectile.checkCollision(enemy, window)) {
                // Handle explosion
                if (projectile.hasExploded()) {
                    sf::Vector2f explosionPos = projectile.getPosition();
                    float radius = projectile.getExplosionRadius();

                    // Create explosion with lifetime and tick timer
                    Explosion explosion;
                    explosion.shape.setRadius(radius);
                    explosion.shape.setOrigin(radius, radius);
                    explosion.shape.setPosition(explosionPos);
                    explosion.shape.setFillColor(sf::Color(255, 165, 0, 10)); // Orange color
                    explosion.lifetime = EXPLOSION_DURATION;
                    explosion.tickTimer = 0.0f; // Apply damage immediately
                    explosion.position = explosionPos;
                    explosions.push_back(explosion);
                }

                // Remove projectile if no pierces remain
                if (projectile.getRemainingPierces() <= 0) {
                    projectile.toBeRemoved = true;
                }
                break;
            }
        }
    }
}

void ProjectileManager::handleMouseInput(const sf::Vector2f& playerPosition, const sf::View& view) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition, view);

        shoot(playerPosition, worldPosition);
    }
}
