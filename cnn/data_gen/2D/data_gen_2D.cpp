#include<bits/stdc++.h>
#include "helper_2D.cpp"
using namespace std;

int main () {
    srand(time(NULL));
    double scene_height,scene_width,cellXCount,cellYCount;
    int obs_count,max_height,max_width,min_height,min_width;
    scene_height = scene_width = 256;
    cellXCount = cellYCount = 64;
    double obs_area = 0;
    double obs_percent = .2;
    ofstream out("../../datasets/2D/test_coordinates.txt");
    ofstream out2("../../datasets/2D/test_encodings.txt");
    //while (obs_percent<=.45) {
        for (int j=1;j<=12000;j++) {
            Scene2D scene(scene_width,scene_height);
            cout<<obs_percent<<" "<<j<<endl;
            obs_count = 16;
            obs_area = 0;
            for (int i=1;obs_area<cellXCount*cellYCount*obs_percent;i++) {
                max_height = max(1.0,ceil(sqrt((cellXCount*cellYCount*obs_percent-obs_area)/obs_count)));
                max_width = max(1.0,ceil(sqrt((cellXCount*cellYCount*obs_percent-obs_area)/obs_count)));
                min_height = max(max_height/4.0,1.0);
                min_width = max(max_width/4.0,1.0);
                int curr = 1;
                while (curr<=obs_count) {
                    double x1,x2,y1,y2;
                    int height,width;
                    x1 = randRangeInt(0,cellXCount-max_width)*(scene_width/cellXCount);
                    y1 = randRangeInt(0,cellYCount-max_height)*(scene_height/cellYCount);
                    width = randRangeInt(min_width,max_width+1);
                    height = randRangeInt(min_height,max_height+1);
                    x2 = x1 + width*(scene_width/cellXCount);
                    y2 = y1 + height*(scene_height/cellYCount);
                    obs_area += width*height;
                    scene.addObstacle(x1,x2,y1,y2);
                    curr++;
                }
                obs_count = sqrt(obs_count);
            }
            randomgen(scene,750,cellXCount,cellYCount,out,out2);
        }
        obs_percent += 0.1;
    //}
    out.close();
    out2.close();
    return 0;
}





