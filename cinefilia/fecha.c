#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "fecha.h"
#include "util.h"
#include "indice.h"      /* OK / ERROR */

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    solicita la fecha de proceso por teclado hasta obtener una
                valida. Si el usuario solo presiona Enter toma la fecha actual
                del sistema.
Parametros:     fecha_proceso: fecha destino.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void pedir_fecha_proceso_ALU(t_fecha *fecha_proceso)
{
    int fecha_valida = ERROR, resultado_valido;

    while(fecha_valida == ERROR)
    {
        printf("Ingrese fecha de proceso (dd/mm/aaaa) [Enter = fecha actual]: ");
        resultado_valido = lee_fecha(fecha_proceso);

        if(resultado_valido == FECHA_VACIA)
        {
            fecha_actual(fecha_proceso);
            fecha_valida = OK;
        }
        else
        {
            if(resultado_valido == OK)
            {
                resultado_valido = fecha_correcta(fecha_proceso);
                if(resultado_valido == OK)
                {
                    fecha_valida = OK;
                }
                else
                {
                    printf("\nFECHA FORMALMENTE INCORRECTA");
                    fecha_valida = ERROR;
                }
            }
            else
            {
                printf("\nFECHA FORMALMENTE INCORRECTA");
                fecha_valida = ERROR;
            }
            printf("\n");
        }
    }
}
