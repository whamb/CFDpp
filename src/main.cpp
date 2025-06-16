#include <iostream>

#include <Mesh.hpp>

int main(){
    Double lowerBound = 0.0;
    Double upperBound = 10.0;
    Double dx = 1.0;

    Mesh mesh(lowerBound,upperBound,dx);

    for(const auto &node : mesh.Mesh::getNodes()){
        std::cout << *node;
    }

    for(const auto &face : mesh.Mesh::getFaces()){
        std::cout << *face;
    }

    for(const auto &cell : mesh.Mesh::getCells()){
        std::cout << *cell;
    }
    
    return 0;
}