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

    for (int i = 0; i < CANTIDAD_HIJOS; i++) {

        pid_t pid = fork();

        if (pid < 0) {
            perror("Error al crear proceso con fork");
            break;
        }

        if (pid == 0) {

            cout << "\nHijo " << i + 1 << endl;
            cout << "PID: " << getpid() << endl;
            cout << "PPID: " << getppid() << endl;
            cout << "Ejecutando sleep 5..." << endl;
            cout.flush();

          
            execlp("sleep", "sleep", "5", static_cast<char*>(nullptr));

         
            perror("Error al ejecutar sleep");
            _exit(127);
        }

        
        hijos[creados] = pid;
        creados++;

        cout << "Padre: hijo " << i + 1
             << " creado con PID " << pid << endl;
    }

    cout << "Padre: PID = " << getpid()
         << ", PPID = " << getppid() << endl;

    cout << "Esperando la finalizacion de "
         << creados << " hijo(s)..." << endl;

    for (int i = 0; i < creados; i++) {

        int estado = 0;
        pid_t resultado;

        
        do {
            resultado = waitpid(hijos[i], &estado, 0);
        } while (resultado == -1 && errno == EINTR);

        if (resultado == -1) {
            perror("Error en waitpid");
            continue;
        }

        if (WIFEXITED(estado)) {
            cout << "Hijo PID = " << hijos[i]
                 << " finalizo normalmente. Codigo de salida = "
                 << WEXITSTATUS(estado) << endl;
        }
        else if (WIFSIGNALED(estado)) {
            cout << "Hijo PID = " << hijos[i]
                 << " finalizo por la senal "
                 << WTERMSIG(estado) << endl;
        }
        else {
            cout << "Hijo PID = " << hijos[i]
                 << " cambio de estado." << endl;
        }
    }

    cout << "\nModulo de procesos terminado." << endl;
}
