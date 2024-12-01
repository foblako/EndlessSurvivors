#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <unordered_set>

class Projectile {
public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, 
               float speed, float damage, float size, 
               float lifetime, int pierces, 
               float critChance, float critMult,
               float explChance, float explRadius); // Added explRadius

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const std::unique_ptr<Enemy>& enemy, sf::RenderWindow& window);
    void applyExplosionDamage(const std::vector<std::unique_ptr<Enemy>>& enemies, sf::RenderWindow& window);
    bool isExpired() const;
    sf::FloatRect getBounds() const;
    float getDamage() const;
    bool toBeRemoved = false;
    float getExplosionRadius() const { return explosionRadius; }
    sf::Vector2f getPosition() const { return position; }
    int getRemainingPierces() const { return remainingPierces; }
    bool hasExploded() const { return isExploded; }

private:
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    float damage;
    float size;
    float lifeTime;
    int remainingPierces;    // Оставшиеся пробивания
    bool isCritical;         // Критический удар
    float explosionChance;   // Chance for explosion
    float explosionRadius;   // Radius of explosion
    bool isExploded = false;
    bool isExplosive = false; // Indicates if the projectile is explosive
    float critMultiplier;    // Множитель крита
    sf::CircleShape shape;
    std::unordered_set<Enemy*> hitEnemies; // Enemies already hit
};