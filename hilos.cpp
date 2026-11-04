#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "hilos.h"

using namespace std;

const int TAMANO_BUFFER = 5;
const int ELEMENTOS_POR_PRODUCTOR = 5;
const int NUM_PRODUCTORES = 2;
const int NUM_CONSUMIDORES = 2;
const int TOTAL_ITEMS = NUM_PRODUCTORES * ELEMENTOS_POR_PRODUCTOR;

queue<int> buffer;
pthread_mutex_t mutexBuffer;
pthread_cond_t condBufferLleno;
pthread_cond_t condBufferVacio;

int contadorItems = 1;
int itemsProducidos = 0;

struct DatosHilo {
    int id;
};

void* productor(void* arg) {
    DatosHilo* datos = static_cast<DatosHilo*>(arg);
    int id = datos->id;

    for (int i = 0; i < ELEMENTOS_POR_PRODUCTOR; ++i) {
        pthread_mutex_lock(&mutexBuffer);

        while (buffer.size() == TAMANO_BUFFER) {
            cout << "[Productor " << id << "] Bufer LLENO. Esperando espacio libre..." << endl;
            pthread_cond_wait(&condBufferVacio, &mutexBuffer);
        }

        int item = contadorItems++;
        buffer.push(item);
        itemsProducidos++;

        cout << "[Productor " << id << "] Produjo elemento: " << item 
             << " | Ocupacion del bufer: " << buffer.size() << "/" << TAMANO_BUFFER << endl;

        pthread_cond_signal(&condBufferLleno);
        pthread_mutex_unlock(&mutexBuffer);

        usleep(150000);
    }

    cout << "--> [Productor " << id << "] Finalizo su produccion." << endl;
    return nullptr;
}

void* consumidor(void* arg) {
    DatosHilo* datos = static_cast<DatosHilo*>(arg);
    int id = datos->id;

    while (true) { 
        pthread_mutex_lock(&mutexBuffer);

        while (buffer.empty() && itemsProducidos < TOTAL_ITEMS) {
            cout << "  [Consumidor " << id << "] Bufer VACIO. Esperando elementos..." << endl;
            pthread_cond_wait(&condBufferLleno, &mutexBuffer);
        }

        if (buffer.empty() && itemsProducidos == TOTAL_ITEMS) {
            pthread_mutex_unlock(&mutexBuffer);
            break;
        }

        int item = buffer.front();
        buffer.pop();
        cout << "  [Consumidor " << id << "] Consumio elemento: " << item 
             << " | Ocupacion del bufer: " << buffer.size() << "/" << TAMANO_BUFFER << endl;

        pthread_cond_signal(&condBufferVacio);
        pthread_mutex_unlock(&mutexBuffer);

        usleep(250000);
    }

    cout << "--> [Consumidor " << id << "] Finalizo su consumo." << endl;
    return nullptr;
}

// Parámetros de la simulación
const int TAMANO_BUFFER = 5;
const int ELEMENTOS_POR_PRODUCTOR = 5;
const int NUM_PRODUCTORES = 2;
const int NUM_CONSUMIDORES = 2;

// Recursos compartidos y sincronización
queue<int> buffer;
pthread_mutex_t mutexBuffer;
pthread_cond_t condBufferLleno;  // Avisa cuando se inserta un elemento
pthread_cond_t condBufferVacio;   // Avisa cuando se libera un espacio

int contadorItems = 1;

// Estructura para identificar los hilos
struct DatosHilo {
    int id;
};

// Función ejecutada por los hilos productores
void* productor(void* arg) {
    DatosHilo* datos = static_cast<DatosHilo*>(arg);
    int id = datos->id;

    for (int i = 0; i < ELEMENTOS_POR_PRODUCTOR; ++i) {
        pthread_mutex_lock(&mutexBuffer);

        // Esperar mientras el búfer esté lleno
        while (buffer.size() == TAMANO_BUFFER) {
            cout << "[Productor " << id << "] Bufer LLENO. Esperando espacio libre..." << endl;
            pthread_cond_wait(&condBufferVacio, &mutexBuffer);
        }

        // Producir e insertar elemento
        int item = contadorItems++;
        buffer.push(item);
        cout << "[Productor " << id << "] Produjo elemento: " << item 
             << " | Ocupacion del bufer: " << buffer.size() << "/" << TAMANO_BUFFER << endl;

        // Notificar que hay elementos disponibles
        pthread_cond_signal(&condBufferLleno);
        pthread_mutex_unlock(&mutexBuffer);

        usleep(150000); // Retardo simulado (150 ms)
    }

    cout << "--> [Productor " << id << "] Finalizo su produccion." << endl;
    pthread_exit(nullptr);
}

