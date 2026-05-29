#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObj.hpp"

enum class SelectedPlant {
    None,
    Peashooter,
    Sunflower
};

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
    sf::Texture sunflowerTexture;
    sf::Texture peaTexture;
    sf::Texture sunTexture;
//y axis for rows
    inline static const float rowPositions[5] = {140.0f, 250.0f, 360.0f, 470.0f, 580.0f};
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

    sf::Font font;
    sf::Text sunText;

    sf::RectangleShape peaCard;
    sf::RectangleShape sunCard;

    SelectedPlant currentSelection = SelectedPlant::None;

    public:
    Game();
    void run();
    void spawnNewObject(std::unique_ptr<GameObject> newObj);
    bool isZombieInRow(int row) const;
    //getters
    const sf::Texture& getPlantTexture() const     { return plantTexture; }
    const sf::Texture& getSunflowerTexture() const { return sunflowerTexture; }
    const sf::Texture& getPeaTexture() const       { return peaTexture; }
    const sf::Texture& getSunTexture() const       { return sunTexture; }
    const sf::Texture& getZombTexture() const      { return zombTexture; }
};

#endif