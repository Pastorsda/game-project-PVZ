#include "Game.hpp"
#include "Plant.hpp"
#include "Zomb.hpp"
#include "basicZomb.hpp"
#include "FastZomb.hpp"
#include "HeavyZomb.hpp"
#include "Peashooter.hpp"
#include "Pea.hpp"
#include "Sunflower.hpp"
#include "Wallnut.hpp"
#include "Cherry.hpp"
#include "Sun.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : state(1), sunPool(150), sunTimer(0.0f), currentSelection(SelectedPlant::None), sunText(font), gameOverText(font) {
    
    window.create(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "PVZ - test");
    window.setFramerateLimit(60);

    // Texture loading with fallback
    if (!plantTexture.loadFromFile("textures/peashooter.png")) {
        std::cout << "[WARN] peashooter.png not found. creating placeholder...\n";
        plantTexture = createColorPlaceholder(60, 60, sf::Color::Green);
    }

    if (!sunflowerTexture.loadFromFile("textures/sunflower.png")) {
        std::cout << "[WARN] sunflower.png not found. creating placeholder...\n";
        sunflowerTexture = createColorPlaceholder(60, 60, sf::Color::Yellow);
    }

    if (!NutTexture.loadFromFile("textures/wallnut.png")) {
        std::cout << "[WARN] wallnut.png not found. creating placeholder...\n";
        NutTexture = createColorPlaceholder(60, 60, sf::Color(120, 80, 15));
    }

    if (!cherryTexture.loadFromFile("textures/cherry.png")){
        std::cout << "[WARN] cherry.png not found. creating placeholder...\n";
        cherryTexture = createColorPlaceholder(60, 60, sf::Color::Red);
    }

    if (!zombTexture.loadFromFile("textures/zombie.png")) {
        std::cout << "[WARN] zombie.png not found. creating placeholder...\n";
        zombTexture = createColorPlaceholder(60, 80, sf::Color(139, 69, 19));
    }

    if (!peaTexture.loadFromFile("textures/pea.png")) {
        std::cout << "[WARN] pea.png not found. creating placeholder...\n";
        peaTexture = createColorPlaceholder(16, 16, sf::Color(50, 205, 50));
    }

    if (!sunTexture.loadFromFile("textures/sun.png")) {
        std::cout << "[WARN] sun.png not found. creating placeholder...\n";
        sunTexture = createColorPlaceholder(16, 16, sf::Color(255, 215, 0));
    }

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "[Warn] arial.ttf not found. Sun not visible\n";
    }

    // peacard on ui
    peaCard.setSize({80.0f, 60.0f});
    peaCard.setPosition({200.0f, 10.0f});
    peaCard.setFillColor(sf::Color(0, 150, 0));
    peaCard.setOutlineThickness(2.0f);
    peaCard.setOutlineColor(sf::Color::White);

    // suncard on ui
    sunCard.setSize({80.0f, 60.0f});
    sunCard.setPosition({290.0f, 10.0f}); 
    sunCard.setFillColor(sf::Color(200, 200, 0)); 
    sunCard.setOutlineThickness(2.0f);
    sunCard.setOutlineColor(sf::Color::White);

    //wallnutcard on ui
    nutcard.setSize({80.0f, 60.0f});
    nutcard.setPosition({380.0f, 10.0f});
    nutcard.setFillColor(sf::Color(120, 80, 15));
    nutcard.setOutlineThickness(2.0f);
    nutcard.setOutlineColor(sf::Color::White);

    //cherrycard on ui
    cherrycard.setSize({80.0f, 60.0f});
    cherrycard.setPosition({470.0f, 10.0f});
    cherrycard.setFillColor(sf::Color(200, 0, 0));
    cherrycard.setOutlineThickness(2.0f);
    cherrycard.setOutlineColor(sf::Color::White);

    // Grid initialization
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 9; ++c) {
            grid[r][c] = false;
        }
    }

    // Dynamic centering of text
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER\nPress R to start again");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    
    float textX = (static_cast<float>(SCREEN_WIDTH) - 500.0f) / 2.0f; // Przybliżona szerokość tekstu
    float textY = (static_cast<float>(SCREEN_HEIGHT) - 100.0f) / 2.0f;
    gameOverText.setPosition({textX, textY});
    sunText.setPosition({10.0f, 85.0f});
}

