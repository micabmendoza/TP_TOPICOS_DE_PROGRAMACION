#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DIGITOS_DNI 8
#define MAX_STR_EMAIL 31

// Prototipos
void calcular_cuil_ALU(long dni, char sexo, char *dest);
void insertar_dni_en_cuil(long nro_dni, char *dest);
int email_valido_ALU(const char *email);

int main()
{
    return 0;
}
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
        if(*p_src == ' ' || largo >= 30)
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

void calcular_cuil_ALU(long dni, char sexo, char *dest)
{
    int pesos[10] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    int suma = 0;
    int prefijo = (sexo == 'H') ? 20 : 27;

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
        prefijo = 23;
    }
    else
    {
        z = (resto == 0) ? 0 : 11 - resto;
    }

    dest[0] = (prefijo / 10) + '0';
    dest[1] = (prefijo % 10) + '0';
    dest[2] = '-';
    insertar_dni_en_cuil(dni, dest);
    dest[11] = '-';
    dest[12] = z + '0';
    dest[13] = '\0';

}

void insertar_dni_en_cuil(long nro_dni, char *dest)
{
    int posicion_escritura = 10;

    for(int i = 0; i < DIGITOS_DNI; i++)
    {
        dest[posicion_escritura] = (nro_dni % 10) + '0';
        posicion_escritura--;
        nro_dni = nro_dni / 10;
    }
}
