# Guía de aprendizaje de ExploraPC v2

## Qué demuestra el proyecto

La computadora es un sistema: la placa madre conecta; el procesador ejecuta instrucciones; la RAM mantiene temporalmente lo que se usa; el SSD conserva programas y archivos; la fuente proporciona energía.

La RAM pierde su contenido al apagar. El SSD conserva datos sin energía. Al abrir un programa, se cargan sus datos del almacenamiento a la RAM para que el procesador trabaje con ellos. Las comparaciones cotidianas ayudan a comprender una función; no sustituyen la explicación técnica.

La disposición, los tamaños y colores son didácticos. Hay dos módulos RAM y un SSD representado como una caja. Los detalles geométricos no son circuitos reales.

## Recorrido por seis módulos pequeños

1. **Inicio (`main.cpp`)**. GLFW crea la ventana y el contexto 3.3 core. GLAD obtiene las funciones del controlador. Se inicia ImGui. El ciclo recoge eventos, mide tiempo, actualiza estado y dibuja escena y panel. Al salir se liberan recursos antes de cerrar el contexto.
2. **Cámara (`Camara`)**. Guarda azimut, elevación y distancia. Calcula la posición de observación y los límites. Un arrastre debe empezar en la escena; la interfaz puede capturar el ratón.
3. **Componentes (`Componentes`)**. Un arreglo de cinco elementos contiene textos, colores y posiciones. Cada pieza usa cajas pequeñas. Una posición común mueve todos sus detalles juntos.
4. **Dibujo (`Dibujo`)**. Prepara un cubo y un programa de shaders. Cada llamada a `caja` envía matriz, color y opciones. El gabinete reutiliza ese cubo.
5. **Interfaz educativa (`InterfazEducativa`)**. Dibuja botones y explicaciones. Cambia selección y objetivo de animación. Proyecta los marcadores desde el espacio 3D a la pantalla.
6. **Aprendizaje (`Aprendizaje`)**. Guarda modo, paso y tiempo de la demostración. Define el orden educativo, las tres fases y las rutas de los símbolos. No depende de OpenGL ni de ImGui. Centralizar este estado evita que texto y resaltado se contradigan.

`tests/PruebaGrafica.h` ayuda a comprobar el programa. Las bibliotecas están en `third_party`; el código para estudiar está en `src` y `shaders`.

## Vértices y triángulos

Cada vértice contiene seis números: posición `(x,y,z)` y normal `(nx,ny,nz)`. La normal indica hacia dónde apunta la superficie y permite iluminarla.

El cubo unitario va de -0,5 a +0,5. Sus seis caras tienen dos triángulos cada una: **12 triángulos y 36 vértices enviados** a `glDrawArrays`.

Las caras tienen normales independientes. Aunque dos vértices ocupen la misma esquina, se repiten con normales diferentes para que los bordes sean definidos. Con normales compartidas podrían parecer suavizados.

- **VBO**: almacena los vértices en memoria accesible por la GPU.
- **VAO**: describe cómo interpretarlos. El atributo 0 es posición; el 1 es normal. Cada registro ocupa seis `float`.
- **GL_STATIC_DRAW**: los datos se cargan una vez. La animación cambia matrices, no reconstruye vértices.

`GL_DEPTH_TEST` hace que una superficie cercana tape una lejana. El gabinete abierto permite ver el interior sin usar transparencia ni ordenar superficies translúcidas.

## Modelo, vista y proyección

La transformación es:

```text
posición_clip = proyección × vista × modelo × posición_local
```

GLM y los shaders usan vectores columna: la operación de la derecha actúa primero.

**Modelo** convierte el cubo en una pieza. `modelo = traslación × escala`: primero cambia su tamaño y después lo coloca. La placa madre es ancha y alta, pero delgada; la fuente es más profunda. Sus detalles tienen posiciones relativas.

**Vista** representa desde dónde observamos. `glm::lookAt` recibe ojo, objetivo y vector vertical. Mover la cámara cambia esta matriz; las piezas mantienen sus posiciones.

**Proyección** introduce perspectiva: lo lejano se ve más pequeño. Se usa un ángulo vertical de 42°, plano cercano 0,1 y lejano 80. La relación de aspecto considera sólo la escena, porque el panel ocupa la derecha.

