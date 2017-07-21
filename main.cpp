#include <iostream>
#include <string>
#include <vector>
#include <math.h>

class Body {

private:
    double m_x, m_y, m_z, m_vx, m_vy, m_vz,m_x0, m_y0, m_z0, m_vx0, m_vy0, m_vz0, m_radius, m_gm, m_mass;
    std::string m_name;

public:
    Body(std::string name, double pos0[3], double vel0[3] ,
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

    std::vector<double> get_position(){
        return std::vector<double> {m_x, m_y, m_z};
    };

    std::vector<double> get_velocity() {
        return std::vector<double> {m_vx, m_vy, m_vz};
    };


    void set_position(double pos[3]) {
        m_x = pos[0], m_y = pos[1], m_z = pos[2];

    };

    void set_velocity(double vel[3]) {
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

    long self_n;

public:
    System(std::vector<std::string> names, double pos0[][3], double vel0[][3],
           double masses[], double gms[], double radii[]){

        // Initialize self_n obejects of Body type
        for (int i=0; i < self_n; i++){
            self_bodies.emplace_back(names[i], pos0[i], vel0[i], masses[i], gms[i], radii[i]);
        }

        for (int j = 0; j < self_n; ++j) {
            self_names.push_back(self_bodies[j].get_name());
        }

    };
    std::vector<std::vector<double>> get_positions(){

        std::vector<std::vector<double>> positions;

        for (int i = 0; i < self_n; ++i) {
            positions.push_back(self_bodies[i].get_position());
        }

        return positions;
    };

    std::vector<std::vector<double>> get_velocities(){

        std::vector<std::vector<double>> velocities;

        for (int i = 0; i < self_n; ++i) {
            velocities.push_back(self_bodies[i].get_velocity());
        }

        return velocities;
    };

    std::vector<double> acceleration(Body body1, Body body2){
        // Function to calulate the vector force on body2 from
        // body 1

        std::vector<double> pos1 = body1.get_position();
        std::vector<double> pos2 = body2.get_position();

        // If the positions are equal return the zero-vector
        if(pos1 == pos2) {
            return std::vector<double> {0, 0, 0};
        }

        std::vector<double> r12 = {pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]};
        double dist = std::sqrt(pow(r12[0],2)+pow(r12[0], 2)+pow(r12[0], 2));

        return std::vector<double> {-body1.get_GM()*r12[0]/pow(dist, 3), -body1.get_GM()*r12[2]/pow(dist, 3),
                                    body1.get_GM()*r12[2]/pow(dist, 3)};
    }

    std::vector<std::vector<double>> get_accelerations(){
        // Initialize the matrix that will hold the force vectors

        double accels[self_n][self_n][3];

        std::vector<std::vector<double>> accelerations;

        // Enter the values into the force matrix
        for (int i = 0; i < self_n; ++i) {
            for (int j = 0; j < self_n; ++j){
                std::vector<double> temp_accel = acceleration(self_bodies[i], self_bodies[j]);
                accels[i][j][0] = temp_accel[0];
                accels[i][j][1] = temp_accel[1];
                accels[i][j][2] = temp_accel[2];
            }
        }

        for (int i = 0; i < self_n; ++i) {
            double a = 0;
            double b = 0;
            double c = 0;

            for (int j = 0; j < self_n; ++j) {
                a += accels[i][j][0];
                b += accels[i][j][1];
                c += accels[i][j][2];
            }

            accelerations.push_back({a, b, c});
        }

        return accelerations;
    }


    void set_positions(double positions[][3]){
        for (int i = 0; i < self_n; ++i) {
            self_bodies[i].set_position(positions[i]);
        }
    };
    void set_velocities(double velocities[][3]){
        for (int i = 0; i < self_n; ++i) {
            self_bodies[i].set_velocity(velocities[i]);
        }
    };
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

