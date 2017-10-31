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
                                            std::vector<double> errors) {
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