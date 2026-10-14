# Comprobaciones de ExploraPC v2

Fecha de referencia: 4 de octubre de 2026. Se trabajó sobre el proyecto existente, en Windows con acceso al compilador y a un contexto gráfico. Los paquetes originales de v1 se conservaron.

## Entorno utilizado

- Visual Studio Community 2026; instalación actual 18.10.12224.181.
- Compilador MSVC x64 19.51.36256, Windows SDK 10.0.26100.0.
- CMake 4.3.1-msvc1 incluido con Visual Studio.
- Intel HD Graphics 630: contexto OpenGL 3.3.0, controlador 27.20.100.8280.
- GLFW informó escala de pantalla **1 × 1 (100 %)**.
- C++17, GLFW 3.4, GLAD generado para 3.3 core, GLM 1.0.1 y Dear ImGui 1.91.9b reutilizados. No se descargaron ni instalaron nuevas bibliotecas.

## Pruebas realizadas

Compilación de la versión final en **Release y Debug**, usando compilar.ps1. CTest ejecutó comprobacion_rapida e interaccion_grafica en ambas configuraciones: **2/2 aprobadas en Release y 2/2 en Debug**. Debug mantiene las aserciones de ImGui. No se observaron errores OpenGL ni errores de interfaz.

| Área | Resultado verificado |
|---|---|
| Base de v1 | Cinco teclas, cinco botones, cámara, rueda, límites, reset, separación y ensamblaje |
| Marcadores | Clics en los cinco números seleccionan la pieza; un arrastre iniciado sobre el SSD gira sin cambiar la selección |
| Umbral del gesto | Movimiento pequeño produce clic sin girar; superar el umbral y regresar al origen sigue siendo arrastre |
| Recorrido | Orden fuente, placa, SSD, RAM, CPU; límites inferior/superior; Anterior; vuelta a exploración |
| Estado educativo | Selección coincide con el paso o fase en todos los cuadros del recorrido y la demostración |
| Controles por modo | Teclas no alteran el paso guiado; seleccionar una pieza abandona la demostración y muestra su explicación |
| Demostración | Abrir, reproducir, tres fases, pausa, continuar, fin a 12 s, reinicio y entrada al recorrido durante reproducción |
| Tiempo | Pausa conserva reloj y posiciones de símbolos; demo y separación comparadas a 30/120 FPS |
| Rutas | Símbolos consultan posiciones actuales; comprobados con computadora ensamblada y separada |
| Ventana | Redimensionamiento real de 1280 × 900 a 960 × 720; centros de controles dentro de la ventana |
| Cierre | Esc termina con código 0; recursos liberados sin aserciones |
| Ejecutable de entrega | ExploraPC-v2.exe ejecutó --smoke desde otra carpeta y encontró sus shaders junto al ejecutable |
| Revisión visual | Exploración, paso 5, RAM ensamblada/separada y CPU pausada a 960 × 720 |

Las pruebas de interacción envían teclas al callback de la aplicación y eventos de ratón a ImGui. Los botones cambian el estado mediante su código normal. Los marcadores se pulsan en sus posiciones proyectadas, sin asignar la selección directamente.

La prueba completa utiliza un paso **simulado de 1/30 s** para reproducir la secuencia. La aplicación normal utiliza el tiempo real de GLFW. Las comprobaciones de lógica adicionales cubren saturación de pasos, pausa, reinicio, fin de reproducción y seguimiento de posiciones separadas.

## Evidencias

Registros finales:

- evidencias/v2-ctest-release.txt
- evidencias/v2-ctest-debug.txt
- evidencias/v2-portable.txt

Capturas finales del framebuffer, convertidas de PPM a PNG:

- v2_libre_960.png
- v2_recorrido_960.png
- v2_demo_ram_ensamblada.png
- v2_demo_ram_separada.png
- v2_demo_cpu_pausada.png
- v2_separado.png

Al ejecutar CTest, las capturas originales se guardan en build/vs2026/evidencias; las copias PNG de entrega están en evidencias. Las imágenes muestran controles accesibles, nombre junto al marcador, flechas de datos, línea de energía y estado de pausa. El panel dispone de una barra de lectura visible y un recordatorio fijo de RAM temporal frente a SSD persistente.

Las comprobaciones son automáticas y la revisión visual se hizo sobre capturas. Esto no sustituye una sesión manual con los dispositivos del usuario.

## Pruebas manuales pendientes

- [ ] Probar con ratón y teclado físicos los cinco marcadores, botones y teclas.
- [ ] Leer completas las cinco explicaciones, desplazando el área de lectura.
- [ ] Arrastrar desde la escena hacia el panel y desde el panel hacia la escena; confirmar el comportamiento percibido.
- [ ] Observar la demostración con tiempo real, pausarla y separar/ensamblar repetidamente mientras está pausada o reproduciendo.
- [ ] Cerrar con la X; minimizar, restaurar y maximizar.
- [ ] Probar escalado de Windows **125 % y 150 %**, cambio entre monitores y otras GPU. Sólo se observó y comprobó el 100 %; no se alteró la configuración de Windows.
- [ ] Compilar y ejecutar con Visual Studio 2022 y en el equipo de exposición.

## Alcance didáctico

Los marcadores, flechas y símbolos son superposiciones 2D calculadas a partir de posiciones 3D; no usan ocultación por profundidad y pueden superponerse desde otros ángulos. Los cinco componentes y las rutas son simplificados, sin escala física ni simulación eléctrica. La fuente se mantiene indicada durante toda la demostración; energía e información tienen color y forma diferentes.

Reiniciar deja la demostración detenida al inicio. Cambiar de modo detiene y reinicia su reloj, manteniendo cámara y disposición de las piezas. Seleccionar libremente durante la demostración vuelve a la explicación de esa pieza.

## Entrega

ExploraPC-v2-ejecutable.zip contiene ExploraPC-v2.exe y shaders junto al ejecutable. ExploraPC-v2-fuentes.zip contiene fuentes, pruebas, scripts, documentación, dependencias con licencias y evidencias. La solución local continúa llamándose ExploraPC y se genera con CMake. La integridad de los ZIP y la presencia de los archivos necesarios se verifican al preparar la entrega.
