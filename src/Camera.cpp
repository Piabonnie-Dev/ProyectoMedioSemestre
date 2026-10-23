#include "Camera.hpp"
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

//Inicia o termina el movimiento de la camara con el raton.
void Camera::startDrag(bool active, double x, double y){

    //Guardamos si el boton del raton esta presionado.
    dragging = active;

//Almacenamos la posicion inicial del cursor.
lastMouseX =x;
lastMouseY = y;
}
//Actualiza los angulos de la camara al mover el raton.
void Camera::move(double x, double y){
    if(dragging){

        //Calculamos el giro horizontal segun el movimiento del raton.
        yaw += static_cast<float>(x - lastMouseX) * 0.3f;

        //Calculamos la inclinacion vertical

        pitch = std::clamp(
            pitch - static_cast<float>(y - lastMouseY) * 0.3f,
            -89.0f, 89.0f
        );
    }
//Actualizamos la ultima posicion del cursor.
    lastMouseX = x;
    lastMouseY = y;

}
//Acerca o aleja la camara con la rueda del raton.
void Camera::zoom(double amount){

    distanceFactor = std::clamp(
        distanceFactor * std::pow(0.9f, static_cast<float>(amount)), 1.3f, 30.0f);
    
    


}

// Calcula la matriz de vista de nuestra camara orbital.
glm::mat4 Camera::view(float radius) const {

    // Calculamos la distancia respecto al radio del modelo.
    float distance = radius * distanceFactor;

    // Convertimos los angulos de grados a radianes.
    float ry = glm::radians(yaw);
    float rp = glm::radians(pitch);

    // Calculamos la posicion de la camara en el espacio 3D.
    // Utilizamos seno y coseno para obtener X, Y y Z.
    glm::vec3 eye(
        distance * std::cos(rp) * std::sin(ry),
        distance * std::sin(rp),
        distance * std::cos(rp) * std::cos(ry)
    );

    // lookAt construye la matriz de vista.
    // eye: posicion de la camara.
    // (0,0,0): punto hacia donde observa.
    // (0,1,0): direccion vertical de referencia.
    return glm::lookAt(
        eye,
        glm::vec3(0.0f),
        glm::vec3(0, 1, 0)
    );
}


// Calcula la matriz de proyeccion en perspectiva.
glm::mat4 Camera::projection(float radius, float aspect) const {

    // Obtenemos la distancia actual de la camara.
    float distance = radius * distanceFactor;

    // Creamos una proyeccion con campo visual de 45 grados.
    // aspect es la relacion entre ancho y alto de la ventana.
    // Los ultimos dos valores delimitan el espacio visible.
    return glm::perspective(
        glm::radians(45.0f),
        aspect,
        std::max(0.001f, distance - 2.5f * radius),
        distance + 3.0f * radius
    );
}




