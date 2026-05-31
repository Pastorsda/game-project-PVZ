#ifndef ZOMB_HPP
#define ZOMB_HPP

#include "GameObj.hpp"

class Plant;

class Zomb : public GameObject {
protected:
    int hp, dmg, row;
    float speed;
    bool isEating;
    float damageBuffer = 0.0f;
    sf::Sprite sprite;

public:
    // constructor grabs stats from derived class
    Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int maxHp, float moveSpeed, int attackDmg);
    
    virtual ~Zomb() = default;

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void eat(Plant& plant, float dt);
    void setEating(bool eating);

    //getters
    int getRow() const;
    sf::FloatRect getBounds() const override;

    void takeDamage(int amount);
};

#endif