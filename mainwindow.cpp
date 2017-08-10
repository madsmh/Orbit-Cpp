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

    std::vector<double> radii = prop.get_radii();
    std::vector<Vector3> start_pos = horizons.get_starting_positions();

    std::vector<Vector3> start_pos_offset {};

    // Setting the Comboboxes
    for (int l = 0; l < prop.get_names().size(); ++l) {
        QString name = QString::fromUtf8(prop.get_names()[l].c_str());
        this->ui->comboBoxCamCenter->addItem(name);
        this->ui->comboBoxCamPos->addItem(name);
    }

    // Genrating the offset positions
    for (int k = 0; k < prop.get_names().size(); ++k) {
        double radius = radii[k];
        start_pos_offset.emplace_back(start_pos[k] + Vector3 (-5.0 * radius, 0, -5.0 * radius));
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

    vtkSmartPointer<vtkInteractorStyleFlight> style =
            vtkSmartPointer<vtkInteractorStyleFlight>::New(); //like paraview
    interactor->SetInteractorStyle(style);


    this->ui->widget->GetRenderWindow()->AddRenderer(renderer);
}

MainWindow::~MainWindow()
{
    delete ui;
}


