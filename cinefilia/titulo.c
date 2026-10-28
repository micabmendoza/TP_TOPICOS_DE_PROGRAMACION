#include <stdlib.h>
#include <string.h>
#include "titulo.h"
#include "cadena.h"
#include "util.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    convierte una linea CSV de 4 campos en t_titulo. Los campos
                numericos invalidos quedan en valores que fallan la validacion
                (id = 0, stock = -1). El estado se fija en 'A'.
Parametros:     linea: linea CSV.
                reg: puntero a t_titulo destino.
Retorno:        OK si la cantidad de campos es correcta, ERROR en caso
                contrario.
Observaciones:  Se pasa por puntero como parsear a leer_archivo_texto.
******************************************************************************/
int texto_a_titulo_ALU(const char *linea, void *reg)
{
    char linea_aux[TAM_LINEA];
    char *campos[CAMPOS_TITULO];
    t_titulo *titulo = (t_titulo *)reg;
    int cant_campos;

    if (!linea || !reg)
    {
        return ERROR;
    }

    copiar_cadena(linea_aux, linea, TAM_LINEA);
    cant_campos = separar_campos_ALU(linea_aux, campos, CAMPOS_TITULO);

    if (cant_campos != CAMPOS_TITULO)
    {
        return ERROR;
    }

    if (sscanf(campos[0], "%d", &titulo->id) != 1)
    {
        titulo->id = 0;
    }

    copiar_cadena(titulo->titulo, campos[1], TAM_TITULO);

    copiar_cadena(titulo->genero, campos[2], TAM_GENERO);

    if (sscanf(campos[3], "%d", &titulo->stock) != 1)
    {
        titulo->stock = -1;
    }

    titulo->estado = 'A';

    return OK;
}

/******************************************************************************
Descripcion:    aplica las validaciones del punto 6.2 en orden: ID > 0 y no
                existente en el indice, titulo normalizado no vacio, genero
                dentro de GENEROS[] y stock >= 0. Se detiene en el primer
                error.
Parametros:     reg: puntero a t_titulo (se modifica al normalizar el titulo).
                ctx: contexto con indice e id_modificando.
                tipo_error: salida; nombre de la validacion que fallo.
Retorno:        OK si el titulo es valido, ERROR en caso contrario.
Observaciones:  Se pasa por puntero como validar a leer_archivo_texto. Se reutiliza en las altas del
                lote de novedades.
******************************************************************************/
int valida_titulo_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
{
    t_titulo *titulo = (t_titulo *)reg;
    t_reg_indice reg_busc;

    if (titulo->id <= 0)
    {
        strcpy(tipo_error, "ID");
        return ERROR;
    }

    if (ctx->id_modificando != titulo->id)
    {
        reg_busc.id = titulo->id;
        if (indice_buscar_ALU(ctx->indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) != NO_EXISTE) {
            strcpy(tipo_error, "ID");
            return ERROR;
        }
    }

    arreglar_titulo(titulo->titulo);
    if (cadena_vacia(titulo->titulo))
    {
        strcpy(tipo_error, "TITULO");
        return ERROR;
    }

    if (genero_indice(titulo->genero) < 0)
    {
        strcpy(tipo_error, "GENERO");
        return ERROR;
    }

    if (titulo->stock < 0)
    {
        strcpy(tipo_error, "STOCK");
        return ERROR;
    }

    return OK;
}

/******************************************************************************
Descripcion:    recorre el indice (ordenado por ID y solo con activos) y lista
                los titulos leyendo cada registro del archivo.
Parametros:     indice: indice de titulos.
                path: ruta de titulos.dat.
Retorno:        OK si se pudo abrir el archivo, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int titulos_listar_ALU(const t_indice *indice, const char *path)
{
    FILE *fp;
    const t_reg_indice *arreglo_indice;
    t_titulo titulo_leido;
    unsigned i;

    // Abrir titulos.dat en modo de lectura binaria
    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        printf("\nError al intentar abrir el archivo binario %s.", path);
        return ERROR;
    }

    // Castear vindice, que apunta a void, para que apunte al tipo t_reg_indice
    arreglo_indice = (t_reg_indice*) indice->vindice;

    // Imprimir encabezado
    titulos_encabezado();

    // Recorrer vindice completo
    for (i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        // Lectura directa por numero de registro fisico
        if (titulo_leer(fp, arreglo_indice[i].nro_reg, &titulo_leido) == OK)
        {
            // Impresion formateada del titulo
            titulo_mostrar(&titulo_leido);
        }
    }

    // Imprimir contador final de titulos activos
    printf("\nTotal de titulos activos: %u\n", indice->cantidad_elementos_actual);

    fclose(fp);
    return OK;
}
