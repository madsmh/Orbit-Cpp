//
// Created by mads on 23-07-17.
//

#ifndef ORBIT3D_PLANETDATA_H
#define ORBIT3D_PLANETDATA_H

#include <vector>
#include "vector3.h"

struct HorizonsFile {
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
};

class PlanetData {
    std::vector<std::string> self_planet_names;
    std::vector<std::vector<Vector3> > self_positions;
    std::vector<std::vector<Vector3> > self_velocities;

    void structs_to_arrays();
public:
    explicit PlanetData(std::vector<std::string> names);

    std::vector<HorizonsFile> horizons_to_structs(std::string planet);

    std::vector<Vector3> get_body_positions(int body);

    std::vector<Vector3> get_body_velocities(int body);

    std::vector<Vector3> get_starting_positions();

    std::vector<Vector3> get_starting_velocities();

};

#endif //ORBIT3D_PLANETDATA_H
