#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Button.h"
#include "ObstacleManager.h"
#include "Utils.h"
#include "CoinManager.h"
#include "StatisticsManager.h"
#include <iostream>

enum GameState { MainMenu, OptionsMenu, Achievements, Statistics, Gameplay, Pause, GameOver };

// Ustawienie domyślnego stanu gry
GameState gameState = MainMenu;

int main() {
    // Ustawienia okna
    sf::VideoMode videoMode(1200, 640);
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close;

    // Tworzenie okna
    sf::RenderWindow window(videoMode, "Dino Game", style);
    window.setFramerateLimit(60); // Ograniczenie FPS do 60

    // Załaduj teksturę tła
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Tekstury/menu.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła!" << std::endl;
        return -1;
    }
    
    // Utwórz sprite z tekstury tła
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Oblicz skalę dla tła menu
    float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    // Załaduj teksturę tła trybu fabularnego
    sf::Texture storyBackgroundTexture;
    if (!storyBackgroundTexture.loadFromFile("Tekstury/background-dino.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła trybu fabularnego!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury tła trybu fabularnego
    sf::Sprite storyBackgroundSprite;
    storyBackgroundSprite.setTexture(storyBackgroundTexture);

    // Oblicz skalę dla tła trybu fabularnego
    float storyScaleX = static_cast<float>(window.getSize().x) / storyBackgroundTexture.getSize().x;
    float storyScaleY = static_cast<float>(window.getSize().y) / storyBackgroundTexture.getSize().y;
    storyBackgroundSprite.setScale(storyScaleX, storyScaleY);

    // Załaduj teksturę tła trybu GameOver
    sf::Texture gameOverBackgroundTexture;
    if (!gameOverBackgroundTexture.loadFromFile("Tekstury/GAMEOVER-background.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła trybu GameOver!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury tła trybu GameOver
    sf::Sprite gameOverBackgroundSprite;
    gameOverBackgroundSprite.setTexture(gameOverBackgroundTexture);

    // Oblicz skalę dla tła trybu GameOver
    float gameOverScaleX = static_cast<float>(window.getSize().x) / gameOverBackgroundTexture.getSize().x;
    float gameOverScaleY = static_cast<float>(window.getSize().y) / gameOverBackgroundTexture.getSize().y;
    gameOverBackgroundSprite.setScale(gameOverScaleX, gameOverScaleY);

    // Załaduj teksturę tła trybu Statistics
    sf::Texture statisticsBackgroundTexture;
    if (!statisticsBackgroundTexture.loadFromFile("Tekstury/statistic-menu.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła trybu Statistics!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury tła trybu Statistics
    sf::Sprite statisticsBackgroundSprite;
    statisticsBackgroundSprite.setTexture(statisticsBackgroundTexture);

    // Oblicz skalę dla tła trybu Statistics
    float statisticsScaleX = static_cast<float>(window.getSize().x) / statisticsBackgroundTexture.getSize().x;
    float statisticsScaleY = static_cast<float>(window.getSize().y) / statisticsBackgroundTexture.getSize().y;
    statisticsBackgroundSprite.setScale(statisticsScaleX, statisticsScaleY);

    // Załaduj teksturę tła trybu Achievements
    sf::Texture achievementsBackgroundTexture;
    if (!achievementsBackgroundTexture.loadFromFile("Tekstury/archivment-menu.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła trybu Achievements!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury tła trybu Achievements
    sf::Sprite achievementsBackgroundSprite;
    achievementsBackgroundSprite.setTexture(achievementsBackgroundTexture);

    // Oblicz skalę dla tła trybu Achievements
    float achievementsScaleX = static_cast<float>(window.getSize().x) / achievementsBackgroundTexture.getSize().x;
    float achievementsScaleY = static_cast<float>(window.getSize().y) / achievementsBackgroundTexture.getSize().y;
    achievementsBackgroundSprite.setScale(achievementsScaleX, achievementsScaleY);

    // Załaduj teksturę tła trybu OptionsMenu (Settings)
    sf::Texture optionsMenuBackgroundTexture;
    if (!optionsMenuBackgroundTexture.loadFromFile("Tekstury/settings-menu.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła trybu OptionsMenu!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury tła trybu OptionsMenu
    sf::Sprite optionsMenuBackgroundSprite;
    optionsMenuBackgroundSprite.setTexture(optionsMenuBackgroundTexture);

    // Oblicz skalę dla tła trybu OptionsMenu
    float optionsMenuScaleX = static_cast<float>(window.getSize().x) / optionsMenuBackgroundTexture.getSize().x;
    float optionsMenuScaleY = static_cast<float>(window.getSize().y) / optionsMenuBackgroundTexture.getSize().y;
    optionsMenuBackgroundSprite.setScale(optionsMenuScaleX, optionsMenuScaleY);

    // Załaduj teksturę tła trybu Pause
    sf::Texture pauseBackgroundTexture;
    if (!pauseBackgroundTexture.loadFromFile("Tekstury/pause-background.png")) {
        std::cerr << L"Nie udało się załadować tekstury tła trybu Pause!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury tła trybu Pause
    sf::Sprite pauseBackgroundSprite;
    pauseBackgroundSprite.setTexture(pauseBackgroundTexture);

    // Oblicz skalę dla tła trybu Pause
    float pauseScaleX = static_cast<float>(window.getSize().x) / pauseBackgroundTexture.getSize().x;
    float pauseScaleY = static_cast<float>(window.getSize().y) / pauseBackgroundTexture.getSize().y;
    pauseBackgroundSprite.setScale(pauseScaleX, pauseScaleY);

    // Sprawdzenie czy udało się załadować czcionkę
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << L"Nie udało się załadować czcionki!" << std::endl;
        return -1;
    }

    // Tworzenie przycisków
    Button storyButton(sf::Vector2f(324, 54), sf::Vector2f(250, 372), " ", font);
    storyButton.setTexture("Tekstury/przyciskSTART.png", "Tekstury/przyciskENDLESS.png");
    Button endlessButton(sf::Vector2f(324, 54), sf::Vector2f(626, 372), " ", font);
    endlessButton.setTexture("Tekstury/przyciskENDLESS.png", "Tekstury/przyciskSTART.png");
    Button optionsButton(sf::Vector2f(54, 54), sf::Vector2f(300, 486), " ", font);
    optionsButton.setTexture("Tekstury/settings.png", "Tekstury/statistic.png");
    Button achievementsButton(sf::Vector2f(54, 54), sf::Vector2f(482, 486), " ", font);
    achievementsButton.setTexture("Tekstury/archievment.png", "Tekstury/statistic.png");
    Button statisticsButton(sf::Vector2f(54, 54), sf::Vector2f(664, 486), " ", font);
    statisticsButton.setTexture("Tekstury/statistic.png", "Tekstury/settings.png");
    Button backButton(sf::Vector2f(200, 50), sf::Vector2f(500, 400), L"Powrót", font);
    Button exitButton(sf::Vector2f(54, 54), sf::Vector2f(846, 486), " ", font);
    exitButton.setTexture("Tekstury/exit.png", "Tekstury/statistic.png");
    Button pauseButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), "Pause", font);
    //pauseButton.setTexture("Tekstury/.png");
    Button resumeButton(sf::Vector2f(200, 50), sf::Vector2f(10, 10), L"Wznów", font);
    //resumeButton.setTexture("Tekstury/.png");
    Button restartButton(sf::Vector2f(200, 50), sf::Vector2f(500, 370), "Restart", font);
    Button mainMenuButton(sf::Vector2f(200, 50), sf::Vector2f(500, 440), L"Menu Główne", font);

    // Tworzenie gracza
    Player player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);

    // Tworzenie generatora przeszkód
    ObstacleManager cactusManager(window.getSize().x, window.getSize().y, "cactus");
    ObstacleManager birdManager(window.getSize().x, window.getSize().y, "bird");
    // Tworzenie menedzera monet
    float obstacleInitialSpeed = cactusManager.getInitialSpeed();
    CoinManager coinManager(2.0f, obstacleInitialSpeed, obstacleInitialSpeed, obstacleInitialSpeed);
    int coinCount = 0, currentCoinCount;

    // Zaladowanie statystyk
    StatisticsManager::loadStatistics(coinCount);

    // Teksty do wyświetlania
    std::vector<std::string> achievements = {"Achievement 1", "Achievement 2", "Achievement 3"};
    std::vector<std::string> statistics = {"Statistic 1", "Statistic 2", "Statistic 3"};

    // Tworzenie tekstu
    sf::Text nameText(" ", font, 70);

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

    sf::Text coinCountMainMenuText("Zebrane monety: 0", font, 50);
    coinCountMainMenuText.setPosition(20, 20);
    coinCountMainMenuText.setString("Zebrane monety: " + std::to_string(coinCount));

    float deltaTime;
    sf::Clock clock;

    float distance;

    // Zmienne "zapadki"

    bool if_changed_speed = false;

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
                        coinCountMainMenuText.setString("Zebrane monety: " + std::to_string(coinCount));
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
                        cactusManager.restart();
			            birdManager.restart();
                        coinManager.restart();
                        gameState = Gameplay; // Przejście do Gameplay
                    }
                    if (endlessButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
			            cactusManager.restart();
                        birdManager.restart();
                        coinManager.restart();
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
                        currentCoinCount = 0.0f;
                        coinCountMainMenuText.setString("Zebrane monety: " + std::to_string(coinCount));
   			            cactusManager.restart();
                        birdManager.restart();
                        coinManager.restart();
                        gameState = Gameplay; // Restart gry
                    }
                    if (mainMenuButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == GameOver) {
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        player = Player(sf::Vector2f(50.f, 80.f), sf::Vector2f(100.f, 500.f), sf::Color::Cyan);
                        distance = 0.0f;
    			        cactusManager.restart();
                        birdManager.restart(); 
                        coinManager.restart();  
			            currentCoinCount = 0.0f;
                        coinCountMainMenuText.setString("Zebrane monety: " + std::to_string(coinCount));
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
                    coinManager.restart();
                    cactusManager.restart();
                    birdManager.restart();
                    distance = 0.0f;
                    coinCount = 0.0f;
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
	        cactusManager.update(deltaTime);
	        birdManager.update(deltaTime);
	        std::vector<sf::FloatRect> allObstacles;
	        auto cactusBounds = cactusManager.getObstacleBounds();
	        auto birdBounds = birdManager.getObstacleBounds();
	        allObstacles.insert(allObstacles.end(), cactusBounds.begin(), cactusBounds.end());
            allObstacles.insert(allObstacles.end(), birdBounds.begin(), birdBounds.end());
	        coinManager.update(deltaTime, player.getGlobalBounds(), currentCoinCount, allObstacles);

            // Obliczanie odległości
            float cactusSpeed = cactusManager.getSpeed();
	        float birdSpeed = birdManager.getSpeed();
            // float obstacleInitialSpeed = cactusManager.getInitialSpeed();
            float playerVelocity = player.getVelocity().x;
            distance += (deltaTime * (cactusSpeed + playerVelocity)) / obstacleInitialSpeed;
            
            // Zmiana prędkości co 50 jednostek
            int distanceInt = static_cast<int>(distance);
            if(distanceInt % 50 == 0 && distanceInt <= 1000 && if_changed_speed == false){
                cactusManager.setSpeed(cactusSpeed + 20);
		        birdManager.setSpeed(birdSpeed + 20);
                coinManager.setObstacleSpawnSpeed(cactusSpeed + 20);
                coinManager.setSpeed(cactusSpeed + 20);
                if_changed_speed = true;
            } else if(distanceInt % 50 != 0) {
                if_changed_speed = false;
            }

            distanceText.setString(L"Odleglosc: " + std::to_wstring(distanceInt));
            coinCountText.setString(L"Monety: " + std::to_wstring(currentCoinCount));
          
            // Sprawdzenie kolizji i koniec gry lub restart
            if ((cactusManager.checkCollisions(player.getGlobalBounds())) || (birdManager.checkCollisions(player.getGlobalBounds()))) {
                coinCount += currentCoinCount;
                StatisticsManager::saveStatistics(coinCount);
                gameState = GameOver;
            }
        }

        // Aktualizacja przycisków zależnie od stanu
        if (gameState == MainMenu) {
            storyButton.update(sf::Mouse::getPosition(window));
            endlessButton.update(sf::Mouse::getPosition(window));
            optionsButton.update(sf::Mouse::getPosition(window));
            achievementsButton.update(sf::Mouse::getPosition(window));
            statisticsButton.update(sf::Mouse::getPosition(window));
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
            window.draw(backgroundSprite); // Rysowanie tła
            storyButton.draw(window);
            endlessButton.draw(window);
            optionsButton.draw(window);
            achievementsButton.draw(window);
            statisticsButton.draw(window);
            exitButton.draw(window);
            window.draw(nameText);
            window.draw(coinCountMainMenuText);
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            window.draw(optionsMenuBackgroundSprite); // Rysowanie tła trybu OptionsMenu
            backButton.draw(window);
        } else if (gameState == Achievements) {
            drawScrollableList(window, achievements, font);
            window.draw(achievementsBackgroundSprite); // Rysowanie tła trybu Achievements
            backButton.draw(window);
        } else if (gameState == Statistics) {
            drawScrollableList(window, statistics, font);
            window.draw(statisticsBackgroundSprite); // Rysowanie tła trybu Statistics
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            window.draw(storyBackgroundSprite); // Rysowanie tła trybu fabularnego
            player.draw(window); // Rysowanie gracza
	        cactusManager.draw(window);  // Rysowanie przeszkód
            coinManager.draw(window);
            birdManager.draw(window);
            window.draw(gameplayText);
            pauseButton.draw(window);
            window.draw(distanceText);
            window.draw(coinCountText);
        } else if (gameState == Pause) {
            window.draw(pauseText);
            window.draw(pauseBackgroundSprite); // Rysowanie tła trybu Pause
            resumeButton.draw(window);
            restartButton.draw(window);
            mainMenuButton.draw(window);
        } else if (gameState == GameOver) {
            window.clear(sf::Color::Black);
            window.draw(gameOverBackgroundSprite); // Rysowanie tła trybu GameOver
            restartButton.draw(window);
            exitButton.draw(window);
        }
        window.display();
    }
    return 0;
}


