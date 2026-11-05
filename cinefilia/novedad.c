#include <string.h>
#include "novedad.h"
#include "cadena.h"

/// Funciones auxiliares extra
// MEJORA Nº 4
// Se descompone novedades_procesar_ALU en cada tipo de operación (A, B, M) para que no ocupe tantas líneas y quede modularizada
static int procesar_alta_novedad(t_indice *indice, FILE *arch_tit, t_novedad *nov, t_contexto *ctx)
{
    t_reg_indice reg_busc;
    char tipo_error[TAM_TIPO_ERROR];

    reg_busc.id = nov->titulo.id;
    if (indice_buscar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) != NO_EXISTE)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "el ID ya existe");
        return ERROR;
    }

    ctx->id_modificando = 0;
    nov->titulo.estado = 'A';

    if (valida_titulo_ALU(&nov->titulo, ctx, tipo_error) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, tipo_error);
        return ERROR;
    }

    fseek(arch_tit, 0L, SEEK_END);
    reg_busc.nro_reg = ftell(arch_tit) / sizeof(t_titulo);

    // MEJORA Nº 3
    // Validamos el retorno de titulo_escribir y de las operaciones de índice
    if (titulo_escribir(arch_tit, reg_busc.nro_reg, &nov->titulo) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error de escritura en disco");
        return ERROR;
    }

    if (indice_insertar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error al insertar en indice");
        return ERROR;
    }

    printf("  ALTA       id=%-6d %s\n", nov->titulo.id, nov->titulo.titulo);
    return OK;
}

static int procesar_baja_novedad(t_indice *indice, FILE *arch_tit, const t_novedad *nov)
{
    t_reg_indice reg_busc;
    t_titulo tit_existente;

    reg_busc.id = nov->titulo.id;
    if (indice_buscar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) == NO_EXISTE)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "el ID no existe o ya esta dado de baja");
        return ERROR;
    }

    // MEJORA Nº 3
    // Validamos retorno de titulo_leer, titulo_escribir e indice_eliminar_ALU
    if (titulo_leer(arch_tit, reg_busc.nro_reg, &tit_existente) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error de lectura en disco");
        return ERROR;
    }

    tit_existente.estado = 'B';

    if (titulo_escribir(arch_tit, reg_busc.nro_reg, &tit_existente) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error de actualizacion en disco");
        return ERROR;
    }

    if (indice_eliminar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error al eliminar del indice");
        return ERROR;
    }

    printf("  BAJA       id=%-6d %s\n", tit_existente.id, tit_existente.titulo);
    return OK;
}

static int procesar_modificacion_novedad(t_indice *indice, FILE *arch_tit, const t_novedad *nov, t_contexto *ctx)
{
    t_reg_indice reg_busc;
    t_titulo tit_existente;
    char tipo_error[TAM_TIPO_ERROR];
    int nuevo_stock;

    reg_busc.id = nov->titulo.id;
    if (indice_buscar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) == NO_EXISTE)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "el ID no existe o esta dado de baja");
        return ERROR;
    }

    // MEJORA Nº 3
    // Validar retorno de titulo_leer y titulo_escribir
    if (titulo_leer(arch_tit, reg_busc.nro_reg, &tit_existente) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error de lectura en disco");
        return ERROR;
    }

    nuevo_stock = tit_existente.stock + nov->titulo.stock;

    if (nuevo_stock < 0)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "STOCK (quedaria negativo)");
        return ERROR;
    }

    copiar_cadena(tit_existente.titulo, nov->titulo.titulo, TAM_TITULO);
    copiar_cadena(tit_existente.genero, nov->titulo.genero, TAM_GENERO);
    tit_existente.stock = nuevo_stock;

    ctx->id_modificando = tit_existente.id;

    if (valida_titulo_ALU(&tit_existente, ctx, tipo_error) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, tipo_error);
        return ERROR;
    }

    if (titulo_escribir(arch_tit, reg_busc.nro_reg, &tit_existente) != OK)
    {
        printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov->codigo_operacion, nov->titulo.id, "error de escritura en disco");
        return ERROR;
    }

    printf("  MODIFICADA id=%-6d %s (stock %d)\n", tit_existente.id, tit_existente.titulo, tit_existente.stock);
    return OK;
}

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    recorre secuencialmente novedades_catalogo.dat y aplica cada
                operacion sobre el indice y titulos.dat: 'A' agrega si el ID
                no esta en el indice (validando como en la carga), 'B' pone
                estado 'B' en el archivo y elimina del indice, 'M' reemplaza
                titulo y genero y suma el stock (rechaza si quedaria
                negativo). Al final muestra el reporte de procesadas y
                rechazadas.
Parametros:     indice: indice de titulos.
                path_novedades: ruta de novedades_catalogo.dat.
                path_titulos: ruta de titulos.dat.
                fecha_proceso: fecha de proceso (para el contexto de
                validacion).
Retorno:        OK si se pudieron abrir los archivos, ERROR en caso contrario.
Observaciones:  Como el indice solo tiene activos, un 'A' sobre un ID dado de
                baja se agrega como registro nuevo al final del archivo.
******************************************************************************/
int novedades_procesar_ALU(t_indice *indice, const char *path_novedades,
                       const char *path_titulos, const t_fecha *fecha_proceso)
{
    FILE *arch_nov, *arch_tit;
    t_novedad nov;
    t_contexto ctx;
    unsigned procesadas = 0, rechazadas = 0;

    arch_nov = fopen(path_novedades, "rb");
    if (arch_nov == NULL)
    {
        printf("No se pudo abrir %s.\n", path_novedades);
        return ERROR;
    }

    arch_tit = fopen(path_titulos, "r+b");
    if (arch_tit == NULL)
    {
        printf("No se pudo abrir %s.\n", path_titulos);
        fclose(arch_nov);
        return ERROR;
    }

    printf("Procesando %s...\n", path_novedades);

    ctx.indice = indice;
    ctx.arch_bin = arch_tit;
    ctx.fecha_proceso = fecha_proceso;

    while (fread(&nov, sizeof(t_novedad), 1, arch_nov) == 1)
    {
        if (nov.codigo_operacion == OP_ALTA)
        {
            if (procesar_alta_novedad(indice, arch_tit, &nov, &ctx) == OK)
                procesadas++;
            else
                rechazadas++;
        }
        else if (nov.codigo_operacion == OP_BAJA)
        {
            if (procesar_baja_novedad(indice, arch_tit, &nov) == OK)
                procesadas++;
            else
                rechazadas++;
        }
        else if (nov.codigo_operacion == OP_MODIFICACION)
        {
            if (procesar_modificacion_novedad(indice, arch_tit, &nov, &ctx) == OK)
                procesadas++;
            else
                rechazadas++;
        }
        else
        {
            printf("  RECHAZADA  op=%c id=%-6d motivo: %s\n", nov.codigo_operacion, nov.titulo.id, "codigo de operacion invalido");
            rechazadas++;
        }
    }

    fclose(arch_nov);
    fclose(arch_tit);

    printf("\nNovedades procesadas: %u   Rechazadas: %u\n", procesadas, rechazadas);

    return OK;
}
