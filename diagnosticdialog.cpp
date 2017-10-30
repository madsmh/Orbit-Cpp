#include "diagnosticdialog.h"
#include "ui_diagnosticdialog.h"
#include "importdata.h"

DiagnosticDialog::DiagnosticDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagnosticDialog)
{
    ui->setupUi(this);

    QStringList headers;
    headers << "Body name" << "Max. error (km)";

    std::vector<std::string> names = importdata::get_names();

    this->ui->tableWidget->setHorizontalHeaderLabels(headers);

}

DiagnosticDialog::~DiagnosticDialog()
{
    delete ui;
}
