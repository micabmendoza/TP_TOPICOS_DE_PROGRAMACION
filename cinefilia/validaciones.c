#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "validaciones.h"
#define DIGITOS_DNI 8

void insertar_dni_en_cuil_aux(long nro_dni, char *dest)
{
    int posicion_escritura = 10;

    for(int i = 0; i < DIGITOS_DNI; i++)
    {
        dest[posicion_escritura] = (nro_dni % 10) + '0';
        posicion_escritura--;
        nro_dni = nro_dni / 10;
    }
}
/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    calcula el CUIL con el algoritmo modulo 11 del Anexo A.1:
                prefijo 20 (hombre) o 27 (mujer), DNI a 8 digitos y digito
                verificador. Si el resto es 1 el prefijo pasa a 23 y Z es 9
                (hombre) o 4 (mujer).
Parametros:     dni: numero de documento.
                sexo: 'H' o 'M'.
                dest: buffer de al menos TAM_CUIL bytes donde se deja "XY-
                NNNNNNNN-Z".
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void calcular_cuil_ALU(long dni, char sexo, char *dest)
{
    int pesos[10] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    int suma = 0;
    int prefijo = (sexo == 'H') ? PREFIJO_HOMBRE : PREFIJO_MUJER;

    suma += (prefijo / 10) * pesos[0];
    suma += (prefijo % 10) * pesos[1];

    long dni_aux = dni;
    for(int i = 9; i >= 2; i--)
    {
        suma += (dni_aux % 10) * pesos[i];
        dni_aux = dni_aux / 10;
    }

    int resto = suma % 11;
    int z;

    if(resto == 1)
    {
        z = (sexo == 'H') ? 9 : 4;
        prefijo = PREFIJO_RESTO1;
    }
    else
    {
        z = (resto == 0) ? 0 : 11 - resto;
    }

    dest[0] = (prefijo / 10) + '0';
    dest[1] = (prefijo % 10) + '0';
    dest[2] = '-';
    insertar_dni_en_cuil_aux(dni, dest);
    dest[11] = '-';
    dest[12] = z + '0';
    dest[13] = '\0';
}

/******************************************************************************
Descripcion:    valida un email segun el Anexo A.3: hasta 30 caracteres sin
                espacios, una unica '@', parte local de letras/digitos/puntos
                que no empieza ni termina con punto, y dominio de
                letras/digitos/guiones/puntos con al menos un punto, que no
                empieza ni termina con punto o guion y cuyo ultimo segmento
                tiene al menos 2 letras.
Parametros:     email: cadena a validar.
Retorno:        1 si es valido, 0 en caso contrario.
Observaciones:  No verifica unicidad; eso lo hace socio_email_existe.
******************************************************************************/
int email_valido_ALU(const char *email)
{
    if(!email)
    {
        return 0;
    }
    const char *p_src = email;
    int largo = 0;
    int posicion_arroba = -1;
    int posicion_ultimo_punto = -1;

    while(*p_src != '\0')
    {
        if(*p_src == ' ' || largo >= EMAIL_MAX_LEN)
        {
            return 0;
        }

        if(*p_src == '@')
        {
            if(posicion_arroba != -1)
            {
                return 0;
            }
            posicion_arroba = largo;
        }
        else
        {
            if(*p_src == '.')
            {
                posicion_ultimo_punto = largo;
            }
        }

        largo++;
        p_src++;
    }

    if(largo == 0 || posicion_arroba == -1)
    {
        return 0;
    }

    if(posicion_arroba == 0)
    {
        return 0;
    }
    if(email[0] == '.' || email[posicion_arroba - 1] == '.')
    {
        return 0;
    }

    if(posicion_arroba == largo - 1)
    {
        return 0;
    }

    if(email[posicion_arroba + 1] == '.' || email[posicion_arroba + 1] == '-')
    {
        return 0;
    }

    if(email[largo - 1] == '.' || email[largo - 1] == '-')
    {
        return 0;
    }

    if(posicion_ultimo_punto <= posicion_arroba)
    {
        return 0;
    }

    for(int i = 0; i < largo; i++)
    {
        char c = email[i];
        int es_letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        int es_numero = (c >= '0' && c <= '9');

        if(!es_letra && !es_numero && c != '.' && c != '@' && c != '-')
        {
            return 0;
        }

        if(c == '-' && i < posicion_arroba)
        {
            return 0;
        }
    }

    int cont_letras = 0;
    for(int i = posicion_ultimo_punto + 1; i < largo; i++)
    {
        char c = email[i];
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            cont_letras++;
        }
        else
        {
            return 0;
        }
    }

    if(cont_letras < 2)
    {
        return 0;
    }

    return 1;
}
