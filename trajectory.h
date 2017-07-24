//
// Created by mads on 23-07-17.
//

#ifndef ORBIT3D_TRAJECTORY_H
#define ORBIT3D_TRAJECTORY_H

#include "vector3.h"
#include <vector>

class Trajectory{

    long self_n_rows;
    long self_n_trajectories;
    std::vector<std::vector<Vector3>> self_positions;
    std::vector<std::vector<Vector3>>  self_velocities;

public:
    Trajectory(long n_trajectories, long n_rows);

    void set_position(std::vector<Vector3> pos, std::vector<Vector3> vel);

    std::vector<Vector3> get_trajectory_positions(int body);

    std::vector<Vector3> get_trajectory_velocities(int body);

    std::vector<Vector3 > get_positions_at_index(int index);

    std::vector<Vector3 > get_velocities_at_index(int index);

    long get_number_of_rows(){ return self_n_rows; }
};


#endif //ORBIT3D_TRAJECTORY_H
