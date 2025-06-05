#include <iostream>

#include <Domain.hpp>

int main(){
    double lowerBound = 0.0;
    
    double upperBound = 10.0;
    double dx = 1.0;

    Domain domain(lowerBound,upperBound,dx);
    for(auto &node : domain.Domain::getNodes()){
        std::cout << *node;
    }

    for(auto &face : domain.Domain::getFaces()){
        std::cout << *face;
    }

    for(auto &cell : domain.Domain::getCells()){
        std::cout << *cell;
    }

    return 0;
}