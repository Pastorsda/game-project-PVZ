#ifndef SUN_HPP
#define SUN_HPP

#include "GameObj.hpp" 
#include <SFML/Graphics.hpp>

class Sun : public GameObject {
private:
    float lifetime;
    const float maxlifetime = 10.0f;
    sf::Sprite sprite;
public:
    Sun(float startX, float startY, const sf::Texture& tex)
    : GameObject(startX, startY, 30.0f, 30.0f), sprite(tex), lifetime(0.0f) {

        isActive = true;
        sprite.setTexture(tex);
        sprite.setPosition({x, y});

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    }
    void update(float dt) override {
        if (!isActive) return;

        lifetime += dt;
        if (lifetime >= maxlifetime) {
            destroy();
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(sprite);
        }
    }
};

#endif