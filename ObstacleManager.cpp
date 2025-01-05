#include "ObstacleManager.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <random>
#include <chrono>
enum GameState { MainMenu, OptionsMenu, Gameplay, Pause, GameOver }; 

extern GameState gameState;

ObstacleManager::ObstacleManager(float windowWidth, float windowHeight, std::string Type)
    : screenWidth(windowWidth),  obstacleType(Type), screenHeight(windowHeight), obstacleSpawnTimer(0.f) {
	    rng.seed(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
    randomSpawnTime = 2.0f; // Ustaw pierwszy czas
    if (obstacleType == "bird") {
        spawnTimeDistribution = std::uniform_real_distribution<float>(5.0f, 12.0f);
    } else if (obstacleType == "cactus") {
        spawnTimeDistribution = std::uniform_real_distribution<float>(3.0f, 5.0f);
    }
}

void ObstacleManager::update(float deltaTime) {
    obstacleSpawnTimer += deltaTime;
    if (obstacleSpawnTimer >= randomSpawnTime) {
        obstacleSpawnTimer = 0.0f;
        randomSpawnTime = spawnTimeDistribution(rng);
        generateObstacle();
    }

    // Aktualizacja przeszkód
    for (auto& obstacle : obstacles) {
        obstacle.update(deltaTime);
    }

    // Usuwanie przeszkód poza ekranem
    removeOffscreenObstacles();

}

void ObstacleManager::draw(sf::RenderWindow& window) {
    for (auto& obstacle : obstacles) {
        obstacle.draw(window);
    }
}

void ObstacleManager::generateObstacle() {
    float obstacleHeight; 
    float obstacleWidth;
    float obstacleY;
    float obstacleSpeed;
    if(obstacleType == "bird"){
	obstacleHeight = birdHeight;  //  wysokość przeszkody
        obstacleY = skyHeight - obstacleHeight;  // przeszkoda jest w powietrzu
	obstacleWidth = birdWidth;
	obstacleSpeed = birdSpeed;
    }else if(obstacleType == "cactus"){
	obstacleHeight = cactusHeight;
	obstacleY = groundHeight - obstacleHeight;
	obstacleWidth = cactusWidth;
	obstacleSpeed = cactusSpeed;
    }
    // Tworzenie przeszkody po prawej stronie ekranu
    obstacles.push_back(Obstacle(screenWidth, obstacleY, obstacleWidth, obstacleHeight, obstacleSpeed));
}

bool ObstacleManager::checkCollisions(const sf::FloatRect& playerBounds) {
    for (auto& obstacle : obstacles) {
        if (playerBounds.intersects(obstacle.getBounds())) {
            gameState = GameOver;
            return true;
        }
    }
    return false;
}

void ObstacleManager::removeOffscreenObstacles() {
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obstacle) {
        return obstacle.getBounds().left + obstacle.getBounds().width < 0; // Sprawdzanie, czy przeszkoda jest poza ekranem
    }), obstacles.end());
}
void ObstacleManager::restart() {
    obstacles.clear();  // Usuwamy wszystkie przeszkody
    obstacleSpawnTimer = 0.f;   // Resetujemy timer spawnu
}

std::vector<sf::FloatRect> ObstacleManager::getObstacleBounds() const {
    std::vector<sf::FloatRect> bounds;
    for (const auto& obstacle : obstacles) {
        bounds.push_back(obstacle.getBounds());
    }
    return bounds;
}

