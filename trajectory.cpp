//
// Created by mads on 23-07-17.
//

#include "trajectory.h"

Trajectory::Trajectory(long n_trajectories, long n_rows) {

    self_n_trajectories = n_trajectories;

    for (unsigned int i = 0; i < self_n_trajectories; ++i) {
        self_positions.emplace_back(std::vector<Vector3> {});
        self_velocities.emplace_back(std::vector<Vector3> {});
    }
    self_n_rows = n_rows;
}

void Trajectory::set_position(std::vector<Vector3> pos, std::vector<Vector3> vel){
    for (unsigned int i = 0; i< self_n_trajectories ; ++i) {
        self_positions[i].emplace_back(pos[i]);
        self_velocities[i].emplace_back(vel[i]);
    }
}

std::vector<Vector3> Trajectory::get_trajectory_positions(int body){
    std::cout << self_positions[body].size() << "\n";
    return self_positions[body];
}

std::vector<Vector3> Trajectory::get_trajectory_velocities(int body) {
    return self_velocities[body];
}
std::vector<Vector3 > Trajectory::get_positions_at_index(int index){

    std::vector<Vector3 > positions;

    for (int j = 0; j < self_n_trajectories; ++j) {
        positions.emplace_back(self_positions[j][index]);
    }

    return positions;
}

std::vector<Vector3> Trajectory::get_velocities_at_index(int index) {

    std::vector<Vector3> velocities;

    for (int j = 0; j < self_n_trajectories; ++j) {
        velocities.emplace_back(self_velocities[j][index]);
    }

    return velocities;
}

