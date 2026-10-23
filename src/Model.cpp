#include "Model.hpp"
#include <algorithm>
#include <iostream>
// Calcula los limites, el centro y el radio del modelo.
void computeBounds(Model& m){

    //Si el modelo no tiene vertices, terminamos la funcion.
    if(m.vertices.empty())
    return;

    //Utilizamos el primer vertice como minimo y maximo inicial.
    m.minimum = m.maximum = m.vertices.front();

//Recorremos todos los vertices para encontrar sus extremos.

for(const auto& v : m.vertices){
    m.minimum = glm::min(m.minimum, v);
    m.maximum = glm::max(m.maximum, v);
}
//Calculamos el centro de la caja delimitadora.
m.center = 0.5f * (m.minimum + m.maximum); 

//Calculamos la mitad de la diagonal de la caja
m.radius =std::max(
    0.001f, 0.5f * glm::length(m.maximum - m.minimum)
);
}

//Muestra en la terminal la informacion del modelo actual.
void printModelInfo(const Model& m, size_t current, size_t total){

    std::cout << "\n===========================================\n"
              << "Modelo " << (current + 1) << '/' << total
              << ": " << m.name << '\n'

              // Cantidad de vertices almacenados.
              << "Vertices: " << m.vertices.size() << '\n'

              // Cada triangulo utiliza tres indices.
              << "Triangulos: " << m.indices.size() / 3 << '\n'

              // Coordenadas minimas del modelo.
              << "Minimo (X,Y,Z): "
              << m.minimum.x << ", "
              << m.minimum.y << ", "
              << m.minimum.z << '\n'

              // Coordenadas maximas del modelo.
              << "Maximo (X,Y,Z): "
              << m.maximum.x << ", "
              << m.maximum.y << ", "
              << m.maximum.z << '\n'

              // Mostramos las alturas minima y maxima sobre Y.
              << "Altura minima/maxima: "
              << m.minimum.y << " / "
              << m.maximum.y << '\n'

              << "===========================================\n";
}