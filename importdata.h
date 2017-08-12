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
    ~importdata();

    std::vector<Vector3> get_pos() const { return  body_positions;};
    std::vector<double> get_radii() const { return body_radii;};
    std::vector<std::string> get_names() const { return body_names;};

private:
    Ui::importdata *ui;

    std::vector<Vector3> body_positions;
    std::vector<double > body_radii;
    std::vector<std::string> body_names;
};

#endif // IMPORTDATA_H
