#ifndef PEA_HPP
#define PEA_HPP

#include "GameObj.hpp"

class Pea : public GameObject {
private:
    float speed;
    int damage;
    int row;
    sf::CircleShape shape;

public:
    Pea(float startX, float startY, int rowNum, int dmg = 20, float spd = 350.0f)
        : GameObject(startX, startY, 16.0f, 16.0f), speed(spd), damage(dmg), row(rowNum) {

        width = 16.0f;
        height = 16.0f;
        isActive = true;
        
        //pea sprite
        shape.setRadius(8.0f);
        shape.setFillColor(sf::Color(50, 205, 50));
        shape.setPosition({x, y});
        }

    void update(float dt) override {
        if (!isActive) return;

        x += speed * dt;
        shape.setPosition({x, y});

        if (x > 1200.0f) {
            destroy();
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(shape);
        }
    }

    //getters
    int getRow() const { return row; }
    int getDamage() const { return damage; }
};

#endif