void Game::run() {
    float zombtimer = 0.0f;
    while (window.isOpen()) {
        handleInput();

        // Frame time computing
        float dt = clock.restart().asSeconds();

        // Spawn countdown loop - only when game is active
        if (state == 1) {
            if (initialDelayTimer >= INITIAL_DELAY_MAX) {
                zombtimer += dt;
                if (zombtimer >= 6.0f) {
                    spawnZomb(dt);
                    zombtimer = 0.0f;
                }
            }
        }

        update(dt);
        render();
    }
}

void Game::handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        // Game end
        if (state == 2) {
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    // Reset to start values
                    state = 1;
                    sunPool = 150;
                    sunTimer = 0.0f;
                    currentSelection = SelectedPlant::None;
                    objects.clear();
                    creationBuffer.clear();
                    peaCooldown = 0.0f;
                    sunCooldown = 0.0f;
                    nutCooldown = 0.0f;
                    cherryCooldown = 0.0f;
                    
                    for (int r = 0; r < 5; ++r) {
                        for (int c = 0; c < 9; ++c) {
                            grid[r][c] = false;
                        }
                    }
                    std::cout << "[GAME] Game Restarted\n";
                }
            }
            continue; 
        }

        // hover over sun to get
        if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePosF = window.mapPixelToCoords(mouseMove->position);

            for (auto& obj : objects) {
                if (!obj->getIsActive()) continue;

                if (auto sunObj = dynamic_cast<Sun*>(obj.get())) {
                    if (sunObj->getBounds().contains(mousePosF)) {
                        sunPool += 25;
                        sunObj->destroy();
                        std::cout << "[SUN] Hovered & Collected! Current pool: " << sunPool << "\n";
                    }
                }
            }
        }

        // click events mapped to view
        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
            
            // Left mouse click
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePosF = window.mapPixelToCoords(mouseClick->position);

                // select peashooter
                if (peaCard.getGlobalBounds().contains(mousePosF)) {
                    if (peaCooldown <= 0.0f) { 
                        currentSelection = SelectedPlant::Peashooter;
                        std::cout << "[UI] picked: peashooter\n";
                    } else {
                        std::cout << "[UI] Peashooter is recharging! Wait " << std::to_string(peaCooldown) << "s\n";
                    }
                    continue;
                }
                // select sunflower
                else if (sunCard.getGlobalBounds().contains(mousePosF)) {
                    if (sunCooldown <= 0.0f) { 
                        currentSelection = SelectedPlant::Sunflower;
                        std::cout << "[UI] picked: sunflower\n";
                    } else {
                        std::cout << "[UI] Sunflower is recharging!\n";
                    }
                    continue;
                }
                // select wallnut
                else if (nutcard.getGlobalBounds().contains(mousePosF)) {
                    if (nutCooldown <= 0.0f) { 
                        currentSelection = SelectedPlant::Wallnut;
                        std::cout << "[UI] picked: wallnut\n";
                    } else {
                        std::cout << "[UI] Wallnut is recharging!\n";
                    }
                    continue;
                }
                // select cherry bomb
                else if (cherrycard.getGlobalBounds().contains(mousePosF)) {
                    if (cherryCooldown <= 0.0f) { 
                        currentSelection = SelectedPlant::Cherry;
                        std::cout << "[UI] picked: cherry\n";
                    } else {
                        std::cout << "[UI] Cherry Bomb is recharging!\n";
                    }
                    continue;
                }

                // check mouse on grid
                int clickedCol = (mousePosF.x >= 200.0f) ? static_cast<int>((mousePosF.x - 200.0f) / 90.0f) : -1;
                int clickedRow = -1;

                // Change pixel into row
                for (int r = 0; r < 5; ++r) {
                    if (mousePosF.y >= rowPositions[r] && mousePosF.y <= rowPositions[r] + 80.0f) {
                        clickedRow = r;
                        break;
                    }
                }

                // checking grid and plant
                if (clickedRow >= 0 && clickedRow < 5 && clickedCol >= 0 && clickedCol < 9) {
                    if (!grid[clickedRow][clickedCol]) {
                        // Centered pixel position
                        float plantX = 200.0f + clickedCol * 90.0f + 15.0f;
                        float plantY = rowPositions[clickedRow] + 10.0f;

                        // Create peashooter
                        if (currentSelection == SelectedPlant::Peashooter && sunPool >= 100) {
                            auto newPlant = std::make_unique<Peashooter>(plantX, plantY, plantTexture, clickedRow, *this);
                            spawnNewObject(std::move(newPlant));

                            grid[clickedRow][clickedCol] = true;
                            sunPool -= 100;
                            peaCooldown = PEA_COOLDOWN_MAX;
                            currentSelection = SelectedPlant::None;
                        }
                        // Create sunflower
                        else if (currentSelection == SelectedPlant::Sunflower && sunPool >= 50) {
                            auto newPlant = std::make_unique<Sunflower>(plantX, plantY, sunflowerTexture, clickedRow, *this);
                            spawnNewObject(std::move(newPlant));

                            grid[clickedRow][clickedCol] = true;
                            sunPool -= 50;
                            sunCooldown = SUN_COOLDOWN_MAX;
                            currentSelection = SelectedPlant::None;
                        }
                        // Create wallnut
                        else if (currentSelection == SelectedPlant::Wallnut && sunPool >= 50) {
                            auto newPlant = std::make_unique<Wallnut>(plantX, plantY, NutTexture, clickedRow);
                            spawnNewObject(std::move(newPlant));

                            grid[clickedRow][clickedCol] = true;
                            sunPool -= 50;
                            nutCooldown = NUT_COOLDOWN_MAX;
                            currentSelection = SelectedPlant::None;
                        }
                        // Create cherry
                        else if (currentSelection == SelectedPlant::Cherry && sunPool >= 150) {
                            auto newPlant = std::make_unique<Cherry>(plantX, plantY, cherryTexture, clickedRow);
                            spawnNewObject(std::move(newPlant));

                            grid[clickedRow][clickedCol] = true;
                            sunPool -= 150;
                            cherryCooldown = CHERRY_COOLDOWN_MAX;
                            currentSelection = SelectedPlant::None;
                        }
                    }
                }
                // left mouse click deselects when clicked outside of grid
                else {
                    if (currentSelection != SelectedPlant::None) {
                        currentSelection = SelectedPlant::None;
                        std::cout << "[UI] Deselected plant (clicked outside grid)\n";
                    }
                }
            }
            // Right mouse click
            else if (mouseClick->button == sf::Mouse::Button::Right) {
                if (currentSelection != SelectedPlant::None) {
                    currentSelection = SelectedPlant::None;
                    std::cout << "[UI] Deselected plant (Right-Click)\n";
                }
            }
        }
    } // end of loop pollEvent
}

