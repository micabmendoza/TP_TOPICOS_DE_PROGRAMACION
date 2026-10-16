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
    if(cadena_vacia(s))return;

    char temp[TAM_APYN] = {0};
    char *p_src = s;
    char *p_dst = temp;

    int coma_puesta = 0;
    int nueva_palabra = 1;

    while (isspace((unsigned char)*p_src))
    {
        p_src++;
    }

    while (*p_src != '\0' && (p_dst - temp) < 59)
    {
        if (*p_src == ',')
        {
            if (!coma_puesta)
            {
                if (p_dst > temp && *(p_dst - 1) == ' ')
                {
                    p_dst--;
                }
                *p_dst++ = ',';
                *p_dst++ = ' ';
                coma_puesta = 1;
                nueva_palabra = 1;
            }
        }
        else if (isspace((unsigned char)*p_src))
        {
            if (!coma_puesta)
            {
                if (p_dst > temp && *(p_dst - 1) != ' ')
                {
                    *p_dst++ = ',';
                    *p_dst++ = ' ';
                    coma_puesta = 1;
                    nueva_palabra = 1;
                }
            }
            else
            {
                if (p_dst > temp && *(p_dst - 1) != ' ')
                {
                    *p_dst++ = ' ';
                    nueva_palabra = 1;
                }
            }
        }
        else
        {
            if (nueva_palabra)
            {
                *p_dst++ = toupper((unsigned char)*p_src);
                nueva_palabra = 0;
            }
            else
            {
                *p_dst++ = tolower((unsigned char)*p_src);
            }
        }
        p_src++;
    }

    if (p_dst > temp && isspace((unsigned char)*(p_dst - 1))) p_dst--;
    if (p_dst > temp && *(p_dst - 1) == ',') p_dst--;

    *p_dst = '\0';

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
