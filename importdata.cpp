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

#include "importdata.h"
#include "ui_importdata.h"

#include "planetdata.h"
#include "propertiesfile.h"


importdata::importdata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importdata)
{
    ui->setupUi(this);

    auto *prop = new PhysicalProperties;
    auto *horizons = new PlanetData(prop->get_names());

    QObject::connect(horizons, &PlanetData::getText, this->ui->textEdit, &QTextEdit::append);
    QObject::connect(this->ui->importButton, &QPushButton::clicked,
                     [=]() {
                         horizons->read_data();
                         body_positions = horizons->get_starting_positions();
                         body_radii = prop->get_radii();
                         body_names = prop->get_names();
                     }
    );
}

importdata::~importdata()
{
    delete ui;
}
