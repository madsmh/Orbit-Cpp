#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

class Body {

    double m_x, m_y, m_z, m_vx, m_vy, m_vz,m_x0, m_y0, m_z0, m_vx0, m_vy0, m_vz0, m_radius, m_gm;
    std::string m_name;

public:
    Body(std::string name, std::vector<double> posvel0,
         double gm, double radius) {
        m_x0 = posvel0[0];
        m_y0 = posvel0[1];
        m_z0 = posvel0[2];

        m_x = m_x0;
        m_y = m_y0;
        m_z = m_z0;

        m_vx0 = posvel0[3];
        m_vy0 = posvel0[4];
        m_vz0 = posvel0[5];

        m_vx = m_vx0;
        m_vy = m_vy0;
        m_vz = m_vz0;

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

    std::vector<std::string> self_names;
    std::vector<Body> self_bodies;

    long self_n;

public:
    System(std::vector<std::string> names, std::vector<std::vector<double> > posvel0,
           std::vector<double> gms, std::vector<double> radii){

        self_n = names.size();

        // Initialize self_n obejects of Body type
        for (int i=0; i < self_n; i++){
            self_bodies.emplace_back(Body(names[i], posvel0[i], gms[i], radii[i]));
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

            accelerations.emplace_back(std::vector<double> {a, b, c});
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
    long self_n_trajectories;
    std::vector<std::vector<std::vector<double > > > self_trajectories;

public:
    Trajectory(long n_trajectories, long n_rows) {

        for (int i = 0; i < n_trajectories; ++i) {
            self_trajectories.emplace_back(std::vector<std::vector<double > > {});
        }

        self_n_trajectories = n_trajectories;
        self_n_rows = n_rows;
    }

    void set_position(std::vector<std::vector<double> > pos, std::vector<std::vector<double> > vel){
        for (int i = 0; i< self_n_trajectories ; ++i) {
            self_trajectories[i].emplace_back(std::vector<double > {pos[i][0],
                                                                    pos[i][1],
                                                                    pos[i][2],
                                                                    vel[i][0],
                                                                    vel[i][1],
                                                                    vel[i][2]});
        }
    }

    std::vector<std::vector<double > > get_trajectory(long i){
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
    long n_bodies = system.get_number_of_bodies();

    double delta2 = pow(delta, 2);

    for (int i = 0; i < n; ++i) {
        if(i == 0){
            std::vector<std::vector<double> > x0 = system.get_positions();
            std::vector<std::vector<double> > v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
        }
        else {
            std::vector<std::vector<double > > x0 = trajectory.get_positions_at_index(i-1);
            std::vector<std::vector<double > > v0 = trajectory.get_velocities_at_index(i-1);

            std::vector<std::vector<double> > a0 = system.get_accelerations();

            std::vector<std::vector<double> > x1;

            for (long j = 0; j < n_bodies; ++j) {
                x1.emplace_back(std::vector<double> {x0[j][0]+v0[j][0]*delta+0.5*a0[j][0]*delta2,
                                                     x0[j][1]+v0[j][1]*delta+0.5*a0[j][1]*delta2,
                                                     x0[j][2]+v0[j][2]*delta+0.5*a0[j][2]*delta2});
            }

            system.set_positions(x1);

            std::vector<std::vector<double> > a1 = system.get_accelerations();

            std::vector<std::vector<double> > v1;

            for (long k = 0; k < n_bodies; ++k) {
                v1.emplace_back(std::vector<double> {v0[k][0]+ 0.5* (a0[k][0]+a1[k][0]) * delta,
                                                     v0[k][1]+ 0.5* (a0[k][1]+a1[k][1]) * delta,
                                                     v0[k][2]+ 0.5* (a0[k][2]+a1[k][2]) * delta});
            }

            system.set_velocities(v1);

            trajectory.set_position(x1, v1);

        }
    }

}


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
public:
    explicit PlanetData(std::vector<std::string> names){
        self_planet_names = std::move(names);
    }

    std::vector<HorizonsFile> horizons_to_struct(const std::string &planet){
        std::string path = "data/" + planet + ".csv";
        std::transform(path.begin(), path.end(), path.begin(), ::tolower);

        std::ifstream filereader;
        HorizonsFile h = {};
        std::vector<HorizonsFile> data;

        std::string buffer;

        filereader.open(path);
        if (filereader.is_open()){
            std::getline(filereader, buffer);
            while(!filereader.eof()){
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                h.x = stod(buffer);
                std::getline(filereader, buffer, ',');
                h.y = stod(buffer);
                std::getline(filereader, buffer, ',');
                h.z = stod(buffer);
                std::getline(filereader, buffer, ',');
                h.vx = stod(buffer);
                std::getline(filereader, buffer, ',');
                h.vy = stod(buffer);
                std::getline(filereader, buffer, ',');
                h.vz = stod(buffer);
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, '\n');
                data.push_back(h);
            }
            filereader.close();
        }
        return data;
    }
    std::vector<std::vector<double > > get_horizons_data(const std::string &planet){
        std::vector<HorizonsFile> h_structs = horizons_to_struct(planet);
        std::vector<std::vector<double> > buffer;

        long n_of_structs = h_structs.size();

        for (int i = 0; i < n_of_structs; ++i) {
            HorizonsFile h = h_structs[i];
            buffer.emplace_back(std::vector<double> {h.x, h.y, h.z, h.vx, h.vy, h.vz});
        }

        return buffer;
    }

    std::vector<std::vector<double> > get_data(int i){
        return get_horizons_data(self_planet_names[i]);
    }

    std::vector<std::vector<double> > get_starting_values(){
        long n = self_planet_names.size();
        std::vector<std::vector<double> > statingvalues;

        for (int i = 0; i < n; ++i) {
            statingvalues.emplace_back(get_data(i)[0]);
        }

        return statingvalues;
    }
};


struct PropertiesFile {
    std::string name;
    double r{};
    double GM{};
};


class PhysicalProperties{
    std::vector<std::string> self_names;
    std::vector<double> self_radii;
    std::vector<double> self_GMs;

public:
    PhysicalProperties(){
        std::string path = "physical_properties/properties.csv";
        std::ifstream filereader;

        PropertiesFile p;

        std::vector<PropertiesFile> data;
        std::string buffer;

        filereader.open(path);
        if(filereader.is_open()){
            getline(filereader, buffer, '\n');
            while (!filereader.eof()){
                getline(filereader, buffer, ',');
                p.name = buffer;
                std::cout << "Reading data for object " << buffer << std::endl;
                getline(filereader, buffer, ',');
                p.GM = stod(buffer);
                getline(filereader, buffer, ',');
                p.r = stod(buffer);
                getline(filereader, buffer, '\n');

                data.emplace_back(p);
            }
            filereader.close();
        }


        long n = data.size();
        for (int i = 0; i < n; ++i) {

            PropertiesFile d = data[i];
            self_names.emplace_back(d.name);
            self_radii.emplace_back(d.r);
            self_GMs.emplace_back(d.GM);
        }
    }

    std::vector<std::string> get_names(){
        return self_names;
    };

    std::vector<double> get_radii(){
        return self_radii;
    }

    std::vector<double> get_GMs(){
        return self_GMs;
    }
};


int main() {

    int detail = 64;
    double dt = 86400/detail;
    long rows = 1131*detail;

    PhysicalProperties prop;
    PlanetData planet_data (prop.get_names());
    System sol (prop.get_names(),
                planet_data.get_starting_values(),
                prop.get_GMs(),
                prop.get_radii());

    long n_bodies = prop.get_names().size();

    Trajectory tra (n_bodies,rows);

    verlet(sol, tra, dt);

    std::vector<double> dists;

    std::vector<std::vector<double > > earth = tra.get_trajectory(0);
    //std::vector<std::vector<double > > earth_ref = planet_data.get_data(3);
    /*
    for (int i = 0; i < rows; i += detail) {
        dists.emplace_back(sqrt( pow(earth[i][0]-earth_ref[i][0], 2)+
                                 pow(earth[i][1]-earth_ref[i][1], 2)+
                                 pow(earth[i][2]-earth_ref[i][2], 2)));

    }
     double max_dist = *max_element(dists.begin(), dists.end());
    std::cout << max_dist << std::endl;*/
    return 0;
}

