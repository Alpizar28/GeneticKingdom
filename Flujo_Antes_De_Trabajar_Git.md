# 🟢 Flujo de trabajo Git: Antes de comenzar a trabajar

Este documento describe los pasos recomendados para actualizar tu rama local con los últimos cambios del equipo antes de comenzar a programar.

------------------------------------------------------------------------------------------------------------

## 🔁 Paso A: Traé los cambios más recientes de `main` a tu rama

```bash
git checkout main
git pull origin main         # Trae lo más nuevo del equipo
git checkout tu-rama         # Vuelve a tu rama personal
git merge main               # Integra lo más nuevo en tu rama
```

- Si hay conflictos: resolvés, luego hacés `git add .` y `git commit`

------------------------------------------------------------------------------------------------------------

## 🧠 RESUMEN RÁPIDO

```bash
git checkout main
git pull origin main
git checkout tu-rama
git merge main
```