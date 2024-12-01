#pragma once

#include <SFML/Graphics.hpp>
#include "Const.h"

// Класс Block представляет собой отдельный блок в мире
class Block {
public:
    // Конструктор
    Block(float x, float y, float size, const sf::Texture& texture, bool isWalkable);

    // Конструктор копирования
    Block(const Block& other);

    // Оператор присваивания
    Block& operator=(const Block& other);

    // Конструкторы перемещения
    Block(Block&&) noexcept = default;
    Block& operator=(Block&&) noexcept = default;

    // Отрисовка блока
    void draw(sf::RenderTarget& target);

    // Проверка возможности прохождения через блок
    bool isWalkable() const;

    // Получение границ блока
    sf::FloatRect getBounds() const;

    // Установка цвета блока
    void setColor(const sf::Color& color) {
        sprite.setColor(color);
    }

private:
    sf::Sprite sprite;  // Спрайт блока
    bool canAcross;     // Флаг, указывающий, можно ли пройти через блок
};

