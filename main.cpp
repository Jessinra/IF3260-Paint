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

                switch(event.button.button){
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
                break;
                
            case SDL_QUIT:
                application_running = false;
                break;
        }
        usleep(15000);
    }
    pthread_exit(nullptr);
}

enum AppState{
    NORMAL,
    CREATE_SHAPE,
    CREATE_RECTANGLE,
    CREATE_TRIANGLE
};

class Runner : public Master {
protected:

    /* =================================
                GUI Section 
    ================================== */
    View toolbar, workspace, verticalscroll, horizontalscroll;
    Object scrollbar;
    MoveableObject backgroundToolbar, backgroundVerScroll, backgroundHorScroll;
    MoveableObject verScrollBar, horScrollBar;
    vector<MoveableObject> tools;
    float widthratio, heightratio, zoomratio;
    AppState state;

    /* =================================
                Config Section 
    ================================== */
    const int toolbarButtonSize = 40;

    /* =================================
                Selector Section 
    ================================== */
    MoveableObject workingObject;
    vector<MoveablePlane> *workingShapes;
    MoveablePlane tempPlane;
    int focusedObjectIndex;
    unsigned int currentColor;

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
        tools.push_back(Object(440, 0, "Asset/icon_fill_color.txt"));
        tools.push_back(Object(480, 0, "Asset/icon_fill_color.txt"));
        tools.push_back(Object(520, 0, "Asset/icon_shape_triangle.txt"));
        tools.push_back(Object(560, 0, "Asset/icon_shape_square.txt"));
        tools.push_back(Object(600, 0, "Asset/icon_shape.txt"));
        tools.push_back(Object(640, 0, "Asset/icon_scale_up.txt"));
        tools.push_back(Object(680, 0, "Asset/icon_scale_down.txt"));
        tools.push_back(Object(720, 0, "Asset/icon_trash.txt"));
        tools.push_back(Object(760, 0, "Asset/icon_exit.txt"));

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

    /* =================================
                Gui Section 
    ================================== */

    void preprocess(){
        moveVer = moveHor = zoom = 0;
        zoomratio = 1;
        state = AppState::NORMAL;
        focusedObjectIndex = -1;
        currentColor = 0x1000000;
        tempPlane.setColor(currentColor);
    }

    void render(){
        clearWindow();

        for(const Object obj : tools){
//            cerr<<obj.getConstRefPos().getX()<<" "<<obj.getConstRefPos().getY()<<endl;
            drawSolidObject(toolbar, obj);
        }
        drawSolidObject(toolbar, backgroundToolbar);
        drawSolidObject(verticalscroll, verScrollBar);
        drawSolidObject(verticalscroll, backgroundVerScroll);
        drawSolidObject(horizontalscroll, horScrollBar);
        drawSolidObject(horizontalscroll, backgroundHorScroll);

        drawSolidPlane(workspace, 0, 0, tempPlane);
        drawSolidObject(workspace, workingObject);

        flush();
    }

    /* =================================
            Workspace Section 
    ================================== */

    void resizeScrollBar(){
        float widthTotal = max(0.0f, -workingObject.getConstRefPos().getX()) + (workspace.getWidth())
                           + max(0.0f, workingObject.getConstRefPos().getX() + workingObject.getLowerRight().getX()
                                       - (workspace.getConstRefBox().getXMax()));
        widthratio = 1.0f * (horizontalscroll.getWidth()) / widthTotal;
        horScrollBar = scrollbar;
        horScrollBar.selfStretchX(0, 0, widthratio * (1.0f*horizontalscroll.getWidth() / (horScrollBar.getWidth() - 1)));

        float heightTotal = max(0.0f, -workingObject.getConstRefPos().getY()) + (workspace.getHeight())
                            + max(0.0f, workingObject.getConstRefPos().getY() + workingObject.getLowerRight().getY()
                                        - (workspace.getConstRefBox().getYMax()));
        heightratio = 1.0f * (verticalscroll.getHeight()) / heightTotal;
        verScrollBar = scrollbar;
        verScrollBar.selfStretchY(0, 0, heightratio * (1.0f*verticalscroll.getHeight() / (verScrollBar.getHeight() - 1)));

        reposScrollBar();
    }

