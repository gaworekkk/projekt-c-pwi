#include "ObstacleManager.h"
#include <cstdlib>
#include <ctime>
#include <string>

enum GameState { MainMenu, OptionsMenu, Gameplay, Pause, GameOver }; 
extern GameState gameState;

ObstacleManager::ObstacleManager(float windowWidth, float windowHeight, std::string Type)
    : screenWidth(windowWidth), birdCounter(0), screenHeight(windowHeight), isSkyLevelOn(false), obstacleType(Type), obstacleSpawnTimer(0.f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    if(!cactusTexture.loadFromFile("Tekstury/kaktusy/kaktus(50x84).png") || !birdTexture.loadFromFile("Tekstury/crow.gif")){
	    //obsluz blad
    }
    if(obstacleType == "cactus"){
	    speed = initialCactusSpeed;
    }else if(obstacleType == "bird"){
	    speed = initialBirdSpeed;
    }else{
	    speed = defaultInitialSpeed;
    }
    spawnInterval = getRandomSpawnInterval(baseInterval);
}
void ObstacleManager::turnSkyLevelOn() {
	isSkyLevelOn = true;
	spawnInterval = skyLevelPause;
}
void ObstacleManager::turnSkyLevelOff() {
	isSkyLevelOn = false;
}
void ObstacleManager::update(float deltaTime) {
    obstacleSpawnTimer += deltaTime;
		
    // Generowanie przeszkód co pewien czas
    if (obstacleSpawnTimer >= spawnInterval) {
		if(isSkyLevelOn){

			generateObstacle();
			obstacleSpawnTimer = 0.f;
			if(obstacleType == "bird"){
				spawnInterval = skyLevelBirdSpawnInterval;
			}else if(obstacleType == "cactus"){
				spawnInterval = skyLevelCactusSpawnInterval;
			}
		}else{
        	generateObstacle();
        	obstacleSpawnTimer = 0.f;
        	spawnInterval = getRandomSpawnInterval(baseInterval);
		}
    }

    // Aktualizacja przeszkód
    for (auto& obstacle : obstacles) {
        obstacle.update(deltaTime, speed);
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
    sf::Texture obstacleTexture;
    if(obstacleType == "cactus"){
	    obstacleY = groundHeight - cactusHeight;
	    obstacles.push_back(Obstacle(screenWidth, groundHeight - cactusHeight, cactusWidth, cactusHeight, cactusTexture));    
    }else if(obstacleType == "bird"){
		int skyLaneNum;
		if(isSkyLevelOn){
			skyLaneNum = rand()%skyLevelSkyLanesCount;
			obstacleY = skyMinHeight - birdHeight - skyLaneHeight*skyLaneNum;
		}else{
	    	birdCounter++;
	    	skyLaneNum = rand()%skyLanesCount;
	    	if(birdCounter == 8){
		    	birdCounter = 0;
		    	obstacleY = veryHighSkyHeight - skyLaneHeight*skyLaneNum;
	    	}else{
			    obstacleY = skyMinHeight - birdHeight - skyLaneHeight*skyLaneNum;		
		}
		}
	    obstacles.push_back(Obstacle(screenWidth, obstacleY, birdWidth, birdHeight, birdTexture));

    }

    // Tworzenie przeszkody po prawej stronie ekranu
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
	turnSkyLevelOff(); //Wyłączamy poziom powietrzny
    obstacles.clear();  // Usuwamy wszystkie przeszkody
    setSpeed(getInitialSpeed());  // Resetujemy prędkość
    obstacleSpawnTimer = 0.f;   // Resetujemy timer spawnu
}

std::vector<sf::FloatRect> ObstacleManager::getObstacleBounds() const {
    std::vector<sf::FloatRect> bounds;
    for (const auto& obstacle : obstacles) {
        bounds.push_back(obstacle.getBounds());
    }
    return bounds;
}

float ObstacleManager::getRandomSpawnInterval(float baseInterval) const {
    return baseInterval + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 2.0f)); 
    // Losowe generowanie czasu spawnu przeszkód (od baseInterval do baseInterval + 2)
}

float ObstacleManager::getSpeed() const {
    return speed;
}

void ObstacleManager::setSpeed(float newSpeed) {
    // Zmiana prędkości przeszkody
    speed = newSpeed;
}
float ObstacleManager::getInitialSpeed() const {
	if(obstacleType == "cactus"){
		return initialCactusSpeed;
	}else if(obstacleType == "bird"){
		return initialBirdSpeed;
	}else{
		return defaultInitialSpeed;
	}
}

void ObstacleManager::setDifficulty(Difficulty newDifficulty) {
    difficulty = newDifficulty;
    switch (difficulty) {
        case Easy:
            if(obstacleType == "cactus"){
                initialCactusSpeed = 180.0f;
            } else if(obstacleType == "bird"){
                initialBirdSpeed = 280.0f;
            }
            baseInterval = 6.0f;
            break;
        case Normal:
            if(obstacleType == "cactus"){
                initialCactusSpeed = 250.0f;
            } else if(obstacleType == "bird"){
                initialBirdSpeed = 400.0f;
            }
            baseInterval = 1.5f;
            break;
        case Hard:
            if(obstacleType == "cactus"){
                initialCactusSpeed = 400.0f;
            } else if(obstacleType == "bird"){
                initialBirdSpeed = 550.0f;
            }
            baseInterval = 0.5f;
            break;
    }
}