// Random number generator & zombie initiation
void Game::spawnZomb(float dt) {
    //grace period for planting defences
    if (initialDelayTimer < INITIAL_DELAY_MAX) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, 4); //random row line
    std::uniform_int_distribution<> typeDist(1, 100); //random type chance

    int randomRow = rowDist(gen);
    float spawnY = rowPositions[randomRow];
    float spawnX = static_cast<float>(SCREEN_WIDTH);

    // Create zombie object
    int typeRoll = typeDist(gen);
    std::unique_ptr<Zomb> newZomb = nullptr; //poliformic base
    std::string typeName = "";

    // 1-60 -> common zombie
    // 61-85 -> fast zombie
    // 86-100 -> strong zombie
        if (typeRoll <= 60) {
            newZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
            typeName = "Basic";
        } 
        else if (typeRoll > 60 && typeRoll <= 85) {
            newZomb = std::make_unique<FastZomb>(spawnX, spawnY, zombTexture, randomRow);
            typeName = "Fast";
        } 
        else {
            newZomb = std::make_unique<HeavyZomb>(spawnX, spawnY, zombTexture, randomRow);
            typeName = "Heavy";
        }
        if (newZomb != nullptr) {
            spawnNewObject(std::move(newZomb));
            std::cout << "[SPAWNER] " << typeName << " zombie spawned in row: " << randomRow <<"\n";
        }
    }

