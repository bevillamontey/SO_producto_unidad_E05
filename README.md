# MiniSO - Simulador de Sistema Operativo

1. Descripcion del proyecto

MiniSO es una aplicacion de consola desarrollada
en C++ que permite implementar y observar
operaciones relacionadas con la gestion de
procesos, concurrencia y administracion de memoria.

2. Requisitos

- Sistema operativo Linux.
- Compilador G++.
- Soporte para C++17.
- Biblioteca de hilos POSIX mediante pthread.

3. Compilacion

Ubicarse en la carpeta del proyecto y ejecutar:

make

Este comando compila automaticamente todos los archivos
del proyecto y genera el ejecutable miniso.

4. Ejecucion

Para iniciar la aplicacion:

./miniso

Limpiar archivos generados

Para eliminar el ejecutable generado:

make clean

5. Estructura del proyecto

MiniSO/
│
├── .gitignore
├── Makefile
│
├── main.cpp
│
├── procesos.cpp
├── procesos.h
│
├── hilos.cpp
├── hilos.h
│
├── memoria.cpp
├── memoria.h
│
├── pag_memoria.py
│
└── README.md

6. Funcionalidades

Modulo I: Gestion de procesos
- Creacion de procesos hijos mediante fork().
- Ejecucion de programas externos mediante exec().
- Identificacion de PID y PPID.
- Espera y finalizacion mediante waitpid().

Modulo II: Hilos y concurrencia
- Creacion de multiples hilos.
- Implementacion del problema productor-consumidor.
- Uso de mutex y variables de condicion.
- Control del buffer compartido.

Modulo III: Gestion de memoria
- Particionamiento mediante First-Fit.
- Asignacion y liberacion de bloques.
- Identificacion de fragmentacion externa.
- Simulacion de paginacion.
- Tabla de paginas y traduccion de direcciones.

7. Integrantes

Betto Estiwar Villamonte Yunga 	2020-119010
Mauricio José Flores Gómez		2018-119061
Pedro Czar Paolo Serrano Yanapa	2024-119023
