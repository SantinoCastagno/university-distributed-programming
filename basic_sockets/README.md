### Laboratorio de Programación Distribuida - Proyecto de Práctica de Sockets en C

¡Bienvenido al proyecto de práctica de sockets en C para la asignatura "Laboratorio de Programación Distribuida"!

Este proyecto tiene como objetivo proporcionar a los estudiantes una oportunidad para practicar y comprender los conceptos fundamentales de la programación distribuida utilizando sockets en el lenguaje de programación C.

### Características del Proyecto:

- **Lenguaje de Programación:** C
- **Tema:** Práctica de Sockets
- **Asignatura:** Laboratorio de Programación Distribuida
- **Profesor:** Claudio Zanellato
- **Integrantes del Equipo:** 
  1. Santino Castagno
  2. Fabio Apablaza
  3. Manuel Triñanes

### Objetivos del Proyecto:

1. Practicar el diseño y la implementación de aplicaciones distribuidas en C.
2. Entender los conceptos de comunicación cliente-servidor y sus aplicaciones en entornos distribuidos.
3. Experimentar con la transferencia de datos y la gestión de conexiones utilizando sockets en C.

### Estructura del Proyecto:

- **`client.c`:** Archivo fuente que contiene la implementación del cliente.
- **`server_central.c`:** Archivo fuente que contiene la implementación del servidor central (encargado de delegar consultas).
- **`server_horoscope.c:`** Archivo fuente que contiene la implementación del servidor encargado de responder consultas del horoscopo.
- **`server_weather.c:`** Archivo fuente que contiene la implementación del servidor encargado de responder consultas del clima.
- **`dictionary.c`** y **`dictionary.h`:** Archivos que contienen la funcionalidad de la estructura de datos diccionario mediante la cuál se implementó la memoría caché del servidor centra.
- **`utils.c` y `utils.h`:** Archivos fuente que contienen funciones y utilidades compartidas por el cliente y el servidor.
- **`README.md`:** Este archivo, que proporciona una descripción general del proyecto y su estructura.

### Instrucciones de Uso:

1. Clona o descarga el repositorio en tu máquina local.
2. Compila los archivos fuente utilizando el compilador de C gcc.
```bash
    make
```
3. Ejecuta los servidores y los clientes en diferentes terminales en la misma computadora.
```bash
    ./sw & ./sh & ./sc 
```

```bash
    ./c capricornio 10-10-10
```

```bash
    ./c sagitario 10-10-10
```