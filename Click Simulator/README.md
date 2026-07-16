# Click Simulator — Guía de assets y estructura

Este documento resume qué archivos tienes que añadir, con qué nombre exacto
y en qué carpeta, para que el juego funcione tal como lo pediste. También
explica dónde quedó cada estructura de datos.

## 1. Por qué "video" y "gif" son en realidad secuencias de imágenes

SFML (la librería que usa este proyecto) **no puede reproducir archivos de
video (.mp4) ni .gif directamente** sin instalar dependencias extra pesadas
(decodificadores de video). La solución estándar — y la que usa este
proyecto — es convertir tu video y tu gif en una **secuencia de imágenes
PNG numeradas**, y el juego las reproduce como una animación de fotogramas.

Puedes hacerlo gratis con `ffmpeg` (línea de comandos):

```bash
# Video de intro -> frames (ajusta el fps que quieras, 20 va bien)
ffmpeg -i mi_video.mp4 -vf fps=20 assets/images/intro/frame_%04d.png

# Gif del menú -> frames
ffmpeg -i mi_gif.gif assets/images/menu/gif_%04d.png
```

Esto genera automáticamente `frame_0001.png`, `frame_0002.png`, ... El
juego detecta solo cuántos frames hay (no necesitas tocar el código si
agregas o quitas frames).

## 2. Archivos que debes añadir (nombre exacto y carpeta)

### Pantalla de intro (video)
- `assets/images/intro/frame_0001.png`, `frame_0002.png`, ... — los frames de tu video.

### Pantalla de inicio / menú
- `assets/images/menu/fondo_menu.png` — el fondo del menú (distinto al del juego).
- `assets/images/menu/gif_0001.png`, `gif_0002.png`, ... — los frames de tu gif (se reproduce en bucle, centrado pero hacia la derecha).
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

### Sonido — música (loop)
- `assets/sonidos/musica_menu.ogg` — suena en bucle en la pantalla de inicio.
- `assets/sonidos/musica_juego.ogg` — suena en bucle una vez que le das a Jugar.

### Sonido — voces de click (10 clips, al azar)
- `assets/sonidos/voces/voz_click_01.ogg` ... `voz_click_10.ogg`

### Sonido — despedida
- `assets/sonidos/despedida.ogg` — suena una sola vez al llegar a cualquiera de los 4 finales (click 30).

> Todos los sonidos deben ser `.ogg` (formato que usa SFML). Si los tienes en
> `.mp3` o `.wav`, conviértelos con `ffmpeg -i sonido.mp3 sonido.ogg`.

## 3. Carpeta `estructuras/` — las 5 estructuras de datos pedidas

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

## 4. Árbol de diálogo implementado (grafo)

```
Click 10 -> "Hola nena"
   Coquetear -> Click 20 -> "Da una flor"
        Aceptas    -> Click 30 -> Final 1: tira un beso (se queda en el centro)
        No gracias -> Click 30 -> Final 2: saluda y se desliza hacia la izquierda
   Ignorar -> Click 20 -> "¿Cómo te llamas?"
        Decir nombre -> Click 30 -> Final 3: pose sexy (se queda en el centro)
        Queti      -> Click 30 -> Final 4: se va en patineta hacia la derecha
```

## 5. Progreso guardado

El progreso (clicks + elecciones) se guarda automáticamente en
`guardado/progreso.dat` cada vez que haces click. Si cierras el juego antes
de llegar a un final, al volver a abrirlo continúa exactamente donde
quedaste (incluso si un cuadro de diálogo estaba abierto sin responder).
Al llegar a cualquier final, ese archivo se borra al cerrar la ventana, así
que la próxima partida empieza de cero.

## 6. Compilar

El archivo `.vscode/tasks.json` ya quedó actualizado con los nuevos
`.cpp` y con `-lsfml-audio`. Solo corre la tarea de build de siempre
("Compilar Click Simulator") o:

```bash
g++ -std=c++17 src/main.cpp src/core/Game.cpp src/core/EscenaIntro.cpp src/core/EscenaMenu.cpp src/core/EscenaJuego.cpp src/core/Personaje.cpp src/core/ManejadorAudio.cpp -Iinclude -o Click_Simulator.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```
