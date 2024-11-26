#pragma once
#include "Game.h"
#include "Scene.h"
#include "Systems.h"
#include <iostream>
#include <entt/entt.hpp>
#include "Entity.h"


class SquareRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_Rect r = { 10, 10, 100, 100 };
        SDL_RenderFillRect(renderer, &r);
    }
}; 

class DemoGame : public Game {
  public:
    Scene* sampleScene;
    entt::registry registry;

    DemoGame()
        : Game("SAMPLE GAME", 1024, 768) {}

    void setup() override {
        std::cout << "HELLO WORLD" << std::endl;
        //sampleScene = new Scene("SAMPLE SCENE", registry);
        sampleScene = new Scene("SAMPLE SCENE", registry, &isRunning);


        // Fondo
        SDL_Texture* backgroundTexture = loadTexture("src/DemoGame/Fondo.png", renderer);
        sampleScene->renderSystems.push_back(new BackgroundRenderSystem(backgroundTexture));

        // Crear personaje
        auto* character = sampleScene->createEntity("Player");
        character->addComponent<PositionComponent>(400, 300);
        character->addComponent<DirectionComponent>(DirectionComponent::RIGHT);

        SDL_Texture* idleFrame1 = loadTexture("src/DemoGame/Personaje.png", renderer);
        SDL_Texture* idleFrame2 = loadTexture("src/DemoGame/Personaje3.png", renderer);
        character->addComponent<TextureComponent>(idleFrame1, 150, 100);
        //character->addComponent<AnimationComponent>({idleFrame1, idleFrame2}, 0, 0.1f, 0.0f);
        character->addComponent<AnimationComponent>(std::vector<SDL_Texture*>{idleFrame1, idleFrame2}, 0, 0.1f, 0.0f);

        sampleScene->renderSystems.push_back(new CharacterRenderSystem());
        sampleScene->inputSystems.push_back(new InputSystem());

        // Crear fuegos
        SDL_Texture* fireTexture = loadTexture("src/DemoGame/Fuego.png", renderer);
        if (!fireTexture) {
            std::cerr << "Error: No se pudo cargar Fuego.png" << std::endl;
            return;
        }

        for (int i = 0; i < 10; ++i) { // Crear 10 entidades de fuego
            auto* fireEntity = sampleScene->createEntity("Fire");
            fireEntity->addComponent<PositionComponent>(std::rand() % 1024, std::rand() % 768);
            fireEntity->addComponent<FireComponent>(false, 0.0f); // Inicialmente inactivos
            fireEntity->addComponent<TextureComponent>(fireTexture, 50, 50);
        }

        // Agregar sistemas de fuego
        sampleScene->renderSystems.push_back(new FireRenderSystem(fireTexture));
        sampleScene->updateSystems.push_back(new FireSystem());

        //Colisiones
        sampleScene->updateSystems.push_back(new CollisionSystem());


        setScene(sampleScene);
    }



};
