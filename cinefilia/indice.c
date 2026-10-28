#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indice.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    toma memoria para nmemb elementos e inicializa la estructura
                vacia.
Parametros:     indice: TDA indice.
                nmemb: cantidad inicial de elementos (CANTIDAD_ELEMENTOS).
                tamanyo: espacio en bytes ocupado por cada elemento.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int indice_crear_ALU(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->vindice = malloc(nmemb * tamanyo);
    if(indice->vindice == NULL)
    {
        return ERROR;
    }
    indice_vaciar_ALU(indice);
    indice->cantidad_elementos_maxima = nmemb;

    //return indice_crear(indice, nmemb, tamanyo);
    return OK;
}

/******************************************************************************
Descripcion:    redimensiona el arreglo interno del indice con realloc. La
                nueva capacidad es la actual multiplicada por INCREMENTO (30%
                mas); la calcula la propia funcion.
Parametros:     indice: TDA indice.
                tamanyo: espacio en bytes ocupado por cada elemento.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario.
Observaciones:  Si el producto no aumenta la capacidad (arreglos muy chicos)
                se garantiza al menos un elemento mas.
******************************************************************************/
int indice_redimensionar_ALU(t_indice *indice, size_t tamanyo)
{
    void *p_aux;
    unsigned nueva_cant_elementos;

    nueva_cant_elementos = (unsigned)(indice->cantidad_elementos_maxima * INCREMENTO);

    if (nueva_cant_elementos == indice->cantidad_elementos_maxima)
    {
        nueva_cant_elementos++;
    }
    p_aux = realloc(indice->vindice, nueva_cant_elementos * tamanyo);
    if (p_aux == NULL)
    {
        return ERROR;
    }

    indice->vindice = p_aux;
    indice->cantidad_elementos_maxima = nueva_cant_elementos;

    //return indice_redimensionar(indice, tamanyo);
    return OK;
}

/******************************************************************************
Descripcion:    inserta un elemento manteniendo el orden segun la clave. Si el
                arreglo esta lleno invoca indice_redimensionar.
Parametros:     indice: TDA indice.
                registro: elemento a insertar.
                tamanyo: espacio en bytes ocupado por el elemento.
                cmp: funcion de comparacion provista.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario
                (clave duplicada o falta de memoria).
Observaciones:  -
******************************************************************************/
int indice_insertar_ALU(t_indice *indice, const void *registro, size_t tamanyo,
                        int (*cmp)(const void *, const void *))
{
    char *p_inicio = (char *)indice->vindice;
    char *p_actual;

    //Verifico si el vector esta lleno
    if(indice_lleno_ALU(indice) == OK)
    {
        //Ampliar el vector
        if(indice_redimensionar_ALU(indice,tamanyo) == ERROR)
        {
            puts("Error en la solicitud de memoria.\n");
            return ERROR;
        }
        // Actualizamos p_inicio por si realloc movió la memoria a otra dirección
        p_inicio = (char *)indice->vindice;
    }

    // Si el vector está vacío, inserta al principio y sale
    if (indice->cantidad_elementos_actual == 0)
    {
        memcpy(p_inicio, registro, tamanyo);
        indice->cantidad_elementos_actual++;
        return OK;
    }

    //Apunto a al ultimo elemento del vector
    p_actual = (char*)indice->vindice + ((indice->cantidad_elementos_actual - 1) * tamanyo);

    // Buscar y desplazar de derecha a izquierda (solo si hay elementos cargados)
    while(p_actual >= p_inicio && cmp(p_actual, registro) > 0)
    {
        //copio en la siguiente posicion el valor
        memcpy(p_actual + tamanyo, p_actual, tamanyo);
        //Resto una posicion
        p_actual -= tamanyo;
    }
    //Verifico si no salio por duplicados
    if(p_actual >= p_inicio && cmp(p_actual, registro) == 0)
    {
        puts("Error por duplicados\n");
        return ERROR;
    }

    // El hueco para insertar quedó en (p_actual + tamanyo) o en p_inicio si p_actual quedó antes del inicio
    char *p_hueco = (p_actual < p_inicio) ? p_inicio : p_actual + tamanyo;

    //Copiamos el registro nuevo en el espacio libre
    memcpy(p_hueco, registro, tamanyo);
    indice->cantidad_elementos_actual++;

    //return indice_insertar(indice, registro, tamanyo, cmp);
    return OK;
}

/******************************************************************************
Descripcion:    elimina el elemento cuya clave coincide con la de registro,
                desplazando los siguientes.
Parametros:     indice: TDA indice.
                registro: elemento a eliminar (solo importa su clave).
                tamanyo: espacio en bytes ocupado por el elemento.
                cmp: funcion de comparacion provista.
Retorno:        OK si la operacion fue exitosa y ERROR si la clave no existe.
Observaciones:  -
******************************************************************************/
int indice_eliminar_ALU(t_indice *indice, const void *registro, size_t tamanyo,
                        int (*cmp)(const void *, const void *))
{
    char *p_inicio = (char *)indice->vindice;
    char *p_limite = p_inicio + (indice->cantidad_elementos_actual * tamanyo); // Puntero al final del arreglo
    char *p_actual;
    char *p_siguiente;
    int pos;

    //Valido que el vector no este vacio
    if (indice_vacio_ALU(indice) == OK)
    {
        return ERROR;
    }

    pos = indice_buscar_ALU(indice, (void *)registro, tamanyo, cmp);
    if ( pos == NO_EXISTE)
    {
        return ERROR;
    }
    p_actual = (p_inicio + (pos * tamanyo));
    p_siguiente = p_actual + tamanyo;
    while (p_siguiente < p_limite)
    {
        memcpy(p_actual, p_siguiente, tamanyo);
        p_actual += tamanyo;
        p_siguiente += tamanyo;
    }

    indice->cantidad_elementos_actual--;

    return OK;
}

