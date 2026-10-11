#include <string.h>
#include "novedad.h"
#include "cadena.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    recorre secuencialmente novedades_catalogo.dat y aplica cada
                operacion sobre el indice y titulos.dat: 'A' agrega si el ID
                no esta en el indice (validando como en la carga), 'B' pone
                estado 'B' en el archivo y elimina del indice, 'M' reemplaza
                titulo y genero y suma el stock (rechaza si quedaria
                negativo). Al final muestra el reporte de procesadas y
                rechazadas.
Parametros:     indice: indice de titulos.
                path_novedades: ruta de novedades_catalogo.dat.
                path_titulos: ruta de titulos.dat.
                fecha_proceso: fecha de proceso (para el contexto de
                validacion).
Retorno:        OK si se pudieron abrir los archivos, ERROR en caso contrario.
Observaciones:  Como el indice solo tiene activos, un 'A' sobre un ID dado de
                baja se agrega como registro nuevo al final del archivo.
******************************************************************************/
int novedades_procesar_ALU(t_indice *indice, const char *path_novedades,
                       const char *path_titulos, const t_fecha *fecha_proceso)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return novedades_procesar(indice, path_novedades, path_titulos, fecha_proceso);
}