void Game::checkCollis(float dt) {
    std::vector<Plant*> activePlants;
    std::vector<Zomb*> activeZombs;
    std::vector<Pea*> activePeas;
    std::vector<Cherry*> activeCherries;

    // Polymorphic cast safe filter for abstract base objects
    for (auto& obj : objects) {
        if (!obj->getIsActive()) continue;

        if (auto c = dynamic_cast<Cherry*>(obj.get())) {
            activeCherries.push_back(c);
            activePlants.push_back(c);
        }
        else if (auto p = dynamic_cast<Plant*>(obj.get())) {
            activePlants.push_back(p);
        }
        else if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            activeZombs.push_back(z);
        }
        else if (auto pea = dynamic_cast<Pea*>(obj.get())) {
            activePeas.push_back(pea);
        }
    }

    for (auto* c : activeCherries) {
        if (c->isReadyToExplode()) {
            std::cout << "[BOOM]\n";
            for (auto* z : activeZombs) {
                //check if in 3x3 radius
                if (std::abs(z->getRow() - c->getRow()) <= 1) {
                //check distance in pixels
                float distX = std::abs(z->getX() - c->getX());
                if (distX <= 180.0f) {
                    z->takeDamage(200);
                    }
                }
            }
            c->destroy();
        }
    }
    // Lane based collision (Zombie eats Plant)
    for (auto* z : activeZombs) {
        bool zombieEats = false;

        for (auto* p : activePlants) {
            if (z->getRow() == p->getRow() && z->getBounds().findIntersection(p->getBounds())) {
                z->eat(*p, dt);
                zombieEats = true;
                break;
            }
        }
        if (!zombieEats) {
            z->setEating(false);
        }
    }

    // Collision Zombie and Pea
    for (auto* pea : activePeas) {
        for (auto* z : activeZombs) {
            if (pea->getRow() == z->getRow() && pea->getBounds().findIntersection(z->getBounds())) {
                z->takeDamage(pea->getDamage());
                pea->destroy();
                break;
            }
        }
    }
}

void Game::update(float dt) {
    // stop logic when gameover
    if (state == 2) return;

    //start countdown
    if (initialDelayTimer < INITIAL_DELAY_MAX) {
        initialDelayTimer += dt;
    }

    // plants cooldown
    if (peaCooldown > 0.0f) peaCooldown -= dt;
    if (sunCooldown > 0.0f) sunCooldown -= dt;
    if (nutCooldown > 0.0f) nutCooldown -= dt;
    if (cherryCooldown > 0.0f) cherryCooldown -= dt;

    // passive sun
    sunTimer += dt;
    if (sunTimer >= 8.0f) {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // random spot ajusted to SCREEN_WIDTH
        std::uniform_real_distribution<float> xDist(200.0f, static_cast<float>(SCREEN_WIDTH) - 200.0f);
        
        float spawnX = xDist(gen);
        float spawnY = -50.0f;

        auto newSun = std::make_unique<Sun>(spawnX, spawnY, sunTexture, true);
        spawnNewObject(std::move(newSun));

        sunTimer = 0.0f;
        std::cout << "[SUN SPAWNER] Sun spawned from the sky at X: " << spawnX << "\n";
    }

    // Dynamic polymorphic updater
    for (auto& obj : objects) {
        obj->update(dt);
    }

    // Intersection evaluation
    checkCollis(dt);
    
    // detect zombs that left
    for (auto& obj : objects) {
        if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            if (z->getX() < 0.0f) { 
                state = 2;
                std::cout << "[GAME OVER] Zombie crossed the garden\n";
            }
        }
    }

    // Asset buffering conversion
    if (!creationBuffer.empty()) {
        for (auto& newObj : creationBuffer) {
            objects.push_back(std::move(newObj));
        }
        creationBuffer.clear();
    }

    // Freeing space before plant deletion
    for (auto& obj : objects) {
        if (!obj->getIsActive()) {
            if (auto p = dynamic_cast<Plant*>(obj.get())) {
                int col = static_cast<int>((p->getX() - 200) / 90);
                if (p->getRow() >= 0 && p->getRow() < 5 && col >= 0 && col < 9) {
                    grid[p->getRow()][col] = false;
                    std::cout << "[GRID] Plant died space [" << p->getRow() << "][" << col << "] is now free\n";
                }
            }
        }
    }

    // Memory cleanup
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [](const std::unique_ptr<GameObject>& obj) { return !obj->getIsActive(); }),
        objects.end());
}

