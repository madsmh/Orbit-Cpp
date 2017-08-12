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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "planetdata.h"
#include "propertiesfile.h"
#include "scale.h"
#include "importdata.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void render(std::vector<std::string> names,
                std::vector<Vector3> start_pos,
                std::vector<double> radii);

private:
    Ui::MainWindow *ui;
    void populateCombos(std::vector<std::string> names);
};

#endif // MAINWINDOW_H
