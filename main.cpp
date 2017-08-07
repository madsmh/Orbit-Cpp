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

#include <boost/timer.hpp>

#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qorbitcameracontroller.h>



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

void verlet(System &system, Trajectory &trajectory, double delta){
    boost::timer t;
    long n = trajectory.get_number_of_rows();
    long n_bodies = system.get_number_of_bodies();

    std::vector<double> mechanical_energy;
    std::vector<Vector3> momentum;
    std::vector<Vector3> angular_momentum;

    double delta2 = std::pow(delta, 2);

    std::cout << "Starting integrator." << std::endl;

    for (int i = 0; i < n; ++i) {
        if(i == 0){
            std::vector<Vector3 > x0 = system.get_positions();
            std::vector<Vector3> v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
            system.set_positions(x0);
            system.set_velocities(v0);

            mechanical_energy.emplace_back(system.get_total_mechanical_energy());
        }
        else {

            std::vector<Vector3> x0 = trajectory.get_positions_at_index(i-1);
            std::vector<Vector3> v0 = trajectory.get_velocities_at_index(i-1);

            std::vector<Vector3> a0 = system.get_accelerations();

            std::vector<Vector3> x1;

            for (long j = 0; j < n_bodies; ++j) {
                x1.emplace_back(x0[j] + v0[j] * delta + delta2 * 0.5 * a0[j]);
            }

            system.set_positions(x1);

            std::vector<Vector3 > a1 = system.get_accelerations();

            std::vector<Vector3 > v1;

            for (long k = 0; k < n_bodies; ++k) {
                v1.emplace_back(v0[k]+ 0.5 * delta *( a0[k]+a1[k]) );
            }

            system.set_velocities(v1);

            mechanical_energy.emplace_back(system.get_total_mechanical_energy());

            trajectory.set_position(x1, v1);
        }
    }
    std::cout << "Integration finished in " << t.elapsed() << " seconds."<< std::endl;
    double max_mech_energy = *std::max_element(mechanical_energy.begin(), mechanical_energy.end());
    double min_mech_energy = *std::min_element(mechanical_energy.begin(), mechanical_energy.end());
    double diff_mech_energy =  std::abs(max_mech_energy-min_mech_energy);
    double rel_mech_energy = diff_mech_energy/std::abs(max_mech_energy);

    std::cout << "Mechanical energy, relative difference "<< rel_mech_energy << std::endl;

}

void help(char *name) {
    std::cout << "Usage: " << name << " <options>" << std::endl;
    std::cout << "-d <detail> : Number of time steps each day (default: 64)." << std::endl;
}

int main(int argc, char **argv) {
    // Adapted from https://doc.qt.io/qt-5/qt3d-basicshapes-cpp-main-cpp.html
    QApplication app (argc, argv);
    auto *view = new Qt3DExtras::Qt3DWindow();

    PhysicalProperties prop;
    PlanetData horzons(prop.get_names());
    std::vector<Vector3> starting_pos = horzons.get_starting_positions();

    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    auto *widget = new QWidget;
    auto *hlayout = new QHBoxLayout(widget);
    auto *vlayout = new QVBoxLayout();
    vlayout->setAlignment(Qt::AlignTop);
    hlayout->addWidget(container, 1);
    hlayout->addLayout(vlayout);

    widget->setWindowTitle(QStringLiteral("Orbit Solar System Simulator"));

    auto *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    auto *rootEntity = new Qt3DCore::QEntity();

    Qt3DRender::QCamera *cameraEntity = view->camera();

    Vector3 earth_pos = starting_pos[3];
    double earth_rad = prop.get_radii()[3];

    QVector3D start_cam_pos ((float) (earth_pos.x() +  2*earth_rad),
                             (float) (earth_pos.y() +  2*earth_rad),
                             (float) (earth_pos.z() +  2*earth_rad)
    );


    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(start_cam_pos);
    cameraEntity->setViewCenter(QVector3D(
            (float) starting_pos[3].x(),
            (float) starting_pos[3].y(),
            (float) starting_pos[3].z()) );


    // For camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    view->setRootEntity(rootEntity);

    auto *scn = new Scene(rootEntity);

    //The sun
    //scn->createStar(horizons.get_starting_positions()[0], prop.get_radii()[0]);

    // The Earth
    scn->createBody(starting_pos[3], earth_rad);

    // The Moon
    scn->createBody(starting_pos[10], prop.get_radii()[10]);



    widget->show();
    widget->resize(1200, 800);

    return app.exec();

}

