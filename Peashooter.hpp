#ifndef PEASHOOTER_HPP
#define PEASHOOTER_HPP

#include "Plant.hpp"
#include "Pea.hpp"
#include "Game.hpp"
#include <iostream>
#include <memory>

class Game;

class Peashooter : public Plant {
    private:
    float shootTimer;
    Game& gameContext;
    public:
    Peashooter(float startX, float startY, const sf::Texture& texture, int rowNum, Game& game)
    : Plant(startX, startY, texture, rowNum, 100, 100), shootTimer(0.0f), gameContext(game){
        sprite.setPosition({x, y});
    }

   void update(float dt) override {
        if (!isActive) return;

        shootTimer += dt;
        if (shootTimer >= 2.5f) {
            // attack only when zomb on lane
            if (gameContext.isZombieInRow(row)) {
                attack();
                shootTimer = 0.0f;
            }
        }
    }

    void attack() override {
        float peaX = x + 50.0f;
        float peaY = y + 15.0f;

        auto newPea = std::make_unique<Pea>(peaX, peaY, row, gameContext.getPeaTexture());
        gameContext.spawnNewObject(std::move(newPea));
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) window.draw(sprite);
    }
};

#endif