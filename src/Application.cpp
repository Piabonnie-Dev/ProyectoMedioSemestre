
#include "Application.hpp"
#include "OBJLoader.hpp"
#include "SurfaceGenerator.hpp"

#include <GL/glew.h>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <string>
#include <utility>

// Estas bibliotecas se utilizan solamente si tenemos ImGui.
#ifdef VISUALIZADOR_IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

// Abreviamos std::filesystem como fs para trabajar con rutas.
namespace fs = std::filesystem;


// Muestra en la terminal la informacion del modelo seleccionado.
void Application::printCurrentModel() const {

    printModelInfo(models.at(currentModel), currentModel, models.size());
}


// Cambia el modelo actual y restablece sus transformaciones.
void Application::selectModel(size_t index) {

    if (models.empty())
        return;

    // El modulo permite regresar al primer modelo al llegar al ultimo.
    currentModel = index % models.size();

    rotationX = rotationY = rotationZ = 0.f;
    modelScale = 1.f;

    printCurrentModel();
}


// Procesa las teclas que utiliza nuestro visualizador.
void Application::onKey(GLFWwindow* window, int key, int action) {

    // Ignoramos eventos diferentes a presionar o repetir una tecla.
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    // ESC permite cerrar el programa.
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Cambiamos entre los modelos disponibles.
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_N)
        selectModel(currentModel + 1);

    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_B)
        selectModel(currentModel + models.size() - 1);

    // Seleccionamos los tres modos de dibujo.
    if (key == GLFW_KEY_1)
        renderMode = 0;

    if (key == GLFW_KEY_2)
        renderMode = 1;

    if (key == GLFW_KEY_3)
        renderMode = 2;

    // Cada tecla aumenta 10 grados la rotacion correspondiente.
    if (key == GLFW_KEY_X)
        rotationX += 10.f;

    if (key == GLFW_KEY_Y)
        rotationY += 10.f;

    if (key == GLFW_KEY_Z)
        rotationZ += 10.f;

    // R restaura las rotaciones y la escala.
    if (key == GLFW_KEY_R) {
        rotationX = rotationY = rotationZ = 0.f;
        modelScale = 1.f;
    }

    // Aumentamos la escala sin superar el limite de 5.
    if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD)
        modelScale = std::min(5.f, modelScale * 1.1f);

    // Reducimos la escala sin bajar de 0.1.
    if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT)
        modelScale = std::max(0.1f, modelScale / 1.1f);
}


// GLFW llama esta funcion cuando detecta una tecla.
void Application::keyCallback(
    GLFWwindow* w,
    int key,
    int,
    int action,
    int
) {

    // Recuperamos nuestro objeto Application desde la ventana.
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));

#ifdef VISUALIZADOR_IMGUI
    // Evitamos procesar teclas mientras ImGui las esta utilizando.
    if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureKeyboard)
        return;
#endif

    app->onKey(w, key, action);
}


// Controla cuando presionamos o soltamos el boton izquierdo.
void Application::mouseButtonCallback(
    GLFWwindow* w,
    int button,
    int action,
    int
) {

    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

#ifdef VISUALIZADOR_IMGUI
    if (ImGui::GetCurrentContext() &&
        ImGui::GetIO().WantCaptureMouse &&
        action == GLFW_PRESS)
        return;
#endif

    // Obtenemos la posicion actual del cursor.
    double x, y;
    glfwGetCursorPos(w, &x, &y);

    // Iniciamos o terminamos el arrastre de la camara.
    static_cast<Application*>(glfwGetWindowUserPointer(w))
        ->camera.startDrag(action == GLFW_PRESS, x, y);
}


// Actualiza la camara cuando movemos el cursor.
void Application::cursorCallback(GLFWwindow* w, double x, double y) {

    static_cast<Application*>(glfwGetWindowUserPointer(w))
        ->camera.move(x, y);
}


// La rueda del raton permite acercar o alejar la camara.
void Application::scrollCallback(GLFWwindow* w, double, double y) {

#ifdef VISUALIZADOR_IMGUI
    if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse)
        return;
#endif

    static_cast<Application*>(glfwGetWindowUserPointer(w))
        ->camera.zoom(y);
}


