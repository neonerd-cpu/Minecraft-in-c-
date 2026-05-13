#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include "world.h"

class Player {
public:
    Player(glm::vec3 position);

    void handleInput(const SDL_Event& event);
    void update(float deltaTime, const World& world);
    
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    
    float getHealth() const { return health; }
    void takeDamage(float amount) { health = std::max(0.0f, health - amount); }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    
    float health;
    float fallDistance;
    bool onGround;
    bool isSneaking;

    void updateCameraVectors();
    bool checkCollision(glm::vec3 pos, const World& world);
};

#endif
