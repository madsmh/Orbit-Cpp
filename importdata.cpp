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
                         this->ui->pushButton_diag->setDisabled(true);
                         this->ui->progressBar->reset();
                         m_trajectory.setup(m_sol.get_number_of_bodies());
                         this->ui->abortButton->setEnabled(false);
                     });

    QObject::connect(this->ui->simulateButton, &QPushButton::clicked,
                     [=](){
                         this->ui->pushButton_diag->setDisabled(true);
                         this->ui->progressBar->setEnabled(true);
                         this->ui->progressBar->reset();
                         this->ui->progressBar->setMaximum(this->ui->stepSpinBox->value()*
                                                                   this->ui->daysSpinBox->value()-1);
                         this->ui->abortButton->setEnabled(true);
                         m_verlet->set_abort(false);
                         m_sol.set_param(prop->get_names(),
                                         horizons->get_starting_positions(),
                                         horizons->get_starting_velocities(),
                                         prop->get_GMs(),
                                         get_radii());
                         m_trajectory.setup(m_sol.get_number_of_bodies());
                         m_verlet->setup(this->ui->daysSpinBox->value(),
                                         this->ui->stepSpinBox->value());
                         m_verlet->run(m_sol, m_trajectory);
                     }
    );

    auto *diagnostic = new DiagnosticDialog(this);

    QObject::connect(m_verlet, &Verlet::success,
                     this->ui->pushButton_diag, &QPushButton::setEnabled);

    QObject::connect(this->ui->pushButton_diag, &QPushButton::clicked,
                    [=](){
                        test_accuracy(m_trajectory, horizons, this->ui->stepSpinBox->value());
                        diagnostic->populate_plot(m_body_names, m_plot_points);
                        diagnostic->populate_error_table(m_body_names, m_max_errors);
                        diagnostic->exec();
                    });
}

importdata::~importdata()
{
    delete ui;
}

void importdata::test_accuracy(Trajectory trajectory, PlanetData *data, int detail)
    {
        m_plot_points.clear();
        m_max_errors.clear();

        for (int j = 0; j < trajectory.get_number_of_trajectories(); ++j) {

            std::vector<Vector3> sim_vector = trajectory.get_trajectory_positions(j);
            std::vector<Vector3> ref_vector = data->get_body_positions(j);

            m_plot_points.emplace_back(std::vector<float > {});

            for (int i = 0; i < std::min(this->ui->daysSpinBox->value(), (int) sim_vector.size()); ++i) {
                m_plot_points.back().emplace_back((sim_vector[i*detail]-ref_vector[i]).norm()/1000);
                QCoreApplication::processEvents();
            }

        m_max_errors.emplace_back(*std::max_element(m_plot_points.back().begin(), m_plot_points.back().end()));
    }
}


