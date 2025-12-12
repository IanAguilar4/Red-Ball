# Red Ball - Estructura del Proyecto

## Organización del Código

El proyecto ha sido refactorizado para una mejor organización y mantenibilidad:

### Archivos de Cabecera (`include/`)

- **GameState.hpp** - Enumeración de estados del juego (Title, EnterName, Playing, etc.)
- **Platform.hpp** - Estructuras `Platform` y `Spike` con sus funciones auxiliares
- **GameWorld.hpp** - Estructura `Level` y declaraciones de funciones `createLevel1-10()`
- **PlayerScore.hpp** - Estructura `PlayerScore` y funciones de carga/guardado de puntajes
- **Utils.hpp** - Funciones utilitarias (como `centerTextX`)

### Archivos de Implementación (`src/`)

- **RedBall.cpp** - Archivo principal con la función `main()` que orquesta el juego
- **Platform.cpp** - Implementación del constructor de `Platform` y función `createSpike()`
- **GameWorld.cpp** - Constructor de `Level` e implementación de todos los niveles (1-10)
- **PlayerScore.cpp** - Implementación de `loadScores()` y `saveScores()`
- **Utils.cpp** - Implementación de funciones utilitarias
- **RedBall_backup.cpp** - Respaldo del archivo monolítico original (no se compila)

### Archivos de Datos (`data/`)

- **scores.txt** - Almacena las puntuaciones de los jugadores (nombre, nivel alcanzado, tiempo). Se genera automáticamente durante el juego y se actualiza cuando un jugador completa niveles.

## Compilación

El makefile ha sido actualizado para compilar todos los archivos necesarios:

```bash
# Compilar el juego
make bin/RedBall.exe

# Ejecutar el juego
./bin/RedBall.exe
```

## Ventajas de esta Estructura

1. **Modularidad**: Cada componente está en su propio archivo
2. **Mantenibilidad**: Más fácil encontrar y modificar código específico
3. **Reutilización**: Las estructuras y funciones pueden usarse en otros proyectos
4. **Escalabilidad**: Fácil agregar nuevos niveles o características
5. **Compilación**: El compilador solo recompila lo que cambió

## Física del Juego

El juego ahora usa **delta time** para asegurar comportamiento consistente en diferentes tasas de frames:
- Todos los cálculos de física se multiplican por `dt * 60.0f`
- El juego funciona igual en monitores de 60Hz, 144Hz, 240Hz, etc.
