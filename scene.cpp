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

Scene::Scene(Qt3DCore::QEntity *rootEntity)
        : self_rootEntity(rootEntity) {

}

void Scene::createStar(QVector3D pos, float radius) {


    auto *mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(50);
    mesh->setSlices(50);
    mesh->setRadius(radius);

    auto *transform = new Qt3DCore::QTransform();
    transform->setTranslation(pos);

    auto *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0xa69929)));

    self_heavenly_entities.append(new Qt3DCore::QEntity(&self_rootEntity));

    self_heavenly_entities.last()->addComponent(mesh);
    self_heavenly_entities.last()->addComponent(transform);
    self_heavenly_entities.last()->addComponent(material);

    // Create starlight
    self_light_entities.push_back(new Qt3DCore::QEntity(&self_rootEntity));

    auto *light = new Qt3DRender::QPointLight(self_light_entities.last());

    light->setColor("white");
    light->setIntensity(1);
    self_light_entities.last()->addComponent(light);

    auto *lightTransform = new Qt3DCore::QTransform(self_light_entities.last());

    lightTransform->setTranslation(pos);
    self_light_entities.last()->addComponent(lightTransform);

}

void Scene::createBody(QVector3D pos, float radius) {

    auto *mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(50);
    mesh->setSlices(50);
    mesh->setRadius(radius);

    auto *transform = new Qt3DCore::QTransform();
    transform->setTranslation(pos);

    auto *material = new Qt3DExtras::QPhongMaterial();
    //material->setDiffuse(QColor(QRgb(0xa69929)));

    self_heavenly_entities.push_back(new Qt3DCore::QEntity(&self_rootEntity));

    self_heavenly_entities.last()->addComponent(mesh);
    self_heavenly_entities.last()->addComponent(transform);
    self_heavenly_entities.last()->addComponent(material);

}
