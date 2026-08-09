#pragma once

#include <vector>
#include <string>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3> vertices;
    std::vector<Face> faces;

public:
    Model(const std::string& filename);

    const std::vector<Vec3>& get_vertices() const;
    const std::vector<Face>& get_faces() const;
};