// Función ejecutada por los hilos consumidores
void* consumidor(void* arg) {
    DatosHilo* datos = static_cast<DatosHilo*>(arg);
    int id = datos->id;

    int totalConsumir = (NUM_PRODUCTORES * ELEMENTOS_POR_PRODUCTOR) / NUM_CONSUMIDORES;

    for (int i = 0; i < totalConsumir; ++i) {
        pthread_mutex_lock(&mutexBuffer);

        // Esperar mientras el búfer esté vacío
        while (buffer.empty()) {
            cout << "  [Consumidor " << id << "] Bufer VACIO. Esperando elementos..." << endl;
            pthread_cond_wait(&condBufferLleno, &mutexBuffer);
        }

        // Consumir elemento
        int item = buffer.front();
        buffer.pop();
        cout << "  [Consumidor " << id << "] Consumio elemento: " << item 
             << " | Ocupacion del bufer: " << buffer.size() << "/" << TAMANO_BUFFER << endl;

        // Notificar que hay espacio disponible
        pthread_cond_signal(&condBufferVacio);
        pthread_mutex_unlock(&mutexBuffer);

        usleep(250000); // Retardo simulado (250 ms)
    }

    cout << "--> [Consumidor " << id << "] Finalizo su consumo." << endl;
    pthread_exit(nullptr);
}

void moduloHilos() {
    cout << "\n==============================================" << endl;
    cout << "   MODULO II: GESTION DE HILOS Y CONCURRENCIA" << endl;
    cout << "==============================================" << endl;
    cout << "Parametros iniciales:" << endl;
    cout << "- Productores: " << NUM_PRODUCTORES << " | Consumidores: " << NUM_CONSUMIDORES << endl;
    cout << "- Capacidad maxima del bufer: " << TAMANO_BUFFER << endl;
    cout << "- Items a producir por hilo: " << ELEMENTOS_POR_PRODUCTOR << endl;
    cout << "----------------------------------------------\n" << endl;

    // Reiniciar variables globales
    contadorItems = 1;
    itemsProducidos = 0;
    while (!buffer.empty()) buffer.pop();

    // Inicialización de mutex y variables de condición

    pthread_mutex_init(&mutexBuffer, nullptr);
    pthread_cond_init(&condBufferLleno, nullptr);
    pthread_cond_init(&condBufferVacio, nullptr);

    pthread_t productores[NUM_PRODUCTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];
    DatosHilo datosProd[NUM_PRODUCTORES];
    DatosHilo datosCons[NUM_CONSUMIDORES];

    // Creación de hilos productores
    int resultado;

    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        datosProd[i].id = i + 1;
        resultado = pthread_create(&productores[i], nullptr, productor, &datosProd[i]);
        if (resultado != 0) {
            cerr << "Error al crear el hilo productor: " << strerror(resultado) << endl;
        }
    }

    // Creación de hilos consumidores
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        datosCons[i].id = i + 1;
        resultado = pthread_create(&consumidores[i], nullptr, consumidor, &datosCons[i]);
        if (resultado != 0) {
            cerr << "Error al crear el hilo consumidor: " << strerror(resultado) << endl;
        }
    }

    // Espera de finalización

    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        pthread_join(productores[i], nullptr);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        pthread_join(consumidores[i], nullptr);
    }

    // Liberación de recursos

    pthread_mutex_destroy(&mutexBuffer);
    pthread_cond_destroy(&condBufferLleno);
    pthread_cond_destroy(&condBufferVacio);

    cout << "\n[OK] Simulación de hilos finalizada exitosamente." << endl;

}
