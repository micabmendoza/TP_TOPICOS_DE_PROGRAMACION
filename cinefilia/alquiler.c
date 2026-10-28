#include <stdlib.h>
#include <string.h>
#include "alquiler.h"
#include "cadena.h"
#include "util.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    imprime por pantalla la matriz de consumo con totales por fila
                (plan) y por columna (genero).
Parametros:     m: matriz de consumo.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void tablero_mostrar_ALU(t_matriz_consumo m)
{
    int i, j, total=0, total_por_plan, total_por_genero[CANT_GENEROS] = {0};

    // Encabezado
    printf("\n%-11s", "PLAN");
    for(i=0; i < CANT_GENEROS; i++)
    {
        printf("%21s", GENEROS[i]);
    }
    printf("%11s", "TOTAL");

    // Cuerpo
    for(i = 0; i < CANT_PLANES; i++)
    {
        total_por_plan = 0;

        printf("\n%-11s", PLANES[i]);

        for(j = 0; j < CANT_GENEROS; j++)
        {
            total_por_plan += m[i][j];
            total_por_genero[j] += m[i][j];
            total += m[i][j];

            printf("%21d", m[i][j]);
        }

        printf("%11d", total_por_plan);
    }

    // Fila final total por genero
    printf("\n%-11s", "TOTAL");
    for(i = 0; i < CANT_GENEROS; i++)
        printf("%21d", total_por_genero[i]);

    // Ultimo total general
    printf("%11d", total);

    printf("\n");
}

/******************************************************************************
Descripcion:    ejecuta el flujo de alquiler de un titulo: 1) habilitacion del
                socio (debe estar en el indice), 2) inventario (el titulo debe
                estar en el indice y con stock > 0), 3) descuenta una unidad
                de stock en el archivo, 4) incrementa la celda [plan][genero]
                de la matriz, 5) emite el comprobante.
Parametros:     ind_socios: indice de socios.
                ind_titulos: indice de titulos.
                path_socios: ruta de socios.dat.
                path_titulos: ruta de titulos.dat.
                matriz: matriz de consumo a alimentar.
                fecha_proceso: fecha de proceso para el comprobante.
Retorno:        OK si el alquiler se concreto, ERROR si alguna etapa fallo.
Observaciones:  Si una etapa falla se informa el motivo y no se ejecutan las
                siguientes. No se exige cuota al dia.
******************************************************************************/
int alquilar_ALU(t_indice *ind_socios, t_indice *ind_titulos, const char *path_socios,
             const char *path_titulos, t_matriz_consumo matriz_consumo, const t_fecha *fecha_proceso)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return alquilar(ind_socios, ind_titulos, path_socios, path_titulos, matriz_consumo, fecha_proceso);
}
