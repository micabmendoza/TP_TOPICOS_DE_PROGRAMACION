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
    return titulos_listar(indice, path);
}
