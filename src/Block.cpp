#include "Block.h"

// Реализация методов класса Block

Block::Block(float x, float y, float size, const sf::Texture& texture, bool isWalkable)
    : canAcross(isWalkable) {
    // Инициализация спрайта
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(size / texture.getSize().x, size / texture.getSize().y);
}

Block::Block(const Block& other) {
    sprite = other.sprite;
    canAcross = other.canAcross;
}

Block& Block::operator=(const Block& other) {
    if (this != &other) {
        sprite = other.sprite;
        canAcross = other.canAcross;
    }
    return *this;
}

void Block::draw(sf::RenderTarget& target) {
    target.draw(sprite);
}

bool Block::isWalkable() const {
    return canAcross;
}

sf::FloatRect Block::getBounds() const {
    return sprite.getGlobalBounds();
}