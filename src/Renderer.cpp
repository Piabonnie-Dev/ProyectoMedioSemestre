
#include "Renderer.hpp"
#include "Shader.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Inicializa los shaders y configura OpenGL.
bool Renderer::initialize() {

    // Cargamos y compilamos los shaders.
    shaderProgram = makeProgram(
        "shaders/height.vert",
        "shaders/height.frag"
    );

    // Si el programa de shaders falla, terminamos.
    if (!shaderProgram)
        return false;

    // Preparamos los ejes coordenados X, Y y Z.
    setupAxes();

    // Activamos la prueba de profundidad para que
    // los objetos cercanos oculten los lejanos.
    glEnable(GL_DEPTH_TEST);

    // Establecemos el tamaño de los puntos en pixeles.
    glPointSize(4.0f);

    return true;
}


// Envia los vertices e indices del modelo a la GPU.
void Renderer::uploadModel(Model& m) {

    // Generamos los identificadores de los buffers.
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    // Activamos el VAO de nuestro modelo.
    glBindVertexArray(m.vao);

    // Activamos el buffer de vertices.
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);

    // Copiamos las posiciones de los vertices a la GPU.
    glBufferData(
        GL_ARRAY_BUFFER,
        m.vertices.size() * sizeof(glm::vec3),
        m.vertices.data(),
        GL_STATIC_DRAW
    );

    // Indicamos como interpretar cada vertice:
    // tres componentes de tipo float (X, Y, Z).
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );

    // Activamos el atributo de posicion.
    glEnableVertexAttribArray(0);

    // Activamos el buffer de indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);

    // Copiamos los indices de los triangulos a la GPU.
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m.indices.size() * sizeof(unsigned int),
        m.indices.data(),
        GL_STATIC_DRAW
    );

    // Desactivamos el VAO al terminar su configuracion.
    glBindVertexArray(0);
}


// Prepara las lineas de los ejes coordenados XYZ.
void Renderer::setupAxes() {

    // Cada vertice contiene seis valores:
    // posicion (X,Y,Z) y color (R,G,B).
    //
    // Eje X: rojo.
    // Eje Y: verde.
    // Eje Z: azul.
    const float vertices[] = {

        -1, 0, 0,  1, 0, 0,
         1, 0, 0,  1, 0, 0,

         0,-1, 0,  0, 1, 0,
         0, 1, 0,  0, 1, 0,

         0, 0,-1,  0, 0, 1,
         0, 0, 1,  0, 0, 1
    };

    // Creamos los buffers de los ejes.
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);

    // Enviamos los vertices y colores a la GPU.
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Atributo 0: posicion del vertice.
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // Atributo 1: color del vertice.
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


