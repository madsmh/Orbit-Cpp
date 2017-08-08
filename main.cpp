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
#include "scale.h"

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
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



    PhysicalProperties prop;
    PlanetData horizons(prop.get_names());

    std::vector<Vector3> starting_pos = horizons.get_starting_positions();
    std::vector<Vector3> above_planets {};

    for (int l = 0; l < prop.get_names().size(); ++l) {
        above_planets.emplace_back(starting_pos[l] + Vector3(-3.0 * prop.get_radii()[l],
                                                             0,
                                                             -3.0 * prop.get_radii()[l]));
    }

    QApplication app (argc, argv);
    auto *view = new Qt3DExtras::Qt3DWindow();

    Scale s(1e-6);

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


    QVector3D earth_pos = s.vector(starting_pos[3]);
    float earth_rad = s.scalar(prop.get_radii()[3]);

    QVector3D start_cam_pos (earth_pos.x() +  -3.0f*earth_rad,
                             earth_pos.y() +  0.0f*earth_rad,
                             earth_pos.z() +  -3.0f*earth_rad
    );


    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 100000.0f);
    //cameraEntity->setPosition(start_cam_pos);
    //cameraEntity->setViewCenter(earth_pos);
    //cameraEntity->setUpVector(QVector3D (0.0f, 1.0f, 0.0f))

    // For camera controls
    auto *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);
    camController->setLinearSpeed(30.0f);
    camController->setLookSpeed(75.0f);

    auto *scn = new Scene(rootEntity);

    // The sun
    scn->createStar(s.vector(starting_pos[0]), s.scalar(prop.get_radii()[0]));

    // Make all the bodies
    for (int j = 1; j < prop.get_names().size(); ++j) {
        scn->createBody(s.vector(starting_pos[j]), s.scalar(prop.get_radii()[j]));
    }

    view->setRootEntity(rootEntity);

    auto *camGoupBox = new QGroupBox(widget);

    auto *setCamPosCombo = new QComboBox(camGoupBox);
    auto *setCamCenterCombo = new QComboBox(camGoupBox);

    for (int k = 0; k < prop.get_names().size(); ++k) {
        QString item = QString::fromUtf8(prop.get_names()[k].c_str());
        setCamPosCombo->addItem(item);
        setCamCenterCombo->addItem(item);
    }

    auto *posLabel = new QLabel(camGoupBox);
    posLabel->setText(QString ("Position:"));
    auto *centerLabel = new QLabel(camGoupBox);
    centerLabel->setText(QString ("Focus:"));

    auto *camGroupBoxLayout = new QVBoxLayout;

    camGroupBoxLayout->addWidget(posLabel);
    camGroupBoxLayout->addWidget(setCamPosCombo);
    camGroupBoxLayout->addWidget(centerLabel);
    camGroupBoxLayout->addWidget(setCamCenterCombo);

    auto *quitButton = new QPushButton(widget);
    quitButton->setText(QString ("Quit"));
    quitButton->setToolTip(QString ("Quit Orbit"));

    camGoupBox->setTitle(QString("Camera control"));
    camGoupBox->setLayout(camGroupBoxLayout);

    QObject::connect(setCamCenterCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [&]( int ix ) { cameraEntity->setViewCenter(s.vector(starting_pos[ix]));
                     }
    );

    QObject::connect(setCamPosCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [&]( int ix ) { cameraEntity->setPosition(s.vector(above_planets[ix]));
                                     setCamCenterCombo->setCurrentIndex(ix);
                     }
    );

    QObject::connect(quitButton, &QPushButton::clicked, widget, &QWidget::close);

    setCamPosCombo->setCurrentIndex(3);

    vlayout->addWidget(camGoupBox);
    vlayout->addWidget(quitButton);
    widget->show();

    widget->showMaximized();
    return app.exec();

}

