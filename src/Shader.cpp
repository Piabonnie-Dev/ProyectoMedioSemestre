
#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


// Este namespace mantiene las funciones auxiliares
// disponibles solamente dentro de este archivo.
namespace {


// Lee el contenido completo de un archivo GLSL.
std::string readText(const std::filesystem::path& p) {

    // Abrimos el archivo del shader.
    std::ifstream input(p);

    // Verificamos que el archivo se haya abierto.
    if (!input) {

        std::cerr << "No se pudo abrir el shader: "
                  << p << '\n';

        return {};
    }

    // Creamos un flujo para almacenar el contenido.
    std::ostringstream out;

    // Copiamos todo el texto del archivo al flujo.
    out << input.rdbuf();

    // Convertimos el contenido en una cadena.
    return out.str();
}


// Compila un shader y devuelve su identificador.
GLuint compileShader(GLenum type, const char* source) {

    // Creamos el shader segun el tipo recibido.
    GLuint id = glCreateShader(type);

    // Enviamos el codigo fuente GLSL a OpenGL.
    glShaderSource(id, 1, &source, nullptr);

    // Solicitamos la compilacion del shader.
    glCompileShader(id);

    // Variable para comprobar si la compilacion funciono.
    GLint success = GL_FALSE;

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    // Si ocurre un error, mostramos el mensaje.
    if (!success) {

        char log[2048];

        glGetShaderInfoLog(
            id,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr << "Error compilando shader: "
                  << log << '\n';

        // Eliminamos el shader que no pudo compilarse.
        glDeleteShader(id);

        return 0;
    }

    // Devolvemos el identificador del shader compilado.
    return id;
}

}


// Crea un programa utilizando un vertex y un fragment shader.
GLuint makeProgram(
    const std::filesystem::path& vertexFile,
    const std::filesystem::path& fragmentFile
) {

    // Leemos el codigo de ambos archivos GLSL.
    std::string vs = readText(vertexFile);
    std::string fs = readText(fragmentFile);

    // Si alguno esta vacio, no podemos continuar.
    if (vs.empty() || fs.empty())
        return 0;


    // Compilamos los dos shaders.
    GLuint v = compileShader(
        GL_VERTEX_SHADER,
        vs.c_str()
    );

    GLuint f = compileShader(
        GL_FRAGMENT_SHADER,
        fs.c_str()
    );


    // Verificamos que ambos se hayan compilado.
    if (!v || !f) {

        glDeleteShader(v);
        glDeleteShader(f);

        return 0;
    }


    // Creamos un programa de shaders en OpenGL.
    GLuint program = glCreateProgram();

    // Asociamos los shaders compilados al programa.
    glAttachShader(program, v);
    glAttachShader(program, f);

    // Enlazamos ambos shaders en un mismo programa.
    glLinkProgram(program);

    // Ya no necesitamos los objetos shader individuales.
    glDeleteShader(v);
    glDeleteShader(f);


    // Comprobamos que el programa se haya enlazado.
    GLint success = GL_FALSE;

    glGetProgramiv(
        program,
        GL_LINK_STATUS,
        &success
    );


    // Si el enlace falla, mostramos el error.
    if (!success) {

        char log[2048];

        glGetProgramInfoLog(
            program,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr << "Error enlazando programa: "
                  << log << '\n';

        // Eliminamos el programa incorrecto.
        glDeleteProgram(program);

        return 0;
    }


    // Devolvemos el programa listo para utilizarse.
    return program;
}
