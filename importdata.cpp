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
                         m_trajecotry.setup(m_sol.get_number_of_bodies());
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
                    [=](){
                        std::vector<double> accu_table = test_accuracy(m_trajecotry,
                                                                       horizons,
                                                                       this->ui->stepSpinBox->value());
                        diagnostic->populate_error_table(prop->get_names(), accu_table);
                        diagnostic->exec();
                    });
}

importdata::~importdata()
{
    delete ui;
}

std::vector<double> importdata::test_accuracy(Trajectory trajectory, PlanetData *data, int detail)
    {

    std::vector<double> errors;

    std::vector<double> dists;

    for (int j = 0; j < trajectory.get_number_of_trajectories(); ++j) {
        std::vector<Vector3> sim_vector = trajectory.get_trajectory_positions(j);
        std::vector<Vector3> ref_vector = data->get_body_positions(j);

        for (int i = 0; i < ref_vector.size(); ++i) {
            double error = (sim_vector[i*detail]-ref_vector[i]).norm();
            dists.emplace_back(error);
        }

        errors.emplace_back(*std::max_element(dists.begin(), dists.end())/1000);
        dists.clear();
    }

    return errors;
}

Trajectory importdata::error_plots(PlanetData *data, int detail) {

    long n_tra = m_trajecotry.get_number_of_trajectories();

    std::vector<Vector3> error_data;

    Trajectory error_plots;
    error_plots.setup(m_trajecotry.get_number_of_trajectories());

    for (int j = 0; j < n_tra; ++j) {

        std::vector<Vector3> current_body_sim = m_trajecotry.get_trajectory_positions(j);
        std::vector<Vector3> current_body_ref = data->get_body_positions(j);

        for (int k = 0; k < current_body_ref.size(); ++k) {
            error_data.emplace_back((current_body_ref[k]-current_body_sim[k*detail]).norm()/1000);
        }

        error_plots.set_position(error_data, error_data);
        error_data.clear();
    }



    return error_plots;
}


