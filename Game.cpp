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
#include "ExplosionFX.hpp"
#include "HitParticleFX.hpp"
#include "Sun.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Game::Game() : gen(std::random_device{}()), state(1), sunPool(150), sunTimer(0.0f), currentSelection(SelectedPlant::None),
    sunText(font), gameOverText(font), pauseButtonText(font), pauseMenuText(font), hordeWarningText(font),
    controlsTitleText(font), controlsBodyText(font),
    peaCardText(font), sunCardText(font), nutCardText(font), cherryCardText(font), shovelCardText(font) {
    
   window.create(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "PVZ - test");
    window.setFramerateLimit(60);

    // Loading textures
    if (!plantTexture.loadFromFile("textures/peashooter.png")) {
        std::cout << "[WARN] peashooter.png not found. creating placeholder...\n";
        plantTexture = createColorPlaceholder(60, 60, sf::Color::Green);
    }

    if (!sunflowerTexture.loadFromFile("textures/sunflower.png")) {
        std::cout << "[WARN] sunflower.png not found. creating placeholder...\n";
        sunflowerTexture = createColorPlaceholder(60, 60, sf::Color::Yellow);
    }

    if (!nutTexture.loadFromFile("textures/wallnut.png")) {
        std::cout << "[WARN] wallnut.png not found. creating placeholder...\n";
        nutTexture = createColorPlaceholder(60, 60, sf::Color(139, 69, 19));
    }

    if (!cherryTexture.loadFromFile("textures/cherry.png")) {
        std::cout << "[WARN] cherry.png not found. creating placeholder...\n";
        cherryTexture = createColorPlaceholder(60, 60, sf::Color::Red);
    }

    if (!zombTexture.loadFromFile("textures/zombie.png")) {
        std::cout << "[WARN] zombie.png not found. creating placeholder...\n";
        zombTexture = createColorPlaceholder(60, 80, sf::Color(139, 69, 19));
    }

    if (!fastZombTexture.loadFromFile("textures/fast_zombie.png")) {
        std::cout << "[WARN] fast_zombie.png not found. creating placeholder...\n";
        fastZombTexture = createColorPlaceholder(60, 80, sf::Color::Cyan);
    }

    if (!heavyZombTexture.loadFromFile("textures/heavy_zombie.png")) {
        std::cout << "[WARN] heavy_zombie.png not found. creating placeholder...\n";
        heavyZombTexture = createColorPlaceholder(60, 80, sf::Color::Blue);
    }

    if (!peaTexture.loadFromFile("textures/pea.png")) {
        std::cout << "[WARN] pea.png not found. creating placeholder...\n";
        peaTexture = createColorPlaceholder(16, 16, sf::Color(50, 205, 50));
    }

    if (!sunTexture.loadFromFile("textures/sun.png")) {
        std::cout << "[WARN] sun.png not found. creating placeholder...\n";
        sunTexture = createColorPlaceholder(30, 30, sf::Color(255, 215, 0));
    }


    if (!font.openFromFile("fonts/Arial.ttf")) {
        std::cerr << "[ERROR] Arial.ttf not found\n";
    }

    // Configuration of plant cards
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

    //shovelcard on ui
    shovelcard.setSize({80.0f, 60.0f});
    float shovelX = static_cast<float>(SCREEN_WIDTH) - 80.0f - 10.0f;
    shovelcard.setPosition({shovelX, 10.0f});
    shovelcard.setFillColor(sf::Color(128, 128, 128));
    shovelcard.setOutlineThickness(2.0f);
    shovelcard.setOutlineColor(sf::Color::White);

    // pause button on ui
    pauseButton.setSize({100.0f, 45.0f});
    pauseButton.setPosition({10.0f, 10.0f});
    pauseButton.setFillColor(sf::Color(100, 100, 100));
    pauseButton.setOutlineThickness(2.0f);
    pauseButton.setOutlineColor(sf::Color::White);

    // Pause button
    // pause text on button
    pauseButtonText.setFont(font);
    pauseButtonText.setString("PAUSE");
    pauseButtonText.setCharacterSize(18);
    pauseButtonText.setFillColor(sf::Color::White);
    pauseButtonText.setPosition({30.0f, 20.0f});

    // text to inform that pause is enabled
    pauseMenuText.setFont(font);
    pauseMenuText.setString("GAME PAUSED\nPress P or click PAUSE to resume");
    pauseMenuText.setCharacterSize(40);
    pauseMenuText.setFillColor(sf::Color::Yellow);
    pauseMenuText.setStyle(sf::Text::Style::Bold);

    // position of text
    float pTextX = (static_cast<float>(SCREEN_WIDTH) - 550.0f) / 2.0f;
    float pTextY = (static_cast<float>(SCREEN_HEIGHT) - 100.0f) / 2.0f;
    pauseMenuText.setPosition({pTextX, pTextY});

    // Grid initialization
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 9; ++c) {
            grid[r][c] = false;
        }
    }

    // Game Over screen
    // Dynamic centering of text
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER\nPress R to start again");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Style::Bold);

    float textX = (static_cast<float>(SCREEN_WIDTH) - 500.0f) / 2.0f; // Przybliżona szerokość tekstu
    float textY = (static_cast<float>(SCREEN_HEIGHT) - 100.0f) / 2.0f;
    gameOverText.setPosition({textX, textY});

    sunText.setFont(font);
    sunText.setPosition({10.0f, 85.0f});

    // Descriptions on cards
    // 1. Peashooter text
    peaCardText.setFont(font);
    peaCardText.setString("Pea\n100");
    peaCardText.setCharacterSize(13);
    peaCardText.setFillColor(sf::Color::White);
    peaCardText.setStyle(sf::Text::Style::Bold);
    peaCardText.setPosition({peaCard.getPosition().x + 25.0f, peaCard.getPosition().y + 15.0f});

    // 2. Sunflower text
    sunCardText.setFont(font);
    sunCardText.setString("Sun\n50");
    sunCardText.setCharacterSize(13);
    sunCardText.setFillColor(sf::Color::White);
    sunCardText.setStyle(sf::Text::Style::Bold);
    sunCardText.setPosition({sunCard.getPosition().x + 25.0f, sunCard.getPosition().y + 15.0f});

    // 3. Wallnut text
    nutCardText.setFont(font);
    nutCardText.setString("Nut\n50");
    nutCardText.setCharacterSize(13);
    nutCardText.setFillColor(sf::Color::White);
    nutCardText.setStyle(sf::Text::Style::Bold);
    nutCardText.setPosition({nutcard.getPosition().x + 25.0f, nutcard.getPosition().y + 15.0f});

    // 4. Cherry text
    cherryCardText.setFont(font);
    cherryCardText.setString("Cherry\n150");
    cherryCardText.setCharacterSize(13);
    cherryCardText.setFillColor(sf::Color::White);
    cherryCardText.setStyle(sf::Text::Style::Bold);
    cherryCardText.setPosition({cherrycard.getPosition().x + 22.0f, cherrycard.getPosition().y + 15.0f});

    // 5. Shovel text
    shovelCardText.setFont(font);
    shovelCardText.setString("Shovel");
    shovelCardText.setCharacterSize(13);
    shovelCardText.setFillColor(sf::Color::White);
    shovelCardText.setStyle(sf::Text::Style::Bold);
    shovelCardText.setPosition({shovelcard.getPosition().x + 18.0f, shovelcard.getPosition().y + 20.0f});

    // Progresion tracker
    //wave visual
        waveProgressBg.setSize({200.0f, 20.0f});
        waveProgressBg.setPosition({static_cast<float>(SCREEN_WIDTH) - 220.0f, static_cast<float>(SCREEN_HEIGHT) - 45.0f});
        waveProgressBg.setFillColor(sf::Color(50, 50, 50));
        waveProgressBg.setOutlineThickness(1.0f);
        waveProgressBg.setOutlineColor(sf::Color::White);
    //wave progress
        waveProgressBar.setSize({0.0f, 20.0f});
        waveProgressBar.setPosition({static_cast<float>(SCREEN_WIDTH) - 220.0f, static_cast<float>(SCREEN_HEIGHT) - 45.0f});
        waveProgressBar.setFillColor(sf::Color(200, 0, 0));

    //horde warning
    hordeWarningText.setFont(font);
    hordeWarningText.setString("A HUGE WAVE OF ZOMBIES IS APPROACHING!");
    hordeWarningText.setCharacterSize(36);
    hordeWarningText.setFillColor(sf::Color::Red);
    hordeWarningText.setStyle(sf::Text::Style::Bold);

    // Center the text horizontally near the top of the screen
    sf::FloatRect textBounds = hordeWarningText.getLocalBounds();
    hordeWarningText.setOrigin({
    textBounds.position.x + textBounds.size.x / 2.0f, 
    textBounds.position.y + textBounds.size.y / 2.0f
    });

    hordeWarningText.setPosition({
        static_cast<float>(SCREEN_WIDTH) / 2.0f,
        (static_cast<float>(SCREEN_HEIGHT) / 2.0f) - 50.0f
    });

    // Controlls
    float panelWidth = 170.0f;
    float panelHeight = 140.0f;
    // Centering
    float panelX = (static_cast<float>(SCREEN_WIDTH) - panelWidth) / 2.0f;
    float panelY = (static_cast<float>(SCREEN_HEIGHT) - panelHeight) / 1.1f;
    
    infoPanelBackground.setSize({panelWidth, panelHeight});
    infoPanelBackground.setPosition({panelX, panelY});
    infoPanelBackground.setFillColor(sf::Color(30, 30, 40, 220));
    infoPanelBackground.setOutlineColor(sf::Color(70, 70, 90));
    infoPanelBackground.setOutlineThickness(2.0f);

    controlsTitleText.setFont(font);
    controlsTitleText.setString(L"Controls:");
    controlsTitleText.setCharacterSize(16);
    controlsTitleText.setFillColor(sf::Color(255, 215, 0));
    controlsTitleText.setStyle(sf::Text::Style::Bold);
    controlsTitleText.setPosition({panelX + 20.0f, panelY + 15.0f});

    controlsBodyText.setFont(font);
    controlsBodyText.setString(
        L"LPM - Choose Card\n"
        L"LPM - Plant on lawn\n"
        L"Mouse Over Sun to collect\n"
        L"PPM - Cancel choice\n"
        L"P - Pause"
    );
    controlsBodyText.setCharacterSize(12);
    controlsBodyText.setFillColor(sf::Color(220, 220, 220));
    controlsBodyText.setPosition({panelX + 10.0f, panelY + 45.0f});
    
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

                float baseSpawnInterval = 7.0f;

                float waveDifficultyFactor = currentWave * 0.8f;

                float midWaveAcceleration = zombiesSpawnedInWave * 0.4f;

                float currentSpawnInterval = std::max(1.5f, baseSpawnInterval - waveDifficultyFactor - midWaveAcceleration);


                if (zombtimer >= currentSpawnInterval) {
                    spawnZomb();
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

        // Pause button 'p' to toggle pause
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::P) {
                if (state == 1) {
                    state = 3; // Włącz pauzę
                    std::cout << "[GAME] Paused via P key\n";
                } else if (state == 3) {
                    state = 1; // Wyłącz pauzę
                    std::cout << "[GAME] Resumed via P key\n";
                }
                continue;
            }
        }

        // Game end
        if (state == 2 || state == 4) {
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
                    initialDelayTimer = 0.0f;
                    currentWave = 1;
                    totalZombiesInWave = 3;
                    zombiesSpawnedInWave = 0;
                    zombiesKilledInWave = 0;
                    massiveWaveTriggered = false;
                    
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
        if (state == 1) {
        if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePosF = window.mapPixelToCoords(mouseMove->position);

            for (auto& obj : objects) {
                if (!obj->getIsActive()) continue;

                if (obj->getType() == ObjectType::Sun) {
                    auto sunObj = dynamic_cast<Sun*>(obj.get());
                    if (sunObj->getBounds().contains(mousePosF)) {
                        sunPool += 25;
                        sunObj->destroy();
                        std::cout << "[SUN] Hovered & Collected! Current pool: " << sunPool << "\n";
                    }
                }
            }
        }
        }

        // click events mapped to view
        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
            
            // Left mouse click
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePosF = window.mapPixelToCoords(mouseClick->position);

                // pause when clicked
                if (pauseButton.getGlobalBounds().contains(mousePosF)) {
                    if (state == 1) {
                        state = 3; // pause on
                        std::cout << "[UI] Game Paused via Button\n";
                    } else if (state == 3) {
                        state = 1; // pause off
                        std::cout << "[UI] Game Resumed via Button\n";
                    }
                    continue;
                }

                //block interface when paused
                if (state == 3) {
                    continue;
                }

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
                //select shovel
                else if (shovelcard.getGlobalBounds().contains(mousePosF)) {
                    currentSelection = SelectedPlant::Shovel;
                    std::cout << "[UI] picked: shovel\n";
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

                    // if shovel is picked and ocupied space is targeted
                    if (currentSelection == SelectedPlant::Shovel) {
                        if (grid[clickedRow][clickedCol]) {
                            for (auto& obj : objects) {
                                if (obj->getIsActive()) {

                                    if (obj->getType() == ObjectType::Plant || obj->getType() == ObjectType::Cherry) {
                                        auto p = dynamic_cast<Plant*>(obj.get());
                                        // column calculation
                                        int col = static_cast<int>((p->getX() - 200) / 90);
                                        if (p->getRow() == clickedRow && col == clickedCol) {
                                            p->destroy();
                                            std::cout << "[SHOVEL] Plant removed from [" << clickedRow << "][" << clickedCol << "]\n";
                                            break;
                                        }
                                    }
                                }
                            }
                                currentSelection = SelectedPlant::None;
                            }
                        }
                        // Plant planting
                        else if (!grid[clickedRow][clickedCol]) {
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
                            auto newPlant = std::make_unique<Wallnut>(plantX, plantY, nutTexture, clickedRow);
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
            // Right mouse click unselect
            else if (mouseClick->button == sf::Mouse::Button::Right) {
                if (currentSelection != SelectedPlant::None) {
                    currentSelection = SelectedPlant::None;
                    std::cout << "[UI] Deselected plant (Right-Click)\n";
                }
            }
        }
        }
    } // end of loop pollEvent


