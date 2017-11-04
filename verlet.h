//
// Created by mads on 14-08-17.
//

#ifndef ORBIT3D_VERLET_H
#define ORBIT3D_VERLET_H

#include <QObject>
#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"


class Verlet : public QObject
{
    Q_OBJECT

public:
    void setup(int days, int detail);
    void run(System &sys, Trajectory &tra);

private:
    int m_rows;
    int m_bodies;
    double m_delta;
    bool m_abort = false;

signals:
    void progress(int p);
    void success(bool s);
    void failure(bool f);

public slots:
    void set_abort (bool abort);


};


#endif //ORBIT3D_VERLET_H
