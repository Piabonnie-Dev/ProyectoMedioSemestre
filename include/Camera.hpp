#pragma once
#include <glm/glm.hpp> // Incluimos GLM para trabajar con vectores y matrices.


struct Camera{

//Angulos de orientacion de la camara.
/*
yaw = rotacion horizontal alrededor del modelo
pitch: inclinacion vertical de la camara.
distanceFactor: controla la distancia al modelo.
*/

    float yaw = 35.0f;
    float pitch = 20.0f;
    float distanceFactor = 3.2f;

    //Indica si estamos arrastrando el raton.
    bool dragging = false;

    //Guardamos la ultima posicion del cursor.
    double lastMouseX =0.0;
    double lastMouseY = 0.0;

    //Inicia o termina el movimiento orbital.
    void startDrag(bool active, double x, double y);

//Actualiza los angulos de la camara dependiendo del movimiento del raton
    void move(double x, double y);
//Acerca o aleja la camara utilizando el desplazamiento de la rueda del raton
    void zoom(double amount);
//Calcula la matriz de vista de la camara.
    glm::mat4 view(float radius) const;
//Calcula la matriz de proyeccion en perspectiva.
//Aspect: representa la relacion ancho / alto
    glm::mat4 projection (float radius, float aspect) const;
};
