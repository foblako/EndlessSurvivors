// Pickable.cpp
#include "Pickable.h"

Pickable::Pickable(float x, float y, float size, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(size / texture.getSize().x, size / texture.getSize().y);
}

void Pickable::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Pickable::getBounds() const {
    return sprite.getGlobalBounds();
}

const sf::Texture* Pickable::getTexture() const {
    return sprite.getTexture();
}