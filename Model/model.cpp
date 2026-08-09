#include "model.h"

#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::string& filename) {

    std::ifstream in(filename);

    if (!in.is_open()) {
        std::cerr << "Cannot open file: " << filename << '\n';
        return;
    }

    std::string line;

    while (std::getline(in, line)) {

        std::istringstream iss(line);

        if (line.substr(0, 2) == "v ") {

            char type;
            Vec3 v;

            iss >> type >> v.x >> v.y >> v.z;

            vertices.push_back(v);
        }

        else if (line.substr(0, 2) == "f ") {

            char type;
            iss >> type;

            std::string s1, s2, s3;
            iss >> s1 >> s2 >> s3;

            Face f;

            f.a = std::stoi(s1.substr(0, s1.find('/'))) - 1;
            f.b = std::stoi(s2.substr(0, s2.find('/'))) - 1;
            f.c = std::stoi(s3.substr(0, s3.find('/'))) - 1;

            faces.push_back(f);
        }
    }
}

const std::vector<Vec3>& Model::get_vertices() const {
    return vertices;
}

const std::vector<Face>& Model::get_faces() const {
    return faces;
}