/******************************************************************************
Descripcion:    busca la clave de registro en el indice. Si existe copia el
                elemento completo (nro_reg incluido) en registro.
Parametros:     indice: TDA indice.
                registro: elemento a buscar (entrada/salida).
                tamanyo: espacio en bytes ocupado por el elemento.
                cmp: funcion de comparacion provista.
Retorno:        NO_EXISTE si la clave no existe; en caso contrario la posicion
                que ocupa dentro del arreglo.
Observaciones:  La comparacion entre claves se realiza exclusivamente mediante
                cmp.
******************************************************************************/
int indice_buscar_ALU(const t_indice *indice, void *registro, size_t tamanyo,
                      int (*cmp)(const void *, const void *))
{
    char *p_inicio = (char *)indice->vindice;
    char *p_actual = p_inicio;
    char *p_limite = p_inicio + (indice->cantidad_elementos_actual * tamanyo); // Puntero al final del arreglo
    int res_cmp;

    // Busqueda del elemento
    while (p_actual < p_limite && (res_cmp = cmp(p_actual, registro)) < 0)
    {
        p_actual += tamanyo;
    }

    // Verifico si la clave no existe o me sali del limite
    if (p_actual >= p_limite || res_cmp != 0)
    {
        return NO_EXISTE;
    }

    memcpy(registro,p_actual, tamanyo);
    return ((p_actual - p_inicio)/tamanyo);
    //return indice_buscar(indice, registro, tamanyo, cmp);
}

/******************************************************************************
Descripcion:    determina si el indice contiene 0 (cero) elementos.
Parametros:     indice: TDA indice.
Retorno:        OK si esta vacio, ERROR si no lo esta.
Observaciones:  -
******************************************************************************/
int indice_vacio_ALU(const t_indice *indice)
{
    return (indice->cantidad_elementos_actual == 0) ? OK : ERROR;
    //return indice_vacio(indice);
}

/******************************************************************************
Descripcion:    determina si el indice alcanzo su capacidad maxima actual.
Parametros:     indice: TDA indice.
Retorno:        OK si esta lleno, ERROR si no lo esta.
Observaciones:  -
******************************************************************************/
int indice_lleno_ALU(const t_indice *indice)
{

    return (indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima) ? OK : ERROR;
    //return indice_lleno(indice);
}

/******************************************************************************
Descripcion:    deja el indice vacio sin liberar la memoria del arreglo.
Parametros:     indice: TDA indice.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void indice_vaciar_ALU(t_indice *indice)
{
    indice->cantidad_elementos_actual = 0;
    //indice_vaciar(indice);
}

/******************************************************************************
Descripcion:    libera toda la memoria dinamica del indice.
Parametros:     indice: TDA indice.
Retorno:        No posee.
Observaciones:  Luego de invocarla el indice no puede usarse sin volver a
                llamar a indice_crear.
******************************************************************************/
void indice_destruir_ALU(t_indice *indice)
{
    if (indice != NULL )
    {
        free(indice->vindice);
        indice->vindice = NULL;
        indice_vaciar_ALU(indice);
        indice->cantidad_elementos_maxima = 0;
    }
    //indice_destruir(indice);
}

/******************************************************************************
Descripcion:    construye el indice a partir de un archivo binario de
                registros. Recorre el archivo secuencialmente y por cada
                registro activo inserta un t_reg_indice (nro_reg = posicion en
                el archivo, id = clave obtenida con obtener_clave).
Parametros:     path: ruta al archivo binario.
                indice: TDA indice (ya creado; se vacia antes de cargar).
                tamanyo_reg: tamanyo en bytes de cada registro del archivo.
                obtener_clave: funcion que devuelve la clave (long) de un
                registro.
                es_activo: funcion que devuelve distinto de 0 si el registro
                tiene estado 'A'.
                cmp: funcion de comparacion provista.
Retorno:        OK si la operacion fue exitosa y ERROR en caso contrario.
Observaciones:  Los registros con estado 'B' se omiten. El archivo no necesita
                estar ordenado: el orden lo garantiza indice_insertar.
******************************************************************************/
int indice_cargar_ALU(const char *path, t_indice *indice, size_t tamanyo_reg,
                      long (*obtener_clave)(const void *),
                      int (*es_activo)(const void *),
                      int (*cmp)(const void *, const void *))
{
    t_reg_indice reg_indice;
    unsigned pos = 0;
    void *buffer;

    FILE *pf = fopen(path, "rb");
    if (!pf)
        return ERROR;

    indice_vaciar_ALU(indice);

    buffer = malloc(tamanyo_reg);
    if (!buffer)
    {
        fclose(pf);
        puts("Fallo en memoria.\n");
        return ERROR;
    }

    fread(buffer, tamanyo_reg, 1, pf);
    while (!feof(pf))
    {
        if (es_activo(buffer))
        {
            reg_indice.id = obtener_clave(buffer);
            reg_indice.nro_reg = pos;

            if (!indice_insertar_ALU(indice, &reg_indice, sizeof(t_reg_indice), cmp))
            {
                indice_destruir_ALU(indice);
                free(buffer);
                fclose(pf);
                return ERROR;
            }
        }

        pos++;
        fread(buffer, tamanyo_reg, 1, pf);
    }

    // Se liberan los recursos
    free(buffer);
    fclose(pf);

    return OK;
    //return indice_cargar(path, indice, tamanyo_reg, obtener_clave, es_activo, cmp);
}
