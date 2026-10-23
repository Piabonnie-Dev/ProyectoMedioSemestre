# Visualizador OBJ — Proyecto de Medio Semestre

**Universidad Nacional Autónoma de México · Facultad de Ciencias**  
**Asignatura:** Visualización · **Grupo:** 7018 · **Semestre:** 2027-1
**Alumno:** Elliot Kenneth Villafaña Pasten 118006642

Visualizador de modelos tridimensionales en formato **Wavefront OBJ**, desarrollado en **C++17 con OpenGL 3.3 Core**. Permite cargar distintos modelos, cambiar entre representaciones de puntos, líneas y superficies, aplicar rotaciones y escalamiento, y explorar la escena mediante una cámara orbital. También incluye un mapa de colores por altura, ejes coordenados XYZ y una interfaz opcional con Dear ImGui.

## 1. Funcionalidades

- **Carga de archivos OBJ:** detecta los archivos `.obj` de la carpeta `modelos/` o recibe rutas desde la terminal.
- **Selección de modelos:** cambio mediante el teclado o desde el panel de Dear ImGui.
- **Tres modos de visualización:** nube de puntos, malla de líneas (*wireframe*) y superficie sólida.
- **Mapa de altura:** shader GLSL que interpola entre **azul** (alturas bajas), **amarillo** (altura media) y **rojo** (alturas altas).
- **Ejes coordenados:** X rojo, Y verde y Z azul.
- **Transformaciones:** rotación sobre los tres ejes y escalamiento del modelo.
- **Cámara orbital:** movimiento con arrastre del ratón y zoom con la rueda.
- **Información en terminal:** nombre, cantidad de vértices y triángulos, extremos XYZ y alturas mínima y máxima.
- **Superficie matemática:** malla de 100 × 100 vértices basada en `y = cos(x) + sin(z)`.
- **Pruebas automatizadas:** comprobaciones para la lectura OBJ y la superficie generada.

## 2. Evidencia del funcionamiento

Las siguientes imágenes son capturas reales de la ejecución del programa. Se conservaron exactamente sus nombres originales: `Captura 1.png` a `Captura 7.png`.

### Captura 1 — Modelo 3D y panel de controles

Visualización de `bunny.obj` como superficie sólida. Se observan el mapa de color por altura, los ejes XYZ y el panel de Dear ImGui para seleccionar modelos, modos de dibujo y transformaciones.

![Captura 1 - Modelo bunny en modo superficie e interfaz Dear ImGui](capturas/Captura%201.png)

### Captura 2 — Nube de puntos

El modelo `bunny.obj` dibujado mediante sus vértices, utilizando el modo **Puntos** (`GL_POINTS`).

![Captura 2 - Bunny en modo nube de puntos](capturas/Captura%202.png)

### Captura 3 — Malla de líneas

Representación de `bunny.obj` en modo **Líneas**, que permite observar la triangulación de su geometría.

![Captura 3 - Bunny en modo lineas o wireframe](capturas/Captura%203.png)

### Captura 4 — Superficie sólida

El mismo modelo renderizado con triángulos rellenos y el gradiente de alturas azul–amarillo–rojo, desde una vista más cercana.

![Captura 4 - Bunny en modo superficie solida](capturas/Captura%204.png)

### Captura 5 — Rotación del modelo y cámara

Ejemplo del modelo con rotaciones configuradas en los ejes X, Y y Z desde el panel de controles, mostrando una orientación diferente.

![Captura 5 - Transformaciones de rotacion en bunny](capturas/Captura%205.png)

### Captura 6 — Superficie matemática

Superficie `SUPERFICIE_100x100_cos(x)+sin(z)` con elevaciones coloreadas mediante el shader de altura.

![Captura 6 - Superficie matematica coseno mas seno](capturas/Captura%206.png)

### Captura 7 — Información en la terminal

Salida de MSYS2 UCRT64 con datos de los modelos `Teapot.obj`, `bunny.obj` y la superficie matemática. La captura muestra, entre otros datos, **34 817 vértices y 69 630 triángulos** para `bunny.obj`, y **10 000 vértices y 19 602 triángulos** para la superficie generada. La sesión mostrada contiene ocho entradas en el selector, incluida la superficie.

