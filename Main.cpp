#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Button.h"
#include "ObstacleManager.h"
#include "Utils.h"
#include "CoinManager.h"
#include <iostream>

enum GameState { MainMenu, OptionsMenu, Achievements, Statistics, Gameplay, Pause, GameOver };

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
    Button storyButton(sf::Vector2f(324, 54), sf::Vector2f(438, 260), "Tryb fabularny", font);
    storyButton.setTexture("Tekstury/ustawienia.png");
    Button endlessButton(sf::Vector2f(324, 54), sf::Vector2f(438, 352), "Endless", font);
    endlessButton.setTexture("Tekstury/ustawienia.png");
    Button optionsButton(sf::Vector2f(54, 54), sf::Vector2f(300, 486), "Opcje", font);
    optionsButton.setTexture("Tekstury/ustawienia.png");
    Button achievementsButton(sf::Vector2f(54, 54), sf::Vector2f(482, 486), "Osiągnięcia", font);
    achievementsButton.setTexture("Tekstury/ustawienia.png");
    Button statisticsButton(sf::Vector2f(54, 54), sf::Vector2f(664, 486), "Statystyki", font);
    statisticsButton.setTexture("Tekstury/ustawienia.png");
    Button backButton(sf::Vector2f(200, 50), sf::Vector2f(500, 400), "Powrót", font);
    Button exitButton(sf::Vector2f(54, 54), sf::Vector2f(846, 486), "Wyjście", font);
    exitButton.setTexture("Tekstury/ustawienia.png");
    Button pauseButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Pause", font);
    //pauseButton.setTexture("Tekstury/.png");
    Button resumeButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Wznów", font);
    //resumeButton.setTexture("Tekstury/.png");
    Button restartButton(sf::Vector2f(200, 50), sf::Vector2f(500, 320), "Restart", font);
    Button mainMenuButton(sf::Vector2f(200, 50), sf::Vector2f(500, 390), "Menu Główne", font);

    // Tworzenie gracza
    Player player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);

    // Tworzenie generatora przeszkód
    ObstacleManager obstacleManager(window.getSize().x, window.getSize().y);
    
    // Tworzenie menedzera monet
    CoinManager coinManager(1.0f, 150.0f);
    int coinCount = 0;

    // Teksty do wyświetlania

    std::vector<std::string> achievements = {"Achievement 1", "Achievement 2", "Achievement 3"};
    std::vector<std::string> statistics = {"Statistic 1", "Statistic 2", "Statistic 3"};

    // Tworzenie tekstu
    sf::Text nameText("Dino Game", font, 70);

    // Wyśrodkowanie tekstu
    centerText(nameText, 1200, 640, 60);  // Wyśrodkowanie w poziomie, wysokość 60

    sf::Text optionsMenuText("Menu Opcji", font, 50);
    centerText(optionsMenuText, 1200, 640, 100);

    sf::Text gameplayText("Gejmplej!", font, 50);
    centerText(gameplayText, 1200, 640, 200);

    sf::Text pauseText("Pause", font, 50);
    centerText(pauseText, 1200, 640, 200);
    

    sf::Text distanceText("Odleglosc:", font, 50);
    distanceText.setPosition(820, 10); //Zmienić setPosition na cos innego (moze centerText?)

    sf::Text coinCountText("Monety: 0", font, 50);
    coinCountText.setPosition(820, 80); 


    float deltaTime;
    sf::Clock clock;

    float distance;

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
                    if (storyButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        distance = 0.0f;
                        coinCount = 0.0f;
                        obstacleManager.restart();
                        gameState = Gameplay; // Przejście do Gameplay
                    }
                    if (endlessButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        distance = 0.0f;
                        coinCount = 0.0f;
                        obstacleManager.restart();
                        gameState = Gameplay; // Przejście do Gameplay
                    }
                    if (optionsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = OptionsMenu; // Przejście do OptionsMenu
                    }
                    if (achievementsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Achievements; // Przejście do Achievements
                    }
                    if (statisticsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = Statistics; // Przejście do Statistics
                    }
                    if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        window.close(); // Zamknięcie okna
                    }

                } else if (gameState == OptionsMenu) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Achievements) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Statistics) {
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
                        player = Player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                        distance = 0.0f;
                        coinCount = 0.0f;
                        obstacleManager.restart();
                        gameState = Gameplay; // Restart gry
                    }
                    if (mainMenuButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == GameOver) {
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        player = Player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                        distance = 0.0f;
                        coinCount = 0.0f;
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
                    if (gameState == Achievements) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Statistics) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }
                    if (gameState == Gameplay) {
                        gameState = Pause; // Przejście do Pause
                    } 
                    else if (gameState == Pause) {
                        gameState = Gameplay; // Powrót do Gameplay
                    }
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close(); // Zamknięcie okna
                }
                if (event.key.code == sf::Keyboard::R) {
                    player = Player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                    obstacleManager.restart();
                    distance = 0.0f;
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
            coinManager.update(deltaTime, player.getGlobalBounds(), coinCount, obstacleManager.getObstacleBounds()); // Aktualizacja monet

            distance += 0.05f;
            distanceText.setString("Odleglosc: " + std::to_string(static_cast<int>(distance)));
            coinCountText.setString("Monety: " + std::to_string(coinCount));

	    
        // Sprawdzenie kolizji i koniec gry lub restart
            if (obstacleManager.checkCollisions(player.getGlobalBounds())) {
                gameState = GameOver;
            }
        }

        // Aktualizacja przycisków zależnie od stanu
        if (gameState == MainMenu) {
            storyButton.update(sf::Mouse::getPosition(window));
            endlessButton.update(sf::Mouse::getPosition(window));
            optionsButton.update(sf::Mouse::getPosition(window));
            exitButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == OptionsMenu) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Achievements) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Statistics) {
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
            storyButton.draw(window);
            endlessButton.draw(window);
            optionsButton.draw(window);
            achievementsButton.draw(window);
            statisticsButton.draw(window);
            exitButton.draw(window);
            window.draw(nameText);
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            backButton.draw(window);
        } else if (gameState == Achievements) {
            drawScrollableList(window, achievements, font);
            backButton.draw(window);
        } else if (gameState == Statistics) {
            drawScrollableList(window, statistics, font);
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            player.draw(window); // Rysowanie gracza
	        obstacleManager.draw(window);  // Rysowanie przeszkód
            coinManager.draw(window);
            window.draw(gameplayText);
            pauseButton.draw(window);
            window.draw(distanceText);
            window.draw(coinCountText);
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


