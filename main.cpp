#include <bits/stdc++.h>
#include <assert.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <utility>
#include <algorithm>
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
int moveVer, moveHor, zoom;
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
                        --moveHor;
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        --moveVer;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        ++moveHor;
                        break;
                    case SDLK_j:
                        ++zoom;
                        break;
                    case SDLK_k:
                        --zoom;
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
//                printf("mouse coordinate %d %d\n", event.motion.x, event.motion.y);
                break;
            case SDL_QUIT:
                application_running = false;
                break;
        }
        usleep(15000);
    }
    pthread_exit(nullptr);
}

class Runner : public Master {
protected:
    View toolbar, workspace, verticalscroll, horizontalscroll;
    Object scrollbar;
    MoveableObject backgroundToolbar, backgroundVerScroll, backgroundHorScroll;
    MoveableObject verScrollBar, horScrollBar;
    MoveableObject workingObject;
    vector<MoveableObject> tools;
    vector<MoveablePlane> *workingShapes;
    float widthratio;
    float heightratio;
    float zoomratio;

public:
    Runner(int h = WINDOWHEIGHT, int w = WINDOWWIDTH) : Master(h, w) {
        toolbar = View(Point(0, 0), Rectangle(0, 0, WINDOWWIDTH, 40));
        workspace = View(Point(0, 40), Rectangle(0, 0, 980, 640));
        verticalscroll = View(Point(980, 40), Rectangle(0, 0, 20, 640));
        horizontalscroll = View(Point(0, 680), Rectangle(0, 0, 980, 20));

        backgroundToolbar = Object(0, 0, "Asset/background_toolbar.txt");
        backgroundVerScroll = Object(0, 0, "Asset/background_vertical_scroll.txt");
        backgroundHorScroll = Object(0, 0, "Asset/background_horizontal_scroll.txt");
        scrollbar = Object(0, 0, "Asset/scroll_bar.txt");

        workingObject = Object(0, 0, "Asset/object_building.txt");
        workingShapes = &workingObject.getRefPlanes();

        resizeScrollBar();
    }

    void start() {
        preprocess();
        while(application_running){
            render();
            processClick();
            adjustZoom();
            adjustMove();

            usleep(6000);
//            application_running = false;
        }
    }

private:
    void preprocess(){
        moveVer = moveHor = zoom = 0;
        zoomratio = 1;
    }

    void render(){
        clearWindow();

        drawSolidObject(toolbar, backgroundToolbar);
        drawSolidObject(verticalscroll, verScrollBar);
        drawSolidObject(verticalscroll, backgroundVerScroll);
        drawSolidObject(horizontalscroll, horScrollBar);
        drawSolidObject(horizontalscroll, backgroundHorScroll);

        drawSolidObject(workspace, workingObject);

        flush();
    }

    void resizeScrollBar(){
        widthratio = max(1.0f * workingObject.getWidth() / (workspace.getWidth() + 1), 1.0f);
        horScrollBar = scrollbar;
        horScrollBar.selfStretchX(0, 0, 1/widthratio * (1.0f*(horizontalscroll.getWidth() + 1) / horScrollBar.getWidth()));

        heightratio = max(1.0f * (workingObject.getHeight()-1) / workspace.getHeight(), 1.0f);
        verScrollBar = scrollbar;
        verScrollBar.selfStretchY(0, 0, 1/heightratio * (1.0f*(verticalscroll.getHeight() + 1) / verScrollBar.getHeight()));

        cerr<<"ratio "<<widthratio<<" "<<heightratio<<endl;
        cerr<<horScrollBar.getWidth()<<" "<<verScrollBar.getHeight()<<endl;
        cerr<<workingObject.getWidth()<<" "<<workingObject.getHeight()<<endl;
        cerr<<workspace.getWidth()<<" "<<workspace.getHeight()<<endl;

        reposScrollBar();
    }

    void reposScrollBar(){
        if(widthratio > 1){
            horScrollBar.getRefPos().setX(max(0.0f, (-workingObject.getConstRefPos().getX()) / ((workingObject.getWidth()-1) - workspace.getWidth()) *(horizontalscroll.getWidth() - (horScrollBar.getWidth() - 1))));
        }
        if(heightratio > 1){
            verScrollBar.getRefPos().setY(max(0.0f, (-workingObject.getConstRefPos().getY()) / ((workingObject.getHeight()-1) - workspace.getHeight()) *(verticalscroll.getHeight() - (verScrollBar.getHeight() - 1))));
        }
    }

    void processClick(){
        // TODO: Process Click
        if(mouseInput.empty()) return;


    }

    void adjustZoom(){
        if(zoom != 0){
            if(zoom > 0){
                if(zoomratio < 20){
                    zoomratio *= constFactor;
                    --zoom;
                    workingObject.selfDilate(workspace.getWidth() / 2, workspace.getHeight() / 2, constFactor);
                    resizeScrollBar();
                }
                else{
                    zoom = 0;
                }
            }
            else{
                if(zoomratio > 1.0f/20){
                    zoomratio /= constFactor;
                    workingObject.selfDilate(workspace.getWidth() / 2, workspace.getHeight() / 2, 1/constFactor);
                    ++zoom;
                    resizeScrollBar();
                }
                else{
                    zoom = 0;
                }
            }
        }
    }

    void adjustMove(){
        if(moveHor != 0){
            if(moveHor > 0){
                if(workingObject.getConstRefPos().getX() < 0){
                    workingObject.getRefPos().setX(min(workingObject.getConstRefPos().getX() + speed, 0.0f));
                    --moveHor;
                }
                else{
                    moveHor = 0;
                }
            }
            else{
                if(workingObject.getConstRefPos().getX() + workingObject.getLowerRight().getX() >= workspace.getConstRefBox().getXMax()){
                    workingObject.getRefPos().setX(max(workingObject.getConstRefPos().getX() - speed, (float)min(workspace.getWidth() - workingObject.getWidth(), 0)));
                    ++moveHor;
                }
                else{
                    moveHor = 0;
                }
            }
        }
        if(moveVer != 0){
            if(moveVer > 0){
                if(workingObject.getConstRefPos().getY() < 0){
                    workingObject.getRefPos().setY(min(workingObject.getConstRefPos().getY() + speed, 0.0f));
                    --moveVer;
                }
                else{
                    moveVer = 0;
                }
            }
            else{
                if(workingObject.getConstRefPos().getY() + workingObject.getLowerRight().getY() >= workspace.getConstRefBox().getYMax()){
                    workingObject.getRefPos().setY(max(workingObject.getConstRefPos().getY() - speed, (float)min(workspace.getHeight() - workingObject.getHeight(), 0)));
                    ++moveVer;
                }
                else{
                    moveVer = 0;
                }
            }
        }
        reposScrollBar();
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

    void panLeft(){
        ++moveHor;
    }

    void panRight(){
        --moveHor;
    }

    void panTop(){
        ++moveVer;
    }

    void panBottom(){
        --moveVer;
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

    /* close */
    pthread_join(thread, nullptr);
    return 0;
}