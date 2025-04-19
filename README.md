# 🏰 Proyecto #2 - Genetic Kingdom (CE2103 - TEC)

Tower Defense medieval en C++ con algoritmos genéticos y pathfinding A*.  
Desarrollado como parte del curso **Algoritmos y Estructuras de Datos II**.

------------------------------------------------------------------------------

## 🔧 Requisitos del sistema

- ✅ Ubuntu 22.04 o superior (también funciona en Linux Mint o derivados)
- ✅ Visual Studio Code (opcional, pero recomendado)
- ✅ g++ 11+ (preferiblemente 13)
- ✅ CMake 3.10 o superior
- ✅ SFML 2.5+ (Simple and Fast Multimedia Library)
- ✅ Git

------------------------------------------------------------------------------

## ⚙️ Instalación de dependencias

```bash
sudo apt update
sudo apt install -y build-essential cmake gdb git libsfml-dev libboost-all-dev
```

> **Nota:** Boost es opcional, útil para estructuras de red o sockets.

------------------------------------------------------------------------------

## 🛠️ Compilación del proyecto

### 🔁 1. Clonar el repositorio

```bash
git clone https://github.com/Alpizar28/GeneticKingdom.git
cd GeneticKingdom
```

### 🔧 2. Compilar con CMake

```bash
mkdir build
cd build
cmake ..
make
```

### ▶️ 3. Ejecutar el juego

```bash
./GeneticKingdom
```

--------------------------------------------------------------------------------

## 🗃️ Estructura del proyecto

```
GeneticKingdom/
├── src/
│   ├── Map.cpp / Map.h
│   ├── Tower.cpp / Tower.h
│   ├── Enemy.cpp / Enemy.h
│   ├── Game.cpp / Game.h
│   ├── GeneticAlgorithm.cpp / .h
│   └── Pathfinding.cpp / .h
├── include/           # Headers compartidos
├── assets/            # Imágenes, sonidos, fuentes
├── CMakeLists.txt     # Configuración de compilación
└── README.md
```

---

## 👥 Organización del equipo

| Nombre   | Rama Git             | Tarea principal                         |
|----------|----------------------|------------------------------------------|
| Pablo    | `juego-pablo`        | Mapa, torres, enemigos, interfaz         |
| Adrián   | `juego-adrian`       | Interfaz, lógica de juego                |
| Steven   | `algoritmos-steven`  | A* y algoritmo genético                  |

---

## 🔀 Flujo de trabajo con Git

Cada integrante trabaja en su propia rama para evitar conflictos.

### 📌 Comandos clave

```bash
# Cambiarse a tu rama
git checkout juego-pablo

# Subir cambios
git add .
git commit -m "Tu mensaje"
git push

# Mantener tu rama actualizada
git checkout main
git pull origin main
git checkout tu-rama
git merge main
```

### 📢 Importante
- No trabajar directamente en `main`.
- Integrar mediante Pull Requests.
- Confirmar que todo compila antes de subir a `main`.

------------------------------------------------------------------------------