#include<bits/stdc++.h>
using namespace std;

// high not included
int randRangeInt(int low, int high) {
    return low+rand()%(high-low);
}

//high also included?
double randRange(double low, double high) {
    return min(low,high)+((double)rand()/RAND_MAX)*(max(high,low)-min(low,high));
}

class Point2D {
public:
    double x;
    double y;
    Point2D() {
        x = y = 0;
    }
    Point2D(double x, double y) {
        this->x = x;
        this->y = y;
    }
    Point2D(const Point2D& point) {
        this->x = point.x;
        this->y = point.y;
    }
    Point2D(Point2D* point) {
        this->x = point->x;
        this->y = point->y;
    }
};

bool intersect(Point2D& pointA, Point2D& pointB, Point2D& mbrA, Point2D& mbrB) {
    // pointA and pointB make a line a1*x+b1*y+c1 = 0
    double a1,b1,c1;
    a1 = (pointA.y-pointB.y);
    b1 = -(pointA.x-pointB.x);
    c1 = -pointA.x*(pointA.y-pointB.y)+pointA.y*(pointA.x-pointB.x);

    // mbrA and mbrB make a line a2*x+b2*y+c2 = 0
    double a2,b2,c2;
    a2 = (mbrA.y-mbrB.y);
    b2 = -(mbrA.x-mbrB.x);
    c2 = -mbrA.x*(mbrA.y-mbrB.y)+mbrA.y*(mbrA.x-mbrB.x);

    if (a1*b2-a2*b1==0) {
        //cout<<"parallel"<<endl;
        return false;       // parallel lines
    }

    // intersect point
    double x,y;
    x = (b1*c2-b2*c1)/(a1*b2-a2*b1);
    y = (c1*a2-c2*a1)/(a1*b2-a2*b1);

    // return true if intersect point is within the line segment
    if (((min(pointA.x,pointB.x)<=x && x<=max(pointA.x,pointB.x)) && (min(pointA.y,pointB.y)<=y && y<=max(pointA.y,pointB.y))) && ((min(mbrA.x,mbrB.x)<=x && x<=max(mbrA.x,mbrB.x)) && (min(mbrA.y,mbrB.y)<=y && y<=max(mbrA.y,mbrB.y)))) {
        return true;
    }
    return false;
}

class Obstacle2D {
public:
    Point2D* a;
    Point2D* b;
    Obstacle2D() {
        a = new Point2D();
        b = new Point2D();
    }
    Obstacle2D(Point2D& a, Point2D& b) {
        a = new Point2D(a);
        b = new Point2D(b);
    }
    Obstacle2D(Point2D* a, Point2D* b) {
        a = new Point2D(a);
        b = new Point2D(b);
    }
    Obstacle2D(const Obstacle2D& obstacle) {
        a = new Point2D(obstacle.a);
        b = new Point2D(obstacle.b);
    }
    Obstacle2D(double x1, double x2, double y1, double y2) {
        a = new Point2D(x1,y1);
        b = new Point2D(x2,y2);
    }
    ~Obstacle2D() {
        delete a;
        delete b;
    }

};

class Scene2D {
public:
    vector<Obstacle2D> obstacles;
    double minX;
    double maxX;
    double minY;
    double maxY;
    Scene2D() {
        this->maxX = -DBL_MAX;
        this->minX = DBL_MAX;
        this->maxY = -DBL_MAX;
        this->minY = DBL_MAX;
    }
    Scene2D(double width, double height) {
        this->maxX = width;
        this->minX = 0;
        this->maxY = height;
        this->minY = 0;
    }
    void addObstacle(double x1,double x2,double y1,double y2) {
        Obstacle2D obs(x1,x2,y1,y2);
        obstacles.push_back(obs);
    }
    void setMinX(double minX) {
        this->minX = minX;
    }
    void setMaxX(double maxX) {
        this->maxX = maxX;
    }
    void setMinY(double minY) {
        this->minY = minY;
    }
    void setMaxY(double maxY) {
        this->maxY = maxY;
    }
    bool isVisible(Point2D& a, Point2D& b) {
        bool ans = true;
        for (int i=0;i<obstacles.size();i++) {
            Point2D c(obstacles[i].a->x,obstacles[i].b->y);
            Point2D d(obstacles[i].b->x,obstacles[i].a->y);
            if (intersect(a,b,*obstacles[i].a,c)) {
                ans = false;
                break;
            }
            else if (intersect(a,b,*obstacles[i].a,d)) {
                ans = false;
                break;
            }
            else if (intersect(a,b,*obstacles[i].b,c)) {
                ans = false;
                break;
            }
            else if (intersect(a,b,*obstacles[i].b,d)) {
                ans = false;
                break;
            }
        }
        return ans;
    }
    vector<vector<int>> encodeScene(int cellXCount, int cellYCount) {
        double cellX, cellY;
        cellX = (maxX-minX)/cellXCount;
        cellY = (maxY-minY)/cellYCount;
        vector<vector<int>> encodedScene(cellXCount,vector<int>(cellYCount));
        for (int i=0;i<cellXCount;i++) {
            for (int j=0;j<cellYCount;j++) {
                encodedScene[i][j] = 0;
            }
        }

        for (int i=0;i<obstacles.size();i++) {
            double minX,maxX,minY,maxY;
            minX = min(obstacles[i].a->x,obstacles[i].b->x);
            maxX = max(obstacles[i].a->x,obstacles[i].b->x);
            minY = min(obstacles[i].a->y,obstacles[i].b->y);
            maxY = max(obstacles[i].a->y,obstacles[i].b->y);
            for (int j=floor((minX-this->minX)/cellX);j<ceil((maxX-this->minX)/cellX);j++) {
                for (int k=floor((minY-this->minY)/cellY);k<ceil((maxY-this->minY)/cellY);k++) {
                    encodedScene[j][k] = i+1;
                }
            }
        }
        return encodedScene;
    }
};

