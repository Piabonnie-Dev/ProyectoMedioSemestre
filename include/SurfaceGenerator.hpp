
#pragma once

// Incluimos Model.hpp para utilizar la estructura Model,
// que almacena los vertices, indices y dimensiones.
#include "Model.hpp"


/* Genera una superficie matematica de 100 x 100 vertices.

 Utilizamos la funcion:
 y = cos(x) + sin(z)

 Los valores de X y Z se encuentran en el intervalo:
 [-2pi, 2pi]

 La funcion devuelve un objeto Model que contiene
 los vertices y triangulos de nuestra superficie.
*/ 

Model makeSurface();
