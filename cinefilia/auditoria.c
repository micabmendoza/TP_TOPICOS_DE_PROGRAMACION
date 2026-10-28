#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auditoria.h"
#include "lectura.h"
#include "cadena.h"
#include "util.h"
#include "indice.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    agrega al final del archivo de auditoria una linea con el
                formato DNI;TIPO_VALIDACION;FECHA_PROCESO.
Parametros:     path: ruta del archivo de texto de errores.
                dni_texto: DNI tal como vino en la entrada.
                tipo_error: nombre de la validacion que fallo.
                fecha_proceso: fecha de proceso.
Retorno:        OK si se pudo grabar, ERROR en caso contrario.
Observaciones:  El archivo se abre en modo append; si no existe se crea.
******************************************************************************/
int auditoria_registrar_ALU(const char *path, const char *dni_texto,
                            const char *tipo_error, const t_fecha *fecha_proceso)
{
    FILE *pf_arch;

    pf_arch = fopen(path, "a");
    if (!pf_arch)
    {
        return ERROR;
    }

    fprintf(pf_arch, "%s;%s;%02d/%02d/%04d\n", dni_texto, tipo_error,
            fecha_proceso->dia, fecha_proceso->mes, fecha_proceso->anio);

    fclose(pf_arch);
    return OK;
    //return auditoria_registrar(path, dni_texto, tipo_error, fecha_proceso);
}

/******************************************************************************
Descripcion:    lee el archivo de auditoria y muestra, por cada par (DNI, tipo
                de validacion), la cantidad de incidencias registradas y el
                total general.
Parametros:     path: ruta del archivo de texto de errores.
Retorno:        OK si se pudo procesar (aunque el archivo no exista), ERROR si
                fallo la memoria.
Observaciones:  Acumula en un vector dinamico que crece con realloc.
******************************************************************************/
int auditoria_mostrar_ALU(const char *path)
{
    FILE *pf_arch;
    char linea[TAM_LINEA];
    char *campos[3];
    int i, encontrado, cant_campos;
    int total_general = 0;
    t_incidencia *vector = NULL;
    t_incidencia *aux = NULL;
    int cant_elementos = 0;
    int error_memoria = 0;

    if (!path)
    {
        return OK;
    }

    pf_arch = fopen(path, "rt");
    if (!pf_arch)
    {
        /* Retorna OK si el archivo no existe, tal como especifica la consigna */
        return OK;
    }

    while (!error_memoria && fgets(linea, sizeof(linea), pf_arch) != NULL)
    {
        if (!cadena_vacia(linea))
        {
            char *salto = strchr(linea, '\n');
            if (salto)
            {
                *salto = '\0';
            }
            salto = strchr(linea, '\r');
            if (salto)
            {
                *salto = '\0';
            }

            cant_campos = separar_campos_ALU(linea, campos, 3);
            if (cant_campos >= 2)
            {
                encontrado = 0;
                i = 0;

                while (i < cant_elementos && !encontrado)
                {
                    if (strcmp(vector[i].dni, campos[0]) == 0 &&
                            strcmp(vector[i].tipo_error, campos[1]) == 0)
                    {
                        vector[i].cantidad++;
                        encontrado = 1;
                    }
                    else
                    {
                        i++;
                    }
                }

                if (!encontrado)
                {
                    aux = (t_incidencia *)realloc(vector, (cant_elementos + 1) * sizeof(t_incidencia));
                    if (!aux)
                    {
                        error_memoria = 1;
                    }
                    else
                    {
                        vector = aux;
                        copiar_cadena(vector[cant_elementos].dni, campos[0], sizeof(vector[cant_elementos].dni));
                        copiar_cadena(vector[cant_elementos].tipo_error, campos[1], sizeof(vector[cant_elementos].tipo_error));
                        vector[cant_elementos].cantidad = 1;
                        cant_elementos++;
                    }
                }

                if (!error_memoria)
                {
                    total_general++;
                }
            }
        }
    }

    fclose(pf_arch);

    if (error_memoria)
    {
        if (vector != NULL)
        {
            free(vector);
        }
        return ERROR;
    }

    printf("%-16s %-23s %s\n", "DNI", "TIPO DE VALIDACION", "CANTIDAD");
    printf("--------------------------------------- --------\n");

    for (i = 0; i < cant_elementos; i++)
    {
        printf("%-16s %-23s %8d\n", vector[i].dni, vector[i].tipo_error, vector[i].cantidad);
    }

    printf("\nTotal de incidencias: %d\n", total_general);

    if (vector != NULL)
    {
        free(vector);
    }

    return OK;
    //return auditoria_mostrar(path);
}
