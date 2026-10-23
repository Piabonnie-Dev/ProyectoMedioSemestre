#include "OBJLoader.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//Abreviamos filesystem para trabajar con rutas
namespace fs = std::filesystem;
//Convierte los indices OBJ a indice de C++
bool parseVertexIndex(
    const std::string& token,
    size_t count,
    unsigned int& index
)  {
    //Obtenemos el numero anterior a la primera diagonal.
const std::string first = token.substr( 0, token.find('/'));

if(first.empty())
return false;

try {

    //Convertimos el texto del indice a un numero entero.
    long long i = std::stoll(first);
//Adaptamos el indice al sistema que utiliza C++
    long long zeroBased = (i > 0)
    ? (i - 1)
    : (static_cast<long long>(count) + i);
   // Comprobamos que el indice exista en nuestro vector.
        if (i == 0 ||
            zeroBased < 0 ||
            zeroBased >= static_cast<long long>(count))
            return false;

        index = static_cast<unsigned int>(zeroBased);

        return true;

    } catch (...) {

        // Si el texto no puede convertirse, indicamos un error.
        return false;
    }
}


// Lee un archivo OBJ y almacena sus datos en un Model.
bool loadOBJ(const fs::path& path, Model& m) {

    // Abrimos el archivo que recibimos mediante su ruta.
    std::ifstream file(path);

    if (!file) {

        std::cerr << "No se pudo abrir: " << path << '\n';

        return false;
    }

    // Guardamos el nombre del archivo en nuestro modelo.
    m.name = path.filename().string();

    std::string line;

    // Contadores de lineas leidas y lineas incorrectas.
    size_t number = 0;
    size_t skipped = 0;


    // Recorremos el archivo linea por linea.
    while (std::getline(file, line)) {

        ++number;

        // Convertimos la linea en un flujo de texto.
        std::istringstream stream(line);

        std::string type;

        // Ignoramos lineas vacias y comentarios.
        if (!(stream >> type) || type[0] == '#')
            continue;


        // Si encontramos "v", estamos leyendo un vertice.
        if (type == "v") {

            float x, y, z;

            // Extraemos las coordenadas y verificamos sus valores.
            if (stream >> x >> y >> z &&
                std::isfinite(x) &&
                std::isfinite(y) &&
                std::isfinite(z)) {

                // Agregamos el vertice a nuestro vector.
                m.vertices.emplace_back(x, y, z);

            } else {

                ++skipped;
            }

        }

        // Si encontramos "f", estamos leyendo una cara.
        else if (type == "f") {

            // Almacenamos temporalmente los indices de la cara.
            std::vector<unsigned int> face;

            std::string token;

            bool valid = true;


            // Leemos todos los indices que forman la cara.
            while (stream >> token) {

                if (token[0] == '#')
                    break;

                unsigned int i = 0;

                // Convertimos y validamos cada indice.
                if (!parseVertexIndex(
                    token,
                    m.vertices.size(),
                    i
                )) {

                    valid = false;
                    break;
                }

                face.push_back(i);
            }


            // Una cara necesita al menos tres vertices.
            if (!valid || face.size() < 3) {

                ++skipped;
                continue;
            }


            // Convertimos la cara en triangulos.
            //
            // Si tenemos cuatro vertices:
            // 0, 1, 2, 3
            //
            // Formamos:
            // Triangulo 1: 0, 1, 2
            // Triangulo 2: 0, 2, 3
            //
            // Este metodo funciona para caras convexas.

            for (size_t i = 1; i + 1 < face.size(); ++i) {

                m.indices.push_back(face[0]);

                m.indices.push_back(face[i]);

                m.indices.push_back(face[i + 1]);
            }
        }
    }


    // Comprobamos que el archivo contenga vertices.
    if (m.vertices.empty()) {

        std::cerr
            << "El OBJ no contiene vertices: "
            << path << '\n';

        return false;
    }


    // Calculamos las dimensiones y el centro del modelo.
    computeBounds(m);


    // Avisamos si encontramos lineas invalidas.
    if (skipped) {

        std::cerr
            << "Aviso: "
            << m.name
            << ": "
            << skipped
            << " lineas invalidas omitidas.\n";
    }


    // Indicamos que el modelo fue cargado correctamente.
    return true;
}