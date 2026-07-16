# Click Simulator — Guía de assets y estructura

Este documento resume qué archivos tienes que añadir, con qué nombre exacto
y en qué carpeta, para que el juego funcione tal como lo pediste. También
explica dónde quedó cada estructura de datos.

## 1. Cómo funciona ahora la apertura del juego (ya no hay video de intro)

Ya no hay "video" de intro ni secuencia de frames para esa pantalla. Ahora,
al abrir el juego:

1. La ventana arranca completamente **negra y sin sonido**.
2. La pantalla se va **aclarando gradualmente** (negro -> blanco).
3. Cuando queda **totalmente blanca**, suena un audio (una sola vez).
4. Cuando ese audio **termina de reproducirse**, la pantalla blanca
   desaparece y aparece la pantalla de inicio (fondo + gif a la derecha +
   botón "Jugar" a la izquierda), y ahí arranca la música del menú.

Si no agregas el archivo de audio, el juego no se queda trabado: espera un
par de segundos en blanco y sigue igual hacia el menú.

## 2. Por qué el "gif" del menú es en realidad un sprite sheet

SFML (la librería que usa este proyecto) **no puede reproducir archivos
.gif directamente** sin instalar dependencias extra pesadas. Antes este
proyecto usaba una carpeta con un PNG por cada fotograma; ahora usa
**una sola imagen "sprite sheet"** (varios fotogramas del gif puestos en
tira, uno al lado del otro), que es justo lo que generan herramientas
online de "gif to sprite sheet" como la que ya usaste. El juego recorta
automáticamente cada fotograma de esa tira y los reproduce en bucle como
si fuera el gif.

La configuración actual espera una tira de **5 columnas x 1 fila (5
fotogramas)**, igual a la que ya generaste. Si en el futuro generas una
tira con otra cantidad de columnas/filas/fotogramas, solo hay que ajustar
esos 3 números en `EscenaMenu.cpp` (la línea de `animGif.cargarSpriteSheet(...)`).

## 3. Archivos que debes añadir (nombre exacto y carpeta)

### Pantalla de inicio / menú
- `assets/images/menu/fondo_menu.png` — el fondo del menú (distinto al del juego).
- `assets/images/menu/gif_spritesheet.png` — el sprite sheet del gif (5 columnas x 1 fila, 5 fotogramas), tal como el que exportaste con la herramienta de conversión. Se reproduce en bucle, centrado pero hacia la derecha.
- `assets/images/menu/boton_jugar.png` — el botón "Jugar" en pixel art (abajo a la izquierda).

### Personaje — idle (5 sprites)
- `assets/images/personaje/idle/idle_0001.png` ... `idle_0005.png`

### Personaje — finales (animaciones)
- Final 1 (tira un beso, 4 sprites): `assets/images/personaje/final_beso/beso_0001.png` ... `beso_0004.png`
- Final 2 (saluda, 4 sprites, se desliza a la izquierda): `assets/images/personaje/final_saludo/saludo_0001.png` ... `saludo_0004.png`
- Final 3 (pose sexy, 8 sprites): `assets/images/personaje/final_pose/pose_0001.png` ... `pose_0008.png`
- Final 4 (patineta, 2 sprites, se desliza a la derecha): `assets/images/personaje/final_patineta/patineta_0001.png` ... `patineta_0002.png`

### UI del gameplay
- `assets/images/ui/flor.png` — el ícono de la flor (abajo a la izquierda, junto al contador).

*(El cuadro de diálogo negro con bordes blancos NO necesita imagen: se dibuja
directamente con formas de SFML, así que no tienes que crear ningún archivo
para eso.)*

### Fuente de texto (obligatoria para que se vea cualquier texto)
- `assets/fuentes/pixel_font.ttf` — cualquier fuente pixel art gratuita (ej. de fonts.google.com o itch.io). Sin este archivo el juego corre pero los textos no se dibujan.

### Sonido — apertura del juego (pantalla que va de negro a blanco)
- `assets/sonidos/apertura.ogg` — suena una sola vez, justo cuando la pantalla queda totalmente blanca. Cuando termina de sonar, se pasa a la pantalla de inicio. Si falta este archivo, el juego igual sigue tras una breve espera (no se traba).

### Sonido — música (loop)
- `assets/sonidos/musica_menu.ogg` — suena en bucle en la pantalla de inicio.
- `assets/sonidos/musica_juego.ogg` — suena en bucle una vez que le das a Jugar.

### Sonido — voces de click (10 clips, al azar)
- `assets/sonidos/voces/voz_click_01.ogg` ... `voz_click_10.ogg`

### Sonido — despedida
- `assets/sonidos/despedida.ogg` — suena una sola vez al llegar a cualquiera de los 4 finales (click 30).

> Todos los sonidos deben ser `.ogg` (formato que usa SFML). Si los tienes en
> `.mp3` o `.wav`, conviértelos con `ffmpeg -i sonido.mp3 sonido.ogg`.

## 4. Carpeta `estructuras/` — las 5 estructuras de datos pedidas

```
estructuras/
  arrays/ArrayAnimacion.hpp        -> ARREGLO: fotogramas de cada animación
  pilas/PilaEscenas.hpp            -> PILA: historial de cambio de escena (Intro->Menu->Juego)
  colas/ColaDialogos.hpp           -> COLA: orden de aparición del texto y las opciones del diálogo
  grafos/GrafoInteracciones.hpp    -> GRAFO: árbol de preguntas/respuestas y sus 4 finales
  tablas_hash/TablaHashGuardado.hpp -> TABLA HASH: guarda tu progreso (clicks, elección, final) en un archivo
```

Cada archivo tiene, al inicio, un comentario explicando qué estructura usa y
por qué. Si ya tenías creada una carpeta vacía llamada `estructures` (con
esa ortografía), puedes borrarla: el proyecto ahora usa `estructuras`
(con la ortografía correcta) como carpeta única.

## 5. Árbol de diálogo implementado (grafo)

```
Click 10 -> "Hola nena"
   Coquetear -> Click 20 -> "Da una flor"
        Aceptas    -> Click 30 -> Final 1: tira un beso (se queda en el centro)
        No gracias -> Click 30 -> Final 2: saluda y se desliza hacia la izquierda
   Ignorar -> Click 20 -> "¿Cómo te llamas?"
        Decir nombre -> Click 30 -> Final 3: pose sexy (se queda en el centro)
        Queti      -> Click 30 -> Final 4: se va en patineta hacia la derecha
```

## 6. Progreso guardado

El progreso (clicks + elecciones) se guarda automáticamente en
`guardado/progreso.dat` cada vez que haces click. Si cierras el juego antes
de llegar a un final, al volver a abrirlo continúa exactamente donde
quedaste (incluso si un cuadro de diálogo estaba abierto sin responder).
Al llegar a cualquier final, ese archivo se borra al cerrar la ventana, así
que la próxima partida empieza de cero.

## 7. Compilar

El archivo `.vscode/tasks.json` ya quedó actualizado con los nuevos
`.cpp` y con `-lsfml-audio`. Solo corre la tarea de build de siempre
("Compilar Click Simulator") o:

```bash
g++ -std=c++17 src/main.cpp src/core/Game.cpp src/core/EscenaIntro.cpp src/core/EscenaMenu.cpp src/core/EscenaJuego.cpp src/core/Personaje.cpp src/core/ManejadorAudio.cpp -Iinclude -o Click_Simulator.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```