void randomgen(Scene2D& scene, int pointCount, int cellXCount, int cellYCount, ofstream& out, ofstream& out2) {
    double cellX,cellY;
    int perGridPoint;
    cellX = (scene.maxX-scene.minX)/cellXCount;
    cellY = (scene.maxY-scene.minY)/cellYCount;
    perGridPoint = 1;
    int posCount, negCount;
    posCount = negCount = 0;
    int maxClassCount = pointCount*.6;
    vector<vector<int>> encodedScene = scene.encodeScene(cellXCount,cellYCount);
    for (int l=0;l<encodedScene.size();l++) {
        for (int m=0;m<encodedScene[l].size();m++) {
            if (encodedScene[l][m]>0) {
                out2<<1<<" ";
            }
            else {
                out2<<0<<" ";
            }
        }
        out2<<endl;
    }
    out2<<endl;
    for (int i=0;posCount+negCount<pointCount;i++) {
        int indexX,indexY;
        indexX = randRangeInt(0,cellXCount);
        indexY = randRangeInt(0,cellYCount);
        for (int j=0;j<perGridPoint;j++) {
            double x,y;
            x = scene.minX+indexX*cellX+randRange(0.0,cellX);
            y = scene.minY+indexY*cellY+randRange(0.0,cellY);
            int indexX1,indexY1;
            double x1,y1;
            indexX1 = randRangeInt(0,cellXCount);
            indexY1 = randRangeInt(0,cellYCount);
            x1 = scene.minX+indexX1*cellX+randRange(0.0,cellX);
            y1 = scene.minY+indexY1*cellY+randRange(0.0,cellY);
            Point2D a(x,y);
            Point2D b(x1,y1);
            bool isVisible = false;
            if (encodedScene[indexX][indexY]==encodedScene[indexX1][indexY1] && encodedScene[indexX][indexY]!=0) {
                isVisible = false;
            }
            else {
                isVisible = scene.isVisible(a,b);
            }
            if (isVisible) {
                if (posCount<maxClassCount) {
                    out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" ";
                    out<<isVisible<<endl;
                    posCount++;
                }
            }
            else {
                if (negCount<maxClassCount) {
                    out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" ";
                    out<<isVisible<<endl;
                    negCount++;
                }
            }
        }
    }
}

pair<double,double> draw_point(Scene2D& scene, int obs_index, int direction, double area) {
    // 0 -left 1- right 2- bottom 3- top
    pair<double,double> point;
    double x,y;
    if (direction==0) {
        y = randRange(max(scene.minY,scene.obstacles[obs_index].a->y-area),min(scene.maxY,scene.obstacles[obs_index].b->y+area));
        x = randRange(max(scene.minX,scene.obstacles[obs_index].a->x-area),scene.obstacles[obs_index].a->x);
    }
    else if (direction==1) {
        y = randRange(max(scene.minY,scene.obstacles[obs_index].a->y-area),min(scene.maxY,scene.obstacles[obs_index].b->y+area));
        x = randRange(min(scene.maxX,scene.obstacles[obs_index].b->x+area),scene.obstacles[obs_index].b->x);
    }
    else if (direction==2) {
        x = randRange(max(scene.minX,scene.obstacles[obs_index].a->x-area),min(scene.maxX,scene.obstacles[obs_index].b->x+area));
        y = randRange(max(scene.minY,scene.obstacles[obs_index].a->y-area),scene.obstacles[obs_index].a->y);
    }
    else if (direction==3) {
        x = randRange(max(scene.minX,scene.obstacles[obs_index].a->x-area),min(scene.maxX,scene.obstacles[obs_index].b->x+area));
        y = randRange(min(scene.maxY,scene.obstacles[obs_index].b->y+area),scene.obstacles[obs_index].b->y);
    }
    point.first = x;
    point.second = y;
    return point;
}

