#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Obstacle.h"
class ObstacleManager {
public:
    ObstacleManager(float windowWidth, float windowHeight, std::string Type);
    void update(float deltaTime);
    void restart();
    void draw(sf::RenderWindow& window);
    void generateObstacle();
    bool checkCollisions(const sf::FloatRect& playerBounds);
    void removeOffscreenObstacles();
    std::vector<sf::FloatRect> getObstacleBounds() const; 
    float getRandomSpawnInterval() const;
    void setSpeed(float speed);
    float getSpeed() const;
    void setInitialSpeed(float speed);
    float getInitialSpeed() const;
private:
    std::string obstacleType;
    float screenWidth;
    float screenHeight;
    sf::Texture cactusTexture;
    sf::Texture birdTexture;
    const float groundHeight = 589.0f;
    const float veryHighSkyHeight = 200.0f; 
    const int   skyLanesCount = 5;
    const float skyLaneHeight = 15.0f;
    const float skyMinHeight = 535.0f;
    const float cactusHeight = 80.0f; //to też
    const float cactusWidth = 50.0f; //to też
    const float birdHeight = 30.0f; //to chyba staje się nieistotne po załadowaniu tekstury
    const float birdWidth = 30.0f; //to też
    std::vector<Obstacle> obstacles;
    int birdCounter;
    float obstacleSpawnTimer;
    float spawnInterval;
    float speed;
    float initialCactusSpeed = 250.0f;
    float initialBirdSpeed = 400.0f;
    float defaultInitialSpeed = 250.0f;
};

#endif // OBSTACLEMANAGER_H

