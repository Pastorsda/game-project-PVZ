#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP

#include <SFML/Graphics.hpp>

enum class ObjectType {
    Plant,
    Zombie,
    Pea,
    Sun,
    Cherry
};

class GameObject {
protected:
    float x, y, width, height;
    bool isActive;

public:
    GameObject(float startX, float startY, float w = 0.0f, float h = 0.0f);

    virtual ~GameObject() = default;

    virtual ObjectType getType() const = 0;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual sf::FloatRect getBounds() const;
    bool getIsActive() const;
    void destroy();
    float getX() const;
    float getY() const;
};

#endif