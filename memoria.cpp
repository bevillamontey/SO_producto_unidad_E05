#include <iostream>
#include "memoria.h"

using namespace std;

void firstFit(int MblockSize[], int m, int MprocessSize[], int n)
{
  // Arreglo dinámico para guardar el bloque asignado a cada proceso (-1 si no se asigna)
  int* allocation = new int[n];
  int blockSize[m];
  int processSize[n];
  
  int a;
  for (a = 0; a < m; ++a){
      blockSize[a] = MblockSize[a];
  }
  for (a = 0; a < n; ++a){
      processSize[a] = MprocessSize[a];
  }
  
  for (int i = 0; i < n; ++i)
    allocation[i] = -1;
    
  // Algoritmo First-Fit
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if(blockSize[j] >= processSize[i]) {
        allocation[i] = j;              // asigna bloque j al proceso i
        blockSize[j] -= processSize[i]; // reduce espacio libre del bloque
        break;                          // pasa al siguiente proceso
      }
    }
  }
  
  // Salida
  cout << "\nNo. Proceso\tTamano Proceso\tNo. Bloque\n";
  for (int i = 0; i < n; ++i) {
    cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
    if (allocation[i] != -1)
      cout << (allocation[i] + 1);
    else
      cout << "No Asignado";
    cout << "\n";
  }
}

void bestFit(int MblockSize[], int m, int MprocessSize[], int n)
{
  // Arreglo dinámico para guardar el bloque asignado a cada proceso (-1 si no se asigna)
  int* allocation = new int[n];
  int blockSize[m];
  int processSize[n];
  
  int a;
  for (a = 0; a < m; ++a){
      blockSize[a] = MblockSize[a];
  }
  for (a = 0; a < n; ++a){
      processSize[a] = MprocessSize[a];
  }
  
  int blockOpens[m];
  int blockMin;
  int k = 0;
  
  for (int i = 0; i < n; ++i)
    allocation[i] = -1;
    
  // Algoritmo First-Fit
  for (int i = 0; i < n; ++i) {
    k = 0;
    for (int j = 0; j < m; ++j) {
      if(blockSize[j] >= processSize[i]) {
        blockOpens[k] = blockSize[j];   // Busca los bloques compatibles para el proceso
        k = k + 1;
      }
    }
    blockMin = blockOpens[0];
    for (int l = 1; l < k; ++l){
      if (blockMin >= blockOpens[l]){
        blockMin = blockOpens[l];       // Verifica cual bloque compatible es el mas corto
      }
    }
    for (int o = 0; o < m; ++o){
      if (blockMin == blockSize[o]){
        allocation[i] = o;              // Asigna bloque o al proceso i
        blockSize[o] -= processSize[i]; // Reduce el bloque o
        break;
      }
    }                      
  }
  
  // Salida
  cout << "\nNo. Proceso\tTamano Proceso\tNo. Bloque\n";
  for (int i = 0; i < n; ++i) {
    cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
    if (allocation[i] != -1)
      cout << (allocation[i] + 1);
    else
      cout << "No Asignado";
    cout << "\n";
  }
}

void nextFit(int MblockSize[], int m, int MprocessSize[], int n)
{
  // Arreglo dinámico para guardar el bloque asignado a cada proceso (-1 si no se asigna)
  int* allocation = new int[n];
  int blockSize[m];
  int processSize[n];
  
  int a;
  for (a = 0; a < m; ++a){
      blockSize[a] = MblockSize[a];
  }
  for (a = 0; a < n; ++a){
      processSize[a] = MprocessSize[a];
  }
  
  int punt, j, l;
  
  for (int i = 0; i < n; ++i)
    allocation[i] = -1;
    
  // Algoritmo Next-Fit
  for (int i = 0; i < n; ++i) {
    j = punt;
    l = 0;
    while (l != 2){
      if(j > m){                        // cuando j pasa de los limites del arreglo, vuele a 0
        j = 0;
        l += 1;                         // nuevo valor l para no repetir indefinidamente si no hay memoria
      }
      if(blockSize[j] >= processSize[i]) {
        allocation[i] = j;              // asigna bloque j al proceso i
        blockSize[j] -= processSize[i]; // reduce espacio libre del bloque
        punt = j;                       // asigna el puntero posterior de la ejecución
        break;                          // pasa al siguiente proceso
      }
      j = j + 1;
    }
  }
  
  // Salida
  cout << "\nNo. Proceso\tTamano Proceso\tNo. Bloque\n";
  for (int i = 0; i < n; ++i) {
    cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
    if (allocation[i] != -1)
      cout << (allocation[i] + 1);
    else
      cout << "No Asignado";
    cout << "\n";
  }
}

int main() {
    int blockSize[]   = {100, 500, 200, 300, 600};
    int processSize[] = {212, 417, 112, 301};
 
    int m = sizeof(blockSize)   / sizeof(blockSize[0]);
    int n = sizeof(processSize) / sizeof(processSize[0]);
  
    int opcion = -1;
    int j;
    do {
        cout << "\n========== Gestion de memoria ==========" << endl;
        cout << "\n--Bloques disponibles--"<< endl;
        for (j = 0; j < m; j++){
            cout << "\nBloque "<<j+1<<" : "<<blockSize[j]<<" ";
        }
        cout << endl;
        cout << "\n-------Procesos--------"<<endl;
        for (j = 0; j < n; j++){
            cout << "\nProceso "<<j+1<<" : "<<processSize[j]<<" ";
        }
        cout << endl;
        cout << "\n========================================" << endl;
        cout << "1. First-Fit" << endl;
        cout << "2. Best-Fit" << endl;
        cout << "3. Next-Fit" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                firstFit(blockSize, m, processSize, n);
                break;
            case 2:
                bestFit(blockSize, m, processSize, n);
                break;
            case 3:
                nextFit(blockSize, m, processSize, n);
                break;
            case 0:
                cout << "Saliendo de la gestion de memoria..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }

    } while (opcion != 0);

    return 0;
}