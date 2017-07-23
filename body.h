
#include "vector3.h"

class Body {
    Vector3 r,v,r0,v0;
    double m_radius, m_gm;

    std::string m_name;

public:
    Body(std::string name, Vector3 pos0, Vector3 vel0,
         double gm, double radius) {
        r0 = pos0;
        r = r0;

        v0 = vel0;
        v = v0;

        m_gm = gm;
        m_radius = radius;

        m_name = name;
    }

    Vector3 get_position(){
        return r;
    };

    Vector3 get_velocity() {
        return v;
    };


    void set_position(Vector3 pos) {
        r = pos;
    };

    void set_velocity(Vector3 vel) {
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

