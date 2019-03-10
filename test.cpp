#include <iostream>
#include "Master.hpp"
#include "MoveableObject.hpp"
#include "Object.hpp"

using namespace std;

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
        while(1){
            clearWindow();

            for(MoveableObject & x : v){
                if(z){
                    x.selfDilate(0, 0, 2);
                }
                else{
                    x.selfDilate(0, 0, 0.5);
                }
                drawSolidObject(x);
            }
            z = !z;
            flush();

            sleep(1);
        }
    }
};

int main() {
    Runner run;
    run.start();
    return 0;
}