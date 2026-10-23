#pragma once

//Bibliotecas necesarias para trabajar con OpenGL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

// Estructura que almacena la informacion de un modelo OBJ.
struct Model {
    std::string name; // Nombre del modelo que estamos utilizando.
    //Almacena las posiciones X, Y ,Z de los vertices.
    //glm:: vec3 representa un vector de tres componentes.
std::vector<glm::vec3> vertices;
//Guarda los indices de los vertices que forman
std::vector<unsigned int> indices;
// Coordenadas minimas, maxmias y central del modelo.
glm::vec3 minimum{0.0f};
glm::vec3 maximum{0.0f};
glm::vec3 center {0.0f};
//Radio utilizado para calcular el encuadre
float radius = 1.0f;
//Identificadores de recursos almacenados en la GPU.
GLuint vao =0; //VAO: configuracion de los atributos de vertices.
//VBO: buffer que almacena los vertices.
GLuint vbo = 0;
//EBO: buffer que almacena los indices.
GLuint ebo = 0;

};
//Calcula los limites minimo y maximo del modelo
void computeBounds (Model& model);
//Imprime en la terminal la informacion del modelo.

void printModelInfo(const Model& model, size_t current, size_t total);

