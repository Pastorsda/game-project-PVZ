#ifndef EXPLOSIONFX_HPP
#define EXPLOSIONFX_HPP

#include "GameObj.hpp"

class ExplosionFX : public GameObject {

private:
    sf::CircleShape circle;
    float timer = 0.0f;
    float duration = 0.4f;
    float maxRadius = 110.0f;

public:
    ExplosionFX(float startX, float startY) 
        : GameObject(startX, startY, 0.0f, 0.0f) 
    {
        isActive = true;

        circle.setRadius(0.0f);
        circle.setFillColor(sf::Color(255, 100, 0, 200));
        circle.setOutlineColor(sf::Color(255, 255, 0, 255));
        circle.setOutlineThickness(3.0f);
    }

    ObjectType getType() const override { return ObjectType::Sun; }

    void update(float dt) override {
        timer += dt;
        if (timer >= duration) {
            isActive = false;
            return;
        }
        //progress of animation in real time
        float progress = timer / duration;

        float currentRadius = maxRadius * progress;
        circle.setRadius(currentRadius);

        //positioning of explosion
        circle.setOrigin({currentRadius, currentRadius});
        circle.setPosition({x, y});

        //fadeout
        int alpha = static_cast<int>(200 * (1.0f - progress));
        int outlineAlpha = static_cast<int>(255 * (1.0f - progress));
        circle.setFillColor(sf::Color(255, 100, 0, alpha));
        circle.setFillColor(sf::Color(255, 255, 0, outlineAlpha));
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(circle);
        }
    }

    sf::FloatRect getBounds() const override {
        return circle.getGlobalBounds();
    }
};

#endif