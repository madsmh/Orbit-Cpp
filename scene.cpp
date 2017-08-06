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

#include "scene.h"

Scene::Scene(Qt3DCore::QEntity *rootEntity, int n, std::vector<double> radii, std::vector<Vector3> initial_pos)
        : self_rootEntity(rootEntity) {


    for (int i = 0; i < n; ++i) {

        auto *mesh = new Qt3DExtras::QSphereMesh();
        mesh->setRings(20);
        mesh->setSlices(20);
        mesh->setRadius((float) radii[i]);

        auto *transform = new Qt3DCore::QTransform();
        transform->setTranslation(
                QVector3D((float) initial_pos[i].x(),
                          (float) initial_pos[i].y(),
                          (float) initial_pos[i].z())
        );

        auto *material = new Qt3DExtras::QPhongMaterial();
        material->setDiffuse(QColor(QRgb(0xa69929)));

        auto sphere = new Qt3DCore::QEntity(self_rootEntity);
        sphere->addComponent(mesh);
        sphere->addComponent(transform);
        sphere->addComponent(material);

        self_heavenly_entities.emplace_back(sphere);

    }

}
