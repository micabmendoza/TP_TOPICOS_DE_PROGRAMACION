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
    FILE *fp_socios, *fp_titulos;
    t_reg_indice reg_socio, reg_titulo;
    t_socio socio;
    t_titulo titulo;
    long dni_ingresado;
    int id_titulo_ingresado;
    int id_plan, id_genero;
    char buffer[TAM_LINEA];

    printf("DNI del socio: ");
    leer_linea(buffer, sizeof(buffer));

    if(cadena_vacia(buffer) || sscanf(buffer, "%ld", &dni_ingresado) != 1 || dni_ingresado < DNI_MIN || dni_ingresado > DNI_MAX)
    {
        puts("Alquiler rechazado: DNI invalido.");
        return ERROR;
    }

    // Busqueda del socio en el indice
    reg_socio.id = dni_ingresado;
    if(indice_buscar_ALU(ind_socios, &reg_socio, sizeof(t_reg_indice), compara_reg_indice_ALU) == NO_EXISTE)
    {
        puts("Alquiler rechazado: el socio no existe o esta dado de baja.");
        return ERROR;
    }

    // Lectura del socio
    fp_socios = fopen(path_socios, "rb");
    if(fp_socios == NULL)
        return ERROR;

    if(socio_leer_ALU(fp_socios, reg_socio.nro_reg, &socio) == ERROR)
    {
        puts("Alquiler rechazado: no se pudo leer el socio.");
        fclose(fp_socios);
        return ERROR;
    }

    fclose(fp_socios);

    // Validacion de ID de pelicula usando string
    printf("ID de la pelicula: ");
    leer_linea(buffer, sizeof(buffer));

    // Eliminamos la restriccion <= 0 para igualar la logica del profe
    if (cadena_vacia(buffer) || sscanf(buffer, "%d", &id_titulo_ingresado) != 1)
    {
        puts("Alquiler rechazado: ID invalido.");
        return ERROR;
    }

    reg_titulo.id = id_titulo_ingresado;
    if (indice_buscar_ALU(ind_titulos, &reg_titulo, sizeof(t_reg_indice), compara_reg_indice_ALU) == NO_EXISTE)
    {
        puts("Alquiler rechazado: el titulo no existe o esta dado de baja.");
        return ERROR;
    }

    // Lectura del titulo
    fp_titulos = fopen(path_titulos, "r+b");
    if(fp_titulos == NULL)
        return ERROR;

    if(titulo_leer(fp_titulos, reg_titulo.nro_reg, &titulo) == ERROR)
    {
        printf("Alquiler rechazado: no se pudo leer el titulo.\n");
        fclose(fp_titulos);
        return ERROR;
    }

    // Verificacion de stock
    if (titulo.stock <= 0)
    {
        fclose(fp_titulos);
        printf("Alquiler rechazado: sin stock de \"%s\".\n", titulo.titulo);
        return ERROR;
    }

    // Actualizacion de stock en titulos.dat
    titulo.stock--;
    titulo_escribir(fp_titulos, reg_titulo.nro_reg, &titulo);

    fclose(fp_titulos);

    // Actualizacion de matriz de consumo
    id_plan = plan_indice(socio.plan);
    id_genero = genero_indice(titulo.genero);

    if(id_plan != -1 && id_genero != -1)
        matriz_consumo[id_plan][id_genero]++;

    // Impresion del comprobante
    printf("\n========== COMPROBANTE DE ALQUILER ==========\n");
    printf(" Fecha    : %02d/%02d/%04d\n", fecha_proceso->dia, fecha_proceso->mes, fecha_proceso->anio);
    printf(" Socio    : %ld - %s (plan %s)\n", socio.dni, socio.apellidos_nombres, socio.plan);
    printf(" Titulo   : %d - %s (%s)\n", titulo.id, titulo.titulo, titulo.genero);
    printf(" Stock restante: %d\n", titulo.stock);
    printf("=============================================\n");

    return OK;
}
