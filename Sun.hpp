#ifndef SUN_HPP
#define SUN_HPP

#include "GameObj.hpp" 
#include <SFML/Graphics.hpp>
#include <random> // NOWOŚĆ: do wylosowania wysokości lądowania

class Sun : public GameObject {
private:
    float lifetime;
    const float maxlifetime = 8.0f;
    float targetY;
    const float speed = 90.0f; 
    bool isFalling;
    sf::Sprite sprite;
public:
    Sun(float startX, float startY, const sf::Texture& tex, bool falling)
    : GameObject(startX, startY, 30.0f, 30.0f), sprite(tex), lifetime(0.0f), isFalling(falling){

        isActive = true;
        sprite.setTexture(tex);
        sprite.setPosition({x, y});

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});

        // random "height" where sun falls
        if (isFalling) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> yDist(150.0f, 650.0f);
        targetY = yDist(gen);
        } else {
            targetY = y + 60.0f;
        }
    }

    void update(float dt) override {
        if (!isActive) return;

        // falling logic
        if (y < targetY) {
            y += speed * dt;
            sprite.setPosition({x, y});
        } 
        else {
            // sun arrived on spot
            lifetime += dt;
            if (lifetime >= maxlifetime) {
                destroy();
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(sprite);
        }
    }
};

#endif