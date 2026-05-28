#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObj.hpp"

class Game {
    private:
    sf::RenderWindow window;
    sf::Clock clock;

    int state;
    int sunPool;
    float sunTimer = 0.0f;
//assets
    sf::Texture plantTexture;
    sf::Texture zombTexture;
//y axis for rows
    inline static const float rowPositions[5] = {100.0f, 220.0f, 460.0f, 580.0f};
//generating placeholder textures
    sf::Texture createColorPlaceholder(unsigned int width, unsigned int height, sf::Color color);

    std::vector<std::unique_ptr<GameObject>> objects;

    std::vector<std::unique_ptr<GameObject>> creationBuffer;

    bool grid[5][9];

    void handleInput();
    void update(float dt);
    void render();
    void spawnZomb(float dt);
    void checkCollis(float dt);

    public:
    Game();
    void run();
    void spawnNewObject(std::unique_ptr<GameObject> newObj);
    bool isZombieInRow(int row) const;
};

#endif