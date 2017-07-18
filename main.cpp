#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class Body {

private:
    double m_x, m_y, m_z, m_vx, m_vy, m_vz, m_x0, m_y0, m_z0, m_vx0, m_vy0, m_vz0, m_radius, m_gm;
    double m_mass;
    std::string m_name;
    int m_i;

public:
    Body(std::string, double, double, double, double, double, double, double, double);
    std::vector<double> compute_acceleration(double, double, double);
    std::vector<double> get_coords();
    double get_radius();
    double get_ke();
    std::string get_name();
};

Body::Body(std::string name, double x0, double y0, double z0, double vx0, double vy0, double vz0,
           double gm, double radius) {
    m_x0 = x0;
    m_y0 = y0;
    m_z0 = z0;

    m_x = m_x0;
    m_y = m_y0;
    m_z = m_z0;

    m_vx0 = vx0;
    m_vy0 = vy0;
    m_vz0 = vz0;

    m_gm = gm;
    m_radius = radius;

    m_name = name;
};
std::vector<double> Body::compute_acceleration(double x, double y, double z) {
    // Calculate gravitational acceleration due to self at (x, y, z)

    double delta_x = m_x - x;
    double delta_y = m_y - y;
    double delta_z = m_z - z;

    double r2 = pow(delta_x, 2)+pow(delta_y, 2)+pow(delta_z, 2);
    double r = sqrt(r2);
    double f = m_gm/r2;

    return std::vector<double> {f * delta_x/r, f * delta_y/r, f * delta_z/r};
};

std::vector<double> Body::get_coords(){
    return std::vector<double> {m_x, m_y, m_z};
};

double Body::get_radius() {
    return m_radius;
};

double Body::get_ke() {
    return 0.5 * m_mass * (pow(m_vx,2) + pow(m_vy,2) + pow(m_vz, 2));
}

std::string Body::get_name() {
    return m_name;
};



int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
};