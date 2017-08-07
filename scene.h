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

#ifndef ORBIT3D_SCENE_H
#define ORBIT3D_SCENE_H

#include <QVector>

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <Qt3DRender/qpointlight.h>

#include "vector3.h"

class Scene : public QObject {
    Q_OBJECT

public:
    explicit Scene(Qt3DCore::QEntity *rootEntity);

    ~Scene() {};

public slots:
    void createStar(Vector3 pos, double radius);

    void createBody(Vector3 pos, double radius);
private:
    Qt3DCore::QEntity self_rootEntity;

    QVector<Qt3DCore::QEntity*> self_heavenly_entities {};
    QVector<Qt3DCore::QEntity*> self_light_entities {};
};


#endif //ORBIT3D_SCENE_H
