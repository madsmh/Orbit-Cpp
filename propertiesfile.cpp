//
// Created by mads on 23-07-17.
//

#include <fstream>
#include "propertiesfile.h"

PhysicalProperties::PhysicalProperties(){
    std::string path = "../physical_properties/properties.csv";
    std::ifstream filereader;

    PropertiesFile p;

    std::vector<PropertiesFile> data;
    std::string buffer;

    filereader.open(path);
    if(filereader.is_open()){
        getline(filereader, buffer, '\n');
        while (!filereader.eof()){
            getline(filereader, buffer, ',');
            p.name = buffer;
            std::cout << "Reading data for object " << buffer << std::endl;
            getline(filereader, buffer, ',');
            p.GM = stod(buffer);
            getline(filereader, buffer, ',');
            p.r = stod(buffer);
            getline(filereader, buffer, '\n');

            data.emplace_back(p);
        }
        filereader.close();
    }

    long n = data.size();
    for (int i = 0; i < n; ++i) {

        PropertiesFile d = data[i];
        self_names.emplace_back(d.name);
        self_radii.emplace_back(d.r);
        self_GMs.emplace_back(d.GM);
    }
};
