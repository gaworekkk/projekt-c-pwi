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
#include "ResourceLoader.h"
#include <stdexcept>
#include <iostream>
#include <algorithm> 

enum GameState { MainMenu, OptionsMenu, Achievements, Statistics, Gameplay, Pause, GameOver, Shop };

// Ustawienie domyślnego stanu gry
GameState gameState = MainMenu;
// Poziom trudności
Difficulty difficulty;

int main() {
    // Ustawienia okna
    sf::VideoMode videoMode(1200, 640);
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close;

    // Tworzenie okna
    sf::RenderWindow window(videoMode, "Dino Game", style);
    window.setFramerateLimit(60); // Ograniczenie FPS do 60
    
    // Zmienne stanu gry
    int coinCount = 0, currentCoinCount, cactusCount = 0, birdCount = 0;
    float distance;

    // Zaladowanie statystyk
    int bestDistance = 0, totalDistance = 0, jumpCount = 0, deathCount = 0, gamesPlayed = 0;
    int skinState[6];
    float musicVolume, soundVolume;
    StatisticsManager::loadStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
    // Załaduj wszystkie zasoby
    try {
        loadAllResources(window, coinCount, musicVolume, soundVolume);
    } catch (const std::runtime_error& e) {
        // W przypadku błędu wyświetl komunikat i zakończ program
        std::cerr << e.what() << std::endl;
        return -1;
    }
    cactusManager.setDifficulty(difficulty);
    birdManager.setDifficulty(difficulty);
    menuMusic.setVolume(musicVolume);
    backgroundMusic.setVolume(musicVolume);
    buttonSound.setVolume(soundVolume);
    jumpSound.setVolume(soundVolume);
    deathSound.setVolume(soundVolume);

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
    statistics.push_back(L"Liczba kaktusów: " + std::to_wstring(cactusCount));
    statistics.push_back(L"Liczba ptaków: " + std::to_wstring(birdCount));

    
    float deltaTime;
    sf::Clock clock;

    // Zmienne "zapadki"

    bool if_changed_speed = false;
    bool isJumping = false;
    const float standardLevel = 500.0f; // Standardowy poziom gracza
    sf::Texture aktualna;
    int pressed;
    for(int i=0;i<6;i++){
        if (skinState[i]==0){
            pressed=i;
            buyButton[i]->setText("Wybrano");
            player.setTexture(dinoTexturePath[i]);
            buyButtonSkin.setText("Wybrano");
        }
    }

    // Zmienne do śledzenia miniętych przeszkód
    std::vector<bool> cactusPassed;
    std::vector<bool> birdPassed;

    // Główna pętla gry
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                totalDistance += static_cast<int>(distance);
                StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                window.close();
            }
                 

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
                    statistics[6] = L"Liczba miniętych kaktusów: " + std::to_wstring(cactusCount);
                    statistics[7] = L"Liczba miniętych ptaków: " + std::to_wstring(birdCount);
                    if (storyButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        distance = 0.0f;
                        currentCoinCount = 0.0f;
                        cactusManager.restart();
			            birdManager.restart();
                        coinManager.restart();
                        gameState = Gameplay; // Przejście do Gameplay
                        gamesPlayed++;
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
                        gameState = Gameplay; // Przejście do Gameplay
                        gamesPlayed++;
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
                    if (shopButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = Shop; // Przejście do Shop
                        aktualna=player.getTexture();
                        player.setPosition(sf::Vector2f(800, 100));
                        player.setSize(sf::Vector2f(200, 200));
                        StatisticsManager::loadStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
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
                        musicSlider->reset();
                        soundSlider->reset();
                        musicVolume = musicSlider->getValue();
                        soundVolume = soundSlider->getValue();
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
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
                        totalDistance += static_cast<int>(distance);
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
                        gamesPlayed++;
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
                        totalDistance += static_cast<int>(distance);
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
                    bestDistance = std::max(bestDistance, static_cast<int>(distance));
                    coinCount += currentCoinCount;
                    StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                    currentCoinCount = 0.0f;
                    coinCountMainMenuText.setString(std::to_wstring(coinCount));
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
                        gamesPlayed++;
                        backgroundMusic.play(); // Odtwarzanie muzyki tła od nowa
                    }
                    // Remove the exit button functionality from the GameOver screen
                    // if (exitButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                    //     buttonSound.play(); // Odtwarzanie dźwięku przycisku
                    //     window.close(); // Zamknięcie okna
                    // }
                } else if (gameState == Shop) {
                    if (backButton.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        gameState = MainMenu; // Powrót do MainMenu
                        player.resetPosition();
                        player.resetSize();
                        player.setFrameDuration(0.1f);
                        player.setTexture(aktualna);
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        for (int i = 0;i<6;i++){
                            if (skinState[i] == 0){
                                buyButton[i]->setText("Wybrano");
                            } else {
                                buyButton[i]->setText("Skin" + std::to_string(i+1));
                            }
                        }
                    }
                    
                    for (int i=0;i<6;i++){
                        if (buyButton[i]->isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                            buttonSound.play(); // Odtwarzanie dźwięku przycisku
                            player.setTexture(dinoTexturePath[i]); 
                            pressed = i;
                            buyButton[i]->setText("Wybrano");
                            for (int j=0;j<6;j++){
                                if (j!=i){
                                    buyButton[j]->setText("Skin" + std::to_string(j+1));
                                }
                            }
                            if (skinState[i] == 0){
                                buyButtonSkin.setText("Wybrano");
                            } else if (skinState[i] == 2){
                                buyButtonSkin.setText("Kup");
                            } else {
                                buyButtonSkin.setText("Wybierz");
                            }
                        }
                    }
                    if (buyButtonSkin.isClicked(sf::Mouse::getPosition(window), event.mouseButton)) {
                        buttonSound.play(); // Odtwarzanie dźwięku przycisku
                        if (skinState[pressed] == 2){
                            if (coinCount >= 1){
                                coinCount -= 1;
                                coinCountMainMenuText.setString(std::to_wstring(coinCount));
                                skinState[pressed] = 0;
                                aktualna=player.getTexture();
                                buyButtonSkin.setText("Wybrano");
                                for (int j=0;j<6;j++){
                                    if (j!=pressed && skinState[j] == 0){
                                        skinState[j] = 1;
                                    }
                                }
                            }
                        } else if (skinState[pressed] == 1){
                            skinState[pressed] = 0;
                            buyButtonSkin.setText("Wybrano");
                            aktualna=player.getTexture();
                            for (int j=0;j<6;j++){
                                if (j!=pressed && skinState[j] != 2){
                                    skinState[j] = 1;
                                }
                            }
                        }
                    }
                }
            }

            // Obsługa klawiszy w różnych stanach
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gameState == OptionsMenu) {
                        cactusManager.setDifficulty(difficulty);
                        birdManager.setDifficulty(difficulty);
                        musicSlider->reset();
                        soundSlider->reset();
                        musicVolume = musicSlider->getValue();
                        soundVolume = soundSlider->getValue();
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
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
                    else if (gameState == Shop) {
                        gameState = MainMenu; // Powrót do MainMenu
                        player.resetPosition();
                        player.resetSize();
                        player.setFrameDuration(0.1f);
                        player.setTexture(aktualna);
                        StatisticsManager::saveStatistics(coinCount, bestDistance, totalDistance, jumpCount, deathCount, gamesPlayed, cactusCount, birdCount, skinState, musicVolume, soundVolume, difficulty);
                        for (int i = 0;i<6;i++){
                            if (skinState[i] == 0){
                                buyButton[i]->setText("Wybrano");
                            } else {
                                buyButton[i]->setText("Skin" + std::to_string(i+1));
                            }
                        }
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
                        gamesPlayed++;
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
                musicSlider->update(event, window);
                soundSlider->update(event, window);
                musicVolume = musicSlider->getValue();
                soundVolume = soundSlider->getValue();
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
			if(distanceInt % 25 == 0 && distanceInt != 0){
				if(distanceInt % 50 != 0){
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
			if(((distanceInt-playerFallDelay) % 25 == 0) && ((distanceInt - playerFallDelay)% 50 == 0) && ((distanceInt- playerFallDelay)!=0)){
				player.turnSkyLevelOff();
			}

            distanceText.setString(L"" + std::to_wstring(distanceInt)); // Ustawienie tekstu odległości
            coinCountText.setString(L"" + std::to_wstring(currentCoinCount)); // Usunięcie napisu "Monety:"

            // Sprawdzenie kolizji i koniec gry lub restart
            if ((cactusManager.checkCollisions(player.getGlobalBounds())) || (birdManager.checkCollisions(player.getGlobalBounds()))) {
                deathCount++;
                totalDistance += static_cast<int>(distance); 
                gameState = GameOver;
                backgroundMusic.stop(); // Zatrzymanie muzyki tła dla gry
                deathSound.play(); // Odtwarzanie dźwięku śmierci
            }

            // Zliczanie miniętych kaktusów i ptaków
            if (cactusPassed.size() != cactusBounds.size()) {
                cactusPassed.assign(cactusBounds.size(), false);
            }
            if (birdPassed.size() != birdBounds.size()) {
                birdPassed.assign(birdBounds.size(), false);
            }

            for (size_t i = 0; i < cactusBounds.size(); ++i) {
                if (!cactusPassed[i] && cactusBounds[i].left + cactusBounds[i].width < player.getGlobalBounds().left) {
                    cactusCount++;
                    cactusPassed[i] = true;
                }
            }
            for (size_t i = 0; i < birdBounds.size(); ++i) {
                if (!birdPassed[i] && birdBounds[i].left + birdBounds[i].width < player.getGlobalBounds().left) {
                    birdCount++;
                    birdPassed[i] = true;
                }
            }
        } else if (gameState == Shop) {
            player.updateAnimation(deltaTime);
            player.setFrameDuration(0.5f);
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
            shopButton.update(sf::Mouse::getPosition(window));
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
        } else if (gameState == Shop) {
            backButton.update(sf::Mouse::getPosition(window));
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
            shopButton.draw(window);
            window.draw(nameText);
            window.draw(coinIconMainMenu); // Draw the coin icon
            window.draw(coinCountMainMenuText); // Draw the coin count text
        } else if (gameState == OptionsMenu) {
            window.draw(optionsMenuText);
            window.draw(optionsMenuBackgroundSprite); // Rysowanie tła trybu OptionsMenu
            easyButton.draw(window);
            normalButton.draw(window);
            hardButton.draw(window);
            musicSlider->draw(window);
            soundSlider->draw(window);
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
        } else if (gameState == Shop) {
            window.draw(shopBackgroundSprite); // Rysowanie tła trybu Shop
            window.draw(coinIconMainMenu); // Draw the coin icon
            window.draw(coinCountMainMenuText); // Draw the coin count text
            backButton.draw(window);
            buyButtonSkin.draw(window);
            for (int i = 0; i < 6; i++) {
                buyButton[i]->draw(window);
            }
            player.draw(window);
        }
        window.display();
    }

    // sound.stop();
    // music.stop();

    for (int i = 0; i < 6; i++) {
        delete buyButton[i];
    }
    delete musicSlider;
    delete soundSlider;
    return 0;
}


