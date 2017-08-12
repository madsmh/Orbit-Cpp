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
#include "vector3.h"

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
    std::vector<double> get_radii() const { return m_body_radii;};
    std::vector<std::string> get_names() const { return m_body_names;};

private:
    Ui::importdata *ui;

    std::vector<Vector3> m_body_positions;
    std::vector<double > m_body_radii;
    std::vector<std::string> m_body_names;
};

#endif // IMPORTDATA_H
