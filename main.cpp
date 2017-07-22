#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

class Body {

private:
    double m_x, m_y, m_z, m_vx, m_vy, m_vz,m_x0, m_y0, m_z0, m_vx0, m_vy0, m_vz0, m_radius, m_gm;
    std::string m_name;

public:
    Body(std::string name, std::vector<double> pos0, std::vector<double> vel0,
         double gm, double radius) {
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

        m_name = name;
    }

    std::vector<double> get_position(){
        return std::vector<double> {m_x, m_y, m_z};
    };

    std::vector<double> get_velocity() {
        return std::vector<double> {m_vx, m_vy, m_vz};
    };


    void set_position(std::vector<double> pos) {
        m_x = pos[0], m_y = pos[1], m_z = pos[2];

    };

    void set_velocity(std::vector<double> vel) {
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
};

class System {
private:
    std::vector<std::string> self_names;
    std::vector<Body> self_bodies;

    long self_n;

public:
    System(std::vector<std::string> names, std::vector<std::vector<double> > pos0,
           std::vector<std::vector<double> > vel0, double masses[], double gms[], double radii[]){

        self_n = names.size();

        // Initialize self_n obejects of Body type
        for (int i=0; i < self_n; i++){
            self_bodies.emplace_back(Body(names[i], pos0[i], vel0[i], gms[i], radii[i]));
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

        double f = -body1.get_GM()/pow(dist, 3);

        return std::vector<double> {f*r12[0],f*r12[2], f*r12[2]};
    }

    std::vector<std::vector<double>> get_accelerations(){
        // Initialize the matrix that will hold the force vectors

        double accels[self_n][self_n][3];

        std::vector<std::vector<double>> accelerations;

        // Enter the values into the acceleration matrix
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

    void set_positions(std::vector<std::vector<double> > positions){
        for (int i = 0; i < self_n; ++i) {
            self_bodies[i].set_position(positions[i]);
        }
    }

    void set_velocities(std::vector<std::vector<double> > velocities){
        for (int i = 0; i < self_n; ++i) {
            self_bodies[i].set_velocity(velocities[i]);
        }
    }

    long get_number_of_bodies(){
        return self_n;
    }
};

class Trajectory{
    long self_n_rows;
    int self_n_trajectories;
    std::vector<std::vector<std::vector<double > > > self_trajectories;

public:
    Trajectory(int n_trajectories, long n_rows) {

        for (int i = 0; i < n_trajectories; ++i) {
            self_trajectories.emplace_back(std::vector<std::vector<double > > {});
        }

        self_n_trajectories = n_trajectories;
        self_n_rows = n_rows;
    }

    void set_position(std::vector<std::vector<double> > pos, std::vector<std::vector<double> > vel){
        for (int i = 0; i< self_n_trajectories ; ++i) {
            self_trajectories[i].emplace_back(std::vector<double > {pos[i][0], pos[i][1], pos[i][2], vel[i][0],
                                            vel[i][1], vel[i][2]});
        }
    }

    std::vector<std::vector<double > > get_trajectory(int i){
        return self_trajectories[i];
    }

    std::vector<std::vector<double > > get_positions_at_index(int i){
        std::vector<std::vector<double> > positions;

        for (int j = 0; j < self_n_trajectories; ++j) {
            positions.emplace_back(std::vector<double> {self_trajectories[j][i][0],
                                                        self_trajectories[j][i][1],
                                                        self_trajectories[j][i][2]});
        }

        return positions;
    }

    std::vector<std::vector<double > > get_velocities_at_index(int i){

        std::vector<std::vector<double> > velocities;

        for (int j = 0; j < self_n_trajectories; ++j) {
            velocities.emplace_back(std::vector<double> {self_trajectories[j][i][3],
                                     self_trajectories[j][i][4],
                                     self_trajectories[j][i][5]});
        }

        return velocities;
    }

    long get_number_of_rows(){
        return self_n_rows;
    }
};


void verlet(System system, Trajectory trajectory, double delta){

    long n = trajectory.get_number_of_rows();
    double delta2 = pow(delta, 2);

    for (int i = 0; i < n; ++i) {
        if(i==1){
            std::vector<std::vector<double> > x0 = system.get_positions();
            std::vector<std::vector<double> > v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
        }
        else {
            std::vector<std::vector<double > > x0 = trajectory.get_positions_at_index(i-1);
            std::vector<std::vector<double > > v0 = trajectory.get_velocities_at_index(i-1);

            std::vector<std::vector<double> > a0 = system.get_accelerations();

            std::vector<std::vector<double> > x1;

            for (int j = 0; j < system.get_number_of_bodies(); ++j) {
                x1.emplace_back(std::vector<double> {x0[j][0]+v0[j][0]*delta+0.5*a0[j][0]*delta2,
                                                     x0[j][1]+v0[j][1]*delta+0.5*a0[j][1]*delta2,
                                                     x0[j][2]+v0[j][2]*delta+0.5*a0[j][2]*delta2});
            }

            system.set_positions(x1);

            std::vector<std::vector<double> > a1 = system.get_accelerations();

            std::vector<std::vector<double> > v1;

            for (int k = 0; k < system.get_number_of_bodies(); ++k) {
                v1.emplace_back(std::vector<double> {v0[k][0]+ 0.5* (a0[k][0]+a1[k][0]) * delta,
                                                     v0[k][1]+ 0.5* (a0[k][1]+a1[k][1]) * delta,
                                                     v0[k][2]+ 0.5* (a0[k][2]+a1[k][2]) * delta});
            }

            system.set_velocities(v1);

            trajectory.set_position(x1, v1);

        }
    }

}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

