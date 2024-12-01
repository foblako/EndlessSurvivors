#include "UIManager.h"
#include <sstream>

UIManager::UIManager(sf::RenderWindow& window, Player& player) : window(window), player(player) {
    font.loadFromFile(GAME_FONT);

    // Инициализация полоски здоровья
    healthBarBorder.setSize(sf::Vector2f(204, 24)); // Размер с учетом границы
    healthBarBorder.setFillColor(sf::Color::Transparent);
    healthBarBorder.setOutlineColor(sf::Color::White);
    healthBarBorder.setOutlineThickness(2);
    healthBarBorder.setPosition(10, 10);

    healthBar.setSize(sf::Vector2f(200, 20));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(12, 12);

    // Инициализация полоски опыта
    experienceBar.setSize(sf::Vector2f(window.getSize().x - 20, 10));
    experienceBar.setFillColor(sf::Color::Blue);
    experienceBar.setPosition(10, window.getSize().y - 20);
}

void UIManager::update() {
    updateHealthBar();
    updateExperienceBar();
    updateStatTexts();

    // Update experience bar position based on window size
    experienceBar.setPosition(10, window.getSize().y - 20);

    // Обновляем флаги меню на основе состояния игрока
    showingUpgradeMenu = player.isUpgradeMenuVisible;
    showingDeathMenu = player.isDeathMenuVisible;
    showingWinMenu = player.isWinMenuVisible;
}

void UIManager::draw() {
    // Сохраняем текущий вид
    sf::View originalView = window.getView();
    // Устанавливаем 2D вид для UI
    sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(uiView);

    // Рисуем элементы UI
    window.draw(healthBarBorder);
    window.draw(healthBar);
    window.draw(experienceBar);

    // Draw background for all stat texts
    sf::RectangleShape statsBackground(sf::Vector2f(220, statTexts.size() * (statTexts[0].getCharacterSize() + 5) + 10));
    statsBackground.setFillColor(sf::Color(50, 50, 50, 200));
    statsBackground.setPosition(5, 35);
    window.draw(statsBackground);

    // Рисуем статистику
    float yOffset = 40;
    for (auto& text : statTexts) {
        text.setPosition(10, yOffset);
        window.draw(text);
        yOffset += text.getCharacterSize() + 5; // Отступ между строками
    }

    // Отображаем меню, если необходимо
    if (showingUpgradeMenu) {
        showUpgradeMenu();
    }
    if (showingDeathMenu) {
        showDeathMenu();
    }
    if (showingWinMenu) {
        showWinMenu();
    }

    // Восстанавливаем оригинальный вид
    window.setView(originalView);
}

void UIManager::updateHealthBar() {
    float healthPercentage = player.getHealth() / player.getMaxHealth();
    healthBar.setSize(sf::Vector2f(200 * healthPercentage, 20));
}

void UIManager::updateExperienceBar() {
    float xpPercentage = player.getCurrentExperience() / player.getExperienceForNextLevel();
    experienceBar.setSize(sf::Vector2f((window.getSize().x - 20) * xpPercentage, 10));
}

void UIManager::updateStatTexts() {
    statTexts.clear();
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);

    std::ostringstream ss;

    ss << "Level: " << player.getLevel();
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Health: " << static_cast<int>(player.getHealth()) << "/" << static_cast<int>(player.getMaxHealth());
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Speed: " << player.getSpeed();
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Damage: " << player.getDamage();
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    // Добавляем остальные характеристики
    ss << "Attack Cooldown: " << player.attackCooldown;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Projectile Count: " << player.projectileCount;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Projectile Size: " << player.projectileSize;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Projectile Lifetime: " << player.projectileLifetime;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Critical Hit Chance: " << player.criticalHitChance * 100 << "%";
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Critical Hit Multiplier: x" << player.criticalHitMultiplier;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Pierce Count: " << player.pierceCount;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Explosion Chance: " << player.explosionChance * 100 << "%";
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");

    ss << "Explosion Radius: " << player.explosionRadius;
    text.setString(ss.str());
    statTexts.push_back(text); ss.str("");
}

