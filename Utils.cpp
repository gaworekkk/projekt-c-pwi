#include "Utils.h"

// Funkcja do wyśrodkowania tekstu w oknie o podanych wymiarach
void centerText(sf::Text& text, float windowWidth, float windowHeight, float verticalPosition) {
    // Obliczanie szerokości tekstu
    sf::FloatRect textBounds = text.getLocalBounds();
    
    // Ustawienie originu na środek szerokości tekstu
    text.setOrigin(
        textBounds.left + textBounds.width / 2,  // Środek szerokości tekstu
        textBounds.top                            // Górna krawędź tekstu
    );
    
    // Ustawienie pozycji tekstu na środku okna poziomo oraz wybranej pozycji pionowo
    text.setPosition(windowWidth / 2, verticalPosition);
}

void drawScrollableList(sf::RenderWindow& window, const std::vector<std::wstring>& items, sf::Font& font) {
    sf::View view = window.getDefaultView();
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    window.setView(view);

    float yOffset = 150; 
    for (const auto& item : items) {
        sf::Text text(item, font, 30);
        text.setPosition(200, yOffset); 
        window.draw(text);
        yOffset += 40; 
    }
}