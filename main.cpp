#include <bits/stdc++.h>
#include <assert.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <utility>
#include <SDL2/SDL.h>
#include "Master.hpp"
#include "MoveableObject.hpp"
#include "Object.hpp"

using namespace std;

#define WINDOWHEIGHT 700
#define WINDOWWIDTH 1000
#define maxDilate 10
#define minDilate 0.1
#define constFactor 1.1

bool application_running;

void *readinput(void *thread_id) {
    char c;
    SDL_Event event;
    while (application_running) {
        SDL_WaitEvent(&event);
        switch (event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_w:

                        break;
                }
                printf("pressed key %d\n", event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.key.keysym.sym){
                    case SDL_BUTTON_LEFT:
                        printf("mouse coordinate %d %d\n", event.motion.x, event.motion.y);
                        break;
                }
                break;
            case SDL_QUIT:
                application_running = false;
                break;
        }
        usleep(10000);
    }
    pthread_exit(nullptr);
}

class Runner : public Master {
protected:
    Object peta;
    Object building, road, pond, tree, kotaklucu;

public:
    Runner(int h = WINDOWHEIGHT, int w = WINDOWWIDTH) : Master(h, w) {

    }

    void start() {
        while(application_running){
            clearWindow();
            flush();

            usleep(6000);
        }
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