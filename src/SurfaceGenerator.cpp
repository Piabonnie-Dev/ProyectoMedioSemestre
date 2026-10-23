#include "SurfaceGenerator.hpp"
#include <cmath>
constexpr float PI = 3.14159265358979323846f;

// Extra: malla 100 x 100, x/z en [-2pi,2pi], y = cos(x) + sin(z).
Model makeSurface(){

//Creamos un modelo donde guardaremos la superficie.
Model m;

//Asignamos un nombre al modelo.
m.name = "SUPERFICIE_100x100_cos(x)+sin(z)";
//Numero de vertices por fila y columna.
const int N = 100;

//Recorremos las filas de nuestra cuadricula
for(int j = 0; j < N; ++j){
//Calculamos Z dentro del intervalo [-2PI, 2PI].
    float z = -2.0f * PI + 4.0f * PI * j / (N - 1);
  

    for(int i = 0; i< N; ++i){
        float x = -2.0f * PI +4.0f * PI * i/(N -1);

//Calculamos la altura Y con nuestra funcion.
        m.vertices.emplace_back(
            x,
            std::cos(x) + std::sin(z), z
        );
    }
}
//Recorremos cada celda de la cuadricula
//Para convertir en dos triangulos
for(int j = 0; j<N -1; ++ j){
    for (int i =0; i < N -1; ++i){

        //Obtenemos los cuatro vertices de la celda.
        unsigned int a = j * N + i;
        unsigned int b = a + 1;
        unsigned int c = a + N;
        unsigned int d = c +1;
//Formados dos triangulos utilizando sus indices
//Primer triangulo: a, c, b.
//segundo: b, c, d
        m.indices.insert(m.indices.end(), {a, c, b, b, c, d});


    }
}
// Calculamos los limites, centro y radio del modelo.
computeBounds(m);
//Devolvemos la superficie tridimensional terminada.
return m;

}