// Dibuja el modelo tridimensional y los ejes coordenados.
void Renderer::draw(
    const Model& m,
    const Camera& camera,
    int width,
    int height,
    int renderMode,
    float rotationX,
    float rotationY,
    float rotationZ,
    float modelScale
) {

    // Establecemos el area de dibujo de OpenGL.
    glViewport(0, 0, width, height);

    // Definimos el color del fondo.
    glClearColor(0.09f, 0.11f, 0.15f, 1.0f);

    // Limpiamos el color y la profundidad anteriores.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Calculamos el radio necesario para encuadrar el modelo.
    const float radius = m.radius * std::max(1.0f, modelScale);

    // Calculamos las matrices de vista y proyeccion.
    glm::mat4 view = camera.view(radius);

    glm::mat4 projection = camera.projection(
        radius,
        static_cast<float>(width) / height
    );


    // Creamos una matriz identidad para las rotaciones.
    glm::mat4 rotation(1.0f);

    // Aplicamos la rotacion sobre cada eje.
    rotation = glm::rotate(
        rotation,
        glm::radians(rotationX),
        glm::vec3(1, 0, 0)
    );

    rotation = glm::rotate(
        rotation,
        glm::radians(rotationY),
        glm::vec3(0, 1, 0)
    );

    rotation = glm::rotate(
        rotation,
        glm::radians(rotationZ),
        glm::vec3(0, 0, 1)
    );


    // Construimos la matriz del modelo:
    // centramos el OBJ, aplicamos escala y rotaciones.
    glm::mat4 modelMatrix =
        rotation *
        glm::scale(
            glm::mat4(1.0f),
            glm::vec3(modelScale)
        ) *
        glm::translate(
            glm::mat4(1.0f),
            -m.center
        );


    // Activamos el programa de shaders.
    glUseProgram(shaderProgram);

    // Enviamos las alturas minima y maxima al shader.
    glUniform1f(
        glGetUniformLocation(shaderProgram, "uMinHeight"),
        m.minimum.y
    );

    glUniform1f(
        glGetUniformLocation(shaderProgram, "uMaxHeight"),
        m.maximum.y
    );

    // Indicamos que estamos dibujando el modelo,
    // no los ejes coordenados.
    glUniform1i(
        glGetUniformLocation(shaderProgram, "uAxis"),
        GL_FALSE
    );


    // Combinamos proyeccion, vista y modelo.
    glm::mat4 mvp = projection * view * modelMatrix;

    // Enviamos la matriz MVP al vertex shader.
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "uMVP"),
        1,
        GL_FALSE,
        glm::value_ptr(mvp)
    );


    // Activamos el VAO del modelo.
    glBindVertexArray(m.vao);

    // Modo 0: dibujamos una nube de puntos.
    // Tambien usamos puntos si el OBJ no tiene triangulos.
    if (renderMode == 0 || m.indices.empty()) {

        glDrawArrays(
            GL_POINTS,
            0,
            static_cast<GLsizei>(m.vertices.size())
        );

    } else {

        // Modo 1: dibujamos los bordes de los triangulos.
        // Modo 2: dibujamos los triangulos rellenos.
        glPolygonMode(
            GL_FRONT_AND_BACK,
            renderMode == 1 ? GL_LINE : GL_FILL
        );

        // Dibujamos los triangulos utilizando sus indices.
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(m.indices.size()),
            GL_UNSIGNED_INT,
            nullptr
        );

        // Restauramos el modo de superficies rellenas.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Desactivamos el VAO del modelo.
    glBindVertexArray(0);


    // Ahora dibujamos los ejes coordenados.
    glUniform1i(
        glGetUniformLocation(shaderProgram, "uAxis"),
        GL_TRUE
    );

    // Ajustamos el tamaño de los ejes al radio del modelo.
    glm::mat4 axesScale = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(std::max(radius, 0.01f))
    );

    // Calculamos una nueva MVP para los ejes.
    // No aplicamos las rotaciones del modelo a los ejes.
    mvp = projection * view * axesScale;

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "uMVP"),
        1,
        GL_FALSE,
        glm::value_ptr(mvp)
    );

    // Activamos los vertices de los ejes.
    glBindVertexArray(axisVAO);

    // Dibujamos seis vertices formando tres lineas.
    glDrawArrays(GL_LINES, 0, 6);

    // Desactivamos el VAO.
    glBindVertexArray(0);
}


// Libera los buffers de un modelo.
void Renderer::destroyModel(Model& m) {

    glDeleteBuffers(1, &m.ebo);
    glDeleteBuffers(1, &m.vbo);

    glDeleteVertexArrays(1, &m.vao);

    // Reiniciamos los identificadores.
    m.vao = m.vbo = m.ebo = 0;
}


// Libera los recursos generales del renderizador.
void Renderer::shutdown() {

    // Eliminamos los buffers de los ejes.
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &axisVAO);

    // Eliminamos el programa de shaders.
    glDeleteProgram(shaderProgram);

    // Reiniciamos los identificadores.
    shaderProgram = axisVAO = axisVBO = 0;
}
