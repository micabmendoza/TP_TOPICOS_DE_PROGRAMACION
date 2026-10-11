#include <string.h>
#include "util.h"
#include "cadena.h"
#include "config.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    opcion Salir: copia socios.dat y titulos.dat con la fecha de
                proceso en el nombre, vuelca la matriz de consumo a disco y
                libera la memoria de ambos indices.
Parametros:     ind_socios: indice de socios.
                ind_titulos: indice de titulos.
                matriz: matriz de consumo.
                fecha_proceso: fecha de proceso.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void salir_y_persistir_ALU(t_indice *ind_socios, t_indice *ind_titulos,
                              t_matriz_consumo matriz_consumo, const t_fecha *fecha_proceso)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    salir_y_persistir(ind_socios, ind_titulos, matriz_consumo, fecha_proceso);
}
