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

#include "diagnosticdialog.h"

importdata::importdata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importdata)
{
    ui->setupUi(this);
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    auto *prop = new PhysicalProperties;

    QObject::connect(prop, &PhysicalProperties::getText, this->ui->textEdit, &QTextEdit::append);

    prop->get_data();

    auto *horizons = new PlanetData(prop->get_names());

    QObject::connect(horizons, &PlanetData::getText, this->ui->textEdit, &QTextEdit::append);
    QObject::connect(this->ui->importButton, &QPushButton::clicked,
                     [=]() {
                         horizons->read_data();
                         m_body_positions = horizons->get_starting_positions();
                         m_body_radii = prop->get_radii();
                         m_body_names = prop->get_names();
                     }
    );

    QObject::connect(horizons, &PlanetData::success,
                     [=](){
                         this->ui->daysSpinBox->setEnabled(true);
                         this->ui->daysSpinBox->setValue((int) horizons->get_body_positions(0).size());
                         this->ui->stepSpinBox->setEnabled(true);
                         this->ui->simulateButton->setEnabled(true);
                         this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
                     });

    QObject::connect(this->m_verlet, &Verlet::progress,
                     this->ui->progressBar, &QProgressBar::setValue);

    QObject::connect(this->ui->abortButton, &QPushButton::clicked,
                     [=](){
                         m_verlet->set_abort(true);
                         this->ui->progressBar->setValue(0);
                         this->ui->abortButton->setEnabled(false);
                     });

    QObject::connect(this->ui->simulateButton, &QPushButton::clicked,
                     [=](){
                         this->ui->progressBar->setEnabled(true);
                         this->ui->progressBar->setMaximum(this->ui->stepSpinBox->value()*
                                                                   this->ui->daysSpinBox->value()-1);
                         this->ui->abortButton->setEnabled(true);
                         m_verlet->set_abort(false);
                         m_sol.set_param(prop->get_names(),
                                         horizons->get_starting_positions(),
                                         horizons->get_starting_velocities(),
                                         prop->get_GMs(),
                                         get_radii());
                         m_trajecotry.setup(m_sol.get_number_of_bodies());
                         m_verlet->setup(this->ui->daysSpinBox->value(),
                                         this->ui->stepSpinBox->value());
                         m_verlet->run(m_sol, m_trajecotry);
                     }
    );

    auto *diagnostic = new DiagnosticDialog(this);

    QObject::connect(m_verlet, &Verlet::success,
                     this->ui->pushButton_diag, &QPushButton::setEnabled);

    QObject::connect(this->ui->pushButton_diag, &QPushButton::clicked,
                    diagnostic, &QDialog::exec);
}

importdata::~importdata()
{
    delete ui;
};

