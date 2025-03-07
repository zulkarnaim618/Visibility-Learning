#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

struct Vertex {
    double x, y;
};

struct Polygon {
    std::vector<Vertex> vertices;
};

struct Scene {
    unordered_map<int,Polygon> polygons;
    double min_x,max_x,min_y,max_y;
};

void parseInputFile(const std::string& filename, unordered_map<int,Scene>& scenes) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    int scene_id, polygon_id, vertex_id;
    double x, y;
    string type;
    double min_x,max_x,min_y,max_y;
    scene_id = 0;
    while (infile >> type) {
        if (type=="a") {
            infile >>scene_id>> polygon_id >> vertex_id >> x >> y;
            scenes[scene_id].polygons[polygon_id].vertices.push_back({x, y});
        }
        else if (type=="b") {
            infile >> min_x >> max_x >> min_y >> max_y;
            scenes[scene_id].min_x = min_x;
            scenes[scene_id].max_x = max_x;
            scenes[scene_id].min_y = min_y;
            scenes[scene_id].max_y = max_y;
            cout<<"Input scene "<<scene_id<<" parsing finished"<<endl;
            scene_id++;
        }
    }
}

// Function to calculate the area of a polygon
double calculatePolygonArea(const vector<Vertex>& vertices) {
    if (vertices.size() < 3) {
        cerr << "A polygon must have at least 3 vertices." << endl;
        return 0.0;
    }

    // Find the minimum x and y to normalize the coordinates
    double x_min = vertices[0].x, y_min = vertices[0].y;
    for (const auto& vertex : vertices) {
        x_min = min(x_min, vertex.x);
        y_min = min(y_min, vertex.y);
    }

    // Normalize the coordinates
    vector<Vertex> normalizedVertices;
    for (const auto& vertex : vertices) {
        normalizedVertices.push_back({vertex.x - x_min, vertex.y - y_min});
    }

    // Close the polygon by appending the first vertex at the end
    normalizedVertices.push_back(normalizedVertices[0]);

    // Apply the Shoelace formula
    double area = 0.0;
    for (size_t i = 0; i < normalizedVertices.size() - 1; ++i) {
        double x_i = normalizedVertices[i].x;
        double y_i = normalizedVertices[i].y;
        double x_next = normalizedVertices[i + 1].x;
        double y_next = normalizedVertices[i + 1].y;
        area += (x_i * y_next) - (y_i * x_next);
    }

    return abs(area) / 2.0;
}

void generateScenes(const unordered_map<int,Scene>& scenes, double coverage_lower_limit, double coverage_upper_limit,
                    const std::string& output_filename) {
    std::ofstream outfile(output_filename);
    if (!outfile) {
        std::cerr << "Error creating output file: " << output_filename << std::endl;
        exit(1);
    }

    int scene_id = 0;
    int min_poly, max_poly;
    double scene_area;

    min_poly = INT_MAX;
    max_poly = -INT_MAX;
    int t = 0;
    int cov[5] ={0,0,0,0,0};
    for (auto it1 = scenes.begin(); it1 != scenes.end(); ++it1) {
        Scene scene = it1->second;


        double total_area = 0;
        for (int ll = 0;ll<scene.polygons.size();ll++) {
            total_area+= calculatePolygonArea(scene.polygons[ll].vertices);
        }
        scene_area = (scene.max_x-scene.min_x)*(scene.max_y-scene.min_y);
        //cout<<total_area<<" "<<scene_area<<" "<<total_area/scene_area<<" "<<scene.polygons.size()<<endl;

        //continue;
        if (total_area>scene_area*coverage_lower_limit*0.01 && total_area<=scene_area*coverage_upper_limit*0.01) {
            if (cov[((int)(((total_area/scene_area)*100)/10))]>=250) continue;
            cov[((int)(((total_area/scene_area)*100)/10))]++;
            min_poly = min(min_poly,(int)scene.polygons.size());
            max_poly = max(max_poly,(int)scene.polygons.size());

            //cout<<total_area<<" "<<scene_area<<" "<<total_area/scene_area<<" "<<scene.polygons.size()<<endl;
            int new_polygon_id = 0;

            // Use iterator explicitly to iterate over the map
            for (auto it = scene.polygons.begin(); it != scene.polygons.end(); ++it) {
                Polygon clippedPolygon = it->second;
                for (size_t vertex_id = 0; vertex_id < clippedPolygon.vertices.size(); ++vertex_id) {
                    const Vertex& v = clippedPolygon.vertices[vertex_id];
                    outfile <<"a "<< scene_id << " " << new_polygon_id << " " << vertex_id << " "
                            << std::fixed << std::setprecision(6) << v.x << " " << v.y << "\n";
                }
                ++new_polygon_id;
            }
            outfile<<"b "<<scene.min_x<<" "<<scene.max_x<<" "<<scene.min_y<<" "<<scene.max_y<<"\n";
            scene_id++;
        }
    }
    cout<<min_poly<<" "<<max_poly<<endl;
    cout<<scene_id<<endl;
}

int main() {
    unordered_map<int,Scene> scenes;

    parseInputFile("../../obstacles/mexico_1048576_128.txt", scenes);

    generateScenes(scenes, 0, 60, "../../obstacles/mexico_1048576_128_cov_0_60.txt");

    return 0;
}


