#include<bits/stdc++.h>
#include "helper_3D.cpp"
using namespace std;

int main () {
    srand(time(NULL));
    double scene_height,scene_width,scene_depth,cellXCount,cellYCount,cellZCount;
    int obs_count,max_height,max_width,max_depth,min_height,min_width,min_depth;
    scene_height = scene_width = scene_depth = 256;
    cellXCount = cellYCount = cellZCount = 32;
    double obs_volume = 0;
    double obs_percent = .1;
    ofstream out("../../datasets/3D/test_coordinates.txt");
    ofstream out2("../../datasets/3D/test_encodings.txt");
    //while (obs_percent<=.45) {
        for (int j=1;j<=20;j++) {
            Scene3D scene(scene_width,scene_height,scene_depth);
            cout<<obs_percent<<" "<<j<<endl;
            obs_count = 16;
            obs_volume = 0;
            for (int i=1;obs_volume<cellXCount*cellYCount*cellZCount*obs_percent;i++) {
                max_height = max(1.0,ceil(sqrt(((cellXCount*cellYCount*cellZCount*obs_percent-obs_volume)/obs_count)/cellZCount)));
                max_width = max(1.0,ceil(sqrt(((cellXCount*cellYCount*cellZCount*obs_percent-obs_volume)/obs_count)/cellZCount)));
                max_depth = cellZCount;
                min_height = max(max_height/4.0,1.0);
                min_width = max(max_width/4.0,1.0);
                min_depth = max(max_depth/4.0,1.0);
                int curr = 1;
                while (curr<=obs_count) {
                    double x1,x2,y1,y2,z1,z2;
                    int height,width,depth;
                    z1 = 0;
                    x1 = randRangeInt(0,cellXCount-max_width)*(scene_width/cellXCount);
                    y1 = randRangeInt(0,cellYCount-max_height)*(scene_height/cellYCount);
                    width = randRangeInt(min_width,max_width+1);
                    height = randRangeInt(min_height,max_height+1);
                    depth = randRangeInt(min_depth,max_depth+1);
                    x2 = x1 + width*(scene_width/cellXCount);
                    y2 = y1 + height*(scene_height/cellYCount);
                    z2 = z1 + depth*(scene_depth/cellZCount);
                    obs_volume += width*height*depth;
                    scene.addObstacle(x1,x2,y1,y2,z1,z2);
                    curr++;
                }
                obs_count = sqrt(obs_count);
            }
            randomgen3D(scene,100,cellXCount,cellYCount,cellZCount,out,out2);
        }
        obs_percent += 0.1;
    //}
    out.close();
    out2.close();
    return 0;
}



