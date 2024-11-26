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

class Point3D {
public:
    double x;
    double y;
    double z;
    Point3D() {
        x = y = z = 0;
    }
    Point3D(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point3D(const Point3D& point) {
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
    }
    Point3D(Point3D* point) {
        this->x = point->x;
        this->y = point->y;
        this->z = point->z;
    }
};

class Plane3D {
public:
    Point3D* a;
    Point3D* b;
    Point3D* c;
    Plane3D() {
        a = new Point3D();
        b = new Point3D();
        c = new Point3D();
    }
    Plane3D(Point3D& a, Point3D& b, Point3D& c) {
        a = new Point3D(a);
        b = new Point3D(b);
        c = new Point3D(c);
    }
    Plane3D(Point3D* a, Point3D* b, Point3D* c) {
        a = new Point3D(a);
        b = new Point3D(b);
        c = new Point3D(c);
    }
    Plane3D(const Plane3D& plane) {
        a = new Point3D(plane.a);
        b = new Point3D(plane.b);
        c = new Point3D(plane.c);
    }
    Plane3D(double x1, double x2, double x3, double y1, double y2, double y3, double z1, double z2, double z3) {
        a = new Point3D(x1,y1,z1);
        b = new Point3D(x2,y2,z2);
        c = new Point3D(x3,y3,z3);
    }
    ~Plane3D() {
        delete a;
        delete b;
        delete c;
    }

};

double determinant3x3(const vector<vector<double>>& matrix) {
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

// Function to solve the system using Cramer's rule
bool solveUsingCramer(const vector<vector<double>>& matrix, const vector<double>& results) {
    // Calculate main determinant D
    double D = determinant3x3(matrix);

    if (D == 0) {
        return false;
    }

    // Calculate D_x
    vector<vector<double>> Dx = matrix;
    for (int i = 0; i < 3; i++) Dx[i][0] = results[i];
    double Dx_det = determinant3x3(Dx);

    // Calculate D_y
    vector<vector<double>> Dy = matrix;
    for (int i = 0; i < 3; i++) Dy[i][1] = results[i];
    double Dy_det = determinant3x3(Dy);

    // Calculate D_z
    vector<vector<double>> Dz = matrix;
    for (int i = 0; i < 3; i++) Dz[i][2] = results[i];
    double Dz_det = determinant3x3(Dz);

    // Calculate solutions
    double x = Dx_det / D;
    double y = Dy_det / D;
    double z = Dz_det / D;


    if (x<0 || x>1) return false;
    if (y<0 || y>1) return false;
    if (z<0 || z>1) return false;
    return true;
}


bool intersect3D(Point3D& pointA, Point3D& pointB, Plane3D& plane) {
    vector<vector<double>> matrix = {
        {plane.b->x-plane.c->x, plane.a->x-plane.c->x, pointA.x-pointB.x},
        {plane.b->y-plane.c->y, plane.a->y-plane.c->y, pointA.y-pointB.y},
        {plane.b->z-plane.c->z, plane.a->z-plane.c->z, pointA.z-pointB.z}
    };
    vector<double> results = {pointA.x-plane.c->x, pointA.y-plane.c->y, pointA.z-plane.c->z};

    return solveUsingCramer(matrix, results);

}

class Obstacle3D {
public:
    Point3D* a;
    Point3D* b;
    Obstacle3D() {
        a = new Point3D();
        b = new Point3D();
    }
    Obstacle3D(Point3D& a, Point3D& b) {
        a = new Point3D(a);
        b = new Point3D(b);
    }
    Obstacle3D(Point3D* a, Point3D* b) {
        a = new Point3D(a);
        b = new Point3D(b);
    }
    Obstacle3D(const Obstacle3D& obstacle) {
        a = new Point3D(obstacle.a);
        b = new Point3D(obstacle.b);
    }
    Obstacle3D(double x1, double x2, double y1, double y2, double z1, double z2) {
        a = new Point3D(x1,y1,z1);
        b = new Point3D(x2,y2,z2);
    }
    ~Obstacle3D() {
        delete a;
        delete b;
    }

};

class Scene3D {
public:
    vector<Obstacle3D> obstacles;
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minZ;
    double maxZ;
    Scene3D() {
        this->maxX = -DBL_MAX;
        this->minX = DBL_MAX;
        this->maxY = -DBL_MAX;
        this->minY = DBL_MAX;
        this->maxZ = -DBL_MAX;
        this->minZ = DBL_MAX;
    }
    Scene3D(double width, double height, double depth) {
        this->maxX = width;
        this->minX = 0;
        this->maxY = height;
        this->minY = 0;
        this->maxZ = depth;
        this->minZ = 0;
    }
    void addObstacle(double x1,double x2,double y1,double y2, double z1, double z2) {
        Obstacle3D obs(x1,x2,y1,y2,z1,z2);
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
    void setMinZ(double minZ) {
        this->minZ = minZ;
    }
    void setMaxZ(double maxZ) {
        this->maxZ = maxZ;
    }
    bool isVisible(Point3D& a, Point3D& b) {
        bool ans = true;
        for (int i=0;i<obstacles.size();i++) {
            Plane3D p1(obstacles[i].a->x,obstacles[i].b->x,obstacles[i].a->x,obstacles[i].a->y,obstacles[i].b->y,obstacles[i].b->y,obstacles[i].a->z,obstacles[i].a->z,obstacles[i].a->z);
            Plane3D p2(obstacles[i].a->x,obstacles[i].b->x,obstacles[i].a->x,obstacles[i].a->y,obstacles[i].b->y,obstacles[i].b->y,obstacles[i].b->z,obstacles[i].b->z,obstacles[i].b->z);
            Plane3D p3(obstacles[i].a->x,obstacles[i].a->x,obstacles[i].a->x,obstacles[i].a->y,obstacles[i].b->y,obstacles[i].b->y,obstacles[i].a->z,obstacles[i].b->z,obstacles[i].a->z);
            Plane3D p4(obstacles[i].b->x,obstacles[i].b->x,obstacles[i].b->x,obstacles[i].a->y,obstacles[i].b->y,obstacles[i].b->y,obstacles[i].a->z,obstacles[i].b->z,obstacles[i].a->z);
            Plane3D p5(obstacles[i].a->x,obstacles[i].b->x,obstacles[i].a->x,obstacles[i].a->y,obstacles[i].a->y,obstacles[i].a->y,obstacles[i].a->z,obstacles[i].b->z,obstacles[i].b->z);
            Plane3D p6(obstacles[i].a->x,obstacles[i].b->x,obstacles[i].a->x,obstacles[i].b->y,obstacles[i].b->y,obstacles[i].b->y,obstacles[i].a->z,obstacles[i].b->z,obstacles[i].b->z);
            if (intersect3D(a,b,p1)) {
                ans = false;
                break;
            }
            else if (intersect3D(a,b,p2)) {
                ans = false;
                break;
            }
            else if (intersect3D(a,b,p3)) {
                ans = false;
                break;
            }
            else if (intersect3D(a,b,p4)) {
                ans = false;
                break;
            }
            else if (intersect3D(a,b,p5)) {
                ans = false;
                break;
            }
            else if (intersect3D(a,b,p6)) {
                ans = false;
                break;
            }
        }
        return ans;
    }
    vector<vector<double>> encodeScene(int cellXCount, int cellYCount, int cellZCount) {
        double cellX, cellY, cellZ;
        cellX = (maxX-minX)/cellXCount;
        cellY = (maxY-minY)/cellYCount;
        cellZ = (maxZ-minZ)/cellZCount;
        vector<vector<double>> encodedScene(cellXCount,vector<double>(cellYCount));
        for (int i=0;i<cellXCount;i++) {
            for (int j=0;j<cellYCount;j++) {
                encodedScene[i][j] = 0.0;
            }
        }

        for (int i=0;i<obstacles.size();i++) {
            double minX,maxX,minY,maxY,minZ,maxZ;
            minX = min(obstacles[i].a->x,obstacles[i].b->x);
            maxX = max(obstacles[i].a->x,obstacles[i].b->x);
            minY = min(obstacles[i].a->y,obstacles[i].b->y);
            maxY = max(obstacles[i].a->y,obstacles[i].b->y);
            minZ = min(obstacles[i].a->z,obstacles[i].b->z);
            maxZ = max(obstacles[i].a->z,obstacles[i].b->z);

            for (int j=floor((minX-this->minX)/cellX);j<ceil((maxX-this->minX)/cellX);j++) {
                for (int k=floor((minY-this->minY)/cellY);k<ceil((maxY-this->minY)/cellY);k++) {
                    if (encodedScene[j][k] < (maxZ-this->minZ)/cellZ) {
                        encodedScene[j][k] = (maxZ-this->minZ)/cellZ;
                    }
                }
            }
        }
        return encodedScene;
    }
};

void randomgen3D(Scene3D& scene, int pointCount, int cellXCount, int cellYCount, int cellZCount, ofstream& out, ofstream& out2) {
    double cellX,cellY,cellZ;
    int perGridPoint;
    cellX = (scene.maxX-scene.minX)/cellXCount;
    cellY = (scene.maxY-scene.minY)/cellYCount;
    perGridPoint = 1;
    int posCount, negCount;
    posCount = negCount = 0;
    int maxClassCount = pointCount*.6;
    vector<vector<double>> encodedScene = scene.encodeScene(cellXCount,cellYCount,cellZCount);
    for (int l=0;l<encodedScene.size();l++) {
        for (int m=0;m<encodedScene[l].size();m++) {
            out2<<encodedScene[l][m]<<" ";
        }
        out2<<endl;
    }
    out2<<endl;
    for (int i=0;posCount+negCount<pointCount;i++) {
        int indexX,indexY,indexZ;
        indexX = randRangeInt(0,cellXCount);
        indexY = randRangeInt(0,cellYCount);
        indexZ = randRangeInt(0,cellZCount);
        for (int j=0;j<perGridPoint;j++) {
            double x,y,z;
            x = scene.minX+indexX*cellX+randRange(0.0,cellX);
            y = scene.minY+indexY*cellY+randRange(0.0,cellY);
            z = scene.minZ+indexZ*cellZ+randRange(0.0,cellZ);
            int indexX1,indexY1,indexZ1;
            double x1,y1,z1;
            indexX1 = randRangeInt(0,cellXCount);
            indexY1 = randRangeInt(0,cellYCount);
            indexZ1 = randRangeInt(0,cellZCount);
            x1 = scene.minX+indexX1*cellX+randRange(0.0,cellX);
            y1 = scene.minY+indexY1*cellY+randRange(0.0,cellY);
            z1 = scene.minZ+indexZ1*cellZ+randRange(0.0,cellZ);
            Point3D a(x,y,z);
            Point3D b(x1,y1,z1);
            // need to implemnet inside obstacle condition
            bool isvisible = scene.isVisible(a,b);
            if (isvisible) {
                if (posCount<maxClassCount) {
                    out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(z-scene.minZ)/(scene.maxZ-scene.minZ)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" "<<(z1-scene.minZ)/(scene.maxZ-scene.minZ)<<" ";
                    out<<isvisible<<endl;
                    posCount++;
                }
            }
            else {
                if (negCount<maxClassCount) {
                    out<<(x-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y-scene.minY)/(scene.maxY-scene.minY)<<" "<<(z-scene.minZ)/(scene.maxZ-scene.minZ)<<" "<<(x1-scene.minX)/(scene.maxX-scene.minX)<<" "<<(y1-scene.minY)/(scene.maxY-scene.minY)<<" "<<(z1-scene.minZ)/(scene.maxZ-scene.minZ)<<" ";
                    out<<isvisible<<endl;
                    negCount++;
                }
            }
        }
    }
}

