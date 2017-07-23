#include <vector>
#include <iostream>
#include "math.h"

class Vector3 {
    double m_x, m_y, m_z;
public:
    Vector3(double x = 0.0, double y = 0.0, double z = 0.0) 
        : m_x(x), m_y(y), m_z(z) {}
    Vector3(std::vector<double> v) : m_x(v[0]), m_y(v[1]), m_z(v[2]) {}
    double norm2() const { return m_x*m_x + m_y*m_y + m_z*m_z;}
    double norm()  const { return sqrt(norm2());}
    double x()     const { return m_x;}
    double y()     const { return m_y;}
    double z()     const { return m_z;}
    operator std::vector<double>() const  { return std::vector<double> {m_x, m_y, m_z}; }
    
    Vector3& operator += (const Vector3& rhs) {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        return *this;
    }

    Vector3& operator -= (const Vector3& rhs) {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        return *this;
    }

    friend Vector3 operator + (Vector3& lhs, const Vector3& rhs) {
        return lhs += rhs;
    }

    friend Vector3 operator - (Vector3& lhs, const Vector3& rhs) {
        return lhs -= rhs;
    }

    friend std::ostream& operator <<(std::ostream &os, const Vector3& rhs) {
        return os << "(" << rhs.m_x << "," << rhs.m_y << "," << rhs.m_z << ")";
    }
};

