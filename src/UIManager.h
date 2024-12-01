#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class UIManager {
public:
    UIManager(sf::RenderWindow& window, Player& player);

    void update();
    void draw();

private:
    sf::RenderWindow& window;
    Player& player;
    sf::Font font;

    // UI Elements
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBorder;
    sf::RectangleShape experienceBar;
    std::vector<sf::Text> statTexts;

    // Флаги для отображения меню
    bool showingUpgradeMenu = false;
    bool showingDeathMenu = false;
    bool showingWinMenu = false;

    // Helper methods
    void updateHealthBar();
    void updateExperienceBar();
    void updateStatTexts();

    void showUpgradeMenu();
    void showDeathMenu();
    void showWinMenu();
};