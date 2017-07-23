//
// Created by mads on 23-07-17.
//

#ifndef ORBIT3D_PROPERTIESFILE_H
#define ORBIT3D_PROPERTIESFILE_H

#import <vector>
#include "vector3.h"

struct PropertiesFile {
    std::string name{};
    double r{};
    double GM{};
};



class PhysicalProperties {
    std::vector<std::string> self_names;
    std::vector<double> self_radii;
    std::vector<double> self_GMs;

public:
    PhysicalProperties();
    std::vector<std::string> get_names(){
        return self_names;
    };

    std::vector<double> get_radii(){
        return self_radii;
    };

    std::vector<double> get_GMs(){
        return self_GMs;
    };
};


#endif //ORBIT3D_PROPERTIESFILE_H
