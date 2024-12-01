#include "Projectile.h"
#include <iostream>
#include <cmath>

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction, 
                      float speed, float damage, float size, 
                      float lifetime, int pierces, 
                      float critChance, float critMult,
                      float explChance, float explRadius) // Added explRadius
    : position(position), direction(direction), 
      speed(speed), damage(damage), 
      lifeTime(lifetime), remainingPierces(pierces),
      explosionChance(explChance), explosionRadius(explRadius) {
    
    // Critical hit check
    isCritical = (static_cast<float>(rand()) / RAND_MAX) < critChance;
    critMultiplier = isCritical ? critMult : 1.0f;

    // Explosion check
    isExplosive = (static_cast<float>(rand()) / RAND_MAX) < explosionChance;

    // Projectile appearance
    shape.setPosition(position);
    shape.setRadius(size);
    if (isCritical && isExplosive) {
        shape.setFillColor(sf::Color::Magenta);
    } else if (isCritical) {
        shape.setFillColor(sf::Color::Yellow);
    } else if (isExplosive) {
        shape.setFillColor(sf::Color::Cyan);
    } else {
        shape.setFillColor(sf::Color::Red);
    }
}

void Projectile::update(float dt) {
    position += direction * speed * dt;
    shape.setPosition(position);
    lifeTime -= dt;
    if (lifeTime <= 0.0f) {
        toBeRemoved = true;
    }
    // ������� ������� ������� � ����������� ������� � �������
    // std::cout << "Projectile Current Position: (" << position.x << ", " << position.y << ")" << std::endl;
    // std::cout << "Projectile Direction: (" << direction.x << ", " << direction.y << ")" << std::endl;
}

bool Projectile::isExpired() const {
    return lifeTime <= 0.0f;
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Projectile::getBounds() const {
    return shape.getGlobalBounds();
}

float Projectile::getDamage() const {
    return damage;
}

bool Projectile::checkCollision(const std::unique_ptr<Enemy>& enemy, sf::RenderWindow& window) {
    if (shape.getGlobalBounds().intersects(enemy->getBounds()) && hitEnemies.find(enemy.get()) == hitEnemies.end()) {
        // Apply damage once per enemy
        float actualDamage = damage * critMultiplier;
        enemy->takeDamage(actualDamage, window);
        hitEnemies.insert(enemy.get());

        // Mark explosion
        if (isExplosive && !isExploded) {
            isExploded = true;
        }

        // Decrease remaining pierces
        if (remainingPierces > 0) {
            remainingPierces--;
        } else {
            toBeRemoved = true;
        }
        return true;
    }
    return false;
}

void Projectile::applyExplosionDamage(const std::vector<std::unique_ptr<Enemy>>& enemies, sf::RenderWindow& window) {
    if (explosionRadius <= 0) return;
    
    for (const auto& enemy : enemies) {
        // Проверяем расстояние до врага
        sf::Vector2f enemyPos = enemy->getPosition();
        float dx = enemyPos.x - position.x;
        float dy = enemyPos.y - position.y;
        float distance = std::sqrt(dx*dx + dy*dy);
        
        if (distance <= explosionRadius) {
            // Урон уменьшается с расстоянием
            float damageMultiplier = 1.0f - (distance / explosionRadius);
            float explosionDamage = damage * damageMultiplier * critMultiplier;
            enemy->takeDamage(explosionDamage, window);
        }
    }
}