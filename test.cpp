#include <iostream>
#include <SDL2/SDL.h>
#include "Master.hpp"
#include "MoveableObject.hpp"
#include "Object.hpp"

using namespace std;

bool application_running;

void *readinput(void *thread_id) {
    char c;
    SDL_Event event;
    while (application_running) {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_w:
                    case SDLK_UP:
                        puts("UP PRESSED");
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        //
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        //
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        //
                        break;
                    case SDLK_1:
                        puts("1 PRESSED");
                        break;
                }
                printf("heh %d\n", event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button){
                    case SDL_BUTTON_LEFT:
                        printf("%d %d\n", event.motion.x, event.motion.y);
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
    vector<MoveableObject> v;
public:
    Runner(int h = 700, int w = 1000) : Master(h, w) {
        v.push_back(Object(100, 100, "Asset/object_rotate_symbol.txt"));
    }

    void start() {
        bool z = true;
        while(application_running){
            clearWindow();

            for(const Object &x : v){
                drawSolidObject(x);
            }

//            for(MoveableObject &x : v){
//                if(z){
//                    x.selfStretchX(118, 118, 2);
//                }
//                else{
//                    x.selfStretchX(118, 118, 0.5);
//                }
//                z = !z;
//            }

            flush();
            usleep(10000);
        }
    }
};

int main() {
    Runner run;

    pthread_t thread;
    int rc, id = 0;
    application_running = true;
    rc = pthread_create(&thread, NULL, readinput, (void *) id);

    run.start();

    pthread_join(thread, nullptr);
    return 0;
}