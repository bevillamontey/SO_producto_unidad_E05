#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "memoria.h"

using namespace std;

// Estructura que representa un bloque de memoria
struct Bloque {
    int id;           // ID del proceso ocupante (-1 si está libre)
    size_t inicio;    // Dirección de inicio del bloque
    size_t tamano;    // Tamaño del bloque en bytes/unidades
    bool ocupado;     // Estado del bloque
};

class GestorMemoria {
private:
    size_t memoriaTotal;
    list<Bloque> memoria;

    // Fusiona bloques libres que están juntos (coalescencia)
    void unirBloquesLibres() {
        auto it = memoria.begin();
        while (it != memoria.end()) {
            auto siguiente = next(it);
            if (siguiente != memoria.end() && !it->ocupado && !siguiente->ocupado) {
                it->tamano += siguiente->tamano;
                memoria.erase(siguiente);
                // No avanzamos el iterador para verificar si el nuevo bloque se puede unir con el que sigue
            } else {
                ++it;
            }
        }
    }

public:
    GestorMemoria(size_t total) : memoriaTotal(total) {
        // Inicialmente, toda la memoria es un único bloque libre
        memoria.push_back({-1, 0, total, false});
    }

    // Algoritmo First-Fit para asignar memoria
    bool asignar(int idProceso, size_t tamanoSolicitado) {
        if (tamanoSolicitado == 0) return false;

        for (auto it = memoria.begin(); it != memoria.end(); ++it) {
            // First-Fit: Busca el primer bloque libre que sea lo suficientemente grande
            if (!it->ocupado && it->tamano >= tamanoSolicitado) {
                size_t tamanoRestante = it->tamano - tamanoSolicitado;

                // Modificar el bloque actual para el proceso
                it->id = idProceso;
                it->tamano = tamanoSolicitado;
                it->ocupado = true;

                // Si sobra espacio, crear un nuevo bloque libre a continuación
                if (tamanoRestante > 0) {
                    memoria.insert(next(it), {-1, it->inicio + tamanoSolicitado, tamanoRestante, false});
                }

                cout << "* Proceso " << idProceso << " asignado con éxito (" << tamanoSolicitado << " unidades).\n";
                return true;
            }
        }

        // Si no se pudo asignar, verificar si hay fragmentación externa
        identificarFragmentacionExterna(tamanoSolicitado);
        return false;
    }

    // Liberar un bloque de memoria por ID de proceso
    bool liberar(int idProceso) {
        bool encontrado = false;
        for (auto& bloque : memoria) {
            if (bloque.ocupado && bloque.id == idProceso) {
                bloque.ocupado = false;
                bloque.id = -1;
                encontrado = true;
                cout << "* Memoria del proceso " << idProceso << " liberada.\n";
            }
        }

        if (encontrado) {
            unirBloquesLibres(); // Une bloques libres contiguos inmediatamente
        } else {
            cout << "* No se encontró ningún proceso activo con ID " << idProceso << ".\n";
        }
        return encontrado;
    }

    // Identifica si el rechazo se debe a fragmentación externa
    void identificarFragmentacionExterna(size_t tamanoSolicitado) {
        size_t totalLibre = 0;
        size_t bloqueLibreMayor = 0;

        for (const auto& bloque : memoria) {
            if (!bloque.ocupado) {
                totalLibre += bloque.tamano;
                if (bloque.tamano > bloqueLibreMayor) {
                    bloqueLibreMayor = bloque.tamano;
                }
            }
        }

        cout << "* Error: No se pudo asignar " << tamanoSolicitado << " unidades.\n";
        if (totalLibre >= tamanoSolicitado && bloqueLibreMayor < tamanoSolicitado) {
            cout << "* [ALERTA] Fragmentación Externa Detectada: Hay " << totalLibre 
                      << " unidades libres en total, pero el bloque contiguo más grande es de solo " 
                      << bloqueLibreMayor << " unidades.\n";
        } else {
            cerr << "* Motivo: Memoria insuficiente (Memoria total libre disponible: " << totalLibre << " unidades).\n";
        }
    }

    // Muestra el mapa actual de la memoria
    void mostrarEstado() const {
        cout << "\n================ ESTADO DE LA MEMORIA ================\n";
        cout << "Inicio\tFin\tTamaño\tEstado\t\tDetalle\n";
        cout << "------------------------------------------------------\n";
        for (const auto& bloque : memoria) {
            size_t fin = bloque.inicio + bloque.tamano - 1;
            string estadoStr = bloque.ocupado ? "OCUPADO" : "LIBRE";
            string detalleStr = bloque.ocupado ? "Proceso " + std::to_string(bloque.id) : "-";
            
            cout << bloque.inicio << "\t" << fin << "\t" << bloque.tamano << "\t" 
                      << estadoStr << "\t\t" << detalleStr << "\n";
        }
        cout << "======================================================\n\n";
    }
};

void moduloMemoria() {
    size_t memoriaInicial;
    cout << "Defina la cantidad total de memoria: ";
    cin >> memoriaInicial;

    GestorMemoria gestor(memoriaInicial);
    int opcion;

    do {
        cout << "\n===== MENÚ DE GESTIÓN DE MEMORIA =====\n";
        cout << "1. Particionamiento de Memoria" << endl;
                  << "2. Paginación de Memoria" << endl;
                  << "3. Salir" << endl;
                  << "Seleccione una opción: ";
        cin >> opcion;
        if (opcion == 1) {
            int subOpcion;
            do {
                cout << "\n===== PARTICIONAMIENTO DE MEMORIA =====\n";
                cout << "1. Solicitar Memoria (Asignar)\n"
                  << "2. Liberar Memoria\n"
                  << "3. Mostrar Estado de Memoria\n"
                  << "4. Salir\n"
                  << "Seleccione una opción: ";
                cin >> subOpcion;

                if (subOpcion == 1) {
                    int id;
                    size_t tamano;
                    cout << "ID del Proceso: ";
                    cin >> id;
                    cout << "Tamaño requerido: ";
                    cin >> tamano;
                    gestor.asignar(id, tamano);
                }
                else if (subOpcion == 2) {
                    int id;
                    cout << "ID del Proceso a liberar: ";
                    cin >> id;
                    gestor.liberar(id);
                } 
                else if (subOpcion == 3) {
                    gestor.mostrarEstado();
                }
            } while (subOpcion != 4);
        }
        else if (opcion == 2) {
            system("python pag_memoria.py");
        }
    } while (opcion != 3);
    return 0;
}