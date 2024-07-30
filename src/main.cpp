#include <iostream>

#include <Domain.hpp>
#include <Mesh.hpp>

int main(){
    double lowerBound = 0.0;
    double upperBound = 10.0;
    double dx = 1.0;

    Domain domain(lowerBound, upperBound, dx);

    return 0;
}