Al redimensionar se recalculan viewport y proyección. La ventana usa puntos y el framebuffer píxeles: pueden diferir con el escalado de pantalla. Un framebuffer cero pausa el dibujo para evitar dividir entre cero.

## Los shaders

Un shader es un programa de la GPU. `#version 330 core` corresponde a GLSL para OpenGL 3.3.

**pieza.vert**, shader de vértices:

- Recibe posición y normal como atributos.
- Recibe matrices como `uniform`: son constantes durante esa llamada de dibujo.
- Transforma la posición y la escribe en `gl_Position`.
- Transforma la normal con la inversa transpuesta del modelo para corregir escalas diferentes en cada eje.
- Envía normal y posición local al shader de fragmentos, interpoladas sobre el triángulo.

**pieza.frag**, shader de fragmentos:

- Normaliza la normal interpolada.
- Calcula `max(dot(normal, direcciónLuz), 0)`: iluminación difusa de Lambert.
- Añade luz ambiental para que las caras en sombra sigan siendo visibles.
- Mezcla un tono claro con la pieza seleccionada.
- Genera patrones de circuitos y ranuras mediante coordenadas locales.

Las texturas son **procedurales**: `fract`, `step` y `smoothstep` producen patrones sin imágenes, UV adicionales ni samplers. Son ayudas para reconocer las piezas.

La iluminación es básica: no hay sombras proyectadas, reflejos realistas ni simulación de materiales. Se solicita antialiasing multisample para suavizar bordes si el controlador lo admite.

## La cámara orbital

Con distancia `d`, azimut `a` y elevación `e`:

```text
x = d × cos(e) × sin(a)
y = d × sin(e)
z = d × cos(e) × cos(a)
ojo = objetivo + (x,y,z)
```

El arrastre cambia los ángulos y la rueda cambia `d`. La elevación permanece entre -1,1 y +1,1 radianes para evitar polos. La distancia está entre 6,5 y 20 unidades. El botón y R restablecen los valores iniciales.

ImGui recibe las entradas. La cámara sólo las usa si el cursor está en la escena y `WantCaptureMouse` es falso. El gesto debe comenzar fuera del panel. Se actualiza la posición previa incluso sobre el panel para evitar saltos al regresar. La rueda sobre una explicación desplaza el texto.

## Selección y marcadores

Un índice entre 0 y 4 indica el componente activo. En exploración, teclas, botones y marcadores pasan por `Aprendizaje::seleccionar`. En el recorrido se usa el arreglo de orden `{4,0,3,2,1}`: fuente, placa, SSD, RAM y CPU. En la demostración, el índice se obtiene de la fase. La selección determina nombre, texto y resaltado principal; fuente y placa tienen además iluminación de apoyo durante la demostración.

El marcador toma la posición 3D de una pieza, aplica vista y proyección, divide por `w` y convierte el resultado a coordenadas de pantalla. ImGui dibuja el círculo y el número. Para seleccionar, se calcula la distancia del cursor al centro: debe estar dentro de 18 puntos y en una parte visible de la escena. Si se superponen marcadores se elige el más cercano al cursor. Es selección de un indicador 2D, no intersección de un rayo con la malla 3D.

La cámara guarda el inicio de la pulsación. Si el desplazamiento supera cinco puntos, el gesto es un arrastre y no puede seleccionar, aunque regrese al origen. Un movimiento pequeño termina en clic sin girar. El gesto debe empezar y terminar en la escena; el panel conserva la captura de ratón. Los marcadores son superposiciones y no se ocultan por profundidad.

## Animación según el tiempo

`glfwGetTime` mide los segundos transcurridos:

```text
dt = tiempo_actual - tiempo_anterior
avance += dirección × dt / 1,25
avance se limita a [0,1]
suavizado = avance² × (3 - 2 × avance)
posición = posición_inicial + desplazamiento × suavizado
```

Al separar, avance llega a 1; al ensamblar, a 0. La curva tiene velocidad cero en los extremos. Los detalles siguen la posición de su pieza.

