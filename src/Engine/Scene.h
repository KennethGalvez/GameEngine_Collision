#pragma once

#include <string>
#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include "Systems.h"

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

class Entity;

class Scene {
public:
    Scene(const std::string& name, entt::registry& registry, bool* isRunning = nullptr);
    ~Scene();

    Entity* createEntity(const std::string& name);
    void setup();
    void render(SDL_Renderer* renderer);

    // Define processEvents and update
    void processEvents(SDL_Event& event);
    void update(float deltaTime);

    entt::registry& registry;
    std::string name;
    std::vector<RenderSystem*> renderSystems;
    std::vector<System*> inputSystems; // Sistemas de input
    std::vector<System*> updateSystems; // Sistemas de actualización
    bool* isRunning;

};
