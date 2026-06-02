#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "GameObj.hpp"

enum class SelectedPlant {
    None,
    Peashooter,
    Sunflower,
    Wallnut,
    Cherry,
    Shovel
};

class Game {
    private:
    sf::RenderWindow window;
    inline static const unsigned int SCREEN_WIDTH = 1200;
    inline static const unsigned int SCREEN_HEIGHT = 800;
    sf::Clock clock;

    //wave managment
    int currentWave = 1;
    int totalZombiesInWave = 3;
    int zombiesSpawnedInWave = 0;
    int zombiesKilledInWave = 0;
    bool massiveWaveTriggered = false;

    int state;
    int sunPool;
    float sunTimer = 0.0f;
    float initialDelayTimer = 0.0f;
    // grace timer in seconds
    inline static const float INITIAL_DELAY_MAX = 33.0f;
//assets
    sf::Texture plantTexture;
    sf::Texture zombTexture;
    sf::Texture fastZombTexture;
    sf::Texture heavyZombTexture;
    sf::Texture sunflowerTexture;
    sf::Texture nutTexture;
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
    void spawnZomb();
    void checkCollis(float dt);

    //pause option
    sf::RectangleShape pauseButton;
    sf::Text pauseButtonText;
    sf::Text pauseMenuText;

    // Info panel
    sf::RectangleShape infoPanelBackground;
    sf::Text controlsTitleText;
    sf::Text controlsBodyText;
    sf::Text peaCardText;
    sf::Text sunCardText;
    sf::Text nutCardText;
    sf::Text cherryCardText;
    sf::Text shovelCardText;

    sf::Font font;
    sf::Text sunText;
    sf::Text gameOverText;

    // progress bar
    sf::RectangleShape waveProgressBg;
    sf::RectangleShape waveProgressBar;

    // horde warning
    sf::Text hordeWarningText;
    bool showHordeWarning = false;
    float hordeWarningTimer = 0.0f;
    const float HORDE_WARNING_DURATION = 3.0f;

    // plant seedpacket
    sf::RectangleShape peaCard;
    sf::RectangleShape sunCard;
    sf::RectangleShape nutcard;
    sf::RectangleShape cherrycard;
    sf::RectangleShape shovelcard;

    SelectedPlant currentSelection = SelectedPlant::None;

    std::mt19937 gen;

    public:
    Game();
    void run();
    void spawnNewObject(std::unique_ptr<GameObject> newObj);
    bool isZombieInRow(int row) const;
    //getters
    const sf::Texture& getPlantTexture() const     { return plantTexture; }
    const sf::Texture& getSunflowerTexture() const { return sunflowerTexture; }
    const sf::Texture& getNutTexture() const       { return nutTexture; }
    const sf::Texture& getPeaTexture() const       { return peaTexture; }
    const sf::Texture& getCherryTexture() const    { return cherryTexture; }
    const sf::Texture& getSunTexture() const       { return sunTexture; }
    const sf::Texture& getZombTexture() const      { return zombTexture; }
    const sf::Texture& getFastZombTexture() const  { return fastZombTexture; }
    const sf::Texture& getHeavyZombTexture() const { return heavyZombTexture; }
};

#endif