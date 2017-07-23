
#include "vector3.h"

class Body {
    Vector3 r,v,r0,v0;
    double m_radius, m_gm;

    std::string m_name;

public:
    Body(std::string name, std::vector<double> posvel0,
         double gm, double radius) {
        r0 = Vector3( posvel0[0], posvel0[1], posvel0[2]);
        r = r0;

        v0 = Vector3( posvel0[3], posvel0[4], posvel0[5]);
        v = v0;

        m_gm = gm;
        m_radius = radius;

        m_name = name;
    }

    std::vector<double> get_position(){
        return r;
    };

    std::vector<double> get_velocity() {
        return v;
    };


    void set_position(std::vector<double> pos) {
        r = pos;
    };

    void set_velocity(std::vector<double> vel) {
        v = vel;
    };


    double get_radius() {
        return m_radius;
    };

    double get_GM() {
        return m_gm;
    };

    std::string get_name() {
        return m_name;
    };
};

