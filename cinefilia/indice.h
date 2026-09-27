#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include <stddef.h>
#define CANTIDAD_ELEMENTOS 100
#define INCREMENTO 1.3
#define OK 1
#define ERROR 0
#define NO_EXISTE -1

typedef struct
{
    unsigned nro_reg;
    long id;
} t_reg_indice;

typedef struct
{
    void *vindice;
    unsigned cantidad_elementos_actual;
    unsigned cantidad_elementos_maxima;
} t_indice;

/**************************************************************************
Descripción:
toma memoria para nmemb elementos e inicializa la
estructura vacía.
Parámetros:
indice: TDA índice.
nmemb: cantidad inicial de elementos del índice
(CANTIDAD_ELEMENTOS).
tamanyo: espacio en bytes ocupado por cada elemento.
Retorno:
OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: -
**************************************************************************/

int indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo);

/**************************************************************************
Descripción:
redimensiona el arreglo interno del índice.
Parámetros:
indice: TDA índice.
tamanyo: espacio en bytes ocupado por cada elemento.
Retorno:
OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: La nueva capacidad es la capacidad actual multiplicada
por INCREMENTO (30 % más). La calcula la propia función.
**************************************************************************/

int indice_redimensionar(t_indice *indice, size_t tamanyo);

/**************************************************************************
Descripción:
inserta un elemento manteniendo el orden según la clave.
Parámetros:
indice: TDA índice.
registro: elemento a insertar.
tamanyo: espacio en bytes ocupado por el elemento.
cmp: función de comparación provista.
Retorno:
OK si la operación fue exitosa y ERROR en caso contrario
(por ejemplo, clave duplicada o falta de memoria).
Observaciones: Si el arreglo está lleno, invoca indice_redimensionar.
**************************************************************************/

int indice_insertar(t_indice *indice, const void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *));

/**************************************************************************
Descripción:
elimina el elemento cuya clave coincide con la de registro.
Parámetros:
indice: TDA índice.
registro: elemento a eliminar.
tamanyo: espacio en bytes ocupado por el elemento.
cmp: función de comparación provista.
Retorno:
OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: -
**************************************************************************/

int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *));

/**************************************************************************
Descripción:
busca la clave de registro en el índice. Si existe, copia
el elemento completo en registro.
Parámetros:
indice: TDA índice.
registro: elemento a buscar (entrada/salida).
tamanyo: espacio en bytes ocupado por el elemento.
cmp: función de comparación provista.
Retorno:
NO_EXISTE si la clave no existe; en caso contrario, la
posición que ocupa dentro del arreglo.
Observaciones: La comparación entre claves se realiza exclusivamente
mediante la función cmp recibida por puntero.
**************************************************************************/

int indice_buscar(const t_indice *indice, void *registro, size_t tamanyo,
int (*cmp)(const void *, const void *));

/**************************************************************************
Descripción:
determina si el índice contiene 0 (cero) elementos.
Parámetros:
indice: TDA índice.
Retorno:
OK si está vacío, cualquier otro valor si no lo está.
Observaciones: -
**************************************************************************/

int indice_vacio(const t_indice *indice);

/**************************************************************************
Descripción:
determina si el índice alcanzó su capacidad máxima actual.
Parámetros:
indice: TDA índice.
Retorno:
OK si está lleno, cualquier otro valor si no lo está.
Observaciones: -
**************************************************************************/

int indice_lleno(const t_indice *indice);

/**************************************************************************
Descripción:
deja el índice vacío sin liberar la memoria del arreglo.
Parámetros:
indice: TDA índice.
Retorno:
No posee.
Observaciones: -
**************************************************************************/

void indice_vaciar(t_indice *indice);

/**************************************************************************
Descripción:
libera toda la memoria dinámica del índice.
Parámetros:
indice: TDA índice.
Retorno:
No posee.
Observaciones: Luego de invocarla el índice no puede usarse sin volver
a llamar a indice_crear.
**************************************************************************/

void indice_destruir(t_indice *indice);

/**************************************************************************
Descripción:
construye el índice a partir de un archivo binario de
registros. Recorre el archivo secuencialmente y, por cada
registro leído que esté activo, inserta en el índice un
t_reg_indice (nro_reg = posición del registro en el
archivo, id = clave obtenida mediante obtener_clave).
Parámetros:
path: ruta al archivo binario.
indice: TDA índice.
tamanyo_reg: tamaño en bytes de cada registro del archivo.
obtener_clave: función que, dado un registro del archivo,
devuelve su clave (long).
es_activo: función que, dado un registro del archivo,
devuelve distinto de 0 si su estado es 'A'.
cmp: función de comparación provista.
Retorno:
OK si la operación fue exitosa y ERROR en caso contrario.
Observaciones: Los registros con estado 'B' se omiten. El archivo no
necesita estar ordenado: el orden lo garantiza
indice_insertar.
**************************************************************************/

int indice_cargar(const char *path, t_indice *indice, size_t tamanyo_reg,
long (*obtener_clave)(const void *),
int (*es_activo)(const void *),
int (*cmp)(const void *, const void *));

#endif // INDICE_H_INCLUDED
