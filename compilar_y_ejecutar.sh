#!/bin/bash

echo "🔧 Compilando Genetic Kingdom..."

# Crear carpeta build si no existe
mkdir -p build
cd build

# Generar archivos con CMake
cmake .. || { echo '❌ Error en CMake'; exit 1; }

# Compilar el proyecto
make || { echo '❌ Error en compilación'; exit 1; }

# Copiar assets al directorio de ejecución si no existen
if [ ! -d "assets" ]; then
  cp -r ../assets ./assets
fi

# Ejecutar
echo '✅ Compilación exitosa. Ejecutando el juego...'
./GeneticKingdom
