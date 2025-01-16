#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Obstacle.h"
#include "Difficulty.h"

class ObstacleManager {
public:
    ObstacleManager(float windowWidth, float windowHeight, std::string Type);
    void update(float deltaTime);
	void turnSkyLevelOn();
	void turnSkyLevelOff();
    void restart();
    void draw(sf::RenderWindow& window);
    void generateObstacle();
    bool checkCollisions(const sf::FloatRect& playerBounds);
    void removeOffscreenObstacles();
    std::vector<sf::FloatRect> getObstacleBounds();
    float getRandomSpawnInterval(float baseInterval);
    void setSpeed(float speed);
    float getSpeed();
    float getInitialSpeed();
    void setDifficulty(Difficulty difficulty);
private:
    Difficulty difficulty;
    std::string obstacleType;
    float screenWidth;
    float screenHeight;
	bool isSkyLevelOn;
	float skyLevelBirdSpawnInterval = 0.8f;
	float skyLevelCactusSpawnInterval = 0.5f;
    sf::Texture cactusTexture;
    sf::Texture birdTexture;
	float skyLevelPause = 5.0f;
    float groundHeight = 589.0f;
    float veryHighSkyHeight = 200.0f; 
    int   skyLanesCount = 5;
	int   skyLevelSkyLanesCount = 35;
    float skyLaneHeight = 15.0f;
    float skyMinHeight = 525.0f;
    float cactusHeight = 80.0f; //to też
    float cactusWidth = 50.0f; //to też
    float birdHeight = 30.0f; //to chyba staje się nieistotne po załadowaniu tekstury
    float birdWidth = 30.0f; //to też
    std::vector<Obstacle> obstacles;
    int birdCounter;
    float obstacleSpawnTimer;
    float spawnInterval;
    float speed;
    float initialCactusSpeed = 250.0f;
    float initialBirdSpeed = 400.0f;
    float defaultInitialSpeed = 250.0f;
    float baseInterval = 1.5f;
};

#endif // OBSTACLEMANAGER_H

