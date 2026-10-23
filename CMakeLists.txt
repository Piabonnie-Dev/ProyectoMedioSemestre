
# Version minima de CMake necesaria para compilar.
cmake_minimum_required(VERSION 3.20)

# Nombre del proyecto, version y lenguaje utilizado.
project(VisualizadorOBJ VERSION 1.0 LANGUAGES CXX)

# Utilizamos el estandar C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Permite activar o desactivar la interfaz Dear ImGui.
option(
    VISUALIZADOR_IMGUI
    "Activar interfaz grafica Dear ImGui (requiere internet en primera configuracion)"
    ON
)

# Buscamos las bibliotecas necesarias para OpenGL.
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)


# Creamos nuestro ejecutable llamado visualizador.
# Incluimos todos los archivos fuente de C++.
add_executable(visualizador
    src/main.cpp
    src/Application.cpp
    src/Model.cpp
    src/OBJLoader.cpp
    src/SurfaceGenerator.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/Renderer.cpp
)

# Indicamos donde se encuentran los archivos .hpp.
target_include_directories(
    visualizador PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Vinculamos las bibliotecas que necesita el ejecutable.
target_link_libraries(
    visualizador PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)


# Configuramos Dear ImGui si esta activado.
if(VISUALIZADOR_IMGUI)

    # FetchContent permite descargar dependencias externas.
    include(FetchContent)

    # Indicamos la version de ImGui que utilizaremos.
    # Se descarga durante la primera configuracion.
    FetchContent_Declare(imgui
        URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(imgui)

    # Creamos una biblioteca estatica con ImGui.
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    # Indicamos donde estan sus archivos de cabecera.
    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )

    # Vinculamos las dependencias de ImGui.
    target_link_libraries(imgui PUBLIC
        glfw
        OpenGL::GL
    )

    # Indicamos que ImGui utilizara GLEW.
    target_compile_definitions(imgui PRIVATE
        IMGUI_IMPL_OPENGL_LOADER_GLEW
    )

    # Conectamos ImGui con nuestro visualizador.
    target_link_libraries(visualizador PRIVATE imgui)

    # Activamos la parte del codigo que utiliza ImGui.
    target_compile_definitions(visualizador PRIVATE
        VISUALIZADOR_IMGUI=1
    )

endif()


# Copiamos modelos y shaders junto al ejecutable.
# Esto permite que el programa encuentre sus recursos.
add_custom_command(
    TARGET visualizador POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/modelos"
        "$<TARGET_FILE_DIR:visualizador>/modelos"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:visualizador>/shaders"

    COMMENT "Copiando recursos OBJ y GLSL al directorio del ejecutable"
)


# Habilitamos las pruebas automatizadas con CTest.
include(CTest)

if(BUILD_TESTING)

    # Creamos un ejecutable para probar el lector OBJ
    # y la generacion de la superficie matematica.
    add_executable(test_obj
        tests/test_obj.cpp
        src/OBJLoader.cpp
        src/Model.cpp
        src/SurfaceGenerator.cpp
    )

    # Incluimos las cabeceras necesarias.
    target_include_directories(test_obj PRIVATE include)

    # Vinculamos las bibliotecas utilizadas por las pruebas.
    target_link_libraries(test_obj PRIVATE
        glm::glm
        GLEW::GLEW
    )

    # Registramos la prueba para ejecutarla con CTest.
    add_test(
        NAME ParserYSuperficie
        COMMAND test_obj
    )

endif()
