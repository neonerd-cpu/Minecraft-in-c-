#include "game.h"

Game::Game() : window(nullptr), glContext(nullptr), running(false), width(0), height(0), 
             world(nullptr), player(nullptr), shader(nullptr), selectionBox(nullptr), 
             inventory(nullptr), ui(nullptr), breakingTime(0.0f), hasTarget(false), showInventory(false) {}

Game::~Game() {
    delete world;
    delete player;
    delete shader;
    delete selectionBox;
    delete inventory;
    delete ui;
    if (glContext) SDL_GL_DeleteContext(glContext);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::init(const char* title, int width, int height) {
    this->width = width;
    this->height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    world = new World();
    player = new Player(glm::vec3(8.0f, 10.0f, 8.0f));
    selectionBox = new SelectionBox();
    inventory = new Inventory();
    ui = new UI(width, height);

    running = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
            if (event.key.keysym.sym == SDLK_e) {
                showInventory = !showInventory;
                SDL_SetRelativeMouseMode(showInventory ? SDL_FALSE : SDL_TRUE);
            }
        }
        
        if (!showInventory) {
            player->handleInput(event);
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_RIGHT && hasTarget) {
                // Determine placement position by stepping back slightly from the hit point
                glm::vec3 pos = player->getPosition();
                glm::vec3 dir = player->getFront();
                
                // Find hit point more accurately
                for (float t = 0; t < 5.0f; t += 0.05f) {
                    glm::vec3 cp = pos + dir * t;
                    if (world->getBlock((int)std::floor(cp.x), (int)std::floor(cp.y), (int)std::floor(cp.z)) != BlockType::Air) {
                        // Move slightly back to get the empty space
                        glm::vec3 pp = pos + dir * (t - 0.1f);
                        glm::ivec3 placePos((int)std::floor(pp.x), (int)std::floor(pp.y), (int)std::floor(pp.z));
                        
                        BlockType selected = inventory->getSelectedType();
                        if (inventory->getCount(selected) > 0) {
                            world->setBlock(placePos.x, placePos.y, placePos.z, selected);
                            inventory->removeBlock(selected);
                        }
                        break;
                    }
                }
            }
        }
        if (event.type == SDL_MOUSEWHEEL) {
            if (event.wheel.y > 0) inventory->selectNext();
            else if (event.wheel.y < 0) inventory->selectPrev();
        }
    }
}

void Game::update(float deltaTime) {
    if (showInventory) return;
    player->update(deltaTime, *world);
    updateRaycast();

    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (hasTarget) {
            BlockType type = world->getBlock(targetedBlock.x, targetedBlock.y, targetedBlock.z);
            if (type != BlockType::Air) {
                BlockData data = getBlockData(type);
                breakingTime += deltaTime;
                if (breakingTime >= data.hardness) {
                    world->setBlock(targetedBlock.x, targetedBlock.y, targetedBlock.z, BlockType::Air);
                    inventory->addBlock(type);
                    breakingTime = 0.0f;
                }
            }
        }
    } else {
        breakingTime = 0.0f;
    }
}

void Game::updateRaycast() {
    glm::vec3 pos = player->getPosition();
    glm::vec3 dir = player->getFront();
    hasTarget = false;

    // More precise raycasting
    for (float t = 0; t < 5.0f; t += 0.05f) {
        glm::vec3 checkPos = pos + dir * t;
        glm::ivec3 blockPos((int)std::floor(checkPos.x), (int)std::floor(checkPos.y), (int)std::floor(checkPos.z));
        if (world->getBlock(blockPos.x, blockPos.y, blockPos.z) != BlockType::Air) {
            targetedBlock = blockPos;
            hasTarget = true;
            break;
        }
    }
}

void Game::render() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setBool("useUniformColor", false);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = player->getViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    world->render(*shader);

    if (hasTarget) {
        selectionBox->render(*shader, targetedBlock);
    }

    ui->render(*inventory, player->getHealth(), showInventory);

    SDL_GL_SwapWindow(window);
}

void Game::stop() {
    running = false;
}
