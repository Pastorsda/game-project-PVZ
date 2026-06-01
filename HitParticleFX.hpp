#ifndef HITPARTICLEFX_HPP
#define HITPARTICLEFX_HPP

#include "GameObj.hpp"
#include <cmath>
#include <random>

// Class representing a single flying dot/particle
class HitParticle : public GameObject {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float timer = 0.0f;
    float duration;
    int baseAlpha;

public:
    HitParticle(float startX, float startY, sf::Color color) 
        : GameObject(startX, startY, 0.0f, 0.0f) 
    {
        isActive = true;

        // Random generator for particle speed and direction
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f); // Any direction
        std::uniform_real_distribution<float> speedDist(50.0f, 150.0f);        // Velocity range
        std::uniform_real_distribution<float> durationDist(0.2f, 0.4f);       // Lifetime
        std::uniform_real_distribution<float> sizeDist(3.0f, 6.0f);           // Particle size

        float angle = angleDist(gen);
        float speed = speedDist(gen);
        
        velocity.x = std::cos(angle) * speed;
        velocity.y = std::sin(angle) * speed;
        duration = durationDist(gen);

        float size = sizeDist(gen);
        shape.setSize({size, size});
        shape.setFillColor(color);
        baseAlpha = color.a;
    }

    ObjectType getType() const override { return ObjectType::FX; }

    void update(float dt) override {
        timer += dt;
        if (timer >= duration) {
            isActive = false;
            return;
        }

        // Apply movement
        x += velocity.x * dt;
        y += velocity.y * dt;
        shape.setPosition({x, y});

        // Fade out based on lifetime progress
        float progress = timer / duration;
        int currentAlpha = static_cast<int>(baseAlpha * (1.0f - progress));
        sf::Color currentColor = shape.getFillColor();
        currentColor.a = currentAlpha;
        shape.setFillColor(currentColor);
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(shape);
        }
    }

    sf::FloatRect getBounds() const override {
        return shape.getGlobalBounds();
    }
};

#endif