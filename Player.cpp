#include "Player.h"
#include <algorithm> // Do użycia std::max

// Konstruktor
Player::Player(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color) {
    player.setSize(size);
	isSkyLevelOn = false;
    player.setPosition(position);
    player.setFillColor(color);
    if(!playerTexture.loadFromFile("Tekstury/skórki dino/dino4(run).gif")){
	    //błąd
    }
    player.setTexture(&playerTexture);
    velocity = sf::Vector2f(0.f, 0.f);
    isCrouching = false;
    originalSize = size;
    crouchSize = sf::Vector2f(size.x, 0.6*size.y);
    isJumping = false;
    isHoldingJump = false;
    jumpHoldTime = 0.f;
}
void Player::turnSkyLevelOn(){
	isSkyLevelOn = true;
}
void Player::turnSkyLevelOff(){
	isSkyLevelOn = false;
}
// Operator przypisania
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        player = other.player;
        velocity = other.velocity;
        isJumping = other.isJumping;
        isHoldingJump = other.isHoldingJump;
        jumpHoldTime = other.jumpHoldTime;
		playerTexture = other.playerTexture;
		player.setTexture(&playerTexture);
		isSkyLevelOn = other.isSkyLevelOn;
	
    }
    return *this;
}
sf::FloatRect Player::getGlobalBounds() {
    return player.getGlobalBounds();
}
// Obsługa wejścia gracza
void Player::handleInput(float deltaTime) {

    // Ruch w lewo i prawo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -moveSpeed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = moveSpeed;
    } else {
        velocity.x = 0.f;
    }

    // Kucanie (tylko jeśli gracz obecnie nie skacze)
    if (!isJumping && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
        if (!isCrouching) {
            player.setSize(crouchSize);
            player.setPosition(player.getPosition().x, groundHeight - crouchSize.y); // Dopasowanie pozycji
            isCrouching = true;
        }
    } else if (isCrouching) {
        player.setSize(originalSize);
        player.setPosition(player.getPosition().x, groundHeight - originalSize.y); // Przywrócenie pozycji
        isCrouching = false;
    }

    // Skakanie obsługa poziomu zwykłego i powietrznego
	if(!isSkyLevelOn){
    	if(!isCrouching && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))) {
        	if(!isJumping) {
            	velocity.y = -baseJumpSpeed;
            	isJumping = true;
            	isHoldingJump = true;
            	jumpHoldTime = 0.f;
        	}else if (isHoldingJump && jumpHoldTime < maxHoldTime) {
        	    jumpHoldTime += deltaTime;
        	    velocity.y = std::max(velocity.y - jumpBoost * deltaTime, -maxJumpSpeed);
        	}
    	}else{
    	    isHoldingJump = false;
   	 	}
	}else{
		if(!isCrouching && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))){
			velocity.y = -baseJumpSpeed;
			isJumping = true;
		}
	}
}

// Aktualizacja pozycji gracza
void Player::update(float deltaTime) {
    // Grawitacja
    velocity.y += gravity * deltaTime;

    // Poruszanie graczem
    player.move(velocity * deltaTime);

    // Ograniczenie pozycji gracza do "podłogi"
    if (player.getPosition().y + player.getSize().y >= groundHeight) {
        player.setPosition(player.getPosition().x, groundHeight - player.getSize().y);
        velocity.y = 0.f;
        isJumping = false;
    }
    // Ograniczenie pozycji gracza "ścian"

    // Lewa krawędź
    if (player.getPosition().x < 0.f) {
        player.setPosition(0.f, player.getPosition().y);
        velocity.x = 0.f;
    }
    // Prawa krawędź
    if (player.getPosition().x + player.getSize().x > screenWidth) {
        player.setPosition(screenWidth - player.getSize().x, player.getPosition().y);
        velocity.x = 0.f;
    }
	// Górna krawędź (potrzebne w poziomie powietrznym)
	if(player.getPosition().y <= skyHeight){
		player.setPosition(player.getPosition().x, skyHeight);
	}
}

// Rysowanie gracza
void Player::draw(sf::RenderWindow& window) {
    window.draw(player);
}

sf::Vector2f Player::getVelocity() const {
    return velocity;
}
