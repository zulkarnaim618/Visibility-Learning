#include<bits/stdc++.h>
#include "helper_polygon.cpp"
using namespace std;

int main () {
    ifstream in("dhaka_1000_2000_175_128_coverage_33_67.txt");
    ofstream out("val_150.txt",ios::app);
    string type;
    int newSceneId = 0;
    int sceneId, obstacleId, vertexId;
    int prevSceneId, prevObstacleId;
    prevSceneId = prevObstacleId = 0;
    Scene2D* scene = new Scene2D();
    Obstacle2D* obs = new Obstacle2D();
    double buffer = 0;
    double x, y;
    while (!in.eof()) {
        in>>type;
        if (in.eof()) break;
        if (type=="a") {
            in>>sceneId>>obstacleId>>vertexId>>x>>y;
            if (sceneId>=25) break;
            if (prevSceneId==sceneId) {
                if (prevObstacleId==obstacleId) {
                    Point2D vertex(x,y);
                    obs->addVertex(vertex);
                }
                else {
                    scene->obstacles.push_back(*obs);
                    delete obs;
                    obs = new Obstacle2D();
                    Point2D vertex(x,y);
                    obs->addVertex(vertex);
                }
            }
            prevSceneId = sceneId;
            prevObstacleId = obstacleId;
        }
        else if (type=="b") {
            double minX,maxX,minY,maxY;
            in>>minX>>maxX>>minY>>maxY;
            scene->obstacles.push_back(*obs);
            delete obs;
            scene->setMinX(minX);
            scene->setMaxX(maxX);
            scene->setMinY(minY);
            scene->setMaxY(maxY);
            //
            cout<<newSceneId<<endl;
            random_rnn_gen(*scene,500,out);
            newSceneId++;
            //
            delete scene;
            scene = new Scene2D();
            obs = new Obstacle2D();
            prevSceneId++;
            prevObstacleId = 0;
        }
    }
    delete obs;
    delete scene;
    out.close();
    in.close();
    return 0;
}
