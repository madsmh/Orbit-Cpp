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

#include <QtCharts/QChartView>
#include <QtCharts/QtCharts>
#include "diagnosticdialog.h"
#include "ui_diagnosticdialog.h"
#include "importdata.h"

DiagnosticDialog::DiagnosticDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagnosticDialog)
{
    ui->setupUi(this);
}

DiagnosticDialog::~DiagnosticDialog()
{
    delete ui;
}

void DiagnosticDialog::populate_error_table(std::vector<std::string> names,
                                            std::vector<float > errors) {

    QStringList headers;
    headers << "Body name" << "Max. error (km)";

    this->ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);
    this->ui->tableWidget->setColumnCount(2);
    this->ui->tableWidget->setRowCount(names.size());

    for (int j = 0; j < names.size(); ++j) {
        auto name = new QTableWidgetItem(QString::fromStdString(names[j]));
        auto error = new QTableWidgetItem(QString::fromStdString(std::to_string(errors[j])));

        this->ui->tableWidget->setItem(j, 0, name);
        this->ui->tableWidget->setItem(j, 1, error);
    }
}

void DiagnosticDialog::populate_plot(std::vector<std::string> names, std::vector<std::vector<float> > points) {

    for (int j = 0; j < names.size(); ++j) {

        this->ui->comboBox->addItem(QString::fromStdString(names[j]));

        m_points.emplace_back(new QtCharts::QLineSeries());

        for (int k = 0; k < points[j].size(); ++k) {
            m_points.back()->append(k, points[j][k]);
        }
    }

    auto *chart = new QChart();

    this->ui->widget->setChart(chart);
    this->ui->widget->setRenderHint(QPainter::Antialiasing);

    QObject::connect(this->ui->comboBox,
                     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     [=](int ix){
                         chart->removeAllSeries();
                         chart->addSeries(m_points[ix]);
                         chart->createDefaultAxes();
                         chart->setTitle("Absolute error vs. days for " + QString::fromStdString(names[ix]));
                     });

}
