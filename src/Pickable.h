#pragma once
#include <SFML/Graphics.hpp>

class Pickable {
public:
    Pickable(float x, float y, float size, const sf::Texture& texture);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    const sf::Texture* getTexture() const;

private:
    sf::Sprite sprite;
};
