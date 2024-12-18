#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

struct Vertex {
    float x, y;
};

struct Polygon {
    std::vector<Vertex> vertices;
};

struct Scene {
    unordered_map<int,Polygon> polygons;
    float min_x,max_x,min_y,max_y;
};

bool isInside(const Vertex& v, float x_min, float x_max, float y_min, float y_max, int edge) {
    if (edge==0) {
        return v.x>=x_min;
    }
    else if (edge==1) {
        return v.x<=x_max;
    }
    else if (edge==2) {
        return v.y>=y_min;
    }
    else if (edge==3) {
        return v.y<=y_max;
    }
    return false;
}

Vertex intersect(const Vertex& v1, const Vertex& v2, float x_min, float x_max, float y_min, float y_max, int edge) {
    Vertex result;
    float slope;
    if (edge == 0) { // Left edge: x = x_min
        slope = (v2.y - v1.y) / (v2.x - v1.x);
        result.x = x_min;
        result.y = v1.y + slope * (x_min - v1.x);
    } else if (edge == 1) { // Right edge: x = x_max
        slope = (v2.y - v1.y) / (v2.x - v1.x);
        result.x = x_max;
        result.y = v1.y + slope * (x_max - v1.x);
    } else if (edge == 2) { // Bottom edge: y = y_min
        slope = (v2.x - v1.x) / (v2.y - v1.y);
        result.y = y_min;
        result.x = v1.x + slope * (y_min - v1.y);
    } else if (edge == 3) { // Top edge: y = y_max
        slope = (v2.x - v1.x) / (v2.y - v1.y);
        result.y = y_max;
        result.x = v1.x + slope * (y_max - v1.y);
    }
    return result;
}

void sutherlandHodgman(Polygon& polygon, float x_min, float x_max, float y_min, float y_max) {
    std::vector<Vertex> inputVertices = polygon.vertices;
    std::vector<Vertex> outputVertices;

    for (int edge = 0; edge < 4; ++edge) {
        outputVertices.clear();
        for (size_t i = 0; i < inputVertices.size(); ++i) {
            Vertex curr = inputVertices[i];
            Vertex prev = inputVertices[(i + inputVertices.size() - 1) % inputVertices.size()];

            bool currInside = isInside(curr, x_min, x_max, y_min, y_max, edge);
            bool prevInside = isInside(prev, x_min, x_max, y_min, y_max, edge);

            if (prevInside && currInside) {
                outputVertices.push_back(curr);
            } else if (prevInside && !currInside) {
                outputVertices.push_back(intersect(prev, curr, x_min, x_max, y_min, y_max, edge));
            } else if (!prevInside && currInside) {
                outputVertices.push_back(intersect(prev, curr, x_min, x_max, y_min, y_max, edge));
                outputVertices.push_back(curr);
            }
        }
        inputVertices = outputVertices;
    }

    polygon.vertices = outputVertices;
}


void parseInputFile(const std::string& filename, unordered_map<int,Scene>& scenes) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    int scene_id, polygon_id, vertex_id;
    float x, y;
    string type;
    float min_x,max_x,min_y,max_y;
    scene_id = 0;
    while (infile >> type) {
        if (type=="a") {
            infile >>scene_id>> polygon_id >> vertex_id >> x >> y;
            if (scene_id>=25) break;
            scenes[scene_id].polygons[polygon_id].vertices.push_back({x, y});
            //cout<<type<<" "<<scene_id<<" "<<polygon_id<<" "<<vertex_id<<" "<<x<<" "<<y<<endl;
        }
        else if (type=="b") {
            cout<<scene_id<<endl;
            infile >> min_x >> max_x >> min_y >> max_y;
            scenes[scene_id].min_x = min_x;
            scenes[scene_id].max_x = max_x;
            scenes[scene_id].min_y = min_y;
            scenes[scene_id].max_y = max_y;
            scene_id++;
            //cout<<type<<" "<<min_x<<" "<<max_x<<" "<<min_y<<" "<<max_y<<endl;
        }

    }
}

void generateScenes(int num_scenes, int scene_size, const unordered_map<int,Scene>& scenes,
                    const std::string& output_filename) {
    std::ofstream outfile(output_filename);
    if (!outfile) {
        std::cerr << "Error creating output file: " << output_filename << std::endl;
        exit(1);
    }

    int scene_id = 0;
    int t = 0;

    for (auto it1 = scenes.begin(); it1 != scenes.end(); ++it1) {
        Scene scene = it1->second;
        t++;
        // Random scene generation
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist_x(scene.min_x, scene.max_x - scene_size);
        std::uniform_real_distribution<float> dist_y(scene.min_y, scene.max_y - scene_size);


        for (; scene_id < num_scenes*t; ++scene_id) {
            float x_min = dist_x(rng);
            float y_min = dist_y(rng);
            float x_max = x_min + scene_size;
            float y_max = y_min + scene_size;

            int new_polygon_id = 0;

            // Use iterator explicitly to iterate over the map
            for (auto it = scene.polygons.begin(); it != scene.polygons.end(); ++it) {
                Polygon clippedPolygon = it->second;
                sutherlandHodgman(clippedPolygon, x_min, x_max, y_min, y_max);

                if (!clippedPolygon.vertices.empty()) {
                    for (size_t vertex_id = 0; vertex_id < clippedPolygon.vertices.size(); ++vertex_id) {
                        const Vertex& v = clippedPolygon.vertices[vertex_id];
                        outfile <<"a "<< scene_id << " " << new_polygon_id << " " << vertex_id << " "
                                << std::fixed << std::setprecision(6) << v.x << " " << v.y << "\n";
                    }
                    ++new_polygon_id;
                }
            }
            outfile<<"b "<<x_min<<" "<<x_max<<" "<<y_min<<" "<<y_max<<"\n";
        }
        cout<<t<<endl;
    }
}

int main() {
    unordered_map<int,Scene> scenes;

    parseInputFile("../obstacles/dhaka_1000_20000.txt", scenes);

    generateScenes(1000, 128, scenes, "../obstacles/dhaka_1000_20000_1000_128.txt");

    return 0;
}


