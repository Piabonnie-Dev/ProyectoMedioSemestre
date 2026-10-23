
#version 330 core

// Recibimos la posicion (X, Y, Z) de cada vertice.
// location = 0 corresponde al atributo configurado en OpenGL.
layout(location = 0) in vec3 aPosition;

// Recibimos el color del vertice.
// Este atributo se utiliza para dibujar los ejes XYZ.
layout(location = 1) in vec3 aColor;

// Matriz que combina modelo, vista y proyeccion.
// Nos permite transformar los vertices a la pantalla.
uniform mat4 uMVP;

// Enviamos la altura del vertice al Fragment Shader.
out float vHeight;

// Enviamos el color del vertice al Fragment Shader.
out vec3 vColor;


// Funcion principal del Vertex Shader.
void main() {

    // Transformamos la posicion del vertice
    // utilizando la matriz MVP.
    gl_Position = uMVP * vec4(aPosition, 1.0);

    // Guardamos la coordenada Y como altura.
    vHeight = aPosition.y;

    // Pasamos el color recibido al siguiente shader.
    vColor = aColor;
}