// Random number generator & zombie initiation
void Game::spawnZomb() {
    //grace period for planting defences
    if (initialDelayTimer < INITIAL_DELAY_MAX) {
        return;
    }

    if (zombiesSpawnedInWave >= totalZombiesInWave) {
        return;
    }

    std::uniform_int_distribution<> rowDist(0, 4); //random row line
    std::uniform_int_distribution<> typeDist(1, 100); //random type chance

    int randomRow = rowDist(gen);
    float spawnY = rowPositions[randomRow];
    float spawnX = static_cast<float>(SCREEN_WIDTH);

    // Create zombie object
    int typeRoll = typeDist(gen);
    std::unique_ptr<Zomb> newZomb = nullptr; //poliformic base
    std::string typeName = "";

    // Zombie types based on wave
    if (currentWave == 1 || currentWave == 2) {
        // wave 1-2: only basic
        newZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
        typeName = "Basic";
    } 
    else if (currentWave == 3 || currentWave == 4) {
        // wave 3-4 basic and 30% fast
        if (typeRoll <= 70) {
            newZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
            typeName = "Basic";
        } else {
            newZomb = std::make_unique<FastZomb>(spawnX, spawnY, fastZombTexture, randomRow);
            typeName = "Fast";
        }
    } 
    else {
        // wave 5+ all heavy 20% fast 30%
        if (typeRoll <= 50) {
            newZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
            typeName = "Basic";
        } else if (typeRoll > 50 && typeRoll <= 80) {
            newZomb = std::make_unique<FastZomb>(spawnX, spawnY, fastZombTexture, randomRow);
            typeName = "Fast";
        } else {
            newZomb = std::make_unique<HeavyZomb>(spawnX, spawnY, heavyZombTexture, randomRow);
            typeName = "Heavy";
        }
    }

        if (newZomb != nullptr) {
            spawnNewObject(std::move(newZomb));
            zombiesSpawnedInWave++;
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

        ObjectType type = obj->getType();

       if (type == ObjectType::Cherry) {
            auto c = static_cast<Cherry*>(obj.get());
            activeCherries.push_back(c);
            activePlants.push_back(c);
        }
        else if (type == ObjectType::Plant) {
            auto p = static_cast<Plant*>(obj.get());
            activePlants.push_back(p);
        }
        else if (type == ObjectType::Zombie) {
            auto z = static_cast<Zomb*>(obj.get());
            activeZombs.push_back(z);
        }
        else if (type == ObjectType::Pea) {
            auto pea = static_cast<Pea*>(obj.get());
            activePeas.push_back(pea);
        }
    }

    for (auto* c : activeCherries) {
        if (c->isReadyToExplode()) {
            std::cout << "[BOOM]\n";
            
            float boomX = c->getBounds().position.x + (c->getBounds().size.x / 2.0f);
            float boomY = c->getBounds().position.y + (c->getBounds().size.y / 2.0f);

            // spawn effect
            auto fx = std::make_unique<ExplosionFX>(boomX, boomY);
            spawnNewObject(std::move(fx));

            for (auto* z : activeZombs) {
                //check if in 3x3 radius
                if (std::abs(z->getRow() - c->getRow()) <= 1) {
                //check distance in pixels
                float distX = std::abs(z->getX() - c->getX());
                if (distX <= 200.0f) {
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
                
                // Particle when pea hits zombie
                float hitX = pea->getBounds().position.x + (pea->getBounds().size.x / 2.0f);
                float hitY = pea->getBounds().position.y + (pea->getBounds().size.y / 2.0f);

                // Set colour of particle to the same as pea
                sf::Color pColor = pea->getColor();

                if (pColor == sf::Color::White) {
                    pColor = sf::Color(50, 205, 50);
                }

                // Multiple particle spread
                for (int i = 0; i < 6; ++i) {
                    auto particle = std::make_unique<HitParticle>(hitX, hitY, pColor);
                    spawnNewObject(std::move(particle));
                }
                
                z->takeDamage(pea->getDamage());
                pea->destroy();
                break;
            }
        }
    }
}

void Game::update(float dt) {
    // stop logic when gameover or paused
    if (state == 2 || state == 3) return;

    // Manage the horde text display countdown
    if (showHordeWarning) {
        hordeWarningTimer += dt;
        if (hordeWarningTimer >= 3.0f) { // HORDE_WARNING_DURATION
            showHordeWarning = false;
            hordeWarningTimer = 0.0f;
        }
    }

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
    if (sunTimer >= 12.0f) {
        
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
        if (obj->getType() == ObjectType::Zombie) {
            auto z = dynamic_cast<Zomb*>(obj.get());
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
            ObjectType type = obj->getType();

            if (type == ObjectType::Plant || type == ObjectType::Cherry) {
                auto p = static_cast<Plant*>(obj.get());
                int col = static_cast<int>((p->getX() - 200) / 90);
                if (p->getRow() >= 0 && p->getRow() < 5 && col >= 0 && col < 9) {
                    grid[p->getRow()][col] = false;
                    std::cout << "[GRID] Plant died space [" << p->getRow() << "][" << col << "] is now free\n";
                }
            }
            // counting killed zombies
            else if (type == ObjectType::Zombie) {
                zombiesKilledInWave++;
                std::cout << "[WAVE] progress: " << zombiesKilledInWave << "/" << totalZombiesInWave << "\n";
            }
        }
    }

    // Memory cleanup
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [](const std::unique_ptr<GameObject>& obj) { return !obj->getIsActive(); }),
        objects.end());

    // progress bar fulfillment
    float progressFraction = (totalZombiesInWave > 0) ? (float)zombiesKilledInWave / totalZombiesInWave : 0.0f;
    if (progressFraction > 1.0f) progressFraction = 1.0f;
    waveProgressBar.setSize({200.0f * progressFraction, 20.0f});

    // massive wave
    if (zombiesKilledInWave >= totalZombiesInWave && !massiveWaveTriggered) {
        massiveWaveTriggered = true;
        showHordeWarning = true;
        hordeWarningTimer = 0.0f;
        std::cout << "[WAVE] A HUGE HORDE OF ZOMBIES IS APPROACHING!\n";

        //dynamic calc the horde numbers
        int hordeSize = 3 + (currentWave * 2);

        std::uniform_int_distribution<> rowDist(0, 4);
        std::uniform_real_distribution<float> xOffsetDist(0.0f, 200.0f); // Spreads them out depth-wise
        std::uniform_int_distribution<> typeDist(1, 100);

        totalZombiesInWave += hordeSize;
        
        // Imidietely spawn zombies on every lane
        for (int i = 0; i < hordeSize; ++i) {
                int randomRow = rowDist(gen);
                float spawnY = rowPositions[randomRow];
                float spawnX = static_cast<float>(SCREEN_WIDTH) + xOffsetDist(gen);
                
                std::unique_ptr<Zomb> massiveZomb = nullptr;
                int typeRoll = typeDist(gen);

            // Determine zombie types allowed in this wave's horde
            if (currentWave == 1 || currentWave == 2) {
                massiveZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
            } 
            else if (currentWave == 3 || currentWave == 4) {
                if (typeRoll <= 60) {
                    massiveZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
                } else {
                    massiveZomb = std::make_unique<FastZomb>(spawnX, spawnY, fastZombTexture, randomRow);
                }
            } 
            else { // Wave 5 and beyond
                if (typeRoll <= 40) {
                    massiveZomb = std::make_unique<BasicZomb>(spawnX, spawnY, zombTexture, randomRow);
                } else if (typeRoll <= 75) {
                    massiveZomb = std::make_unique<FastZomb>(spawnX, spawnY, fastZombTexture, randomRow);
                } else {
                    massiveZomb = std::make_unique<HeavyZomb>(spawnX, spawnY, heavyZombTexture, randomRow);
                }
            }
            
            if (massiveZomb != nullptr) {
                spawnNewObject(std::move(massiveZomb));
                zombiesSpawnedInWave++;
            }
        }
    }

    // check if wave is finished
    if (zombiesKilledInWave >= totalZombiesInWave && massiveWaveTriggered) {
        currentWave++;
        if (currentWave > 7) {
        state = 4;
        std::cout << "[GAME] YOU WIN!\n";
    } else {
        zombiesSpawnedInWave = 0;
        zombiesKilledInWave = 0;
        massiveWaveTriggered = false;

        //threat scaling every wave +3 zombies
        totalZombiesInWave = 5 + (currentWave * 3);
        std::cout << "[WAVE] Advanced to Wave " << currentWave << "! Target: " << totalZombiesInWave << "\n";
        }
    }
}//end of game::update

