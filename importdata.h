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

#ifndef IMPORTDATA_H
#define IMPORTDATA_H

#include <QDialog>
#include <vector>
#include <string>
#include <QtWidgets/QTableWidget>
#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "verlet.h"


namespace Ui {
class importdata;
}

class importdata : public QDialog
{
    Q_OBJECT

public:
    explicit importdata(QWidget *parent = 0);
    virtual ~importdata();

    std::vector<Vector3> get_pos() const { return  m_body_positions;};
    std::vector<Vector3> get_pos_of_body(int body) const {
        return m_trajectory.get_trajectory_positions(body);
    };
    std::vector<double> get_radii() const { return m_body_radii;};
    std::vector<std::string> get_names() const { return m_body_names;};
    void test_accuracy(Trajectory trajectory, PlanetData *data, int detail);

private:
    Ui::importdata *ui;

    System m_sol;
    Trajectory m_trajectory;
    Verlet *m_verlet = new Verlet;

    std::vector<std::vector<float > > m_plot_points;
    std::vector<float > m_max_errors;
    std::vector<Vector3> m_body_positions;
    std::vector<double > m_body_radii;
    std::vector<std::string> m_body_names;
};

#endif // IMPORTDATA_H
