#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "Enemy.h"
#include "Player.h"

// Forward declarations
class Player;

struct Explosion {
    sf::CircleShape shape;
    float lifetime;
    float tickTimer;
    sf::Vector2f position;
};

class ProjectileManager {
public:
    ProjectileManager(sf::RenderWindow& window, Player& player);

    void update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw();
    void shoot(const sf::Vector2f& startPosition, const sf::Vector2f& targetPosition);
    void checkCollisions(const std::vector<std::unique_ptr<Enemy>>& enemies);
    void handleMouseInput(const sf::Vector2f& playerPosition, const sf::View& view);

private:
    Player& player;
    std::vector<Projectile> projectiles;
    sf::RenderWindow& window;
    float shootCooldown = 0.0f;
    const float maxShootCooldown = ATTACK_SPEED; // Use constant instead of player member
    std::vector<Explosion> explosions; // Explosions with lifetime
};