
#pragma once


#include <GL/glew.h>

// Permite manejar las rutas de los archivos GLSL
// que contienen nuestros shaders.
#include <filesystem>


/* Esta funcion crea un programa de shaders en OpenGL.

 vertexFile:
 Ruta del archivo que contiene el vertex shader.
 Este shader procesa los vertices del modelo.

 fragmentFile:
 Ruta del archivo que contiene el fragment shader.
 Este shader calcula el color de los fragmentos.

 GLuint:
 Es un tipo entero sin signo utilizado por OpenGL
 para identificar recursos, como programas de shaders.

 La funcion compila ambos shaders, los enlaza
 y devuelve el identificador del programa creado.
*/
GLuint makeProgram(
    const std::filesystem::path& vertexFile,
    const std::filesystem::path& fragmentFile
);
