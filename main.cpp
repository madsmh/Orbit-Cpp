#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "vector3.h"
#include "system.h"
#include "trajectory.h"




void verlet(System system, Trajectory trajectory, double delta){

    long n = trajectory.get_number_of_rows();
    long n_bodies = system.get_number_of_bodies();

    double delta2 = pow(delta, 2);

    for (int i = 0; i < n; ++i) {
        if(i == 0){
            std::vector<Vector3 > x0 = system.get_positions();
            std::vector<Vector3> v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
        }
        else {
            std::vector<Vector3> x0 = trajectory.get_positions_at_index(i-1);
            std::vector<Vector3> v0 = trajectory.get_velocities_at_index(i-1);

            std::vector<Vector3 > a0 = system.get_accelerations();

            std::vector<Vector3 > x1;

            for (long j = 0; j < n_bodies; ++j) {
                x1.emplace_back(Vector3 x0[j] + v0[j]*delta + delta2*0.5 *a0[j]);
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

