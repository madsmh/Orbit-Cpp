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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "planetdata.h"
#include "propertiesfile.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PhysicalProperties prop;
    PlanetData horizons(prop.get_names());
    Scale s (1.0/4000.0);

    std::vector<double> radii {};
    std::vector<Vector3> start_pos {};

    for (int m = 0; m < prop.get_names().size(); ++m) {
        start_pos.push_back(s.vector(horizons.get_starting_positions()[m]));
        radii.push_back(s.scalar(prop.get_radii()[m]));
    }

    std::vector<Vector3> start_pos_offset {};

    // Genrating the offset positions
    for (int k = 0; k < prop.get_names().size(); ++k) {
        double radius = radii[k];
        start_pos_offset.emplace_back(start_pos[k] + Vector3 (-3.0 * radius, 0, -3.0 * radius));
    }

    // Setting the Comboboxes
    for (int l = 0; l < prop.get_names().size(); ++l) {
        QString name = QString::fromUtf8(prop.get_names()[l].c_str());
        this->ui->comboBoxCamCenter->addItem(name);
        this->ui->comboBoxCamPos->addItem(name);
    }


    std::vector<vtkSmartPointer<vtkActor> > actors {};

    // Adding bodies to vector
    for (int i = 0; i < radii.size(); ++i) {
        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetRadius(radii[i]);
        sphereSource->SetPhiResolution(100);
        sphereSource->SetThetaResolution(100);
        sphereSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetPosition(start_pos[i].x(), start_pos[i].y(), start_pos[i].z());
        actor->SetMapper(sphereMapper);
        actor->GetProperty()->SetInterpolationToFlat();
        actor->GetProperty()->SetFrontfaceCulling(true);

        actors.push_back(actor);

    }

    //Set up renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    // Create new camera
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    renderer->SetActiveCamera(camera);

    for (int j = 0; j < radii.size(); ++j) {
        renderer->AddActor(actors[j]);
    }

    QObject::connect(this->ui->comboBoxCamPos,
                     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=] (int ix){ camera->SetPosition(start_pos_offset[ix].x(),
                                                       start_pos_offset[ix].y(),
                                                       start_pos_offset[ix].z());
                         this->ui->widget->update();
                         this->ui->comboBoxCamCenter->setCurrentIndex(ix);
                     }
    );

    QObject::connect(this->ui->comboBoxCamCenter,
                     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=](int ix){camera->SetFocalPoint(start_pos[ix].x(),
                                                       start_pos[ix].y(),
                                                       start_pos[ix].z());
                     this->ui->widget->update();
                     }
    );

    this->ui->comboBoxCamPos->setCurrentIndex(3);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = this->ui->widget->GetInteractor();

    /*vtkSmartPointer<vtkInteractorStyleFlight> style =
            vtkSmartPointer<vtkInteractorStyleFlight>::New(); //like paraview
    interactor->SetInteractorStyle(style);*/


    this->ui->widget->GetRenderWindow()->AddRenderer(renderer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

