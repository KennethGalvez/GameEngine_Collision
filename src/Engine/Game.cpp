#include "Game.h"
#include <sstream>
#include <SDL2/SDL.h>

Game::Game(const char* title, int width, int height)
    : isRunning(true), screen_width(width), screen_height(height) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (currentScene) {
            currentScene->processEvents(event);
        }
    }
}

void Game::update() {
    static Uint32 lastTime = SDL_GetTicks(); // Tiempo del último frame
    Uint32 currentTime = SDL_GetTicks();    // Tiempo actual
    float deltaTime = (currentTime - lastTime) / 1000.0f; // Delta time en segundos
    lastTime = currentTime;

    if (currentScene && isRunning) {
        currentScene->update(deltaTime); // Pasa deltaTime a la escena
    } else {
        isRunning = false; // Detener el juego si isRunning es falso
    }
}



void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    if (currentScene) {
        currentScene->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

void Game::setScene(Scene* newScene) {
    currentScene = newScene;
}

Scene* Game::getCurrentScene() const {
    return currentScene;
}
