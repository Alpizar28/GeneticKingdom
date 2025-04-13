# 🚀 Flujo de trabajo: Subir código que compila

Este documento describe los pasos recomendados para subir tu código funcional al repositorio usando Git correctamente. Asegura que todo el equipo trabaje ordenado y sin conflictos.

---

## ✅ Escenario

Estás en tu rama (`juego-pablo`, `algoritmos-steven`, etc.), y **tu código compila sin errores**.

----------------------------------------------------------------------------------------

## 🧩 Paso 1: Verificá que estés en tu rama

```bash
git branch
```

Debe marcar tu rama con `*`, ejemplo:
```
* juego-pablo
```

----------------------------------------------------------------------------------------

## 💾 Paso 2: Guardá tus cambios

```bash
git add .
git commit -m "✅ Implementación completa y funcionando"
```

----------------------------------------------------------------------------------------

## 🔄 Paso 3: Traé lo más nuevo de `main`

```bash
git checkout main
git pull origin main        # Baja cambios recientes del equipo
git checkout tu-rama        # Regresás a tu rama
git merge main              # Unís los cambios de main a tu rama
```

- Si hay conflictos: los resolvés, luego hacés `git add` y `git commit`.

----------------------------------------------------------------------------------------

## ☁️ Paso 4: Subí tu rama a GitHub

```bash
git push
```

----------------------------------------------------------------------------------------

## 🔃 Paso 5: Hacé el Pull Request en GitHub

1. Entrá a [tu repositorio en GitHub](https://github.com/Alpizar28/GeneticKingdom)
2. Pestaña **"Pull Requests"** → **"New Pull Request"**
3. Base: `main` ← compare: `tu-rama`
4. Revisá los cambios
5. Título claro y descriptivo
6. Click en **"Create Pull Request"**

----------------------------------------------------------------------------------------

## ✅ Paso 6: Fusionar y borrar rama (opcional)

Si ya está en `main` y no usarás más la rama:

```bash
git branch -d tu-rama
git push origin --delete tu-rama
```

----------------------------------------------------------------------------------------

## 🧠 RESUMEN RÁPIDO

```bash
git add .
git commit -m "mensaje"
git checkout main
git pull origin main
git checkout tu-rama
git merge main
git push
```

Luego: crear Pull Request en GitHub.