#ifndef PEA_HPP
#define PEA_HPP

#include "GameObj.hpp"

class Pea : public GameObject {
private:
    float speed;
    int damage;
    int row;
    sf::Sprite sprite;

public:

    ObjectType getType() const override { return ObjectType::Pea; }

    Pea(float startX, float startY, int rowNum, const sf::Texture& tex, int dmg = 20, float spd = 350.0f)
        : GameObject(startX, startY, 16.0f, 16.0f), sprite(tex), speed(spd), damage(dmg), row(rowNum) {

        isActive = true;
        
        //pea sprite
        sprite.setTexture(tex);
        sprite.setPosition({x, y});
        //centering origin
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        }
    void update(float dt) override {
        if (!isActive) return;

        x += speed * dt;
        sprite.setPosition({x, y});

        if (x > 1200.0f) {
            destroy();
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(sprite);
        }
    }

    //getters
    int getRow() const { return row; }
    int getDamage() const { return damage; }
    sf::Color getColor() const { return sprite.getColor(); }
};

#endif