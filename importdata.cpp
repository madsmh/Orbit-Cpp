#include "importdata.h"
#include "ui_importdata.h"

#include "planetdata.h"
#include "propertiesfile.h"


importdata::importdata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importdata)
{
    ui->setupUi(this);

    PhysicalProperties prop;
    auto *horizons = new PlanetData(prop.get_names());

    QObject::connect(horizons, &PlanetData::getText, this->ui->textEdit, &QTextEdit::append);
    QObject::connect(this->ui->importButton, &QPushButton::clicked,
                     [=]() {
                         horizons->read_data();
                         body_positions = horizons->get_starting_positions();
                         body_radii = prop.get_radii();
                         body_names = prop.get_names();
                     }
    );
}

importdata::~importdata()
{
    delete ui;
}
