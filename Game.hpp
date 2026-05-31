#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObj.hpp"

enum class SelectedPlant {
    None,
    Peashooter,
    Sunflower,
    Wallnut,
    Cherry
};

class Game {
    private:
    sf::RenderWindow window;
    inline static const unsigned int SCREEN_WIDTH = 1200;
    inline static const unsigned int SCREEN_HEIGHT = 800;
    sf::Clock clock;

    int state;
    int sunPool;
    float sunTimer = 0.0f;
    float initialDelayTimer = 0.0f;
    inline static const float INITIAL_DELAY_MAX = 15.0f;
//assets
    sf::Texture plantTexture;
    sf::Texture zombTexture;
    sf::Texture sunflowerTexture;
    sf::Texture NutTexture;
    sf::Texture cherryTexture;
    sf::Texture peaTexture;
    sf::Texture sunTexture;

//plant cooldowns
    float peaCooldown = 0.0f;
    float sunCooldown = 0.0f;
    float nutCooldown = 0.0f;
    float cherryCooldown = 0.0f;

    inline static const float PEA_COOLDOWN_MAX = 7.5f;
    inline static const float SUN_COOLDOWN_MAX = 7.5f;
    inline static const float NUT_COOLDOWN_MAX = 20.0f;
    inline static const float CHERRY_COOLDOWN_MAX = 30.0f;
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
    sf::Text gameOverText;

    sf::RectangleShape peaCard;
    sf::RectangleShape sunCard;
    sf::RectangleShape nutcard;
    sf::RectangleShape cherrycard;

    SelectedPlant currentSelection = SelectedPlant::None;

    public:
    Game();
    void run();
    void spawnNewObject(std::unique_ptr<GameObject> newObj);
    bool isZombieInRow(int row) const;
    //getters
    const sf::Texture& getPlantTexture() const     { return plantTexture; }
    const sf::Texture& getSunflowerTexture() const { return sunflowerTexture; }
    const sf::Texture& getNutTexture() const       { return NutTexture; }
    const sf::Texture& getPeaTexture() const       { return peaTexture; }
    const sf::Texture& getCherryTexture() const    { return cherryTexture; }
    const sf::Texture& getSunTexture() const       { return sunTexture; }
    const sf::Texture& getZombTexture() const      { return zombTexture; }
};

#endif