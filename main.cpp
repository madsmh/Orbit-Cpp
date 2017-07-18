#include <iostream>
#include <string>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor-blas/xlinalg.hpp>
#include <vector>
#include <math.h>

class Body {

private:
    double m_x, m_y, m_z, m_vx, m_vy, m_vz,m_x0, m_y0, m_z0, m_vx0, m_vy0, m_vz0, m_radius, m_gm, m_mass;
    std::string m_name;

public:
    Body(std::string name, xt::xarray <double> pos0, xt::xarray <double> vel0 ,
         double mass, double gm, double radius) {
        m_x0 = pos0[0];
        m_y0 = pos0[1];
        m_z0 = pos0[2];

        m_x = m_x0;
        m_y = m_y0;
        m_z = m_z0;

        m_vx0 = vel0[0];
        m_vy0 = vel0[1];
        m_vz0 = vel0[2];

        m_gm = gm;
        m_radius = radius;
        m_mass = mass;

        m_name = name;
    }

    xt::xarray <double> get_position(){
        return xt::xarray<double> {m_x, m_y, m_z};
    };

    xt::xarray <double> get_velocity() {
        return xt::xarray<double> {m_vx, m_vy, m_vz};
    };

    xt::xarray <double> compute_acceleration(xt::xarray <double> force) {
        return force/m_mass;
    };

    void set_position(xt::xarray<double> pos) {
        m_x = pos[0], m_y = pos[1], m_z = pos[2];

    };

    void set_velocity(xt::xarray<double> vel) {
        m_x = vel[0], m_y = vel[1], m_z = vel[2];
    };


    double get_radius() {
        return m_radius;
    };

    double get_GM() {
        return m_gm;
    };

    std::string get_name() {
        return m_name;
    };

    double get_mass(){
        return m_mass;
    };

};

class System {
private:
    std::vector<std::string> self_names;
    std::vector<Body> self_bodies;

    xt::xarray <double> self_pos;
    xt::xarray <double> vel;
    xt::xarray <double> self_masses;
    xt::xarray <double> self_gms;
    xt::xarray <double> self_radii;

    long self_n;

public:
    System(std::vector<std::string> names, xt::xarray<double> pos, xt::xarray <double> vel, xt::xarray<double> masses,
           xt::xarray<double> gms, xt::xarray<double> radii){

        // Number of names, and thus number of Bodies to initialize
        self_n = names.size();

        // Initialize self_n obejects of Body type
        for (int i=0; i < self_n; i++){
            self_bodies.push_back(Body(names[i], pos[i], vel[i], masses[i], gms[i], radii[i]));
        }


    };
    xt::xarray <double> get_positions(){

        xt::xarray<double> positions = xt::zeros<double>({self_n, 3});

        for (int i = 0; i < self_n; ++i) {
            positions[i] = self_bodies[i].get_position();
        }
        return positions;
    };

    xt::xarray <double> get_velocities(){
        xt::xtensor <double, self_n*3> velocities = xt::zeros({self_n, 3});

        for (int i = 0; i < self_n; ++i) {
            velocities[i] = self_bodies[i].get_velocity();
        }

        return velocities;
    };
    xt::xarray <double> get_accelerations();

    xt::xarray<double> force(Body body1, Body body2){
        // Function to calulate the vector force on body2 from
        // body 1

        xt::xarray<double> pos1 = body1.get_position();
        xt::xarray<double> pos2 = body2.get_position();

        // If the positions are equal return the zero-vector
        if(xt::all(xt::equal(pos1, pos2))) {
            return xt::zeros<double>({1, 3});
        }

        xt::xarray<double> r12 = pos2 - pos1;
        double dist = xt::linalg::norm(r12);

        return -6.67259e-11 * body1.get_mass() * body2.get_mass()/pow(dist, 3) * r12;
    }

    xt::xarray <double> force_matrix(){
        // Initialize the matrix that will hold the force vectors
        xt::xarray <double> forces = xt::zeros({self_n, self_n, 3});

        // Enter the values into the force matrix
        for (int i = 0; i < self_n; ++i) {
            for (int j = 0; j < self_n; ++j)
                forces[{i, j}] = force(self_bodies[i], self_bodies[j]);
            }
    }


    void set_positions(xt::xarray<double>);
    void set_velocities(xt::xarray<double>);
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