![Captura 7 - Informacion de modelos OBJ en terminal](capturas/Captura%207.png)

## 3. Tecnologías utilizadas

| Tecnología | Uso en el proyecto |
|---|---|
| **C++17** | Lectura de archivos, modelos, estructura y lógica de la aplicación |
| **OpenGL 3.3 Core** | Renderizado de puntos, líneas y triángulos |
| **GLFW** | Ventana, teclado y ratón |
| **GLEW** | Acceso a funciones de OpenGL |
| **GLM** | Vectores y matrices de transformaciones, vista y proyección |
| **GLSL** | Vertex shader y fragment shader para el mapa de colores |
| **Dear ImGui** | Interfaz gráfica opcional |
| **CMake + Ninja** | Configuración y compilación |
| **CTest** | Ejecución de las pruebas automatizadas |

## 4. Requisitos en Windows

La compilación documentada se realiza desde **MSYS2 UCRT64** (no desde la terminal MSYS normal). Se necesita un controlador de gráficos compatible con **OpenGL 3.3** y conexión a internet al configurar Dear ImGui por primera vez.

En **MSYS2 UCRT64**, instala las herramientas y bibliotecas:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-glfw mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-glm
```

## 5. Compilación y ejecución

Abre MSYS2 UCRT64 y entra al directorio donde guardaste el proyecto. Por ejemplo, si está en `C:\ProyectoMedioSemestre`:

```bash
cd /c/ProyectoMedioSemestre
```

**Paso 1. Configurar el proyecto** (con interfaz Dear ImGui y pruebas):

```bash
cmake -S . -B build-ucrt64 -G Ninja -DCMAKE_BUILD_TYPE=Debug -DVISUALIZADOR_IMGUI=ON -DBUILD_TESTING=ON
```

**Paso 2. Compilar:**

```bash
cmake --build build-ucrt64
```

**Paso 3. Ejecutar:**

```bash
./build-ucrt64/visualizador.exe
```

Al compilar, CMake copia las carpetas `modelos/` y `shaders/` junto al ejecutable. Si posteriormente agregas más archivos OBJ a la carpeta original `modelos/`, puedes copiarlos al directorio del ejecutable sin recompilar el código:

```bash
cmake -E copy_directory modelos build-ucrt64/modelos
```

Para utilizar el programa **sin Dear ImGui**, configura una carpeta de compilación independiente:

```bash
cmake -S . -B build-sin-imgui -G Ninja -DCMAKE_BUILD_TYPE=Debug -DVISUALIZADOR_IMGUI=OFF
cmake --build build-sin-imgui
./build-sin-imgui/visualizador.exe
```

### Cargar modelos desde la terminal

Sin argumentos, el programa busca archivos `.obj` en el directorio `modelos/` ubicado junto al ejecutable y los ordena alfabéticamente. Además, agrega la superficie matemática generada por código.

También se pueden indicar uno o varios modelos mediante sus rutas:

```bash
./build-ucrt64/visualizador.exe modelos/bunny.obj modelos/Teapot.obj
```

En ese caso se cargan los OBJ indicados, más la superficie matemática. Las rutas con espacios deben ir entre comillas.

## 6. Controles

| Tecla o acción | Función |
|---|---|
| `N` o flecha **derecha** | Siguiente modelo |
| `B` o flecha **izquierda** | Modelo anterior |
| `1` | Visualización por puntos |
| `2` | Visualización por líneas |
| `3` | Visualización de superficie |
| `X` / `Y` / `Z` | Aumentar 10° la rotación en el eje correspondiente |
| `+` / `-` | Aumentar / disminuir la escala |
| `R` | Reiniciar rotación y escala |
| Ratón izquierdo + arrastrar | Orbitar alrededor del modelo |
| Rueda del ratón | Acercar o alejar la cámara |
| `Esc` | Cerrar la ventana |
| Panel Dear ImGui | Elegir modelo, modo, rotación y escala |

## 7. ¿Cómo funciona el programa?

1. `main.cpp` crea una instancia de `Application` y ejecuta `run()`.
2. `Application.cpp` localiza los OBJ, inicia GLFW/OpenGL, registra los controles y mantiene el ciclo de dibujo.
3. `OBJLoader.cpp` interpreta las líneas `v` (vértices) y `f` (caras) de cada OBJ, convirtiendo las caras compatibles en triángulos.
4. `Model.cpp` calcula la caja delimitadora a partir de las posiciones mínima y máxima, su centro y un radio para encuadrar el modelo.
5. `Renderer.cpp` envía posiciones e índices a la GPU mediante **VAO, VBO y EBO** y dibuja el modelo según el modo seleccionado.
6. `Camera.cpp` calcula las matrices de vista y proyección, y permite navegar de forma orbital.
7. Los shaders `height.vert` y `height.frag` transforman las posiciones y colorean cada fragmento según la altura original del vértice.
8. `SurfaceGenerator.cpp` crea la superficie matemática adicional.

### Colores según la altura

La altura de cada punto se normaliza mediante:

$$
t = \frac{y-y_{\min}}{y_{\max}-y_{\min}}
$$

El shader evita dividir entre cero cuando las alturas coinciden y limita `t` al intervalo `[0, 1]`. Después interpola los colores: azul para `t = 0`, amarillo para `t = 0.5` y rojo para `t = 1`.

### Superficie matemática

Se genera una cuadrícula de **100 × 100 vértices** en el dominio:

$$
(x,z) \in [-2\pi,2\pi]\times[-2\pi,2\pi],\qquad y=\cos(x)+\sin(z)
$$

Cada una de las **99 × 99 celdas** se divide en dos triángulos:

- **10 000 vértices**.
- **9 801 celdas**.
- **19 602 triángulos**.
- **58 806 índices**.

Los extremos teóricos de la función son `-2` y `2`. En la captura de la terminal, el muestreo de la cuadrícula produce aproximadamente **-1.99937 y 1.99987**.

## 8. Estructura del repositorio

```text
ProyectoMedioSemestre/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/                 # Declaraciones .hpp
├── src/                     # Implementaciones .cpp
├── shaders/
│   ├── height.vert
│   └── height.frag
├── modelos/                 # Archivos .obj
├── tests/
│   └── test_obj.cpp
└── capturas/
    ├── Captura 1.png
    ├── Captura 2.png
    ├── Captura 3.png
    ├── Captura 4.png
    ├── Captura 5.png
    ├── Captura 6.png
    └── Captura 7.png
