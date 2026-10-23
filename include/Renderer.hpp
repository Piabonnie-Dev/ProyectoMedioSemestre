#pragma once
#include "Camera.hpp"
#include "Model.hpp"
#include <GL/glew.h>
// Encapsula los recursos GPU (VAO/VBO/EBO), shader y dibujo.
//Clase encargada del renderizado de los modelos 3D
class Renderer {

    public:
//Inicializa los shaders y los recursos necesarios para comenzar
// a dibujar con OpenGL.
    bool initialize();
//Envia los vertices e indices del modelo a la GPU.
void uploadModel(Model& model);
//Dibuja el modelo utilizando la camara 
//y las transformaciones seleccionadas por el usuario
void draw(
    const Model& model,
    const Camera& camera,
    int width,
        int height,
        int renderMode,
        float rotationX,
        float rotationY,
        float rotationZ,
        float modelScale
);
//Elimina los buffers OpenGL asociados al modelo, cuando ya no se necesitan
void destroyModel(Model& model);
//Libera los recursos generales del renderizado
void shutdown();

private:
// Identificador del programa de shaders.
//axisVAO y axisVBO almacenan los recursos
GLuint shaderProgram = 0;
GLuint axisVAO = 0;
GLuint axisVBO = 0;

//Prepara los vertices y colores de los ejes XYZ.
void setupAxes();
};

