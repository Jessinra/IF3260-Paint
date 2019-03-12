#include <bits/stdc++.h>
#include <assert.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <utility>
#include "Master.hpp"
#include "MoveableObject.hpp"
#include "MouseInputData.hpp"
#include "Object.hpp"

using namespace std;

#define WINDOWHEIGHT 700
#define WINDOWWIDTH 1000
#define maxDilate 10
#define minDilate 0.1
#define constFactor 1.1

int zoom = 0;
int moveHor = 0;
int moveVer = 0;
int speedx = 1;
int speedy = 1;
bool application_running;
map<string, bool> display;
map<string, int> moveHors;
map<string, int> moveVers;
map<string, float> dxs;
map<string, float> dys;
vector<string> objectIds;

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
                        for (int i = 0; i < objectIds.size(); i++) {
                            ++moveVers[objectIds[i]];
                        }
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        ++moveHor;
                        for (int i = 0; i < objectIds.size(); i++) {
                            ++moveHors[objectIds[i]];
                        }
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        --moveVer;
                        for (int i = 0; i < objectIds.size(); i++) {
                            --moveVers[objectIds[i]];
                        }
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        --moveHor;
                        for (int i = 0; i < objectIds.size(); i++) {
                            --moveHors[objectIds[i]];
                        }
                        break;
                    case SDLK_j:
                        --zoom;
                        break;
                    case SDLK_k:
                        ++zoom;
                        break;
                    case SDLK_u:
                        if(speedx>1 && speedy>1){
                            --speedx;
                            --speedy;
                        }
                        break;
                    case SDLK_i:
                        if(speedx<100 && speedy<100){
                            ++speedx;
                            ++speedy;
                        }
                        break;
                    case SDLK_1:
                        display[objectIds[0]] = !display[objectIds[0]];
                        break;
                    case SDLK_2:
                        display[objectIds[1]] = !display[objectIds[1]];
                        break;
                    case SDLK_3:
                        display[objectIds[2]] = !display[objectIds[2]];
                        break;
                    case SDLK_4:
                        display[objectIds[3]] = !display[objectIds[3]];
                        break;
                }
                break;
            case SDL_QUIT:
                application_running = false;
                break;
        }
        usleep(30000);
    }
    pthread_exit(nullptr);
}

class Runner : public Master {
protected:
    Object peta;
    Object building, road, pond, tree, kotaklucu;

public:
    Runner(int h = WINDOWHEIGHT, int w = WINDOWWIDTH) : Master(h, w) {
        peta = Object(0, 0, "Asset/object_map.txt");

        building = Object(0, 0, "Asset/object_building.txt");
        road = Object(0, 0, "Asset/object_road.txt");
        pond = Object(0, 0, "Asset/object_kolam.txt");
        tree = Object(0, 0, "Asset/object_pohon.txt");
        kotaklucu = Object(0, 0, "Asset/object_kotaksial.txt");

        display[building.getId()] = true;
        objectIds.push_back(building.getId());
        display[road.getId()] = true;
        objectIds.push_back(road.getId());
        display[pond.getId()] = true;
        objectIds.push_back(pond.getId());
        display[tree.getId()] = true;
        objectIds.push_back(tree.getId());
    }

