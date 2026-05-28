#ifndef PEASHOOTER_HPP
#define PEASHOOTER_HPP

#include "Plant.hpp"
#include <iostream>

class Peashooter : public Plant {
    private:
    float shootTimer;
    public:
    Peashooter(float startX, float startY, const sf::Texture& texture, int rowNum)
    : Plant(startX, startY, texture, rowNum, 300, 100), shootTimer(0.0f){}

    void update(float dt) override {
        if (!isActive) return;
        //shoot timer every 2s
        shootTimer += dt;
        if (shootTimer >= 2.0f) {
            attack();
            shootTimer = 0.0f;
        }
        sprite.setPosition({x, y});
    }

    void attack() override {
        std::cout << "[PEASHOOTER] Pew! in row: " << row << "\n";
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(sprite);
        }
    }
};

#endif