
#version 330 core

// Recibimos la altura enviada por el Vertex Shader.
in float vHeight;

// Recibimos el color de los vertices.
// Se utiliza principalmente para los ejes XYZ.
in vec3 vColor;

// Variable donde guardamos el color final del fragmento.
out vec4 FragColor;

// Alturas minima y maxima del modelo.
uniform float uMinHeight;
uniform float uMaxHeight;

// Indica si estamos dibujando los ejes coordenados.
uniform bool uAxis;


// Funcion principal del Fragment Shader.
void main() {

    // Si dibujamos los ejes, utilizamos sus colores originales.
    if (uAxis) {

        FragColor = vec4(vColor, 1.0);
        return;
    }

    // Calculamos la diferencia entre la altura maxima y minima.
    // Evitamos dividir entre cero.
    float span = max(
        uMaxHeight - uMinHeight,
        0.000001
    );

    // Normalizamos la altura entre 0 y 1.
    // 0 representa el punto mas bajo.
    // 0.5 representa el punto medio.
    // 1 representa el punto mas alto.
    float t = clamp(
        (vHeight - uMinHeight) / span,
        0.0,
        1.0
    );

    // Definimos los tres colores principales en RGB.
    vec3 blue = vec3(0.0, 0.2, 1.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red = vec3(1.0, 0.0, 0.0);

    // Interpolamos los colores dependiendo de la altura.
    // De 0 a 0.5: azul a amarillo.
    // De 0.5 a 1: amarillo a rojo.
    vec3 color = (t <= 0.5)
        ? mix(blue, yellow, 2.0 * t)
        : mix(yellow, red, 2.0 * (t - 0.5));

    // Asignamos el color calculado al fragmento.
    // El ultimo valor representa la opacidad.
    FragColor = vec4(color, 1.0);
}