```

> Los directorios de compilación (`build/`, `build-ucrt64/`, `build-sin-imgui/`) no deben subirse al repositorio. Añádelos a `.gitignore`.

## 9. Pruebas automatizadas

Con el proyecto configurado y compilado en modo `Debug`, ejecuta:

```bash
ctest --test-dir build-ucrt64 --output-on-failure
```

La prueba `ParserYSuperficie` verifica un OBJ temporal con una cara cuadrangular, su triangulación y los valores básicos de la superficie de 100 × 100 vértices. Las verificaciones del archivo `tests/test_obj.cpp` usan `assert`, por lo que conviene ejecutarlas en una configuración **Debug** donde las aserciones no estén deshabilitadas.

## 10. Compatibilidad y limitaciones

- El lector procesa posiciones de vértices `v` y caras `f`, y acepta referencias de vértices como `v`, `v/vt`, `v//vn` y `v/vt/vn`.
- Convierte índices OBJ positivos y negativos al sistema de índices usado en C++.
- La triangulación en abanico es adecuada para caras **convexas**; no garantiza representar correctamente polígonos cóncavos.
- No utiliza materiales `.mtl`, texturas UV ni normales del archivo: el aspecto visual se determina mediante el shader de alturas.
- Si un OBJ contiene vértices pero no caras, se muestra como nube de puntos.
- Los modelos deben incluirse en `modelos/` para que otra persona pueda reproducir las capturas. Al publicar modelos externos, hay que respetar sus licencias.

---

**Proyecto académico de Visualización — Facultad de Ciencias, UNAM.**
