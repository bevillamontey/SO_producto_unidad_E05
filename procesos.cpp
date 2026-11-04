#include <iostream>
#include <cstdio>
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "procesos.h"

using namespace std;

void moduloProcesos() {

    cout << "\n--- Modulo de procesos ---" << endl;

    const int CANTIDAD_HIJOS = 2;

    pid_t hijos[CANTIDAD_HIJOS];

    int creados = 0;

    cout.flush();
    fflush(nullptr);

    // 1. CREACION DE LOS PROCESOS HIJOS

    for (int i = 0; i < CANTIDAD_HIJOS; i++) {

        pid_t pid = fork();

        // CASO 1: ERROR AL CREAR EL PROCESO

        if (pid < 0) {

            perror("Error al crear proceso con fork");

            break;

        }
        else {

            // CASO 2: ESTAMOS EN EL PROCESO HIJO

            if (pid == 0) {

                cout << "\nHijo " << i + 1 << endl;
                cout << "PID: " << getpid() << endl;
                cout << "PPID: " << getppid() << endl;
                cout << "Ejecutando sleep 5..." << endl;
                cout.flush();

                // Ejecutar un programa externo
                execlp("sleep", "sleep", "5",
                       static_cast<char*>(nullptr));
                       
                // Estas instrucciones solo se ejecutan
                // si execlp falla.

                perror("Error al ejecutar sleep");

                _exit(127);

            }
            else {

                // CASO 3: ESTAMOS EN EL PROCESO PADRE

                hijos[creados] = pid;
                creados++;
                cout << "Padre: hijo " << i + 1
                     << " creado con PID " << pid << endl;
            }
        }
    }

    // 2. INFORMACION DEL PROCESO PADRE


    cout << "Padre: PID = " << getpid()
         << ", PPID = " << getppid() << endl;

    cout << "Esperando la finalizacion de "
         << creados << " hijo(s)..." << endl;

    // 3. ESPERAR A LOS PROCESOS HIJOS

    for (int i = 0; i < creados; i++) {

        int estado = 0;
        pid_t resultado;
        // Repetir waitpid si la espera fue interrumpida

        do {
            resultado = waitpid(hijos[i], &estado, 0);
        } while (resultado == -1 && errno == EINTR);

        // CASO 1: ERROR AL ESPERAR AL HIJO

        if (resultado == -1) {
            perror("Error en waitpid");
            continue;
        }
        else {

            // CASO 2: EL HIJO TERMINO NORMALMENTE
            if (WIFEXITED(estado)) {
                cout << "Hijo PID = " << hijos[i]
                     << " finalizo normalmente. Codigo de salida = "
                     << WEXITSTATUS(estado) << endl;
            }
            else {
                // CASO 3: EL HIJO TERMINO POR UNA SENAL
                if (WIFSIGNALED(estado)) {

                    cout << "Hijo PID = " << hijos[i]
                         << " finalizo por la senal "
                         << WTERMSIG(estado) << endl;
                }
                else {
                    // OTRO CAMBIO DE ESTADO
                    cout << "Hijo PID = " << hijos[i]
                         << " cambio de estado." << endl;
                }
            }
        }
    }

    // 4. FINALIZACION DEL MODULO

    cout << "\nModulo de procesos terminado." << endl;

}
