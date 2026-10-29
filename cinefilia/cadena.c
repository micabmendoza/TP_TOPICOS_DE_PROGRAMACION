#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "cadena.h"
#define TAM_APYN 61  // Tamaño máximo para el campo apellidos_nombres
/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    normaliza un campo Apellidos y Nombres segun el Anexo A.2 (con
                regla de coma).
Parametros:     s: cadena a normalizar (se modifica in place).
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void arreglar_apyn_ALU(char *s)
{
    char temp[TAM_APYN], *dst = temp;
    const char *src = s;
    int tiene_coma = (strchr(s, ',') != NULL);
    int coma_puesta = 0, coma_pend = 0, sep = 0, inicio = 1;

    for (; *src && (dst - temp) < TAM_APYN - 3; src++)   /* hay lugar para ", " + 1 */
    {
        if (*src == ',' && !coma_puesta && !coma_pend)
            coma_pend = 1;
        else if (*src == ',' || isspace((unsigned char)*src))
            sep = 1;
        else
        {
            if (dst > temp && (sep || coma_pend))
            {
                if (coma_pend || (!tiene_coma && !coma_puesta))
                {
                    *dst++ = ','; *dst++ = ' '; coma_puesta = 1;
                }
                else
                    *dst++ = ' ';
                inicio = 1;
            }
            sep = coma_pend = 0;
            *dst++ = inicio ? toupper((unsigned char)*src) : tolower((unsigned char)*src);
            inicio = 0;
        }
    }
    *dst = '\0';
    copiar_cadena(s, temp, TAM_APYN);
}

/******************************************************************************
Descripcion:    separa una linea CSV in place por SEPARADOR_CSV, reemplazando
                cada separador por '\0' y guardando en campos[] el puntero al
                inicio de cada campo.
Parametros:     linea: linea a separar (se modifica).
                campos: vector donde se guardan los punteros a cada campo.
                max: cantidad maxima de campos a separar.
Retorno:        Cantidad de campos encontrados (los vacios cuentan). Nunca mas
                de max.
Observaciones:  Si la linea tiene mas separadores que max, el ultimo campo
                conserva el resto de la linea.
******************************************************************************/
int separar_campos_ALU(char *linea, char *campos[], int max)
{
    int cant_campos = 0;
    if(!linea || !campos || max <=0)
    {
        return 0;
    }

    quitar_salto(linea);

    campos[cant_campos++] = linea;

    char *p = linea;

    while(*p != '\0' && cant_campos < max)
    {
        if(*p == SEPARADOR_CSV)
        {
            *p = '\0';
            campos[cant_campos++] = p + 1;
        }
        p++;
    }

    return cant_campos;
}
