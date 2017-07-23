#include <vector3.h>

int main() {
    std::vector<double> w {2,3,4};
    Vector3 v(1,2,3);
    Vector3 v0;
    v = w;
    w = v;

    std::cout << v << std::endl;
}