void Game::render() {
    window.clear(sf::Color(34, 112, 34)); // Grass green background

    // Object rendering
    for (auto& obj : objects) {
        obj->draw(window);
    }

    // Pasue highlighting
    sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePosI);
    if (pauseButton.getGlobalBounds().contains(mousePosF)) {
        pauseButton.setFillColor(sf::Color(140, 140, 140)); // Jasnoszary po najechaniu
    } else {
        pauseButton.setFillColor(sf::Color(100, 100, 100)); // Domyślny szary
    }

    // tile highlighting
    if (currentSelection != SelectedPlant::None) {
        // get mouse position and calc to coordinates
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

            // highlight for shovel
            if (currentSelection == SelectedPlant::Shovel) {
                if (grid[hoverRow][hoverCol]) {
                    highlightTile.setFillColor(sf::Color(255, 255, 0, 80));
                    highlightTile.setOutlineColor(sf::Color::Yellow);
                } else {
                    highlightTile.setFillColor(sf::Color(255, 0, 0, 80));
                    highlightTile.setOutlineColor(sf::Color::Red);
                }
            } else {
                // highlight for plants
                if (!grid[hoverRow][hoverCol]) {
                    highlightTile.setFillColor(sf::Color(0, 255, 0, 80)); 
                    highlightTile.setOutlineColor(sf::Color::Green);
                } else {
                    highlightTile.setFillColor(sf::Color(255, 0, 0, 80)); 
                    highlightTile.setOutlineColor(sf::Color::Red);
                }
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
    shovelcard.setOutlineColor(currentSelection == SelectedPlant::Shovel ? sf::Color::Red : sf::Color::White);

    // draw interface
    window.draw(peaCard);
    window.draw(sunCard);
    window.draw(nutcard);
    window.draw(cherrycard);
    window.draw(shovelcard);

    // draw text on cards
    window.draw(peaCardText);
    window.draw(sunCardText);
    window.draw(nutCardText);
    window.draw(cherryCardText);
    window.draw(shovelCardText);

    //draw pause
    window.draw(pauseButton);
    window.draw(pauseButtonText);

    //draw wave warning
    if (showHordeWarning) {
    window.draw(hordeWarningText);
    }
    
    // draw sun text and time left
    if (initialDelayTimer < INITIAL_DELAY_MAX) {
        int timeLeft = static_cast<int>(INITIAL_DELAY_MAX - initialDelayTimer);
        sunText.setString("Sun: " + std::to_string(sunPool) + " | Zombies arrive in: " + std::to_string(timeLeft) + "s" );
    } else {
        sunText.setString("Sun: " + std::to_string(sunPool) + " | WAVE: " + std::to_string(currentWave));
    }
    window.draw(sunText);

    // Draw progression
    window.draw(waveProgressBg);
    window.draw(waveProgressBar);

    // Draw info UI
    if (initialDelayTimer < INITIAL_DELAY_MAX) {
        window.draw(infoPanelBackground);
        window.draw(controlsTitleText);
        window.draw(controlsBodyText);
    }

    // pause text overlay
    if (state == 3) {
        sf::RectangleShape overlay({static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        window.draw(pauseMenuText);
    }

    // game over text overlay
    if (state == 2) {
        sf::RectangleShape overlay({static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)});
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        window.draw(gameOverText);
    }


    if (state == 4) {
    sf::RectangleShape overlay({static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    sf::Text winText(font);
    winText.setString("YOU WIN!\nPress R to play again");
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Yellow);
    
    sf::FloatRect textRect = winText.getLocalBounds();
    winText.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
    winText.setPosition({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f});
    
    window.draw(winText);
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
        if (obj->getIsActive() && obj->getType() == ObjectType::Zombie) {
            auto z = dynamic_cast<Zomb*>(obj.get());
            // addition of end of gardent to SCREEN_WIDTH
            if (z->getIsActive() && z->getRow() == row && z->getX() <= static_cast<float>(SCREEN_WIDTH)) {
                return true;
            }
        }
    }
    return false;
}