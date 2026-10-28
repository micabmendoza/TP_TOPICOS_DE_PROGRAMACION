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
    char path_socios_backup[256], path_titulos_backup[256], path_matriz_backup[256];

    // Armamos los path de los archivos
    sprintf(path_socios_backup, "archivos/socios_%04d%02d%02d.dat",
            fecha_proceso->anio, fecha_proceso->mes, fecha_proceso->dia);

    sprintf(path_titulos_backup, "archivos/titulos_%04d%02d%02d.dat",
            fecha_proceso->anio, fecha_proceso->mes, fecha_proceso->dia);

    sprintf(path_matriz_backup, "archivos/matriz_consumo_%04d%02d%02d.dat",
            fecha_proceso->anio, fecha_proceso->mes, fecha_proceso->dia);

    // Copiamos los archivos binarios
    if(archivo_existe(ARCH_SOCIOS_BIN))
    {
        archivo_copiar(ARCH_SOCIOS_BIN, path_socios_backup);
        printf("%s -> %s\n", ARCH_SOCIOS_BIN, path_socios_backup);
    }

    if (archivo_existe(ARCH_TITULOS_BIN))
    {
        archivo_copiar(ARCH_TITULOS_BIN, path_titulos_backup);
        printf("%s -> %s\n", ARCH_TITULOS_BIN, path_titulos_backup);
    }

    // Volcamos la matriz de consumo a un archivo binario diario
    if (matriz_consumo_persistir(matriz_consumo, path_matriz_backup) == OK)
        printf("matriz de consumo -> %s\n", path_matriz_backup);

    // Liberamos la memoria de los indices
    indice_destruir_ALU(ind_socios);
    indice_destruir_ALU(ind_titulos);

    // Imprimimos el mensaje de despedida
    printf("Memoria liberada. Hasta luego.\n\n");
}
