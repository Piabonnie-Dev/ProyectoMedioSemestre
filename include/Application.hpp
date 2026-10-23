#pragma once

/*

Este archivo declara la clase Application,
Esta clase se encarga de controlar el funcionamiento general
de nuestro visualizador.

Entre sus responsabilidades estan:

    1. Administrar los modelos tridimensionales.
    2. Controlar la camara.
    3. Cambiar las formas de visualizacion.
    4. Aplicar rotaciones y escalamiento.
    5. Detectar las teclas del usuario.
    6. Recibir movimientos del raton.
    7. Coordinar el renderizado mediante OpenGL.

    Aqui declaramos los atributos y metodos.

    Su implementación estan en Application.cpp
*/

#include "Camera.hpp"

#include "Renderer.hpp"

#include <GLFW/glfw3.h>
#include <vector>
#include <filesystem> //permite trabajar con rutas, archivos y directorios

class Application {
public:

int run (int argc, char** argv);

private:


//Creamos un vector que almacena objetos Model
//Cada Model representa un modelo tridimensional.
std::vector<Model> models;
size_t currentModel = 0; //Indice del modelo que estamos visualizando

// 0 = Nube de puntos
// 1 = Conjunto de bordes/lineas.
// 2 = Malla formada por triangulos.
int renderMode =2; //Variable que determina como se dibuja el modelo.

//Variables de rotacion del modelo
float rotationX = 0;
float rotationY = 0;
float rotationZ = 0;

//Factor de escala de nuestro modelo.
float modelScale = 1.0f;

Camera camera;
Renderer renderer;


//Metodo para imprimir en la terminal la informacion del modelo actual
void printCurrentModel() const;
// Metodo para seleccionar otro modelo
void selectModel(size_t index);
//Metodo que procesa las teclas.
void onKey(GLFWwindow* window, int key, int action);
//Funcion callback para el teclado.
static void keyCallback(
GLFWwindow*,
int key,
int scancode,
int action,
int mods

);
//Funcion callback para los botones del raton
static void mouseButtonCallback(

    GLFWwindow*,
    int button,
    int action,
    int mods
);

//Funcion callback para el movimiento del raton.

static void cursorCallback(
    GLFWwindow*,
    double x,
    double y
);

//Funcion callback para la rueda del raton.
static void scrollCallback(
    GLFWwindow*,
    double x,
    double y
);
};