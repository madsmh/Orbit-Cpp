#ifndef DIAGNOSTICDIALOG_H
#define DIAGNOSTICDIALOG_H

#include <QDialog>

namespace Ui {
class DiagnosticDialog;
}

class DiagnosticDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiagnosticDialog(QWidget *parent = 0);
    ~DiagnosticDialog();

public slots:
    void populate_error_table(std::vector<std::string> names,
                              std::vector<double> errors);

private:
    Ui::DiagnosticDialog *ui;
};

#endif // DIAGNOSTICDIALOG_H
