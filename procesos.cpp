#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "procesos.h"

using namespace std;

void moduloProcesos() {
    cout << "\n--- Gestion de procesos ---" << endl;

    // Primera version: crea un proceso hijo y ejecuta un programa externo.
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo con fork");
        return;
    }

    if (pid == 0) {
        // Este bloque se ejecuta solamente en el proceso hijo.
        cout << "Proceso hijo iniciado." << endl;
        cout << "PID: " << getpid() << endl;
        cout << "PPID: " << getppid() << endl;

        // execlp reemplaza el programa del hijo por el comando externo sleep.
        // Si execlp funciona, no vuelve a esta linea.
        execlp("sleep", "sleep", "2", static_cast<char*>(nullptr));

        // Solo se llega aqui si execlp falla.
        perror("Error al ejecutar sleep con execlp");
        _exit(127);
    }

    // Este bloque se ejecuta en el proceso padre.
    cout << "Proceso padre. PID: " << getpid() << endl;
    cout << "PID del hijo creado: " << pid << endl;

    int estado = 0;
    pid_t resultado = waitpid(pid, &estado, 0);

    if (resultado == -1) {
        perror("Error al esperar al proceso hijo con waitpid");
        return;
    }

    if (WIFEXITED(estado)) {
        cout << "El proceso hijo finalizo. Codigo de salida: "
             << WEXITSTATUS(estado) << endl;
    } else if (WIFSIGNALED(estado)) {
        cout << "El proceso hijo termino por una senal: "
             << WTERMSIG(estado) << endl;
    }
}
