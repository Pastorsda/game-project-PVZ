#ifndef ZOMB_HPP
#define ZOMB_HPP

#include "GameObj.hpp"
//forward declaration to break dependency
class Plant;
//class for enemy units handles movement and interactions
class Zomb : public GameObject {
protected:
    int hp, dmg, row;
    float speed;
    bool isEating;
    float damageBuffer = 0.0f; //prevents errors in calculation
public:
    Zomb(float startX, float startY, const sf::Texture& texture, int rowNum, int health, float moveSpeed, int atkPower);
//virtual deconstructor for proper cleanup
    virtual ~Zomb() = default;
//override lifecycle from GameObject
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
//continous damage application onto plant, with buffer to protect agains conversion errors
    void eat(Plant& plant, float dt);
    void setEating(bool eating);
    int getRow() const;

    void takeDamage(int amount);
};

#endif