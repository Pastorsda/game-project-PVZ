#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP

#include <SFML/Graphics.hpp>

class GameObject {
    protected:

    float x, y, width, height;
    bool isActive;
    sf::Sprite sprite;

    public:

    GameObject(float startX, float startY, const sf::Texture& texture);

    virtual ~GameObject() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    sf::FloatRect getBounds() const;

    bool getIsActive() const;
    void destroy();
    float getX() const;
    float getY() const;
};

#endif