    void reposScrollBar(){
        float widthTotal = max(0.0f, -workingObject.getConstRefPos().getX()) + (workspace.getWidth())
                           + max(0.0f, workingObject.getConstRefPos().getX() + workingObject.getLowerRight().getX()
                                       - (workspace.getConstRefBox().getXMax()));
        float leftOffset = max(0.0f, -workingObject.getConstRefPos().getX());
        float heightTotal = max(0.0f, -workingObject.getConstRefPos().getY()) + (workspace.getHeight())
                            + max(0.0f, workingObject.getConstRefPos().getY() + workingObject.getLowerRight().getY()
                                        - (workspace.getConstRefBox().getYMax()));
        float topOffset = max(0.0f, -workingObject.getConstRefPos().getY());
        horScrollBar.getRefPos().setX(leftOffset / widthTotal * (horizontalscroll.getWidth()));
        verScrollBar.getRefPos().setY(topOffset / heightTotal * (verticalscroll.getHeight()));
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
                    resizeScrollBar();
                }
                else{
                    moveHor = 0;
                }
            }
            else{
                if(workingObject.getConstRefPos().getX() + workingObject.getLowerRight().getX() >= workspace.getConstRefBox().getXMax()){
                    workingObject.getRefPos().setX(max(workingObject.getConstRefPos().getX() - speed, (float)min(workspace.getWidth() - workingObject.getWidth(), 0)));
                    ++moveHor;
                    resizeScrollBar();
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
                    resizeScrollBar();
                }
                else{
                    moveVer = 0;
                }
            }
            else{
                if(workingObject.getConstRefPos().getY() + workingObject.getLowerRight().getY() >= workspace.getConstRefBox().getYMax()){
                    workingObject.getRefPos().setY(max(workingObject.getConstRefPos().getY() - speed, (float)min(workspace.getHeight() - workingObject.getHeight(), 0)));
                    ++moveVer;
                    resizeScrollBar();
                }
                else{
                    moveVer = 0;
                }
            }
        }
        reposScrollBar();
    }

    void exitDrawState(){
        state = AppState::NORMAL;
    }

    /* =================================
                Mouse Section 
    ================================== */

    void processClick(){
        while(!mouseInput.empty()){
            MouseInputData mouseClick = mouseInput.front();
            mouseInput.pop();

            if(isLeftClick(mouseClick)){
                cerr<<"mouse coordinate "<<mouseClick.position.getX()<<" "<<mouseClick.position.getY()<<endl;
                if(mouseInsideToolbar(mouseClick) && mouseInsideToolbox(mouseClick)){
                    int buttonIdx = getClickedButtonIdx(mouseClick);
                    
                    if(buttonUndefined(buttonIdx) || !isButtonClicked(mouseClick, buttonIdx)){
                        continue;
                    }
                    runButtonFunction(buttonIdx);
                    break;
                }

                else if(mouseInsideWorkspace(mouseClick)){

                    if(state == AppState::CREATE_SHAPE){
                        drawFreeShape(mouseClick);
                    }
                    else if(state == AppState::CREATE_RECTANGLE){
                        drawRectangle(mouseClick);
                    }
                    else if(state == AppState::CREATE_TRIANGLE){
                        drawTriangle(mouseClick);
                    }
                    else{
                        setFocusOnObject(mouseClick);
                    }
                    break;
                }
            }

            else if(isRightClick(mouseClick)){
                if(state == AppState::CREATE_SHAPE){
                    quitCreateShape();
                }
                else{
                    exitDrawState();
                }
            }
        }
    }

    bool isLeftClick(const MouseInputData &mouseClick){
        return mouseClick.buttonType == MouseButtonType::LEFT_BUTTON;
    }

    bool isRightClick(const MouseInputData &mouseClick){
        return mouseClick.buttonType == MouseButtonType::RIGHT_BUTTON;
    }

    bool mouseInsideWorkspace(MouseInputData mouseClick){
        return workspace.isInside(
            mouseClick.position.getX() - workspace.getRefPos().getX(),
            mouseClick.position.getY() - workspace.getRefPos().getY());
    }

    bool mouseInsideToolbar(const MouseInputData &mouseClick){
        return toolbar.isInside(mouseClick.position);
    }

    bool mouseInsideToolbox(const MouseInputData &mouseClick){
        return mouseClick.position.getY() >= 2 && mouseClick.position.getY() < 38;
    }

    void setFocusOnObject(const MouseInputData &mouseClick){
        float x = mouseClick.position.getX() - workspace.getConstRefPos().getX() - workingObject.getConstRefPos().getX();
        float y = mouseClick.position.getY() - workspace.getConstRefPos().getY() - workingObject.getConstRefPos().getY();

        focusedObjectIndex = -1;

        for(int i=0; i<workingObject.getConstRefPlanes().size(); ++i){
            const MoveablePlane &plane = workingObject.getConstRefPlanes()[i];
            if(x >= plane.getConstRefPos().getX() && x < plane.getConstRefPos().getX() + plane.getLowerRight().getX() &&
                y >= plane.getConstRefPos().getY() && y < plane.getConstRefPos().getY() + plane.getLowerRight().getY()){
                focusedObjectIndex = i;
                break;
            }
        }
    }

    int getClickedButtonIdx(const MouseInputData &mouseClick){
        return (int) mouseClick.position.getX() / toolbarButtonSize;
    }

    /* =================================
                Button Section 
    ================================== */
    
    bool isButtonClicked(const MouseInputData &mouseClick, int buttonIdx){
        return (mouseClick.position.getX() >= buttonIdx * toolbarButtonSize + 2 &&
                mouseClick.position.getX() < (buttonIdx + 1) * toolbarButtonSize - 2);
    }

    bool buttonUndefined(int buttonIdx){
        return buttonIdx >= tools.size();
    }

    /* =================================
                Callback Section 
    ================================== */

    void runButtonFunction(int buttonIdx){
        switch (buttonIdx){
            // TODO : implement the function caller ?
            case 0:
                cerr<<"NEW"<<endl;
                newWorkSpace();
                break;
            case 1:
                cerr<<"LOAD"<<endl;
                loadFile();
                break;
            case 2:
                cerr<<"SAVE"<<endl;
                saveFile();
                break;
            case 3:
                cerr<<"ZOOM IN"<<endl;
                zoomIn();
                break;
            case 4:
                cerr<<"ZOOM OUT"<<endl;
                zoomOut();
                break;
            case 5:
                cerr<<"PAN LEFT"<<endl;
                panLeft();
                break;
            case 6:
                cerr<<"PAN TOP"<<endl;
                panTop();
                break;
            case 7:
                cerr<<"PAN BOTTOM"<<endl;
                panBottom();
                break;
            case 8:
                cerr<<"PAN RIGHT"<<endl;
                panRight();
                break;
            case 9:
                cerr<<"ROTATE CCW"<<endl;
                rotateCCW();
                break;
            case 10:
                cerr<<"ROTATE CW"<<endl;
                rotateCW();
                break;
            case 11:
                cerr<<"PICK COLOR"<<endl;
                pickColor();
                break;
            case 12:
                cerr<<"FILL COLOR"<<endl;
                fillColor();
                break;
            case 13:
                cerr<<"CREATE TRIANGLE"<<endl;
                createTriangle();
                break;
            case 14:
                cerr<<"CREATE RECTANGLE"<<endl;
                createRectangle();
                break;
            case 15:
                cerr<<"CREATE SHAPE"<<endl;
                createShape();
                break;
            case 16:
                cerr<<"SCALE UP"<<endl;
                scaleUp();
                break;
            case 17:
                cerr<<"SCALE DOWN"<<endl;
                scaleDown();
                break;
            case 18:
                cerr<<"DELETE A SHAPE"<<endl;
                deleteShape();
                break;
            case 19:
                cerr<<"EXIT"<<endl;
                exit();
                break;
        }
    }

    void newWorkSpace(){
        workingObject = Object();
        workingShapes = &workingObject.getRefPlanes();
    }

    void loadFile(){
        // TODO: LOAD
    }

    void saveFile(){
        // TODO: Save
    }

    void zoomIn(){
        ++zoom;
    }

    void zoomOut(){
        --zoom;
    }

    void panLeft(){
        ++moveHor;
    }

    void panTop(){
        ++moveVer;
    }

    void panBottom(){
        --moveVer;
    }

    void panRight(){
        --moveHor;
    }

    void rotateCCW(){
        workingObject.selfRotate(workspace.getWidth()/2, workspace.getHeight()/2, -90);
    }

    void rotateCW(){
        workingObject.selfRotate(workspace.getWidth()/2, workspace.getHeight()/2, 90);
    }

    void pickColor(){
        // TODO: Pick Color From STDIN
    }

    void fillColor(){
        MoveablePlane &plane = (*workingShapes)[focusedObjectIndex];
        plane.setColor(currentColor);
    }

    void createTriangle(){
        state = AppState::CREATE_TRIANGLE;
    }

    void createRectangle(){
        state = AppState::CREATE_RECTANGLE;
    }

    void createShape(){
        state = AppState::CREATE_SHAPE;
    }

    void deleteShape(){
        if(state == AppState::CREATE_TRIANGLE || state == AppState::CREATE_RECTANGLE){
            state = AppState::NORMAL;
        }
        else if(state == AppState::CREATE_SHAPE){
            tempPlane.getRefLines().clear();
            tempPlane.setPos(0, 0);
        }
        else if(focusedObjectIndex != -1){
            workingShapes->erase(workingShapes->begin() + focusedObjectIndex);
            workingObject.calculate();
        }
    }

    void exit(){
        application_running = false;
    }

    void scaleUp(){
        (*workingShapes)[focusedObjectIndex].selfDilate(workspace.getWidth() / 2, workspace.getHeight() / 2, constFactor);
        workingObject.calculate();
    }

    void scaleDown(){
        (*workingShapes)[focusedObjectIndex].selfDilate(workspace.getWidth() / 2, workspace.getHeight() / 2, 1/constFactor);
        workingObject.calculate();
    }

    /* =================================
            Feature Section 
    ================================== */

    void drawFreeShape(MouseInputData mouseClick){

        int drawPositionX = mouseClick.position.getX();
        int drawPositionY = mouseClick.position.getY();

        vector<Line> &lines = tempPlane.getRefLines();
        Pixel currPixel = Pixel(drawPositionX, drawPositionY, currentColor);

        if(lines.empty()){
            lines.push_back(Line(currPixel, currPixel));
        }
        else{
            Pixel endPixel = lines.back().getRefEndPixel();
            Pixel startPixel = lines.back().getRefStartPixel();
            lines.pop_back();
            lines.push_back(Line(startPixel, currPixel));
            lines.push_back(Line(currPixel, endPixel));
        }
    }

    void quitCreateShape(){
        if(!tempPlane.getConstRefLines().empty()){
            workingShapes->insert(workingShapes->begin(), tempPlane);
            workingObject.calculate();
            tempPlane.getRefLines().clear();
            tempPlane.setPos(0, 0);
        }
        exitDrawState();
    }

    void drawRectangle(const MouseInputData &mouseClick){
        int drawPositionX = mouseClick.position.getX() - workspace.getConstRefPos().getX() - 25;
        int drawPositionY = mouseClick.position.getY() - workspace.getConstRefPos().getY() - 25;

        MoveableObject tempRectangle = Object(drawPositionX, drawPositionY, "Asset/Shapes/square.txt", currentColor);
        for(const MoveablePlane &plane : tempRectangle.getPlanes()){
            workingObject.addPlane(plane);
        }
    }

    void drawTriangle(const MouseInputData &mouseClick){
        int drawPositionX = mouseClick.position.getX() - workspace.getConstRefPos().getX() - 25;
        int drawPositionY = mouseClick.position.getY() - workspace.getConstRefPos().getY() - 9;

        MoveableObject tempTriangle = Object(drawPositionX, drawPositionY, "Asset/Shapes/triangle.txt", currentColor);
        for(const MoveablePlane &plane : tempTriangle.getPlanes()){
            workingObject.addPlane(plane);
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

    /* close */
    pthread_join(thread, nullptr);
    return 0;
}