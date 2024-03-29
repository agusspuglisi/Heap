#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
 
#define TAM_INICIAL 11
#define REDIMENSION 2
#define REDIMENSION_ABAJO 4
 
//--------------------- STRUCTS ---------------------//
 
struct heap{
	void** datos;
	int cantidad;
	int capacidad;
	cmp_func_t cmp;
};
 
//--------------------- FUNCIONES AUXILIARES ---------------------//
 
void swap(void** arreglo, int a, int b){
	void* aux = arreglo[a];
	arreglo[a] = arreglo[b];
	arreglo[b] = aux;
}
 
int pos_padre(int pos){
	return (pos - 1) / 2;
}
 
int pos_hijo_izq(int pos){
	return (pos * 2) + 1;
}
 
int pos_hijo_der(int pos){
	return (pos * 2) + 2;
}
 
void upheap(void** arreglo, int pos, cmp_func_t cmp){
	if(pos == 0){
		return;
	}
	int padre = pos_padre(pos);
	if(cmp(arreglo[pos], arreglo[padre]) > 0){
		swap(arreglo, padre, pos);
		upheap(arreglo, padre, cmp);
	}
}
 
void downheap(void** arreglo, int capacidad, int pos, cmp_func_t cmp){
	if(pos >= capacidad){
		return;
	}
	int max = pos; // Padre
	int izq = pos_hijo_izq(pos);
	int der = pos_hijo_der(pos);
	if(izq < capacidad && cmp(arreglo[izq], arreglo[max]) > 0){
		max = izq;
	}
	if(der < capacidad && cmp(arreglo[der], arreglo[max]) > 0){
		max = der;
	}
	if(max != pos){
		swap(arreglo, pos, max);
		downheap(arreglo, capacidad, max, cmp);
	}
}
 
void** copiar_datos(void* arreglo[], int n){ 
	void** copia_arreglo = malloc((size_t)n * sizeof(void*));
	if(!copia_arreglo){
		return NULL;
	}
	for(int i = 0; i < n; i++){
		copia_arreglo[i] = arreglo[i];
	}
	return copia_arreglo;
}
 
bool redimensionar(heap_t* heap, int tam){
	void** copia = realloc(heap->datos, (size_t)tam * sizeof(void*));
	if(copia == NULL){
		return false;
	}
	heap->capacidad = tam;
	heap->datos = copia;
	return true;
}
 
//--------------------- HEAP SORT ---------------------
 
void heapify(void *arreglo[], int cantidad, cmp_func_t cmp){
	for(int i = cantidad/2; i > 0; i--){
		downheap(arreglo, cantidad, i - 1, cmp);
	}
}
 
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos, (int)cant, cmp);
	for(int i = 0; i < cant; i++){
		swap(elementos, 0, (int)cant - 1 - i);
		downheap(elementos, (int)cant - 1 - i, 0, cmp);
	}
}
 
//--------------------- PRIMITIVAS ---------------------//
 
heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap){
		return NULL;
	}
	void** datos = malloc(sizeof(void*) * TAM_INICIAL);
	if(!datos){
		return NULL;
	}
	heap->datos = datos;
	heap->cantidad = 0;
	heap->capacidad = TAM_INICIAL;
	heap->cmp = cmp;
	return heap;
}
 
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap){
		return NULL;
	}
	void** copia_datos = copiar_datos(arreglo, (int)n);
	if(!copia_datos){
		return NULL;
	}
	heapify(copia_datos, (int)n, cmp);
	heap->datos = copia_datos;
	heap->cantidad = (int)n;
	heap->capacidad = (int)n;
	heap->cmp = cmp;
	return heap;
}
 
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	int cant = heap->cantidad;
	if(destruir_elemento){
		for(int i = 0; i < cant ; i++){
			destruir_elemento(heap->datos[i]);
		}
	}
	free(heap->datos);
	free(heap);
}
 
size_t heap_cantidad(const heap_t *heap){
	return (size_t)heap->cantidad;
}
 
bool heap_esta_vacio(const heap_t *heap){
	return heap->cantidad == 0 ;
}
 
bool heap_encolar(heap_t *heap, void *elem){
	if(!elem){
		return false;
	}
	if(heap->capacidad == heap->cantidad){
		bool redimension = redimensionar(heap, heap->capacidad * REDIMENSION);
		if(!redimension){
			return false;
		}
	}
	void** datos = heap->datos;
	datos[heap->cantidad] = elem;
	upheap(datos, heap->cantidad, heap->cmp);
	heap->cantidad++;
	return true;
}
 
void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap)){
		return NULL;
	}
	return heap->datos[0];

}
 
void *heap_desencolar(heap_t *heap){
	if(heap_esta_vacio(heap)){
		return NULL;
	}
	void* dato = heap->datos[0];
	swap(heap->datos, 0, heap->cantidad - 1);
	heap->cantidad--;
	downheap(heap->datos, heap->cantidad, 0, heap->cmp);
	if(heap->cantidad < ((heap->capacidad) / REDIMENSION_ABAJO) && heap->capacidad > TAM_INICIAL){
		redimensionar(heap, heap->capacidad / REDIMENSION);
	}
	return dato;
}

//--------------------------------------------------------------//
