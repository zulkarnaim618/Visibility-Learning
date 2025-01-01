#include<bits/stdc++.h>
#include "helper_polygon.cpp"
using namespace std;

int main () {
    srand(time(NULL));
    int cellXCount,cellYCount;
    cellXCount = cellYCount = 64;
    ifstream in("../obstacles/mexico_1000_2000_175_128_coverage_67_100.txt");
    ofstream out("../datasets/dense1_cor_"+to_string(cellXCount)+"x"+to_string(cellYCount)+".txt",ios::app);
    ofstream out2("../datasets/dense1_encoding_"+to_string(cellXCount)+"x"+to_string(cellYCount)+".txt",ios::app);
    ofstream out3("../datasets/dense1_polygon_"+to_string(cellXCount)+"x"+to_string(cellYCount)+".txt",ios::app);
    string type;
    int newSceneId = 2;
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
            //if (sceneId==500) break;
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
            randomgen(*scene,250,cellXCount,cellYCount,out,out2);
            //
            for (int i=0;i<scene->obstacles.size();i++) {
                for (int j=0;j<scene->obstacles[i].vertices.size();j++) {
                    out3<<"a "<<newSceneId<<" "<<i<<" "<<j<<" "<<(scene->obstacles[i].vertices[j].x-scene->minX)/(scene->maxX-scene->minX)<<" "<<(scene->obstacles[i].vertices[j].y-scene->minY)/(scene->maxY-scene->minY)<<endl;
                }
            }
            out3<<"b "<<0<<" "<<1<<" "<<0<<" "<<1<<endl;
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
    out2.close();
    in.close();
    return 0;
}
