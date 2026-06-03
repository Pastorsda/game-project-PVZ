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
    float animTimer;
    Game& gameContext;
    int currentFrame;
    bool isAttacking = false;
    std::vector<sf::IntRect> attackFrames;
    public:
    Peashooter(float startX, float startY, const sf::Texture& texture, int rowNum, Game& game)
    : Plant(startX, startY, texture, rowNum, 100, 100), shootTimer(0.0f), animTimer(0.0f), currentFrame(0), isAttacking(false), gameContext(game) {

        for(int i = 0; i < 3; ++i) {
            attackFrames.push_back(sf::IntRect({i * 45, 0}, {45, 45}));
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(attackFrames[0]);
        sprite.setOrigin({7.0f, 6.0f});
        sprite.setScale({2.0f, 2.0f});
        sprite.setPosition({x, y});
    }

   void update(float dt) override {
        if (!isActive) return;

        shootTimer += dt;
        if (shootTimer >= 1.8f) {
            // attack only when zomb on lane
            if (gameContext.isZombieInRow(row)) {
                attack();
                shootTimer = 0.0f;
                isAttacking = true;
                animTimer = 0.0f;
                currentFrame = 0;
            }
        }

    if (isAttacking) {
            animTimer += dt;
            if (animTimer >= 0.1f) {
                currentFrame++;
                if (currentFrame >= 3) {
                    isAttacking = false;
                    currentFrame = 0;
                    sprite.setTextureRect(attackFrames[0]);
                } else {
                    sprite.setTextureRect(attackFrames[currentFrame]);
                }
                animTimer = 0.0f;
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