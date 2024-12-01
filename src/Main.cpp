#include <SFML/Graphics.hpp>
#include "World.h"
#include "Const.h"
#include "Player.h"
#include "Pickable.h"
#include "TextureManager.h"
#include <sstream>
#include <iomanip>
#include "Jumper.h"
#include <iostream>
#include "ProjectileManager.h"
#include "WaveManager.h"
#include "UIManager.h"

int main() {
    TextureManager textureManager;
    bool restartGame = false;

    do {
        restartGame = false;

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Procedural World Generation");
        window.setFramerateLimit(240);
        World world(CHUNK_SIZE, BLOCK_SIZE, VIEW_RADIUS,
            textureManager.getTexture("grass"),
            textureManager.getTexture("tree"),
            textureManager.getTexture("pickable"),
            textureManager.getTexture("GregStar"),
            textureManager.getTexture("cactus"));
        Player player(400, 300, 64, textureManager.getTexture("player"));
        EnemyManager waveManager(textureManager, player, window);
        ProjectileManager projectileManager(window, player);
        UIManager uiManager(window, player);

        sf::Clock clock;
        sf::Clock gameClock;

        sf::Text timerText;
        timerText.setFont(textureManager.getFont("gameFont"));
        timerText.setCharacterSize(32);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(WINDOW_WIDTH - WINDOW_WIDTH * 0.2, 10);

        sf::View uiView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        sf::View gameView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

        sf::Music& gameMusic = textureManager.getMusic("gameMusic");
        gameMusic.setLoop(true);
        gameMusic.play();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0;
                }
            }

            if (player.restartRequested()) {
                player.resetRestartRequest();
                restartGame = true;
                break;
            }

            float dt = clock.restart().asSeconds();
            player.handleInput();
            player.update(dt, world);
            uiManager.update();

            // ����� ������ handleMouseInput ��� ��������� ������� ����� ������ ����
            projectileManager.handleMouseInput(player.getPosition(), gameView);

            sf::Vector2i playerChunkPos(
                static_cast<int>(player.getPosition().x) / (CHUNK_SIZE * BLOCK_SIZE),
                static_cast<int>(player.getPosition().y) / (CHUNK_SIZE * BLOCK_SIZE)
            );

            sf::Time elapsed = gameClock.getElapsedTime();
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << elapsed.asSeconds();
            timerText.setString("Time: " + ss.str() + " s");

            world.update(playerChunkPos, player, window);
            projectileManager.update(dt, waveManager.getEnemies());
            waveManager.update(dt, player.getPosition());

            window.clear();
            gameView.setCenter(player.getPosition());
            window.setView(gameView);
            world.draw(window, player.getPosition());
            waveManager.draw();
            projectileManager.draw(); // ��������, ��� ������� �������� � gameView
            player.draw(window);

            window.setView(uiView);
            window.draw(timerText);
            uiManager.draw();
            window.display();

        }
    } while (restartGame);

    return 0;
}
