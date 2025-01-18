#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

class Slider {
public:
    Slider(float x, float y, float width, float height, float volume, sf::Font& font);
    void draw(sf::RenderWindow& window);
    void update(const sf::Event& event, const sf::RenderWindow& window);
    float getValue() const;
    void reset();

private:
    sf::RectangleShape track;
    sf::RectangleShape thumb;
    sf::Texture thumbTexture;
    float minValue = 0;
    float maxValue = 100;
    float value = 100;
    sf::Text valueText;
    bool isDragging = false;
    static bool isAnySliderDragging;
};

#endif // SLIDER_H