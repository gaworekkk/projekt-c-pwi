#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Button.h"
#include "ObstacleManager.h"
#include "Utils.h"
#include "CoinManager.h"
#include "StatisticsManager.h"
#include "Difficulty.h"
#include "Slider.h"
#include <iostream>
#include <algorithm> 

enum GameState { MainMenu, OptionsMenu, Achievements, Statistics, Gameplay, Pause, GameOver };

// Ustawienie domyślnego stanu gry
GameState gameState = MainMenu;
// Ustawienie domyślnej trudności
Difficulty difficulty = Difficulty::Normal;

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

    // Załaduj teksturę dino2
    sf::Texture dinoTexture;
    if (!dinoTexture.loadFromFile("Tekstury/skórki dino/dino2.png")) {
        std::cerr << L"Nie udało się załadować tekstury dino2!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury dino2
    sf::Sprite dinoSprite;
    dinoSprite.setTexture(dinoTexture);
    dinoSprite.setPosition(20, 104); // Przeniesienie dino2 do lewej krawędzi na wysokość 104 pikseli
    dinoSprite.setScale(1.4f, 1.4f); // Powiększenie grafiki 1.256-krotnie

    // Sprawdzenie czy udało się załadować czcionkę
    sf::Font font;
    if (!font.loadFromFile("Pi.ttf")) {
        std::cerr << L"Nie udało się załadować czcionki!" << std::endl;
        return -1;
    }

    // Tworzenie sliderów głośności
    Slider musicSlider(545, 313, 110, 20, font, "Tekstury/slider.png");
    Slider soundSlider(545, 351, 110, 20, font, "Tekstury/slider.png");

    // Załaduj muzykę tła dla menu
    sf::Music menuMusic;
    if (!menuMusic.openFromFile("muzyka/muzka-do-menu1.wav")) {
        std::cerr << L"Nie udało się załadować muzyki tła dla menu!" << std::endl;
        return -1;
    }
    menuMusic.setLoop(true); // Ustawienie muzyki na pętlę

    // Załaduj muzykę tła dla gry
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("muzyka/muzykadogry.wav")) {
        std::cerr << L"Nie udało się załadować muzyki tła!" << std::endl;
        return -1;
    }
    backgroundMusic.setLoop(true); // Ustawienie muzyki na pętlę

    // Załaduj dźwięk śmierci
    sf::SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("muzyka/smierc2.ogg")) {
        std::cerr << L"Nie udało się załadować dźwięku śmierci!" << std::endl;
        return -1;
    }
    sf::Sound deathSound;
    deathSound.setBuffer(deathBuffer);

    // Załaduj dźwięk skoku
    sf::SoundBuffer jumpBuffer;
    if (!jumpBuffer.loadFromFile("muzyka/skok.ogg")) {
        std::cerr << L"Nie udało się załadować dźwięku skoku!" << std::endl;
        return -1;
    }
    sf::Sound jumpSound;
    jumpSound.setBuffer(jumpBuffer);

    // Załaduj dźwięk przycisku
    sf::SoundBuffer buttonBuffer;
    if (!buttonBuffer.loadFromFile("muzyka/przycisk2.ogg")) {
        std::cerr << L"Nie udało się załadować dźwięku przycisku!" << std::endl;
        return -1;
    }
    sf::Sound buttonSound;
    buttonSound.setBuffer(buttonBuffer);

    // Załaduj teksturę monety
    sf::Texture coinTexture;
    if (!coinTexture.loadFromFile("Tekstury/coin.png")) {
        std::cerr << L"Nie udało się załadować tekstury monety!" << std::endl;
        return -1;
    }

    // Utwórz sprite z tekstury monety
    sf::Sprite coinSprite;
    coinSprite.setTexture(coinTexture);
    coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20); // Przeniesienie monety poniżej dino2
    coinSprite.setScale(3.15f, 3.15f); // Zmniejszenie grafiki o 10 procent

    // Odtwarzanie muzyki tła dla menu
    menuMusic.play();

    // Tworzenie przycisków
    Button storyButton(sf::Vector2f(324, 54), sf::Vector2f(250, 372), " ", font);
    storyButton.setTexture("Tekstury/przyciskSTART.png", "Tekstury/kliknięte przyciski/clicked-przyciskSTART.png");
    Button endlessButton(sf::Vector2f(324, 54), sf::Vector2f(626, 372), " ", font);
    endlessButton.setTexture("Tekstury/przyciskENDLESS.png", "Tekstury/kliknięte przyciski/clicked-przyciskENDLESS.png");
    Button optionsButton(sf::Vector2f(54, 54), sf::Vector2f(300, 486), " ", font);
    optionsButton.setTexture("Tekstury/settings.png", "Tekstury/kliknięte przyciski/clicked-settings.png");
    Button achievementsButton(sf::Vector2f(54, 54), sf::Vector2f(482, 486), " ", font);
    achievementsButton.setTexture("Tekstury/archievment.png", "Tekstury/kliknięte przyciski/clicked-archievment.png");
    Button statisticsButton(sf::Vector2f(54, 54), sf::Vector2f(664, 486), " ", font);
    statisticsButton.setTexture("Tekstury/statistic.png", "Tekstury/kliknięte przyciski/clicked-statistic.png");
    Button backButton(sf::Vector2f(200, 40), sf::Vector2f(500, 400), " ", font);
    backButton.setTexture("Tekstury/przyciskPowrot.png", "Tekstury/kliknięte przyciski/clicked-przyciskPowrot.png");
    Button exitButton(sf::Vector2f(54, 54), sf::Vector2f(846, 486), " ", font);
    exitButton.setTexture("Tekstury/exit.png", "Tekstury/kliknięte przyciski/clicked-exit.png");
    Button pauseButton(sf::Vector2f(54, 54), sf::Vector2f(80, 40), " ", font);
    pauseButton.setTexture("Tekstury/PAUSE.png", "Tekstury/kliknięte przyciski/clicked-PAUSE.png" );
    Button resumeButton(sf::Vector2f(150, 142), sf::Vector2f(530, 252), " ", font);
    resumeButton.setTexture("Tekstury/PAUSE.png", "Tekstury/kliknięte przyciski/clicked-PAUSE.png");
    Button restartButton(sf::Vector2f(200, 40), sf::Vector2f(375, 490), " ", font); // Adjusted position for GameOver
    restartButton.setTexture("Tekstury/przyciskRESTART.png", "Tekstury/kliknięte przyciski/clicked-przyciskRESTART.png");
    Button restartButtonPause(sf::Vector2f(200, 40), sf::Vector2f(375, 480), " ", font); // Adjusted position for Pause
    restartButtonPause.setTexture("Tekstury/przyciskRESTART.png", "Tekstury/kliknięte przyciski/clicked-przyciskRESTART.png");
    Button mainMenuButton(sf::Vector2f(200, 40), sf::Vector2f(625, 480), " ", font); // Adjusted position for Pause
    mainMenuButton.setTexture("Tekstury/przyciskMenuGlowne.png", "Tekstury/kliknięte przyciski/clicked-przyciskMenuGlowne.png");
    Button easyButton(sf::Vector2f(324, 54), sf::Vector2f(450, 120), " ", font);
    easyButton.setTexture("Tekstury/przyciskLatwy.png", "Tekstury/kliknięte przyciski/clicked-przyciskLatwy.png");
    Button normalButton(sf::Vector2f(324, 54), sf::Vector2f(450, 180), " ", font);
    normalButton.setTexture("Tekstury/przyciskSREDNI.png", "Tekstury/kliknięte przyciski/clicked-przyciskSREDNI.png");
    Button hardButton(sf::Vector2f(324, 54), sf::Vector2f(450, 240), " ", font);
    hardButton.setTexture("Tekstury/przyciskTRUDNY.png", "Tekstury/kliknięte przyciski/clicked-przyciskTRUDNY.png");

    // Tworzenie gracza
    Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę

    // Tworzenie generatora przeszkód
    ObstacleManager cactusManager(window.getSize().x, window.getSize().y, "cactus");
    ObstacleManager birdManager(window.getSize().x, window.getSize().y, "bird");
    cactusManager.setDifficulty(difficulty);
    birdManager.setDifficulty(difficulty);

    // Tworzenie menedzera monet
    CoinManager coinManager(2.0f, cactusManager.getInitialSpeed());
    int coinCount = 0, currentCoinCount;

    // Zaladowanie statystyk
    float bestDistance = 0.0f, totalDistance = 0.0f;
    int jumpCount = 0, deathCount = 0, gamesPlayed = 0;
    StatisticsManager::loadStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed);

    // Teksty do wyświetlania
    std::vector<std::string> achievements = {"Achievement 1", "Achievement 2", "Achievement 3"};
    std::vector<std::wstring> wAchievements;
    for (const auto& achievement : achievements) {
        wAchievements.push_back(std::wstring(achievement.begin(), achievement.end()));
    }
    std::vector<std::wstring> statistics = {};
    statistics.push_back(L"Zdobyte monety: " + std::to_wstring(coinCount));
    statistics.push_back(L"Najlepszy wynik: " + std::to_wstring(bestDistance));
    statistics.push_back(L"Całkowity dystans: " + std::to_wstring(totalDistance));
    statistics.push_back(L"Liczba skoków: " + std::to_wstring(jumpCount));
    statistics.push_back(L"Liczba śmierci: " + std::to_wstring(deathCount));
    statistics.push_back(L"Liczba gier: " + std::to_wstring(gamesPlayed));

    // Tworzenie tekstu
    sf::Text nameText(L" ", font, 70);

    // Wyśrodkowanie tekstu
    centerText(nameText, 1200, 640, 60);  // Wyśrodkowanie w poziomie, wysokość 60

    sf::Text optionsMenuText(L"Menu Opcji", font, 50);
    centerText(optionsMenuText, 1200, 640, 100);

    sf::Text pauseText(L"Pause", font, 50);
    centerText(pauseText, 1200, 640, 200);
    
    sf::Text distanceText(L"0", font, 50);
    distanceText.setFillColor(sf::Color::White); // Ustawienie koloru tekstu na biały
    distanceText.setCharacterSize(distanceText.getCharacterSize() + 20); // Zwiększenie wysokości tekstu odległości o 20 pikseli
    distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20); // Przeniesienie licznika odległości obok dino2 na tej samej wysokości co ikona dino, podniesienie o 20 pikseli

    sf::Text coinCountText(L"0", font, 50);
    coinCountText.setFillColor(sf::Color::White); // Ustawienie koloru tekstu na biały
    coinCountText.setCharacterSize(distanceText.getCharacterSize()); // Ustawienie wysokości tekstu licznika monet na taką samą wartość liczbową jak licznik odległości
    coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20); // Ustawienie środka licznika monet na tej samej wysokości co środek ikony monety, podniesienie o 20 pikseli

    sf::Text coinCountMainMenuText(L"0", font, 80);
    coinCountMainMenuText.setCharacterSize(coinCountMainMenuText.getCharacterSize() ); // Double the size
    coinCountMainMenuText.setPosition(110, 0); // Move 20 pixels to the right
    coinCountMainMenuText.setString(std::to_wstring(coinCount));

    // Adjust the position and size of the coin icon in the menu
    sf::Sprite coinIconMainMenu = coinSprite;
    coinIconMainMenu.setScale(coinSprite.getScale().x * 1.5, coinSprite.getScale().y * 1.5); // Reduce the size by 1.5 times
    coinIconMainMenu.setPosition(20, 20); // Set distance from the top edge to 20 pixels and from the left edge to 20 pixels

    sf::Text gameOverText(L"Game Over", font, 70);
    centerText(gameOverText, 1200, 640, 100);

    sf::Text distanceText2 = distanceText; // Create a copy of distanceText and name it distanceText2
    distanceText2.setFillColor(sf::Color::Black); // Set the color of distanceText2 to black

    sf::Text bestScoreText(L"NAJLEPSZY WYNIK:", font, 70);
    bestScoreText.setFillColor(sf::Color::White); // Set the text color to white

    sf::Text yourScoreText(L"TWÓJ WYNIK:", font, 70); // Create the text "TWÓJ WYNIK:" with font size 70
    yourScoreText.setFillColor(sf::Color::White); // Set the text color to white

    sf::Text coinsText(L"MONETY:", font, 70); // Create the text "MONETY:" with font size 70
    coinsText.setFillColor(sf::Color::White); // Set the text color to white

    float deltaTime;
    sf::Clock clock;

    float distance;

    // Zmienne "zapadki"

    bool if_changed_speed = false;
    bool isJumping = false;
    const float standardLevel = 500.0f; // Standardowy poziom gracza

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
                    //Aktualizacja statystyk
                    statistics[0] = L"Zdobyte monety: " + std::to_wstring(coinCount);
                    statistics[1] = L"Najlepszy wynik: " + std::to_wstring(bestDistance);
                    statistics[2] = L"Całkowity wynik: " + std::to_wstring(totalDistance);
                    statistics[3] = L"Liczba skoków: " + std::to_wstring(jumpCount);
                    statistics[4] = L"Liczba śmierci: " + std::to_wstring(deathCount);
                    statistics[5] = L"Liczba gier: " + std::to_wstring(gamesPlayed);
                    if (storyButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
                        cactusManager.restart();
			            birdManager.restart();
                        coinManager.restart();
                        gamesPlayed++;
                        gameState = Gameplay; // Przejście do Gameplay
                        menuMusic.stop(); // Zatrzymanie muzyki tła dla menu
                        backgroundMusic.play(); // Odtwarzanie muzyki tła dla gry
                    }
                    if (endlessButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
			            cactusManager.restart();
                        birdManager.restart();
                        coinManager.restart();
                        gamesPlayed++;
                        gameState = Gameplay; // Przejście do Gameplay
                        menuMusic.stop(); // Zatrzymanie muzyki tła dla menu
                        backgroundMusic.play(); // Odtwarzanie muzyki tła dla gry
                    }
                    if (optionsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = OptionsMenu; // Przejście do OptionsMenu
                    }
                    if (achievementsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Achievements; // Przejście do Achievements
                    }
                    if (statisticsButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Statistics; // Przejście do Statistics
                    }
                    if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        window.close(); // Zamknięcie okna
                    }

                } else if (gameState == OptionsMenu) {
                    if (easyButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        difficulty = Difficulty::Easy;
                    }
                    if (normalButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        difficulty = Difficulty::Normal;
                    }
                    if (hardButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        difficulty = Difficulty::Hard;
                    }
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        cactusManager.setDifficulty(difficulty);
                        birdManager.setDifficulty(difficulty);
                        musicSlider.reset();
                        soundSlider.reset();
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Achievements) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Statistics) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                    }

                } else if (gameState == Gameplay) {
                    if (pauseButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Pause; // Przejście do Pause
                        backgroundMusic.pause(); // Zatrzymanie muzyki tła dla gry
                    }

                } else if (gameState == Pause) {
                    bestDistance = std::max(bestDistance, distance);
                    coinCount += currentCoinCount;
                    totalDistance += distance;
                    StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed);
                    coinCountMainMenuText.setString(std::to_wstring(coinCount));
                    if (resumeButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Gameplay; // Powrót do Gameplay
                        backgroundMusic.play(); // Wznowienie muzyki tła dla gry
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                    if (restartButtonPause.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        player.resetPosition();
                        Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
   			            cactusManager.restart();
                        birdManager.restart();
                        coinManager.restart();
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        gameState = Gameplay; // Restart gry
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                    if (mainMenuButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                        backgroundMusic.stop(); // Zatrzymanie muzyki tła dla gry
                        menuMusic.play(); // Odtwarzanie muzyki tła dla menu
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }

                } else if (gameState == GameOver) {
                    deathCount++;
                    StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed);
                    if (restartButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        player.resetPosition();
                        Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f); // 3 klatki animacji, 0.1 sekundy na klatkę
                        distance = 0.0f;
    			        cactusManager.restart();
                        birdManager.restart(); 
                        coinManager.restart();  
			            currentCoinCount = 0.0f;
                        coinCountMainMenuText.setString(std::to_wstring(coinCount));
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        gameState = Gameplay; // Restart gry
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                    }
                    // Remove the exit button functionality from the GameOver screen
                    // if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                    //     buttonSound.play(); // Odtwarzanie dźwięku przycisku
                    //     window.close(); // Zamknięcie okna
                    // }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gameState == OptionsMenu) {
                        cactusManager.setDifficulty(difficulty);
                        birdManager.setDifficulty(difficulty);
                        musicSlider.reset();
                        soundSlider.reset();
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
                        backgroundMusic.pause(); // Zatrzymanie muzyki tła dla gry
                    } 
                    else if (gameState == Pause) {
                        gameState = Gameplay; // Powrót do Gameplay
                        backgroundMusic.play(); // Wznowienie muzyki tła dla gry
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                }
                if (event.key.code == sf::Keyboard::Q) {
                    window.close(); // Zamknięcie okna
                }

                if (gameState != MainMenu) { // Block 'P' key in MainMenu
                    if (event.key.code == sf::Keyboard::R) {
                      player.resetPosition();
                      Player player(sf::Vector2f(80, 80), sf::Vector2f(100, 500), sf::Color::White, "Tekstury/skórki dino/dino_sprite_sheet.png", 3, 0.1f);
                        coinManager.restart();
                        cactusManager.restart();
                        birdManager.restart();
                        distance = 0.0f;
                        coinCount = 0.0f;
                        currentCoinCount = 0.0f; // Reset coin counter
                        gameState = Gameplay; // Restart gry
                        backgroundMusic.stop(); // Zatrzymanie muzyki tła
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                        // Reset dino icon position
                        dinoSprite.setPosition(20, 104);
                        // Reset coin icon position
                        coinSprite.setPosition(20, 104 + dinoSprite.getGlobalBounds().height + 20);
                        // Reset coin counter position
                        coinCountText.setPosition(49 + coinSprite.getGlobalBounds().width, 99 + dinoSprite.getGlobalBounds().height + 20 + (coinSprite.getGlobalBounds().height / 2) - (coinCountText.getGlobalBounds().height / 2) - 20);
                        // Reset distance counter position
                        distanceText.setPosition(40 + dinoSprite.getGlobalBounds().width, 104 + (dinoSprite.getGlobalBounds().height / 2) - (distanceText.getGlobalBounds().height / 2) - 20);
                    }
                }
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up) {
                    if (!isJumping && player.getGlobalBounds().top >= standardLevel && gameState == Gameplay) {
                        jumpSound.play(); // Odtwarzanie dźwięku skoku
                        jumpCount++;
                        isJumping = true;
                    }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up) {
                    isJumping = false;
                }
            }

            // Obsługa przesuwania sliderów
            if (gameState == OptionsMenu) {
                musicSlider.update(event, window);
                soundSlider.update(event, window);
                float musicVolume = musicSlider.getValue();
                float soundVolume = soundSlider.getValue();
                menuMusic.setVolume(musicVolume);
                backgroundMusic.setVolume(musicVolume);
                buttonSound.setVolume(soundVolume);
                jumpSound.setVolume(soundVolume);
                deathSound.setVolume(soundVolume);
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
            float obstacleInitialSpeed = cactusManager.getInitialSpeed();
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
			// Zmiana rodzaju poziomu ze zwykłego na powietrzny i odwrotnie
			if(distanceInt % 100 == 0 && distanceInt != 0){
				if(distanceInt % 200 != 0){
					cactusManager.turnSkyLevelOn();
					birdManager.turnSkyLevelOn();
					coinManager.turnSkyLevelOn();
					player.turnSkyLevelOn();
				}else{
					cactusManager.turnSkyLevelOff();
					birdManager.turnSkyLevelOff();
					coinManager.turnSkyLevelOff();
				}
			}
			int playerFallDelay = 4; //do wyczucia
			if(((distanceInt-playerFallDelay) % 100 == 0) && ((distanceInt - playerFallDelay)% 200 == 0) && ((distanceInt- playerFallDelay)!=0)){
				player.turnSkyLevelOff();
			}


            distanceText.setString(L"" + std::to_wstring(distanceInt)); // Ustawienie tekstu odległości
            coinCountText.setString(L"" + std::to_wstring(currentCoinCount)); // Usunięcie napisu "Monety:"

            // Sprawdzenie kolizji i koniec gry lub restart
            if ((cactusManager.checkCollisions(player.getGlobalBounds())) || (birdManager.checkCollisions(player.getGlobalBounds()))) {
                coinCount += currentCoinCount;
                bestDistance = std::max(bestDistance, distance);
                deathCount++;
                StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed);
                gameState = GameOver;
                backgroundMusic.stop(); // Zatrzymanie muzyki tła dla gry
                deathSound.play(); // Odtwarzanie dźwięku śmierci
            }
        }

        // Aktualizacja przycisków i sliderów zależnie od stanu
        if (gameState == MainMenu) {
            if (menuMusic.getStatus() != sf::Music::Playing) {
                menuMusic.play(); // Odtwarzanie muzyki tła dla menu
            }
            storyButton.update(sf::Mouse::getPosition(window));
            endlessButton.update(sf::Mouse::getPosition(window));
            optionsButton.update(sf::Mouse::getPosition(window));
            achievementsButton.update(sf::Mouse::getPosition(window));
            statisticsButton.update(sf::Mouse::getPosition(window));
            exitButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == OptionsMenu) {
            easyButton.update(sf::Mouse::getPosition(window));
            normalButton.update(sf::Mouse::getPosition(window));
            hardButton.update(sf::Mouse::getPosition(window));
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Achievements) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Statistics) {
            backButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Gameplay) {
            pauseButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == Pause) {
            resumeButton.update(sf::Mouse::getPosition(window));
            restartButtonPause.update(sf::Mouse::getPosition(window));
            mainMenuButton.update(sf::Mouse::getPosition(window));
        } else if (gameState == GameOver) {
            restartButton.update(sf::Mouse::getPosition(window));
            // Remove the exit button from the GameOver screen
            // exitButton.update(sf::Mouse::getPosition(window));
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
            window.draw(coinIconMainMenu); // Draw the coin icon
            window.draw(coinCountMainMenuText); // Draw the coin count text
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            window.draw(optionsMenuBackgroundSprite); // Rysowanie tła trybu OptionsMenu
            easyButton.draw(window);
            normalButton.draw(window);
            hardButton.draw(window);
            musicSlider.draw(window);
            soundSlider.draw(window);
            backButton.draw(window);
        } else if (gameState == Achievements) {
            window.draw(achievementsBackgroundSprite); // Rysowanie tła trybu Achievements
            drawScrollableList(window, wAchievements, font);
            backButton.draw(window);
        } else if (gameState == Statistics) {
            window.draw(statisticsBackgroundSprite); // Rysowanie tła trybu Statistics
            drawScrollableList(window, statistics, font);
            backButton.draw(window);
        } else if (gameState == Gameplay) {
            window.draw(storyBackgroundSprite); // Rysowanie tła trybu fabularnego
            player.draw(window); // Rysowanie gracza
	        cactusManager.draw(window);  // Rysowanie przeszkód
            coinManager.draw(window);
            birdManager.draw(window);
            // window.draw(gameplayText); // Usunięcie napisu "Gejmplej!"
            pauseButton.draw(window);
            window.draw(dinoSprite); // Rysowanie dino2
            window.draw(distanceText); // Rysowanie tekstu odległości
            window.draw(coinSprite); // Rysowanie monety
            window.draw(coinCountText); // Rysowanie tekstu obok monety
        } else if (gameState == Pause) {
            window.draw(pauseText);
            window.draw(pauseBackgroundSprite); // Rysowanie tła trybu Pause
            resumeButton.draw(window);
            restartButtonPause.draw(window); // Draw the restart button for Pause
            mainMenuButton.draw(window); // Draw the main menu button for Pause

            // Move the dino icon to coordinates (375, 400) on the pause screen
            dinoSprite.setPosition(375, 400);

            // Move the distance counter to coordinates (445, 385) on the pause screen
            distanceText.setPosition(445, 385);

            // Move the coin icon to coordinates (625, 409) on the pause screen
            coinSprite.setPosition(625, 409);

            // Move the coin counter to coordinates (695, 385) on the pause screen
            coinCountText.setPosition(695, 385);

            window.draw(dinoSprite); // Draw the dino icon
            window.draw(distanceText); // Draw the distance counter
            window.draw(coinSprite); // Draw the coin icon
            window.draw(coinCountText); // Draw the coin counter
        } else if (gameState == GameOver) {
            window.clear(sf::Color::Black);
            window.draw(gameOverBackgroundSprite); // Rysowanie tła trybu GameOver
            restartButton.draw(window); // Draw the restart button for GameOver
            // Remove the exit button from the GameOver screen
            // exitButton.draw(window);

            // Center the distance and coin counters under the "Game Over" text
            bestScoreText.setPosition((window.getSize().x - bestScoreText.getGlobalBounds().width) / 2 - 45, gameOverText.getPosition().y + gameOverText.getGlobalBounds().height + 150);
            distanceText2.setPosition(bestScoreText.getPosition().x + bestScoreText.getGlobalBounds().width + 20, bestScoreText.getPosition().y);
            yourScoreText.setPosition((window.getSize().x - yourScoreText.getGlobalBounds().width) / 2 - 45, distanceText2.getPosition().y + distanceText2.getGlobalBounds().height + 10);
            distanceText.setPosition(yourScoreText.getPosition().x + yourScoreText.getGlobalBounds().width + 20, yourScoreText.getPosition().y);
            coinsText.setPosition((window.getSize().x - coinsText.getGlobalBounds().width) / 2 - 45, distanceText.getPosition().y + distanceText.getGlobalBounds().height + 10);
            coinCountText.setPosition(coinsText.getPosition().x + coinsText.getGlobalBounds().width + 20, coinsText.getPosition().y);

            distanceText.setFillColor(sf::Color::White); // Set the color of distanceText to white
            coinCountText.setFillColor(sf::Color::White); // Set the color of coinCountText to white
            distanceText2.setFillColor(sf::Color::White); // Set the color of distanceText2 to white

            window.draw(distanceText2); // Draw the second distance counter
            window.draw(yourScoreText); // Draw the "TWÓJ WYNIK:" text
            window.draw(distanceText); // Draw the distance counter
            window.draw(coinsText); // Draw the "MONETY:" text
            window.draw(coinCountText); // Draw the coin counter
            window.draw(bestScoreText); // Draw the best score text
        }
        window.display();
    }
    return 0;
}


