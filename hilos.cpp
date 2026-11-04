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

void moduloHilos() {
    cout << "\nModulo de hilos en desarrollo..." << endl;
}
