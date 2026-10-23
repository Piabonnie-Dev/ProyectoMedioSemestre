#pragma once
#include "Model.hpp"
#include <filesystem>

// Lee las lineas v y f de un OBJ; convierte sus caras convexas en triangulos.
bool loadOBJ(const std::filesystem::path& path, Model& model);
