#ifndef WALLNUT_HPP
#define WALLNUT_HPP

#include "Plant.hpp"

class Wallnut : public Plant {
private:
    std::vector<sf::IntRect> healthFrames;
public:
    Wallnut(float startX, float startY, const sf::Texture& texture, int rowNum)
    : Plant(startX, startY, texture, rowNum, 3000, 50) {

        healthFrames.push_back(sf::IntRect({0, 0}, {40, 40}));
        healthFrames.push_back(sf::IntRect({40, 0}, {40, 40}));
        healthFrames.push_back(sf::IntRect({80, 0}, {40, 40}));

        sprite.setTexture(texture);
        sprite.setTextureRect(healthFrames[0]);

        sprite.setOrigin({6.0f, 4.0f});
        sprite.setScale({2.0f, 2.0f});
        sprite.setPosition({x, y});
    }

    
    void attack() override {
        // dummy
    }

    void update(float dt) override {
        // animation for nut breaking
        if (!isActive) return;
        if (hp > 2000) {
            sprite.setTextureRect(healthFrames[0]);
        } else if (hp > 1000) {
            sprite.setTextureRect(healthFrames[1]);
        } else {
            sprite.setTextureRect(healthFrames[2]);
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) window.draw(sprite);
    }
};

#endif