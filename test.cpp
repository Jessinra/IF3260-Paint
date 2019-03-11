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
    Object a,b,c;
    vector<MoveableObject> v;
public:
    Runner(int h = 700, int w = 1000) : Master(h, w) {
        v.push_back(Object(0, 0, "Asset/object_building.txt"));
        v.push_back(Object(0, 0, "Asset/object_kolam.txt"));
        v.push_back(Object(0, 0, "Asset/object_pohon.txt"));
        v.push_back(Object(0, 0, "Asset/object_road.txt"));
    }

    void start() {
        bool z = true;
        while(application_running){

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