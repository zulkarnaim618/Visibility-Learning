#include<bits/stdc++.h>
#include "helper_polygon.cpp"
using namespace std;

int main () {
    int cellXCount,cellYCount, subXCount, subYCount;
    cellXCount = cellYCount = 64;
    subXCount = subYCount = 32;
    ifstream in("../obstacles/mexico_1000_2000_175_128_coverage_33_67.txt");
    ofstream out("../datasets/query_"+to_string(cellXCount)+"x"+to_string(cellYCount)+".txt");
    ofstream out2("../datasets/encoding1_"+to_string(cellXCount)+"x"+to_string(cellYCount)+".txt");
    ofstream out3("../datasets/polygon_"+to_string(cellXCount)+"x"+to_string(cellYCount)+".txt");
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
            if (sceneId>=500) break;
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
            if (newSceneId==17)
                //randomgen(*scene,50,cellXCount,cellYCount,out,out2);
                random_sub_scene_gen(*scene,50,cellXCount,cellYCount,subXCount,subYCount,out,out2);
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
