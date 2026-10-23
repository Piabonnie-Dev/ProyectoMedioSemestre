
#include "OBJLoader.hpp"
#include "SurfaceGenerator.hpp"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>


// Funcion principal de nuestras pruebas.
int main() {

    // Abreviamos filesystem para trabajar con rutas.
    namespace fs = std::filesystem;

    // Creamos la ruta de un archivo OBJ temporal.
    const fs::path path =
        fs::temp_directory_path() / "visualizador_test.obj";


    // Creamos un archivo OBJ sencillo para las pruebas.
    {
        std::ofstream f(path);

        // Escribimos cuatro vertices que forman un cuadrado.
        // La cara utiliza indices negativos del formato OBJ.
        f << "v 0 0 0\n"
          << "v 1 0 0\n"
          << "v 1 1 0\n"
          << "v 0 1 0\n"
          << "f -4 -3 -2 -1\n";
    }


    // Creamos un modelo para almacenar el archivo.
    Model m;

    // Verificamos que el lector pueda cargar el OBJ.
    assert(loadOBJ(path, m));

    // Comprobamos que se hayan cargado cuatro vertices.
    assert(m.vertices.size() == 4);

    // Un cuadrado se divide en dos triangulos,
    // por lo tanto necesitamos seis indices.
    assert(m.indices.size() == 6);

    // Verificamos los limites de la figura.
    assert(m.minimum.x == 0.f && m.maximum.y == 1.f);


    // Eliminamos el archivo temporal.
    fs::remove(path);


    // Generamos nuestra superficie matematica.
    Model surface = makeSurface();

    // Comprobamos que tenga 100 x 100 vertices.
    assert(surface.vertices.size() == 10000);

    // Cada una de las 99 x 99 celdas tiene seis indices.
    assert(surface.indices.size() == 99u * 99u * 6u);

    // Verificamos que las alturas se encuentren
    // aproximadamente entre -2 y 2.
    assert(
        surface.minimum.y >= -2.01f &&
        surface.maximum.y <= 2.01f
    );


    // Mostramos un mensaje cuando terminan las pruebas.
    std::cout
        << "Pruebas de parser y superficie correctas.\n";

    // Finalizamos correctamente.
    return 0;
}
