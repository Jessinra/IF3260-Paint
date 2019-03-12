#include <bits/stdc++.h>
#include <assert.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <utility>
#include <SDL2/SDL.h>
#include "Master.hpp"
#include "MoveableObject.hpp"
#include "Object.hpp"
#include "MouseInputData.hpp"

using namespace std;

#define WINDOWHEIGHT 700
#define WINDOWWIDTH 1000
#define maxDilate 10
#define minDilate 0.1
#define constFactor 1.1

bool application_running;
queue<MouseInputData> mouseInput;
int moveVer, moveHor;
int speed = 1;

void *readinput(void *thread_id) {
    char c;
    MouseButtonType buttonType;
    SDL_Event event;
    while (application_running) {
        SDL_WaitEvent(&event);
        switch (event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_w:
                    case SDLK_UP:
                        ++moveVer;
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        ++moveHor;
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        --moveVer;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        --moveHor;
                        break;
                    case SDLK_j:

                        break;
                    case SDLK_k:

                        break;
                    case SDLK_u:
                        if(speed > 1){
                            --speed;
                        }
                        break;
                    case SDLK_i:
                        if(speed < 100){
                            ++speed;
                        }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.key.keysym.sym){
                    case SDL_BUTTON_LEFT:
                        buttonType = MouseButtonType::LEFT_BUTTON;
                        break;
                    case SDL_BUTTON_RIGHT:
                        buttonType = MouseButtonType::RIGHT_BUTTON;
                        break;
                    default:
                        buttonType = MouseButtonType::UNKNOWN;
                }
                mouseInput.push(MouseInputData(buttonType, event.motion.x, event.motion.y));
                printf("mouse coordinate %d %d\n", event.motion.x, event.motion.y);
                break;
            case SDL_QUIT:
                application_running = false;
                break;
        }
        SDL_Delay(10000);
    }
    pthread_exit(nullptr);
}

class Runner : public Master {
protected:
    ;

public:
    Runner(int h = WINDOWHEIGHT, int w = WINDOWWIDTH) : Master(h, w) {

    }

    void start() {
        // Prerender

        while(application_running){
            render();
            processClick();
            adjustMove();

//            usleep(6000);
        }
    }

private:
    void render(){
        clearWindow();

        flush();
    }

    void processClick(){
        // TODO:
        // Preocess Click
    }

    void adjustMove(){
        // TODO:
        // Move View
    }

    void newWorkSpace(){
        // TODO: NEW
    }

    void loadFile(){
        // TODO: LOAD
    }

    void saveFile(){
        // TODO: Save
    }

    void zoomIn(){
        // TODO: Zoom In
    }

    void zoomOut(){
        // TODO: Zoom out
    }

    void FillColor(){
        // TODO: Fill Shape Color
    }

    void createShape(){
        // TODO: Create Shape
    }

    void createRectangle(){
        // TODO: Create Rectangle
    }

    void createTriangle(){
        // TODO: Create Triangle
    }

    void rotateCW(){
        // TODO: Rotate ClockWise 90
    }

    void rotateCCW(){
        // TODO: Rotate Counter ClockWise 90
    }

    void Exit(){
        // TODO: EXIT
    }

    void pickColor(){
        // TODO: Make Color Picker
    }
};

int main() {
    srand(time(NULL));

    /* Multithreading part */
    pthread_t thread;
    int rc, id = 0;
    application_running = true;
    rc = pthread_create(&thread, NULL, readinput, (void *) id);

    Runner run;
    run.start();
    application_running = false;

    /* close */
    pthread_join(thread, nullptr);
    return 0;
}