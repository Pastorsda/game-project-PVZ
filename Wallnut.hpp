#ifndef WALLNUT_HPP
#define WALLNUT_HPP

#include "Plant.hpp"

class Wallnut : public Plant {
public:
    Wallnut(float startX, float startY, const sf::Texture& texture, int rowNum)
    : Plant(startX, startY, texture, rowNum, 1000, 50) {
        sprite.setPosition({x, y});
    }

    
    void attack() override {
        // dummy
    }

    void update(float dt) override {
        // animation for nut breaking
        if (!isActive) return;
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) window.draw(sprite);
    }
};

#endif