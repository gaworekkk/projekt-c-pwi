#include "Slider.h"
#include <iostream>
#include <stdexcept>

bool Slider:: isAnySliderDragging = false;

Slider::Slider(float x, float y, float width, float height, float volume, sf::Font& font){
    track.setPosition(x, y);
    track.setSize(sf::Vector2f(width, height));
    track.setFillColor(sf::Color::Transparent);

    thumb.setSize(sf::Vector2f(height, height));
    if (!thumbTexture.loadFromFile("Tekstury/slider.png")) {
        throw std::runtime_error("Nie udało się załadować tekstury suwaka!");
    } else {
        thumb.setTexture(&thumbTexture);
    }
    thumb.setPosition(x + (volume - minValue) / (maxValue - minValue) * width - height / 2, y);
    value = volume;

    valueText.setFont(font);
    valueText.setCharacterSize(20);
    valueText.setFillColor(sf::Color::Black);
    valueText.setString(std::to_string(static_cast<int>(value)));
    valueText.setPosition(x + width + 10, y - 3);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(thumb);
    window.draw(valueText);
}

void Slider::update(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (!isAnySliderDragging && thumb.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            isDragging = true;
            isAnySliderDragging = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        isDragging = false;
        isAnySliderDragging = false;
    }
    if (isDragging && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        float newX = sf::Mouse::getPosition(window).x - track.getPosition().x;
        if (newX < 0) newX = 0;
        if (newX > track.getSize().x) newX = track.getSize().x;
        thumb.setPosition(track.getPosition().x + newX - thumb.getSize().x / 2, thumb.getPosition().y);
        value = minValue + (maxValue - minValue) * (newX / track.getSize().x);
        valueText.setString(std::to_string(static_cast<int>(value)));
    }
}

float Slider::getValue() const {
    return value;
}

void Slider::reset() {
    isDragging = false;
    isAnySliderDragging = false;
}