#include <iostream>
#include <limits>

#include "procesos.h"
#include "hilos.h"
#include "memoria.h"

using namespace std;

int main() {
    int opcion = -1;

    do {
        cout << "\n========== MiniSO ==========" << endl;
        cout << "1. Gestion de procesos" << endl;
        cout << "2. Gestion de hilos" << endl;
        cout << "3. Gestion de memoria" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada no valida. Ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                moduloProcesos();
                break;
            case 2:
                moduloHilos();
                break;
            case 3:
                moduloMemoria();
                break;
            case 0:
                cout << "Saliendo de MiniSO..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }

    } while (opcion != 0);

    return 0;
}
