#include<bits/stdc++.h>
using namespace std;

double normalize(double x, double min, double max) {
    return ((x-min)/(max-min));
}

// high not included
int randRangeInt(int low, int high) {
    return low+rand()%(high-low);
}

//high also included?
double randRange(double low, double high) {
    return min(low,high)+((double)rand()/RAND_MAX)*(max(high,low)-min(low,high));
}

int roundDown(double value) {
    // Extract the integer part and the fractional part
    int integerPart = static_cast<int>(value);
    double fractionalPart = value - integerPart;

    // Check if the fractional part is less than or equal to 0.5
    if (fractionalPart <= 0.5) {
        return integerPart; // Round down
    } else {
        return integerPart + 1; // Round up
    }
}

int bound(int value, int min_value, int max_value) {
    return std::max(std::min(value, max_value), min_value);
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

    //cout<<"("<<x<<","<<y<<")"<<endl;

    // return true if intersect point is within the line segment
    if (((min(pointA.x,pointB.x)<=x && x<=max(pointA.x,pointB.x)) && (min(pointA.y,pointB.y)<=y && y<=max(pointA.y,pointB.y))) && ((min(mbrA.x,mbrB.x)<=x && x<=max(mbrA.x,mbrB.x)) && (min(mbrA.y,mbrB.y)<=y && y<=max(mbrA.y,mbrB.y)))) {
        return true;
    }
    return false;
}

vector<double> getXintersect(double y, vector<Point2D>& vertices) {
    vector<double> ans;
    for (int i=0;i<vertices.size();i++) {
        Point2D pointA = vertices[i];
        Point2D pointB = vertices[(i+1)%vertices.size()];
        double a1,b1,c1;
        a1 = (pointA.y-pointB.y);
        b1 = -(pointA.x-pointB.x);
        c1 = -pointA.x*(pointA.y-pointB.y)+pointA.y*(pointA.x-pointB.x);
        double a2,b2,c2;
        a2 = 0;
        b2 = 1;
        c2 = -y;
        if (a1*b2-a2*b1==0) {
            continue;
        }
        double x;
        x = (b1*c2-b2*c1)/(a1*b2-a2*b1);
        if (((min(pointA.x,pointB.x)<=x && x<=max(pointA.x,pointB.x)) && (min(pointA.y,pointB.y)<=y && y<=max(pointA.y,pointB.y)))) {
            ans.push_back(x);
        }
    }
    sort(ans.begin(),ans.end());
    if (ans.size() > 1) {
        if (ans[0] == ans[1]) {
            ans.erase(ans.begin()); // Remove the first element
        }
        if (ans.size() > 1) {
            if (ans[ans.size() - 1] == ans[ans.size() - 2]) {
                ans.pop_back(); // Remove the last element
            }
        }
    }
    return ans;
}

