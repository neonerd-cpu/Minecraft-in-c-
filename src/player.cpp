#include "player.h"
#include <algorithm>

Player::Player(glm::vec3 position) 
    : position(position), velocity(0,0,0), worldUp(0, 1, 0), yaw(-90.0f), pitch(0.0f), 
      movementSpeed(5.0f), mouseSensitivity(0.1f), health(100.0f), fallDistance(0.0f), onGround(false), isSneaking(false) {
    updateCameraVectors();
}

void Player::handleInput(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int xpos, ypos;
        SDL_GetRelativeMouseState(&xpos, &ypos);

        yaw += xpos * mouseSensitivity;
        pitch -= ypos * mouseSensitivity;

        pitch = std::clamp(pitch, -89.0f, 89.0f);
        updateCameraVectors();
    }
}

bool Player::checkCollision(glm::vec3 pos, const World& world) {
    // AABB collision check
    float r = 0.3f; // Player radius
    float h = 1.8f; // Player height
    
    for (int x = (int)std::floor(pos.x - r); x <= (int)std::floor(pos.x + r); x++) {
        for (int y = (int)std::floor(pos.y - h); y <= (int)std::floor(pos.y); y++) {
            for (int z = (int)std::floor(pos.z - r); z <= (int)std::floor(pos.z + r); z++) {
                if (world.getBlock(x, y, z) != BlockType::Air) return true;
            }
        }
    }
    return false;
}

void Player::update(float deltaTime, const World& world) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    isSneaking = state[SDL_SCANCODE_LSHIFT];
    float speed = movementSpeed;
    if (isSneaking) speed *= 0.3f;

    glm::vec3 moveDir(0, 0, 0);
    glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0, front.z));
    glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0, right.z));

    if (state[SDL_SCANCODE_W]) moveDir += flatFront;
    if (state[SDL_SCANCODE_S]) moveDir -= flatFront;
    if (state[SDL_SCANCODE_A]) moveDir -= flatRight;
    if (state[SDL_SCANCODE_D]) moveDir += flatRight;

    if (glm::length(moveDir) > 0) moveDir = glm::normalize(moveDir);
    
    velocity.x = moveDir.x * speed;
    velocity.z = moveDir.z * speed;

    // Gravity
    velocity.y -= 20.0f * deltaTime;
    if (velocity.y < 0) fallDistance += -velocity.y * deltaTime;

    if (onGround && state[SDL_SCANCODE_SPACE]) {
        velocity.y = 7.0f;
    }

    // Move Y
    glm::vec3 nextPos = position;
    nextPos.y += velocity.y * deltaTime;
    if (checkCollision(nextPos, world)) {
        if (velocity.y < 0) {
            onGround = true;
            if (fallDistance > 3.0f) {
                takeDamage((fallDistance - 3.0f) * 10.0f);
            }
            fallDistance = 0.0f;
        }
        velocity.y = 0;
    } else {
        position.y = nextPos.y;
        onGround = false;
    }

    // Move X
    nextPos = position;
    nextPos.x += velocity.x * deltaTime;
    if (!checkCollision(nextPos, world)) {
        position.x = nextPos.x;
    }

    // Move Z
    nextPos = position;
    nextPos.z += velocity.z * deltaTime;
    if (!checkCollision(nextPos, world)) {
        position.z = nextPos.z;
    }
}

glm::mat4 Player::getViewMatrix() const {
    glm::vec3 eye = position;
    if (isSneaking) eye.y -= 0.3f;
    return glm::lookAt(eye, eye + front, up);
}

void Player::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
