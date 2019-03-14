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
    vector<MoveableObject> tools;
public:
    Runner(int h = 700, int w = 1000) : Master(h, w) {
        tools.push_back(Object(0, 0, "Asset/icon_new.txt"));
        tools.push_back(Object(40, 0, "Asset/icon_load.txt"));
        tools.push_back(Object(80, 0, "Asset/icon_save.txt"));
        tools.push_back(Object(120, 0, "Asset/icon_zoom_in.txt"));
        tools.push_back(Object(160, 0, "Asset/icon_zoom_out.txt"));
        tools.push_back(Object(200, 0, "Asset/icon_pan_left.txt"));
        tools.push_back(Object(240, 0, "Asset/icon_pan_up.txt"));
        tools.push_back(Object(280, 0, "Asset/icon_pan_down.txt"));
        tools.push_back(Object(320, 0, "Asset/icon_pan_right.txt"));
        tools.push_back(Object(360, 0, "Asset/icon_rotate_ccw.txt"));
        tools.push_back(Object(400, 0, "Asset/icon_rotate_cw.txt"));
        // here pick color
        // here fill color
        tools.push_back(Object(440, 0, "Asset/icon_shape_triangle.txt"));
        tools.push_back(Object(480, 0, "Asset/icon_shape_square.txt"));
        tools.push_back(Object(520, 0, "Asset/icon_shape.txt"));
        tools.push_back(Object(560, 0, "Asset/icon_scale_up.txt"));
        tools.push_back(Object(600, 0, "Asset/icon_scale_down.txt"));
        tools.push_back(Object(640, 0, "Asset/icon_trash.txt"));
        tools.push_back(Object(680, 0, "Asset/icon_color.txt"));
        tools.push_back(Object(720, 0, "Asset/icon_exit.txt"));
    }

    void start() {
        bool z = true;
        while(application_running){
            clearWindow();

            for(const Object &x : tools ){
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