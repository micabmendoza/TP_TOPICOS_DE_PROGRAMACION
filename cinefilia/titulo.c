#include <stdlib.h>
#include <string.h>
#include "titulo.h"
#include "cadena.h"
#include "util.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    convierte una linea CSV de 4 campos en t_titulo. Los campos
                numericos invalidos quedan en valores que fallan la validacion
                (id = 0, stock = -1). El estado se fija en 'A'.
Parametros:     linea: linea CSV.
                reg: puntero a t_titulo destino.
Retorno:        OK si la cantidad de campos es correcta, ERROR en caso
                contrario.
Observaciones:  Se pasa por puntero como parsear a leer_archivo_texto.
******************************************************************************/
int texto_a_titulo_ALU(const char *linea, void *reg)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return texto_a_titulo(linea, reg);
}

/******************************************************************************
Descripcion:    aplica las validaciones del punto 6.2 en orden: ID > 0 y no
                existente en el indice, titulo normalizado no vacio, genero
                dentro de GENEROS[] y stock >= 0. Se detiene en el primer
                error.
Parametros:     reg: puntero a t_titulo (se modifica al normalizar el titulo).
                ctx: contexto con indice e id_modificando.
                tipo_error: salida; nombre de la validacion que fallo.
Retorno:        OK si el titulo es valido, ERROR en caso contrario.
Observaciones:  Se pasa por puntero como validar a leer_archivo_texto. Se reutiliza en las altas del
                lote de novedades.
******************************************************************************/
int valida_titulo_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return valida_titulo(reg, ctx, tipo_error);
}

/******************************************************************************
Descripcion:    recorre el indice (ordenado por ID y solo con activos) y lista
                los titulos leyendo cada registro del archivo.
Parametros:     indice: indice de titulos.
                path: ruta de titulos.dat.
Retorno:        OK si se pudo abrir el archivo, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int titulos_listar_ALU(const t_indice *indice, const char *path)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
  
    FILE *fp;
    t_reg_indice *arreglo_indice;
    t_titulo titulo_leido;
    unsigned i;

    // Abrir titulos.dat en modo de lectura binaria
    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        printf("\nError al intentar abrir el archivo binario %s.", path);
        return ERROR;
    }

    // Castear vindice, que apunta a void, para que apunte al tipo t_reg_indice
    arreglo_indice = (t_reg_indice*) indice->vindice;

    // Imprimir encabezado
    printf("%-7s %-60s %-20s %5s %s\n", "ID", "TITULO", "GENERO", "STOCK", "E");

    // Recorrer vindice completo
    for (i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        // Saltear al byte físico del archivo titulos.dat, aprovechando la utilidad del índice
        fseek(fp, arreglo_indice[i].nro_reg * sizeof(t_titulo), SEEK_SET);

        // Leer el registro
        fread(&titulo_leido, sizeof(t_titulo), 1, fp);

        // Imprimir el registro
        printf("%-7d %-60s %-20s %5d %c\n",
               titulo_leido.id,
               titulo_leido.titulo,
               titulo_leido.genero,
               titulo_leido.stock,
               titulo_leido.estado);
    }

    // Imprimir contador final de titulos activos
    printf("\nTotal de titulos activos: %u\n", indice->cantidad_elementos_actual);

    fclose(fp);
    return OK;
}