    void start() {
        // Make map
        const float normal = 1 / max(1.0f * peta.getWidth() / xend, 1.0f * peta.getHeight() / yend);
        float ratio = 1;
        float dx = 0;
        float dy = 0;
        float kx = 720;
        float ky = 0;
        float kz = 1.4;

        View view1(Point(0, 0), Rectangle(0, 0, 700, WINDOWHEIGHT));
        View view2(Point(720, 0), Rectangle(0, 0, 280, WINDOWHEIGHT));

        MoveableObject smallmap = peta;
        smallmap.setPos(0, 0);
        smallmap.selfDilate(0, 0, 1.4);
        MoveableObject mKotaklucu = kotaklucu;
        mKotaklucu.setPos(kx, ky);
        mKotaklucu.selfDilate(kx, ky, kz);

        // MoveableObject map = peta;
        // map.selfDilate(0, 0, normal*ratio);

        MoveableObject mBuilding = building;
        mBuilding.selfDilate(0, 0, normal*ratio);
        MoveableObject mRoad = road;
        mRoad.selfDilate(0, 0, normal*ratio);
        MoveableObject mPond = pond;
        mPond.selfDilate(0, 0, normal*ratio);
        MoveableObject mTree = tree;
        mTree.selfDilate(0, 0, normal*ratio);

        dxs[mBuilding.getId()] = 0;
        dys[mBuilding.getId()] = 0;
        dxs[mRoad.getId()] = 0;
        dys[mRoad.getId()] = 0;
        dxs[mPond.getId()] = 0;
        dys[mPond.getId()] = 0;
        dxs[mTree.getId()] = 0;
        dys[mTree.getId()] = 0;

        while(application_running){
//            auto start = std::chrono::system_clock::now();
            // Drawing
            clearWindow();
            drawObject(mKotaklucu);
            drawSolidObject(view2, smallmap);
//            cerr<<smallmap.getRefPos().getX()<<" "<<smallmap.getRefPos().getY()<<endl;

            if (display[mBuilding.getId()]) {
                drawSolidObject(view1, mBuilding);
            }

            if (display[mRoad.getId()]) {
                drawSolidObject(view1, mRoad);
            }

            if (display[mPond.getId()]) {
                drawSolidObject(view1, mPond);
            }

            if (display[mTree.getId()]) {
                drawSolidObject(view1, mTree);
            }

            flush();

            // Update
            // either it's zooming or moving, not both
            if(zoom != 0){
                /* TODO
                 * When zoom out, fix position
                 */
                if(zoom > 0){
                    if(ratio < maxDilate){
                        ratio *= constFactor;
                        kz /= constFactor;
                        --zoom;
                    }
                    else{
                        zoom = 0;
                    }
                }
                else{
                    if(ratio > minDilate){
                        ratio /= constFactor;
                        kz *= constFactor;
                        ++zoom;
                    }
                    else{
                        zoom = 0;
                    }
                }

                mBuilding = building;
                mBuilding.selfDilate(0, 0, normal*ratio);
                mBuilding.setPos(mBuilding.getRefPos().getX() + dxs[mBuilding.getId()], mBuilding.getRefPos().getY() + dys[mBuilding.getId()]);

                mRoad = road;
                mRoad.selfDilate(0, 0, normal*ratio);
                mRoad.setPos(mRoad.getRefPos().getX() + dxs[mRoad.getId()], mRoad.getRefPos().getY() + dys[mRoad.getId()]);

                mPond = pond;
                mPond.selfDilate(0, 0, normal*ratio);
                mPond.setPos(mPond.getRefPos().getX() + dxs[mPond.getId()], mPond.getRefPos().getY() + dys[mPond.getId()]);

                mTree = tree;
                mTree.selfDilate(0, 0, normal*ratio);
                mTree.setPos(mTree.getRefPos().getX() + dxs[mTree.getId()], mTree.getRefPos().getY() + dys[mTree.getId()]);

                mKotaklucu = kotaklucu;
                mKotaklucu.setPos(kx, ky);
                mKotaklucu.selfDilate(kx, ky, kz);
            }
            else{
                /* TODO
                 * Make it smart when size is small
                 */
                if(moveHor != 0){
                    if(moveHor > 0){
                        bool shiftLeft = false;
                        for (string objectId : objectIds) {
                            if ((int) dxs[objectId] < 0) {
                                shiftLeft = true;
                                break;
                            }
                        }

                        if (shiftLeft) {
                            for (string objectId: objectIds) {
                                dxs[objectId] += speedx;
                            }
                            --moveHor;
                            mBuilding.setPos(mBuilding.getRefPos().getX() + speedx, mBuilding.getRefPos().getY());
                            mRoad.setPos(mRoad.getRefPos().getX() + speedx, mRoad.getRefPos().getY());
                            mPond.setPos(mPond.getRefPos().getX() + speedx, mPond.getRefPos().getY());
                            mTree.setPos(mTree.getRefPos().getX() + speedx, mTree.getRefPos().getY());
                        } else {
                            moveHor = 0;
                        }
                    }
                    else{
                        bool shiftRight = false;

                        if ((int)dxs[mBuilding.getId()] + mBuilding.getWidth() >= 700) {
                            shiftRight = true;
                        }
                        if ((int)dxs[mRoad.getId()] + mRoad.getWidth() >= 700) {
                            shiftRight = true;
                        }
                        if ((int)dxs[mPond.getId()] + mPond.getWidth() >= 700) {
                            shiftRight = true;
                        }
                        if ((int)dxs[mTree.getId()] + mTree.getWidth() >= 700) {
                            shiftRight = true;
                        }

                        if (shiftRight) {
                            for (string objectId: objectIds) {
                                dxs[objectId] -= speedx;
                            }
                            ++moveHor;
                            mBuilding.setPos(mBuilding.getRefPos().getX() - speedx, mBuilding.getRefPos().getY());
                            mRoad.setPos(mRoad.getRefPos().getX() - speedx, mRoad.getRefPos().getY());
                            mPond.setPos(mPond.getRefPos().getX() - speedx, mPond.getRefPos().getY());
                            mTree.setPos(mTree.getRefPos().getX() - speedx, mTree.getRefPos().getY());
                        } else {
                            moveHor = 0;
                        }
                    }
                }
                if(moveVer != 0) {
                    if(moveVer > 0){
                        bool shiftUp = false;
                        for (string objectId : objectIds) {
                            if ((int)dys[objectId] < 0) {
                                shiftUp = true;
                                break;
                            }
                        }

                        if (shiftUp) {
                            for (string objectId : objectIds) {
                                dys[objectId] += speedy;
                            }
                            --moveVer;
                            mBuilding.setPos(mBuilding.getRefPos().getX(), mBuilding.getRefPos().getY() + speedy);
                            mRoad.setPos(mRoad.getRefPos().getX(), mRoad.getRefPos().getY() + speedy);
                            mPond.setPos(mPond.getRefPos().getX(), mPond.getRefPos().getY() + speedy);
                            mTree.setPos(mTree.getRefPos().getX(), mTree.getRefPos().getY() + speedy);
                        } else {
                            moveVer = 0;
                        }
                    }
                    else{
                        bool shiftDown = false;

                        if ((int)dys[mBuilding.getId()] + mBuilding.getHeight() >= yend) {
                            shiftDown = true;
                        }
                        if ((int)dys[mRoad.getId()] + mRoad.getHeight() >= yend) {
                            shiftDown = true;
                        }
                        if ((int)dys[mPond.getId()] + mPond.getHeight() >= yend) {
                            shiftDown = true;
                        }
                        if ((int)dys[mTree.getId()] + mTree.getHeight() >= yend) {
                            shiftDown = true;
                        }

                        if (shiftDown) {
                            for (string objectId : objectIds) {
                                dys[objectId] -= speedy;
                            }
                            ++moveVer;
                            mBuilding.setPos(mBuilding.getRefPos().getX(), mBuilding.getRefPos().getY() - speedy);
                            mRoad.setPos(mRoad.getRefPos().getX(), mRoad.getRefPos().getY() - speedy);
                            mPond.setPos(mPond.getRefPos().getX(), mPond.getRefPos().getY() - speedy);
                            mTree.setPos(mTree.getRefPos().getX(), mTree.getRefPos().getY() - speedy);
                        } else {
                            moveVer = 0;
                        }
                    }
                }
                float ww = 0, hh = 0;
                ww = max(ww, (float)mBuilding.getWidth());
                ww = max(ww, (float)mRoad.getWidth());
                ww = max(ww, (float)mPond.getWidth());
                ww = max(ww, (float)mTree.getWidth());
                hh = max(hh, (float)mBuilding.getHeight());
                hh = max(hh, (float)mRoad.getHeight());
                hh = max(hh, (float)mPond.getHeight());
                hh = max(hh, (float)mTree.getHeight());
                kx = 720 - (dxs.begin()->second / ww) * smallmap.getWidth();
                ky = - dys.begin()->second / hh * smallmap.getHeight();
                mKotaklucu.setPos(kx, ky);
            }
//            auto end = std::chrono::system_clock::now();
//            cerr<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<endl;
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