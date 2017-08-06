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
#include <Qt3DExtras/QOrbitCameraController>

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

int main(int argc, char *argv[]) {

    double detail = 64.0;
    long bodies = 0;
    while (1)
    {
        char opt = getopt(argc, argv, "d:b:");
        if (opt == -1)
            break;
        switch (opt)
        {
            case 'd' : detail = atof(optarg); break;
            case 'b' : bodies = atol(optarg); break;
            default  : help(argv[0]); exit(-1); break;
        }
    }

    double dt = 86400.0/detail;
    long rows = 1131*detail;


    PhysicalProperties prop(bodies);
    PlanetData horizons (prop.get_names());


    System sol (prop.get_names(),
                horizons.get_starting_positions(),
                horizons.get_starting_velocities(),
                prop.get_GMs(),
                prop.get_radii());


    long n_bodies = prop.get_names().size();

    Trajectory tra (n_bodies,rows);

    std::cout << std::endl;
    std::cout << "detail = " << detail << std::endl;

    //verlet(sol, tra, dt);

    // compare_with_horizon(tra, horizons, prop.get_names(), detail, 1131);

    // Saving the trajectories to file
    /*for (int j = 0; j < n_bodies; ++j) {
        tra.save_trajectory_positions(j, prop.get_names()[j], 0.0, dt);
    }*/


    // Code for GUI
    //Adapted from https://doc.qt.io/qt-5/qt3d-basicshapes-cpp-main-cpp.html

    QApplication app (argc, argv);
    auto *view = new Qt3DExtras::Qt3DWindow();

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
    ;
    Vector3 europa_start_pos = horizons.get_starting_positions()[18];

    const QVector3D start_cam_pos ((float) (europa_start_pos.x()+5e6),
                                   (float) (europa_start_pos.y()+5e6),
                                   (float) (europa_start_pos.z()+5e6));

    const QVector3D europa_pos ((float) europa_start_pos.x(),
                                (float) europa_start_pos.y(),
                                (float) europa_start_pos.z());

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(start_cam_pos);
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(europa_pos);

    auto *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto *light = new Qt3DRender::QPointLight(lightEntity);

    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    auto *lightTransform = new Qt3DCore::QTransform(lightEntity);

    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // For camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    Scene *scn = new Scene(rootEntity, prop.get_radii().size(), prop.get_radii(), horizons.get_starting_positions());

    view->setRootEntity(rootEntity);

    widget->show();
    widget->resize(1200, 800);

    return app.exec();

}