// Funcion principal de nuestra aplicacion.
int Application::run(int argc, char** argv) {

    // Buscamos las carpetas de modelos y shaders.
    fs::path executableDir = fs::absolute(argv[0]).parent_path();

    fs::path modelsDir = executableDir / "modelos";
    fs::path shadersDir = executableDir / "shaders";

    // Si no encontramos los shaders, intentamos la carpeta actual.
    if (!fs::exists(shadersDir)) {

        executableDir = fs::current_path();

        modelsDir = executableDir / "modelos";
        shadersDir = executableDir / "shaders";
    }


    // Cargamos los archivos OBJ indicados desde la terminal.
    if (argc > 1) {

        for (int i = 1; i < argc; ++i) {

            Model m;

            if (loadOBJ(fs::path(argv[i]), m))
                models.push_back(std::move(m));
        }

    } else if (fs::is_directory(modelsDir)) {

        // Si no recibimos argumentos, buscamos los OBJ de modelos/.
        std::vector<fs::path> paths;

        for (const auto& entry : fs::directory_iterator(modelsDir)) {

            if (!entry.is_regular_file())
                continue;

            std::string ext = entry.path().extension().string();

            // Convertimos la extension a minusculas.
            std::transform(
                ext.begin(),
                ext.end(),
                ext.begin(),
                [](unsigned char c) {
                    return static_cast<char>(std::tolower(c));
                }
            );

            if (ext == ".obj")
                paths.push_back(entry.path());
        }

        // Ordenamos los archivos antes de cargarlos.
        std::sort(paths.begin(), paths.end());

        for (const auto& path : paths) {

            Model m;

            if (loadOBJ(path, m))
                models.push_back(std::move(m));
        }
    }


    // Agregamos la superficie matematica de 100 x 100 vertices.
    models.push_back(makeSurface());


    // Mostramos los controles disponibles en la terminal.
    std::cout
        << "Teclas: B/N o flechas: modelos | "
        << "1/2/3: puntos/lineas/superficie | "
        << "X/Y/Z: rotar | "
        << "+/-: escalar | "
        << "R: restaurar | "
        << "raton: orbitar | "
        << "rueda: zoom | "
        << "ESC: salir\n";


    // Inicializamos GLFW para crear nuestra ventana.
    if (!glfwInit()) {

        std::cerr << "Error al inicializar GLFW\n";
        return 1;
    }


    // Solicitamos OpenGL 3.3 con perfil Core.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // Creamos una ventana de 1150 x 780 pixeles.
    GLFWwindow* window = glfwCreateWindow(
        1150,
        780,
        "Visualizador OBJ - UNAM",
        nullptr,
        nullptr
    );

    if (!window) {

        std::cerr << "No se pudo crear ventana OpenGL 3.3\n";

        glfwTerminate();
        return 1;
    }


    // Activamos el contexto OpenGL y la sincronizacion vertical.
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    // Inicializamos GLEW para acceder a las funciones OpenGL.
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {

        std::cerr << "Error al iniciar GLEW\n";

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    // Limpiamos un posible error generado durante glewInit.
    glGetError();


    // Establecemos la carpeta base para localizar los shaders.
    fs::current_path(executableDir);


    // Inicializamos el renderizador.
    if (!renderer.initialize()) {

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }


    // Enviamos los datos geometricos de cada modelo a la GPU.
    for (auto& m : models)
        renderer.uploadModel(m);


    // Asociamos nuestro objeto Application con la ventana GLFW.
    glfwSetWindowUserPointer(window, this);


    // Registramos las funciones que reciben los eventos.
    glfwSetKeyCallback(window, keyCallback);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glfwSetCursorPosCallback(window, cursorCallback);

    glfwSetScrollCallback(window, scrollCallback);


#ifdef VISUALIZADOR_IMGUI

    // Inicializamos la interfaz grafica Dear ImGui.
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

#endif


    // Mostramos la informacion del primer modelo.
    printCurrentModel();


    // Bucle principal: se repite mientras la ventana permanezca abierta.
    while (!glfwWindowShouldClose(window)) {

        // Procesamos los eventos del teclado, raton y ventana.
        glfwPollEvents();


#ifdef VISUALIZADOR_IMGUI

        // Preparamos un nuevo fotograma para la interfaz.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        // Creamos el panel de controles.
        ImGui::Begin("Controles del visualizador");


        // Lista desplegable para elegir un modelo.
        if (ImGui::BeginCombo("Modelo", models[currentModel].name.c_str())) {

            for (size_t i = 0; i < models.size(); ++i) {

                if (ImGui::Selectable(
                    models[i].name.c_str(),
                    i == currentModel
                )) {
                    selectModel(i);
                }

                if (i == currentModel)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }


        // Seleccionamos entre puntos, lineas y superficie.
        const char* modos[] = {
            "Puntos",
            "Lineas",
            "Superficie"
        };

        ImGui::Combo("Modo", &renderMode, modos, 3);


        // Controles para modificar las rotaciones.
        ImGui::SliderFloat(
            "Rotacion X",
            &rotationX,
            -180.f,
            180.f
        );

        ImGui::SliderFloat(
            "Rotacion Y",
            &rotationY,
            -180.f,
            180.f
        );

        ImGui::SliderFloat(
            "Rotacion Z",
            &rotationZ,
            -180.f,
            180.f
        );


        // Controlamos el tamanio del modelo.
        ImGui::SliderFloat(
            "Escala",
            &modelScale,
            0.1f,
            5.f
        );


        // Boton para regresar a las transformaciones iniciales.
        if (ImGui::Button("Reiniciar transformaciones")) {

            rotationX = rotationY = rotationZ = 0.f;
            modelScale = 1.f;
        }


        // Mostramos informacion del modelo seleccionado.
        const Model& m = models[currentModel];

        ImGui::Separator();

        ImGui::Text(
            "Vertices: %zu | Triangulos: %zu",
            m.vertices.size(),
            m.indices.size() / 3
        );

        ImGui::Text(
            "Min Y: %.4f | Max Y: %.4f",
            m.minimum.y,
            m.maximum.y
        );

        ImGui::TextWrapped(
            "Arrastra con boton izquierdo fuera del panel "
            "para orbitar. Usa rueda para zoom."
        );

        ImGui::End();

#endif


        // Consultamos el tamanio real del area de dibujo.
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);


        // Dibujamos el modelo actual si el area tiene tamanio valido.
        if (width > 0 && height > 0) {

            renderer.draw(
                models[currentModel],
                camera,
                width,
                height,
                renderMode,
                rotationX,
                rotationY,
                rotationZ,
                modelScale
            );
        }


#ifdef VISUALIZADOR_IMGUI

        // Dibujamos el panel ImGui sobre la escena.
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

#endif


        // Intercambiamos los buffers para presentar el fotograma.
        glfwSwapBuffers(window);
    }


#ifdef VISUALIZADOR_IMGUI

    // Liberamos los recursos utilizados por Dear ImGui.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

#endif


    // Liberamos los recursos OpenGL de cada modelo.
    for (auto& m : models)
        renderer.destroyModel(m);


    // Cerramos el renderizador.
    renderer.shutdown();


    // Destruimos la ventana y finalizamos GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();


    // Devolvemos cero para indicar una finalizacion correcta.
    return 0;
}
