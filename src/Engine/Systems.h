#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"
#include "Components.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

class System {
public:
    virtual void run(SDL_Renderer* renderer) = 0;
};

class RenderSystem : public System {
public:
    virtual void run(SDL_Renderer* renderer) override = 0;
};

class BackgroundRenderSystem : public RenderSystem {
public:
    BackgroundRenderSystem(SDL_Texture* backgroundTexture)
        : texture(backgroundTexture) {}

    void run(SDL_Renderer* renderer) override {
        if (texture) {
            SDL_Rect dstRect = {0, 0, 1024, 768}; // Ajustar al tamaño de la ventana
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        }
    }

private:
    SDL_Texture* texture;
};

class InputSystem : public System {
public:
    void run(SDL_Renderer* renderer) override {
        // logica
    }

    void processEvents(SDL_Event& event, entt::registry& registry) {
        auto view = registry.view<PositionComponent, DirectionComponent>();

        for (auto entity : view) {
            auto& position = view.get<PositionComponent>(entity);
            auto& direction = view.get<DirectionComponent>(entity);

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    position.x -= 5; // Mover a la izquierda
                    direction.direction = DirectionComponent::LEFT;
                    break;
                case SDLK_RIGHT:
                    position.x += 5; // Mover a la derecha
                    direction.direction = DirectionComponent::RIGHT;
                    break;
                case SDLK_UP:
                    position.y -= 5;
                    direction.direction = DirectionComponent::UP;
                    break;
                case SDLK_DOWN:
                    position.y += 5;
                    direction.direction = DirectionComponent::DOWN;
                    break;
                }
            }
        }
    }
};


class CharacterRenderSystem : public RenderSystem {
public:
    // Implementación del método virtual puro
    void run(SDL_Renderer* renderer) override {
        // método vacío
    }

    // Método principal que utiliza el sistema
    void run(SDL_Renderer* renderer, entt::registry& registry, float deltaTime) {
        auto view = registry.view<PositionComponent, TextureComponent, AnimationComponent, DirectionComponent>();

        for (auto entity : view) {
            auto& position = view.get<PositionComponent>(entity);
            auto& texture = view.get<TextureComponent>(entity);
            auto& animation = view.get<AnimationComponent>(entity);
            auto& direction = view.get<DirectionComponent>(entity);

            // Actualizar animación
            animation.elapsedTime += deltaTime;
            if (animation.elapsedTime >= animation.frameTime) {
                animation.currentFrame = (animation.currentFrame + 1) % animation.frames.size();
                animation.elapsedTime = 0.0f;
            }

            SDL_Texture* currentTexture = animation.frames[animation.currentFrame];
            SDL_Rect dstRect = { position.x, position.y, texture.width, texture.height };

            // Voltear sprite 
            SDL_RendererFlip flip = (direction.direction == DirectionComponent::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            SDL_RenderCopyEx(renderer, currentTexture, nullptr, &dstRect, 0, nullptr, flip);
        }
    }
};


class FireRenderSystem : public RenderSystem {
public:
    SDL_Texture* fireTexture;

    FireRenderSystem(SDL_Texture* fireTexture)
        : fireTexture(fireTexture) {}

    
    void run(SDL_Renderer* renderer) override {
        
    }

    void run(SDL_Renderer* renderer, entt::registry& registry) {
        auto view = registry.view<PositionComponent, FireComponent>();
        for (auto entity : view) {
            auto& position = view.get<PositionComponent>(entity);
            auto& fire = view.get<FireComponent>(entity);
            if (fire.active) {
                SDL_Rect dstRect = {position.x, position.y, 50, 50};
                SDL_RenderCopy(renderer, fireTexture, nullptr, &dstRect);
            }
        }
    }
};

class FireSystem : public System {
public:
    FireSystem() {
        std::srand(static_cast<unsigned>(std::time(0)));
    }

    
    void run(SDL_Renderer* renderer) override {
        
    }

    void run(entt::registry& registry, float deltaTime) {
        auto view = registry.view<PositionComponent, FireComponent>();
        for (auto entity : view) {
            auto& fire = view.get<FireComponent>(entity);
            if (fire.active) {
                fire.timeRemaining -= deltaTime;
                if (fire.timeRemaining <= 0.0f) {
                    fire.active = false;
                }
            } else {
                if (std::rand() % 100 < 10) {
                    auto& position = view.get<PositionComponent>(entity);
                    position.x = std::rand() % 1024;
                    position.y = std::rand() % 768;
                    fire.active = true;
                    fire.timeRemaining = 3.0f;
                }
            }
        }
    }
};

class CollisionSystem : public System {
public:
    void run(SDL_Renderer* renderer) override {
        
    }

    void run(entt::registry& registry, bool& isRunning) {
        auto playerView = registry.view<PositionComponent, TextureComponent>(entt::exclude<FireComponent>);
        auto fireView = registry.view<PositionComponent, FireComponent>();

        for (auto playerEntity : playerView) {
            auto& playerPos = playerView.get<PositionComponent>(playerEntity);
            auto& playerTex = playerView.get<TextureComponent>(playerEntity);

            SDL_Rect playerRect = {playerPos.x, playerPos.y, playerTex.width, playerTex.height};

            for (auto fireEntity : fireView) {
                auto& firePos = fireView.get<PositionComponent>(fireEntity);
                auto& fire = fireView.get<FireComponent>(fireEntity);

                if (fire.active) {
                    SDL_Rect fireRect = {firePos.x, firePos.y, 50, 50}; // Tamaño fijo de los fuegos
                    if (SDL_HasIntersection(&playerRect, &fireRect)) {
                        std::cout << "Perdiste" << std::endl;
                        isRunning = false; // Detener el juego
                        return;
                    }
                }
            }
        }
    }
};
