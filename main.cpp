#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class Body {

private:
    double m_x, m_y, m_z, m_vx, m_vy, vz,m_x0, m_y0, m_z0, m_vx0, m_vy0, m_vz0, m_radius, m_gm;
    std::string m_name;
public:
    Body(std::string, double, double, double, double, double, double, double, double);
    std::vector<double> compute_acceleration(double, double, double);
    void step(double, std::vector<Body>);
};

Body::Body(std::string name, double x0, double y0, double z0, double vx0, double vy0, double vz0,
           double gm, double radius) {
    m_x0 = x0;
    m_y0 = y0;
    m_z0 = z0;

    m_vx0 = vx0;
    m_vy0 = vy0;
    m_vz0 = vz0;

    m_gm = gm;
    m_radius = radius;

    m_name = name;
}
std::vector<double> Body::compute_acceleration(double x, double y, double z) {
    double delta_x = m_x - x;
    double delta_y = m_y - y;
    double delta_z = m_z - z;

    double delta_x2 = std::pow(delta_x, 2);
    double delta_y2 = std::pow(delta_y, 2);
    double delta_z2 = std::pow(delta_z, 2);

    double ax = m_gm/(delta_x2+delta_y2+delta_z2)* delta_x/sqrt(delta_x2+delta_y2+delta_z2);
    double ay = m_gm/(delta_x2+delta_y2+delta_z2)* delta_y/sqrt(delta_x2+delta_y2+delta_z2);
    double az = m_gm/(delta_x2+delta_y2+delta_z2)* delta_z/sqrt(delta_x2+delta_y2+delta_z2);

    std::vector<double> a(3);

    a[0] = ax;
    a[1] = ay;
    a[2] = az;

    return a;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}