void randomgen_around_obs(Scene2D& scene, int pointCount, int perObstaclePoint, int cellXCount, int cellYCount, ofstream& out, ofstream& out2) {
    double cellX,cellY;
    cellX = (scene.maxX-scene.minX)/cellXCount;
    cellY = (scene.maxY-scene.minY)/cellYCount;
    double area = cellX*5;
    vector<vector<int>> encodedScene = scene.encodeScene(cellXCount,cellYCount);
    for (int l=0;l<encodedScene.size();l++) {
        for (int m=0;m<encodedScene[l].size();m++) {
            if (encodedScene[l][m]>0) {
                out2<<1<<" ";
            }
            else {
                out2<<0<<" ";
            }
        }
        out2<<endl;
    }
    out2<<endl;
    vector<bool> already_taken(scene.obstacles.size());
    for (int i=0;i<already_taken.size();i++) {
        already_taken[i] = false;
    }
    int obstacle_count = pointCount/perObstaclePoint;
    int o = 0;
    int maxClassCount = perObstaclePoint*.6;
    while (o<obstacle_count) {
        int posCount, negCount;
        posCount = negCount = 0;
        int obs_index = randRangeInt(0,scene.obstacles.size());
        while (already_taken[obs_index]) obs_index = randRangeInt(0,scene.obstacles.size());
        already_taken[obs_index] = true;
        for (int i=0;posCount+negCount<perObstaclePoint;i++) {
            int direction = randRangeInt(0,4);
            pair<double,double> p1 = draw_point(scene,obs_index,direction,area);
            double x,y;
            x = p1.first;
            y = p1.second;
            while (encodedScene[min(cellXCount-1,(int)((x-scene.minX)/(cellX)))][min(cellYCount-1,(int)((y-scene.minY)/(cellY)))]>0) {
                direction = randRangeInt(0,4);
                p1 = draw_point(scene,obs_index,direction,area);
                x = p1.first;
                y = p1.second;
            }
            int direction1 = randRangeInt(0,4);
            pair<double,double> p2 = draw_point(scene,obs_index,direction1,area);
            double x1,y1;
            x1 = p2.first;
            y1 = p2.second;
            while (encodedScene[min(cellXCount-1,(int)((x1-scene.minX)/(cellX)))][min(cellYCount-1,(int)((y1-scene.minY)/(cellY)))]>0) {
                direction1 = randRangeInt(0,4);
                p2 = draw_point(scene,obs_index,direction1,area);
                x1 = p2.first;
                y1 = p2.second;
            }
            Point2D a(x,y);
            Point2D b(x1,y1);
            bool isVisible = false;
            if (encodedScene[(int)((x-scene.minX)/(cellX))][(int)((y-scene.minY)/(cellY))]==encodedScene[(int)((x1-scene.minX)/(cellX))][(int)((y1-scene.minY)/(cellY))] && encodedScene[(int)((x-scene.minX)/(cellX))][(int)((y-scene.minY)/(cellY))]!=0) {
                isVisible = false;
            }
            else {
                isVisible = scene.isVisible(a,b);
            }
            if (isVisible) {
                if (posCount<maxClassCount) {
                    out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" ";
                    out<<isVisible<<endl;
                    posCount++;
                }
            }
            else {
                if (negCount<maxClassCount) {
                    out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" ";
                    out<<isVisible<<endl;
                    negCount++;
                }
            }
        }
        o++;
    }
}

//void random_mapgen(Scene2D& scene, int pointCount, ofstream& out, ofstream& out2) {
//    double gridX,gridY;
//    gridX = gridY = 32;             // change this
//    vector<vector<bool>> encodedScene = scene.encodeSceneFile(8,8);  // 256/32=8  change this
//    int i = 0;
//    while (i<pointCount) {
//        int indexX,indexY;
//        indexX = randRangeInt(0,gridX);
//        indexY = randRangeInt(0,gridY);
//        if (encodedScene[indexX][indexY]==1) continue;
//        for (int l=0;l<encodedScene.size();l++) {
//            for (int m=0;m<encodedScene[l].size();m++) {
//                if (indexX==l && indexY==m) out<<5<<" ";
//                else out<<encodedScene[l][m]<<" ";
//            }
//            out<<endl;
//        }
//        out<<endl;
//        double x,y;
//        x = scene.minX+indexX*((scene.maxX-scene.minX)/gridX)+(scene.maxX-scene.minX)/(gridX*2.0);
//        y = scene.minY+indexY*((scene.maxY-scene.minY)/gridY)+(scene.maxY-scene.minY)/(gridY*2.0);
//        Point2D a(x,y);
//        for (int j=0;j<gridX;j++) {
//            for (int k=0;k<gridY;k++) {
//                if (indexX==j && indexY==k) out2<<1<<" ";
//                else {
//                    double x1,y1;
//                    x1 = scene.minX+j*((scene.maxX-scene.minX)/gridX)+(scene.maxX-scene.minX)/(gridX*2.0);
//                    y1 = scene.minY+k*((scene.maxY-scene.minY)/gridY)+(scene.maxY-scene.minY)/(gridY*2.0);
//                    Point2D b(x1,y1);
//                    out2<<scene.isVisible(a,b)<<" ";
//                }
//            }
//            out2<<endl;
//        }
//        out2<<endl;
//        i++;
//    }
//}
