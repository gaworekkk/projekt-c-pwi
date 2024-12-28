#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Button.h"
#include "ObstacleManager.h"
#include <iostream>

enum GameState { MainMenu, OptionsMenu, Gameplay, Pause, GameOver };

// Ustawienie domyślnego stanu gry
GameState gameState = MainMenu;

int main() {
    // Tworzenie okna
    sf::RenderWindow window(sf::VideoMode(1200, 640), "Dino Game");
    window.setFramerateLimit(60); // Ograniczenie FPS do 60

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie udało się załadować czcionki!" << std::endl;
        return -1;
    }

    // Tworzenie przycisków
    Button playButton(sf::Vector2f(200, 50), sf::Vector2f(500, 250), "Graj", font);
    Button optionsButton(sf::Vector2f(200, 50), sf::Vector2f(500, 320), "Opcje", font);
    Button backButton(sf::Vector2f(200, 50), sf::Vector2f(500, 400), "Powrót", font);
    Button exitButton(sf::Vector2f(200, 50), sf::Vector2f(500, 470), "Wyjście", font);
    Button pauseButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Pause", font);
    Button resumeButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Wznów", font);
    Button restartButton(sf::Vector2f(200, 50), sf::Vector2f(500, 320), "Restart", font);
    Button mainMenuButton(sf::Vector2f(200, 50), sf::Vector2f(500, 390), "Menu Główne", font);

    // Tworzenie gracza
    Player player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);

    // Tworzenie generatora przeszkód
    ObstacleManager obstacleManager(window.getSize().x, window.getSize().y);
    // Teksty do wyświetlania
    sf::Text optionsMenuText("Menu Opcji", font, 50);
    optionsMenuText.setPosition(500, 100);

    sf::Text gameplayText("Gejmplej!", font, 50);
    gameplayText.setPosition(500, 200);

    sf::Text pauseText("Pause", font, 50);
    pauseText.setPosition(500, 200);

    sf::Text nameText("Dino Game", font, 50);
    nameText.setPosition(500, 50);

    float deltaTime;
    sf::Clock clock;

    // Główna pętla gry
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Obsługa kliknięć w różnych stanach
            if (event.type == sf::Event::MouseButtonPressed) {
                if (gameState == MainMenu) {
                    if (playButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Gameplay; // Przejście do Gameplay
                    }
                    if (optionsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = OptionsMenu; // Przejście do OptionsMenu
                    }
                    if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        window.close(); // Zamknięcie okna
                    }
                } else if (gameState == OptionsMenu) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                } else if (gameState == Gameplay) {
                    if (pauseButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Pause; // Przejście do Pause
                    }
                } else if (gameState == Pause) {
                    if (resumeButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Gameplay; // Powrót do Gameplay
                    }
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        player = Player(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                        gameState = Gameplay; // Restart gry
                    }
                    if (mainMenuButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                } else if (gameState == GameOver) {
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        player = Player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                        obstacleManager.restart();
                        gameState = Gameplay; // Restart gry
                    }
                    if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                    window.close(); // Zamknięcie gry
                    }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gameState == OptionsMenu) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Gameplay) {
                        gameState = Pause; // Przejście do Pause
                    } else if (gameState == Pause) {
                        gameState = Gameplay; // Powrót do Gameplay
                    }
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close(); // Zamknięcie okna
                }
                if (event.key.code == sf::Keyboard::R) {
                    player = Player(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                    gameState = Gameplay; // Restart gry
                }
                if (event.key.code == sf::Keyboard::P) {
                    gameState = Pause; // Przejście do Pause
                }
            }
        }

        // Logika w zależności od stanu gry
        if (gameState == Gameplay) {
            player.handleInput(deltaTime);
            player.update(deltaTime);
	    obstacleManager.update(deltaTime);  // Aktualizacja przeszkód
	    // Sprawdzenie kolizji i koniec gry lub restart
            if (obstacleManager.checkCollisions(player.getGlobalBounds())) {
                gameState = GameOver;
            }
        }

        // Aktualizacja przycisków zależnie od stanu
        if (gameState == MainMenu) {
            playButton.update(sf::Mouse::getPosition(window));
            optionsButton.update(sf::Mouse::getPosition(window));
            exitButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == OptionsMenu) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Gameplay) {
            pauseButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Pause) {
            resumeButton.update(sf::Mouse::getPosition(window));
            restartButton.update(sf::Mouse::getPosition(window));
            mainMenuButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == GameOver) {
            restartButton.update(sf::Mouse::getPosition(window));
            exitButton.update(sf::Mouse::getPosition(window));
        }

        // Rysowanie
        window.clear(sf::Color::Black);

        if (gameState == MainMenu) {
            playButton.draw(window);
            optionsButton.draw(window);
            exitButton.draw(window);
            window.draw(nameText);
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            player.draw(window); // Rysowanie gracza
	    obstacleManager.draw(window);  // Rysowanie przeszkód
            window.draw(gameplayText);
            pauseButton.draw(window);
        } else if (gameState == Pause) {
            window.draw(pauseText);
            resumeButton.draw(window);
            restartButton.draw(window);
            mainMenuButton.draw(window);
        } else if (gameState == GameOver) {
            window.clear(sf::Color::Black);
            sf::Text gameOverText("Sprobuj jeszcze raz", font, 50);
            gameOverText.setPosition(400, 200);
            window.draw(gameOverText);
            restartButton.draw(window);
            exitButton.draw(window);
        }
        window.display();
    }

    return 0;
}


