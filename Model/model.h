#pragma once

#include <vector>
#include <string>
#include "geometry.h"

class Model {
private:
    std::vector<Vertice> vertices;
    std::vector<Face> faces;

public:
    Model(const std::string& filename);

    const std::vector<Vertice>& get_vertices() const;
    const std::vector<Face>& get_faces() const;
};