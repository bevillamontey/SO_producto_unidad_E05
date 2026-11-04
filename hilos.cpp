#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <cstring>
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

void moduloHilos() {
    cout << "\nModulo de hilos en desarrollo..." << endl;
}
