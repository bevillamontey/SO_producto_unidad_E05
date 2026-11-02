#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "hilos.h"

using namespace std;

// Configuración y variables compartidas
const int TAMANO_BUFFER = 5;
const int ELEMENTOS_POR_PRODUCTOR = 5;
const int NUM_PRODUCTORES = 2;
const int NUM_CONSUMIDORES = 2;

queue<int> buffer;
pthread_mutex_t mutexBuffer;
pthread_cond_t condBufferLleno;  // Señala a consumidores cuando hay items
pthread_cond_t condBufferVacio;   // Señala a productores cuando hay espacio

int contadorItems = 1;

// Estructura para pasar parámetros a los hilos
struct DatosHilo {
    int id;
};

// --- Función para hilos productores ---
void* productor(void* arg) {
    DatosHilo* datos = static_cast<DatosHilo*>(arg);
    int id = datos->id;

    for (int i = 0; i < ELEMENTOS_POR_PRODUCTOR; ++i) {
        pthread_mutex_lock(&mutexBuffer);

        // Si el búfer está lleno, esperar
        while (buffer.size() == TAMANO_BUFFER) {
            cout << "[Productor " << id << "] Bufer LLENO. Esperando espacio..." << endl;
            pthread_cond_wait(&condBufferVacio, &mutexBuffer);
        }

        // Producir elemento
        int item = contadorItems++;
        buffer.push(item);
        cout << "[Productor " << id << "] Produjo elemento: " << item 
             << " | Elementos en bufer: " << buffer.size() << "/" << TAMANO_BUFFER << endl;

        // Notificar a los consumidores que hay al menos un elemento disponible
        pthread_cond_signal(&condBufferLleno);
        pthread_mutex_unlock(&mutexBuffer);

        // Simular tiempo de trabajo del productor
        usleep(150000);
    }

    cout << "--> [Productor " << id << "] Finalizo su produccion." << endl;
    pthread_exit(nullptr);
}

// --- Función para hilos consumidores ---
void* consumidor(void* arg) {
    DatosHilo* datos = static_cast<DatosHilo*>(arg);
    int id = datos->id;

    int totalEsperados = (NUM_PRODUCTORES * ELEMENTOS_POR_PRODUCTOR) / NUM_CONSUMIDORES;

    for (int i = 0; i < totalEsperados; ++i) {
        pthread_mutex_lock(&mutexBuffer);

        // Si el búfer está vacío, esperar
        while (buffer.empty()) {
            cout << "[Consumidor " << id << "] Bufer VACIO. Esperando elementos..." << endl;
            pthread_cond_wait(&condBufferLleno, &mutexBuffer);
        }

        // Consumir elemento
        int item = buffer.front();
        buffer.pop();
        cout << "  [Consumidor " << id << "] Consumio elemento: " << item 
             << " | Elementos en bufer: " << buffer.size() << "/" << TAMANO_BUFFER << endl;

        // Notificar a los productores que hay espacio libre
        pthread_cond_signal(&condBufferVacio);
        pthread_mutex_unlock(&mutexBuffer);

        // Simular tiempo de procesamiento del consumidor
        usleep(250000);
    }

    cout << "--> [Consumidor " << id << "] Finalizo su consumo." << endl;
    pthread_exit(nullptr);
}

// --- Función principal del módulo ---
void moduloHilos() {
    cout << "\n==============================================" << endl;
    cout << "   MODULO II: GESTION DE HILOS Y CONCURRENCIA" << endl;
    cout << "==============================================" << endl;
    cout << "Configuracion:" << endl;
    cout << "- Productores: " << NUM_PRODUCTORES << endl;
    cout << "- Consumidores: " << NUM_CONSUMIDORES << endl;
    cout << "- Capacidad del Bufer: " << TAMANO_BUFFER << endl;
    cout << "- Items por productor: " << ELEMENTOS_POR_PRODUCTOR << endl;
    cout << "----------------------------------------------\n" << endl;

    // Resetear contador de items y asegurar búfer limpio
    contadorItems = 1;
    while (!buffer.empty()) buffer.pop();

    // Inicializar Mutex y Variables de Condición
    pthread_mutex_init(&mutexBuffer, nullptr);
    pthread_cond_init(&condBufferLleno, nullptr);
    pthread_cond_init(&condBufferVacio, nullptr);

    pthread_t productores[NUM_PRODUCTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];

    DatosHilo datosProd[NUM_PRODUCTORES];
    DatosHilo datosCons[NUM_CONSUMIDORES];

    // Crear hilos productores
    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        datosProd[i].id = i + 1;
        if (pthread_create(&productores[i], nullptr, productor, &datosProd[i]) != 0) {
            perror("Error al crear hilo productor");
        }
    }

    // Crear hilos consumidores
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        datosCons[i].id = i + 1;
        if (pthread_create(&consumidores[i], nullptr, consumidor, &datosCons[i]) != 0) {
            perror("Error al crear hilo consumidor");
        }
    }

    // Esperar a que terminen todos los productores
    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        pthread_join(productores[i], nullptr);
    }

    // Esperar a que terminen todos los consumidores
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        pthread_join(consumidores[i], nullptr);
    }

    // Destruir recursos de sincronización
    pthread_mutex_destroy(&mutexBuffer);
    pthread_cond_destroy(&condBufferLleno);
    pthread_cond_destroy(&condBufferVacio);

    cout << "\n[OK] Simulación finalizada correctamente. Todos los elementos fueron procesados sin pérdidas ni duplicaciones." << endl;
}
