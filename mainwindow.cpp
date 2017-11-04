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
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleFlight.h>
#include <vtkInteractorStyle.h>
#include <vtkVectorText.h>
#include <vtkActor.h>
#include <vtkFollower.h>
#include <vtkCubeSource.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "planetdata.h"
#include "propertiesfile.h"
#include "scale.h"
#include "importdata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *importDataDialog = new importdata(this);

    QObject::connect(this->ui->importDataButton, &QPushButton::clicked,
                     importDataDialog, &QDialog::exec);


    QObject::connect(importDataDialog, &QDialog::accepted,
    [=]() {
        this->populateCombos(importDataDialog->get_names());
        this->ui->groupBox->setEnabled(true);
        this->ui->comboBoxCamCenter->setEnabled(true);
        this->ui->comboBoxCamPos->setEnabled(true);
        this->render(importDataDialog->get_names(),
                     importDataDialog->get_pos(),
                     importDataDialog->get_radii());}
    );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateCombos(std::vector<std::string> names) {
    for (int l = 0; l < names.size(); ++l) {
        QString name = QString::fromUtf8(names[l].c_str());
        this->ui->comboBoxCamCenter->addItem(name);
        this->ui->comboBoxCamPos->addItem(name);
    }
}

void MainWindow::render(std::vector<std::string> names,
                        std::vector<Vector3> start_pos,
                        std::vector<double> radii) {

    std::vector<Vector3> start_pos_offset {};

    // Genrating the offset positions
    for (int k = 0; k < names.size(); ++k) {
        double radius = radii[k];
        start_pos_offset.emplace_back(start_pos[k] + Vector3 (-5.0 * radius, 0, -5.0 * radius));
    }

    //Set up renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    Scale s (1.0);

    this->ui->widget->GetRenderWindow()->AddRenderer(renderer);

    // Get active camera
    vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();

    std::vector<vtkSmartPointer<vtkActor> > actors{};
    std::vector<vtkSmartPointer<vtkActor> > text_actors {};

    // Adding bodies to vector and to the renderer
    for (int i = 0; i < names.size(); ++i) {

        //Based on http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/Follower
        // Create some text
        vtkSmartPointer<vtkVectorText> textSource = vtkSmartPointer<vtkVectorText>::New();
        textSource->SetText(names[i].c_str());

        // Create a mapper
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(textSource->GetOutputPort());

        // Create a subclass of vtkActor: a vtkFollower that remains facing the camera
        vtkSmartPointer<vtkFollower> follower = vtkSmartPointer<vtkFollower>::New();
        follower->SetMapper(mapper);
        follower->GetProperty()->SetColor(1, 1, 1);
        follower->SetPosition(start_pos_offset[i].x(),
                              start_pos_offset[i].y(),
                              start_pos_offset[i].z());
        follower->SetCamera(camera);

        text_actors.emplace_back(follower);

        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetRadius(radii[i]);
        sphereSource->SetPhiResolution(100);
        sphereSource->SetThetaResolution(100);
        sphereSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
        sphereActor->SetPosition(start_pos[i].x(),
                                 start_pos[i].y(),
                                 start_pos[i].z());
        sphereActor->SetMapper(sphereMapper);
        sphereActor->GetProperty()->SetInterpolationToFlat();
        sphereActor->GetProperty()->SetFrontfaceCulling(1);

        actors.push_back(sphereActor);

        renderer->AddActor(text_actors.back());
        renderer->AddActor(actors.back());
    }

    QObject::connect(this->ui->comboBoxCamPos,
                     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=](int ix) {
                         camera->SetPosition(start_pos_offset[ix].x(),
                                             start_pos_offset[ix].y(),
                                             start_pos_offset[ix].z());
                         this->ui->widget->update();
                         this->ui->comboBoxCamCenter->setCurrentIndex(ix);
                     }
    );

    QObject::connect(this->ui->comboBoxCamCenter,
                     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=](int ix) {
                         camera->SetFocalPoint(start_pos[ix].x(),
                                               start_pos[ix].y(),
                                               start_pos[ix].z());
                         this->ui->widget->update();
                     }
    );

    this->ui->comboBoxCamPos->setCurrentIndex(3);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
           this->ui->widget->GetInteractor();
    vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle())
            ->AutoAdjustCameraClippingRangeOn();

    //this->ui->widget->GetRenderWindow()->Render();
    //renderer->ResetCamera();
    //this->ui->widget->GetRenderWindow()->Render();
    //interactor->Start();


}