void Game::render() {
    window.clear(sf::Color(34, 112, 34)); // Grass green background

    // Object rendering
    for (auto& obj : objects) {
        obj->draw(window);
    }

    // tile highlighting
    if (currentSelection != SelectedPlant::None) {
        // get mouse position and calc to coordinates
        sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePosI);

        // calculate row and column under mouse
        int hoverCol = (mousePosF.x >= 200.0f) ? static_cast<int>((mousePosF.x - 200.0f) / 90.0f) : -1;
        int hoverRow = -1;

        for (int r = 0; r < 5; ++r) {
            if (mousePosF.y >= rowPositions[r] && mousePosF.y <= rowPositions[r] + 80.0f) {
                hoverRow = r;
                break;
            }
        }
        // if the mouse is on the grid
        if (hoverRow >= 0 && hoverRow < 5 && hoverCol >= 0 && hoverCol < 9) {
            // tile size 
            sf::RectangleShape highlightTile({90.0f, 80.0f});
            highlightTile.setPosition({200.0f + hoverCol * 90.0f, rowPositions[hoverRow]});

            // color indicating placing
            if (!grid[hoverRow][hoverCol]) {
                highlightTile.setFillColor(sf::Color(0, 255, 0, 80)); 
                highlightTile.setOutlineColor(sf::Color::Green);
            } else {
                highlightTile.setFillColor(sf::Color(255, 0, 0, 80)); 
                highlightTile.setOutlineColor(sf::Color::Red);
            }
            highlightTile.setOutlineThickness(2.0f);

            //draw rectangle
            window.draw(highlightTile);
        }
    }
    // cooldown plant colour
    peaCard.setFillColor(peaCooldown > 0.0f ? sf::Color(0, 60, 0) : sf::Color(0, 150, 0));
    sunCard.setFillColor(sunCooldown > 0.0f ? sf::Color(90, 90, 0) : sf::Color(200, 200, 0));
    nutcard.setFillColor(nutCooldown > 0.0f ? sf::Color(50, 35, 5) : sf::Color(120, 80, 15));
    cherrycard.setFillColor(cherryCooldown > 0.0f ? sf::Color(80, 0, 0) : sf::Color(200, 0, 0));

    // highlight selected
    peaCard.setOutlineColor(currentSelection == SelectedPlant::Peashooter ? sf::Color::Red : sf::Color::White);
    sunCard.setOutlineColor(currentSelection == SelectedPlant::Sunflower ? sf::Color::Red : sf::Color::White);
    nutcard.setOutlineColor(currentSelection == SelectedPlant::Wallnut ? sf::Color::Red : sf::Color::White);
    cherrycard.setOutlineColor(currentSelection == SelectedPlant::Cherry ? sf::Color::Red : sf::Color::White);

    // draw interface
    window.draw(peaCard);
    window.draw(sunCard);
    window.draw(nutcard);
    window.draw(cherrycard);
    
    // draw sun text and time left
    if (initialDelayTimer < INITIAL_DELAY_MAX) {
        int timeLeft = static_cast<int>(INITIAL_DELAY_MAX - initialDelayTimer);
        sunText.setString("Sun: " + std::to_string(sunPool) + " | Zombies arrive in: " + std::to_string(timeLeft) + "s" );
    } else {
        sunText.setString("Sun: " + std::to_string(sunPool));
    }
    window.draw(sunText);

    if (state == 2) {
        // dynamic overlay
        sf::RectangleShape overlay({static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)});
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        window.draw(gameOverText);
    }

    window.display();
}

void Game::spawnNewObject(std::unique_ptr<GameObject> newObj) {
    creationBuffer.push_back(std::move(newObj));
}

sf::Texture Game::createColorPlaceholder(unsigned int width, unsigned int height, sf::Color color) {
    sf::Image image({width, height}, color);
    sf::Texture tex;
    if (!tex.loadFromImage(image)) {
        std::cerr << "[ERROR] Failed to load from placeholder\n";
    }
    return tex;
}

// check line for zombs
bool Game::isZombieInRow(int row) const {
    for (const auto& obj : objects) {
        if (auto z = dynamic_cast<Zomb*>(obj.get())) {
            // addition of end of gardent to SCREEN_WIDTH
            if (z->getIsActive() && z->getRow() == row && z->getX() <= static_cast<float>(SCREEN_WIDTH)) {
                return true;
            }
        }
    }
    return false;
}