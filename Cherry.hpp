#ifndef CHERRY_HPP
#define CHERRY_HPP

#include "Plant.hpp"

class Cherry : public Plant {
private:
    float fuseTimer;
    bool readyToExplode;

public:
    Cherry(float startX, float startY, const sf::Texture& texture, int rowNum)
        : Plant(startX, startY, texture, rowNum, 300, 150), fuseTimer(1.2f), readyToExplode(false) {
        sprite.setTexture(texture);
        sprite.setPosition({x, y});
    }

    void attack() override {
        // dummy
    }

    void update(float dt) override {
        if (!isActive) return;

        // Odliczanie do wybuchu
        if (!readyToExplode) {
            fuseTimer -= dt;
            if (fuseTimer <= 0.0f) {
                readyToExplode = true;
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(sprite);
        }
    }

    bool isReadyToExplode() const { return readyToExplode; }
};

#endif