Cambiar de dirección conserva la posición, aunque invierte el sentido de la velocidad. No es una simulación física de inercia. Usar segundos evita que más FPS produzcan una animación más rápida. La prueba compara un segundo a 30 y 120 FPS.

El modo `--self-test` simula tiempo para ser reproducible; la aplicación normal usa el reloj real.

## Recorrido y demostración: estado y tiempo

`Modo` tiene dos valores: Libre y Guiado. `paso` sólo puede estar entre 0 y 4; Anterior/Siguiente usan un límite y actualizan la selección. Al volver a explorar, deja de aplicarse ese orden. Abrir la demostración utiliza una vista educativa dentro de exploración; entrar al recorrido detiene y reinicia la demostración.

La demostración tiene `tiempo`, `iniciada` y `reproduciendo`. Reproducir empieza desde cero; Pausar/Continuar cambia sólo la reproducción; Reiniciar deja el reloj a cero detenido. Se suma `dt` sólo mientras reproduce. Al llegar a 12 segundos se detiene.

```text
fase = mínimo(2, entero(tiempo / 4))
fase 0: SSD conserva el programa
fase 1: SSD → placa madre → RAM
fase 2: RAM → placa madre → CPU, y CPU → placa madre → RAM
```

En la última fase las flechas alternan lectura y escritura. La línea dorada discontinua recuerda la energía durante todas las fases; los cuadrados turquesa representan información. No se simulan voltajes, transferencias reales ni cables. Los tiempos son elegidos para permitir leer y observar.

`rutaDatos` consulta las posiciones actuales de `Componentes`. Se obtienen origen, punto intermedio en la placa y destino. Cada símbolo interpola entre los dos tramos:

```text
t = parte_fraccionaria(tiempo / 1,8 + índice / 3) × 2
si t < 1: posición = mezcla(origen, placa, t)
si t >= 1: posición = mezcla(placa, destino, t - 1)
```

Así los tres símbolos se distribuyen por la ruta y siguen las piezas al separarse. Se calculan en 3D y después se proyectan para dibujarlos en ImGui. La pausa fija el tiempo, pero las posiciones cambian si se mueve una pieza o la cámara: los símbolos siguen anclados a la escena.

El ciclo actualiza entradas y tiempo antes de dibujar la explicación. Los botones sincronizan también la selección sin avanzar tiempo. Esto evita un cuadro con un texto de la fase anterior y un resaltado de la siguiente.

El panel reserva espacio para los modos, navegación, reproducción y acciones del gabinete. La explicación ocupa el resto, con indicación de lectura y barra visible. Ventana y framebuffer siguen medidos por separado. Se observó escalado del sistema del 100 %; otros valores están pendientes, sin modificar la configuración de Windows.

## Etapas y conceptos del curso

| Etapa | Resultado | Conceptos |
|---|---|---|
| Ventana y gabinete | Contexto y estructura abierta | Buffers, triángulos y matrices |
| Componentes y cámara | Cinco piezas y vista orbital | Composición, perspectiva y lookAt |
| Selección e información | Panel en español y teclas | Estado, interfaz y proyección |
| Animación e iluminación | Movimiento y volumen | Tiempo, interpolación, normales y Lambert |
| Comprobación y documentación | Pruebas y capturas | Validación y recursos |

Cada etapa se compiló y ejecutó antes de continuar. El registro de comprobaciones precisa el alcance de las pruebas.

## Ejercicios para el grupo

1. Cambia el color o tamaño de una pieza en `Componentes.cpp`. Explica qué cambia una escala y qué cambia una traslación.
2. Cada integrante explica un componente y su relación con los otros. Comprueba que distingue RAM de SSD.
3. Cambia la dirección de la luz en `pieza.frag`. Justifica qué caras se vuelven más claras usando el producto punto.
4. Cambia la animación a dos segundos. Explica por qué sigue siendo independiente de los FPS.
5. Cambia la perspectiva de 42° a 55° y compara la vista sin mover las piezas.

Para demostrar v2: selecciona un marcador, sigue los cinco pasos, abre un programa, pausa en la RAM, separa las piezas y continúa hacia la CPU. Explica la diferencia entre energía y datos, entre RAM y SSD, y muestra la transformación de un punto y la interpolación de los símbolos en `Aprendizaje.cpp`.