void UIManager::showUpgradeMenu() {
    // Сохраняем текущий вид
    sf::View originalView = window.getView();
    sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(uiView);

    // Затемнённый фон
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(background);

    // Заголовок меню
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Choose Upgrade");
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 150);
    window.draw(titleText);

    // Отображаем доступные апгрейды
    std::vector<sf::FloatRect> upgradeBounds;
    float yOffset = window.getSize().y / 2 - 50;
    
    for (size_t i = 0; i < player.randomUpgrades.size(); ++i) {
        // Создаем фон для каждого апгрейда
        sf::RectangleShape upgradeBackground(sf::Vector2f(window.getSize().x - 200, 40));
        upgradeBackground.setPosition(100, yOffset + i * 50);
        upgradeBackground.setFillColor(sf::Color(50, 50, 50, 200));
        window.draw(upgradeBackground);

        // Текст апгрейда
        sf::Text upgradeText;
        upgradeText.setFont(font);
        upgradeText.setString(player.randomUpgrades[i].name + ": " + player.randomUpgrades[i].description);
        upgradeText.setCharacterSize(24);
        upgradeText.setFillColor(sf::Color::White);
        upgradeText.setPosition(110, yOffset + i * 50 + 5);
        window.draw(upgradeText);

        upgradeBounds.push_back(upgradeBackground.getGlobalBounds());
    }

    // Не вызываем window.display() здесь!

    // Обработка событий
    sf::Event event;
    while (window.pollEvent(event)) {  // Используем pollEvent вместо waitEvent
        if (event.type == sf::Event::MouseButtonPressed) {
            for (size_t i = 0; i < upgradeBounds.size(); ++i) {
                if (upgradeBounds[i].contains(event.mouseButton.x, event.mouseButton.y)) {
                    player.applyUpgrade(player.randomUpgrades[i].name);
                    player.isUpgradeMenuVisible = false;
                    window.setView(originalView);
                    return;
                }
            }
        }
    }

    window.setView(originalView);
}

void UIManager::showDeathMenu() {
    // Сохраняем текущий вид
    sf::View originalView = window.getView();
    // Устанавливаем вид для UI
    sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(uiView);

    // Затемнённый фон
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(background);

    // Текст "Game Over"
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 100);
    window.draw(gameOverText);

    // Кнопка "Restart"
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2);
    window.draw(restartText);

    // Кнопка "Quit"
    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(30);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(window.getSize().x / 2 - 30, window.getSize().y / 2 + 50);
    window.draw(quitText);

    window.display();

    // Обработка событий
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::FloatRect restartBounds(restartText.getPosition(), sf::Vector2f(100, 30));
            sf::FloatRect quitBounds(quitText.getPosition(), sf::Vector2f(60, 30));
            if (restartBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                player.resetRestartRequest();
                player.isDeathMenuVisible = false;
                // Здесь можно добавить код для перезапуска игры
                return;
            } else if (quitBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                window.close();
                return;
            }
        } else if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
    }

    // Восстанавливаем оригинальный ви��
    window.setView(originalView);
}

void UIManager::showWinMenu() {
    // Сохраняем текущий вид
    sf::View originalView = window.getView();
    // Устанавливаем вид для UI
    sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(uiView);

    // Затемнённый фон
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(background);

    // Текст "You Win!"
    sf::Text winText;
    winText.setFont(font);
    winText.setString("You Win!");
    winText.setCharacterSize(50);
    winText.setFillColor(sf::Color::White);
    winText.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 100);
    window.draw(winText);

    // Кнопка "Restart"
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2);
    window.draw(restartText);

    // Кнопка "Quit"
    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(30);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(window.getSize().x / 2 - 30, window.getSize().y / 2 + 50);
    window.draw(quitText);

    window.display();

    // Обработка событий
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::FloatRect restartBounds(restartText.getPosition(), sf::Vector2f(100, 30));
            sf::FloatRect quitBounds(quitText.getPosition(), sf::Vector2f(60, 30));
            if (restartBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                player.resetRestartRequest();
                player.isWinMenuVisible = false;
                // Здесь можно добавить код для перезапуска игры
                return;
            } else if (quitBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                window.close();
                return;
            }
        } else if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
    }

    // Восстанавливаем оригинальный вид
    window.setView(originalView);
}