class Obstacle2D {
public:
    vector<Point2D> vertices;
    Obstacle2D() {

    }
    Obstacle2D(vector<Point2D> &vertices) {
        this->vertices = vertices;
    }
    void addVertex(Point2D vertex) {
        vertices.push_back(vertex);
    }
    ~Obstacle2D() {

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
    Scene2D(string obstacleFilePath) {
        // load obstacle data from file
        ifstream in(obstacleFilePath);
        double x,y;
        int cnt,prev_cnt,idx;
        prev_cnt = -1;
        double minX,minY,maxX,maxY;
        minX = minY = DBL_MAX;
        maxX = maxY = -DBL_MAX;
        while(!in.eof()) {
            in>>cnt>>idx>>x>>y;
            if (in.eof()) break;
            cout<<cnt<<" "<<idx<<" "<<x<<" "<<y<<endl;
            Point2D vertex(x,y);
            if (cnt!=prev_cnt) {
                Obstacle2D obs;
                obstacles.push_back(obs);
            }
            obstacles[cnt].addVertex(vertex);
            minX = min(minX,x);
            maxX = max(maxX,x);
            minY = min(minY,y);
            maxY = max(maxY,y);
        }
        in.close();
        double bufferX,bufferY;
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
        bufferX = (this->maxX-this->minX)*0.05;
        bufferY = (this->maxY-this->minY)*0.05;

        this->minX -= bufferX;
        this->maxX += bufferX;
        this->minY -= bufferY;
        this->maxY += bufferY;
        cout<<"minX: "<<this->minX<<endl;
        cout<<"maxX: "<<this->maxX<<endl;
        cout<<"minY: "<<this->minY<<endl;
        cout<<"maxY: "<<this->maxY<<endl;
        cout<<"bufferX: "<<bufferX<<endl;
        cout<<"bufferY: "<<bufferY<<endl;
    }
    double getArea() {
        return (this->maxX-this->minX)*(this->maxY-this->minY);
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
            for (int j=0;j<obstacles[i].vertices.size();j++) {
                if (intersect(a,b,obstacles[i].vertices[j],obstacles[i].vertices[(j+1)%obstacles[i].vertices.size()])) {
                    ans = false;
                    break;
                }
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
            double minY,maxY;
            minY = DBL_MAX;
            maxY = -DBL_MAX;
            for (int j=0;j<obstacles[i].vertices.size();j++) {
                minY = min(obstacles[i].vertices[j].y,minY);
                maxY = max(obstacles[i].vertices[j].y,maxY);
            }
            for (double j=this->minY+round((maxY-this->minY)/cellY)*cellY-cellY/2.0;j>=this->minY+roundDown((minY-this->minY)/cellY)*cellY;j-=cellY) {
                vector<double> ans = getXintersect(j,obstacles[i].vertices);
                for (int k=0;k<(int)ans.size()-1;k+=2) {
                    for (double l=this->minX+roundDown((ans[k]-this->minX)/cellX)*cellX+cellX/2.0;l<=this->minX+round((ans[k+1]-this->minX)/cellX)*cellX;l+=cellX) {
                        int x_idx, y_idx;
                        x_idx = floor((l-this->minX)/cellX);
                        y_idx = floor((j-this->minY)/cellY);
                        x_idx = bound(x_idx,0,cellXCount-1);
                        y_idx = bound(y_idx,0,cellYCount-1);
                        encodedScene[x_idx][y_idx] = i+1;
                    }
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

void random_transformer_gen(Scene2D& scene, int pointCount, ofstream& out) {
    int posCount, negCount;
    posCount = negCount = 0;
    int maxClassCount = pointCount*.6;
    for (int i=0;i<scene.obstacles.size();i++) {
        for (int j=0;j<scene.obstacles[i].vertices.size();j++) {
            out<<normalize(scene.obstacles[i].vertices[j].x,scene.minX,scene.maxX)<<" "<<normalize(scene.obstacles[i].vertices[j].y,scene.minY,scene.maxY)<<" ";
            out<<normalize(scene.obstacles[i].vertices[(j+1)%scene.obstacles[i].vertices.size()].x,scene.minX,scene.maxX)<<" "<<normalize(scene.obstacles[i].vertices[(j+1)%scene.obstacles[i].vertices.size()].y,scene.minY,scene.maxY)<<" ";
        }
    }
    for (int i=0;posCount+negCount<pointCount;i++) {
        double x,y;
        x = randRange(scene.minX,scene.maxX);
        y = randRange(scene.minY,scene.maxY);
        double x1,y1;
        x1 = randRange(scene.minX,scene.maxX);
        y1 = randRange(scene.minY,scene.maxY);
        Point2D a(x,y);
        Point2D b(x1,y1);
        bool isVisible = scene.isVisible(a,b);
        if (isVisible) {
            if (posCount<maxClassCount) {
                out<<"q ";
                out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" ";
                out<<isVisible<<" ";
                posCount++;
            }
        }
        else {
            if (negCount<maxClassCount) {
                out<<"q ";
                out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" ";
                out<<isVisible<<" ";
                negCount++;
            }
        }
    }
    out<<endl;
}

