#ifndef COIN_MANAGER_H
#define COIN_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>

class CoinManager {
private:
    struct Coin {
        sf::CircleShape shape;
        Coin(sf::Vector2f position, float radius) {
            shape.setRadius(radius);
            shape.setFillColor(sf::Color::Yellow);
            shape.setPosition(position);
        }
    };
    std::vector<Coin> coins;  
    float spawnTimer; // Czas od ostatniego wygenerowania monety
    float spawnInterval; // Czas miedzy generowaniem monet
    float speed;             

public:
    CoinManager(float spawnInterval, float speed); 
    void update(float deltaTime, sf::FloatRect playerBounds, int& coinCount, const std::vector<sf::FloatRect>& obstacleBounds);
    void draw(sf::RenderWindow& window); 
};

#endif
