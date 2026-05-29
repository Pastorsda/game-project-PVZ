#ifndef SUNFLOWER_HPP
#define SUNFLOWER_HPP

#include "Plant.hpp"
#include "Sun.hpp"
#include "Game.hpp"
#include <iostream>
#include <memory>

class Game;

class Sunflower : public Plant {
    private:
    float sunTimer;
    Game& gameContext;
    const float spawnInterval = 10.0f;

    public:
    Sunflower(float startX, float startY, const sf::Texture& texture, int rowNum, Game& game)
    : Plant(startX, startY, texture, rowNum, 100, 100), sunTimer(0.0f), gameContext(game) {
        sprite.setPosition({x, y});
    }
    void update(float dt) override {
        if (!isActive) return;

        sunTimer += dt;
        if (sunTimer >= spawnInterval) {
            attack();
            sunTimer = 0.0f;
        }
    }

    void attack() override {
        //sun spwan position
        float sunX = x + 30.0f;
        float sunY = y - 20.0f;

        auto newSun = std::make_unique<Sun>(sunX, sunY, gameContext.getSunTexture(), false);

        gameContext.spawnNewObject(std::move(newSun));
    }
    void draw(sf::RenderWindow& window) override {
        if (isActive) window.draw(sprite);
    }
};

#endif