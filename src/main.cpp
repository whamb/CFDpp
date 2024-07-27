#include <iostream>

#include <Domain.hpp>
#include <Mesh.hpp>

int main(){
    double lowerBound = 0.0;
    double upperBound = 10.0;
    double dx = 1.0;

    Domain domain;
    Mesh mesh(lowerBound, upperBound, dx, domain);

    return 0;
}