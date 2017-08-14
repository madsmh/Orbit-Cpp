/* Copyright (c) 2017 By the Authors
 *
 * This file is part of Orbit Simulator.

    Orbit Simulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Orbit Simulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"
#include "scene.h"
#include "mainwindow.h"

#include <boost/timer.hpp>



#include <QApplication>



void compare_with_horizon(Trajectory tra, PlanetData data,
                          std::vector<std::string> names,double detail, long ref_rows){

    for (int j = 0; j < tra.get_number_of_trajectories(); ++j) {
        std::vector<Vector3> sim_vector = tra.get_trajectory_positions(j);
        std::vector<Vector3> ref_vector = data.get_body_positions(j);

        std::vector<double> dists;

        for (int i = 0; i < ref_rows; ++i) {
            double error = (sim_vector[i*detail]-ref_vector[i]).norm();
            dists.emplace_back(error);
        }

        double max_dist = *std::max_element(dists.begin(), dists.end())/1000;
        std::cout << "Error for " << names[j] << " is : " << max_dist << " km" << std::endl;

    }

}


void help(char *name) {
    std::cout << "Usage: " << name << " <options>" << std::endl;
    std::cout << "-d <detail> : Number of time steps each day (default: 64)." << std::endl;
}

int main(int argc, char **argv) {
    // Adapted from https://doc.qt.io/qt-5/qt3d-basicshapes-cpp-main-cpp.html

    QApplication app (argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();

}

