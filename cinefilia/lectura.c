#include <string.h>
#include "lectura.h"
#include "util.h"
#include "cadena.h"
#include "socio.h"
#include "titulo.h"
#include "config.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    funcion generica de lectura de archivos de texto. Por cada
                linea no vacia: parsear -> validar -> escribir al final del
                archivo binario e insertar en el indice. Ante el primer error
                (de formato o de validacion) se rechaza la linea invocando
                rechazar y se continua con la siguiente.
Parametros:     path_txt: ruta del archivo de texto (CSV).
                arch_bin: archivo binario destino abierto en "w+b" o "r+b".
                indice: indice de la entidad (se usa para validar duplicados y
                se va completando).
                reg: buffer de un registro de la entidad.
                tamanyo: tamanyo en bytes del registro.
                fecha_proceso: fecha de proceso.
                parsear: convierte una linea en registro.
                validar: valida el registro y deja el tipo de error.
                rechazar: accion ante un rechazo (auditar, ignorar, etc.).
                obtener_clave: devuelve la clave del registro para el indice.
                aceptados, rechazados: salida; contadores de registros.
Retorno:        OK si se pudo abrir el archivo de texto, ERROR en caso
                contrario.
Observaciones:  Se reutiliza para Socios.csv y Titulos.csv; lo especifico de
                cada entidad lo resuelven los punteros a funcion.
******************************************************************************/
int leer_archivo_texto_ALU(const char *path_txt, FILE *arch_bin, t_indice *indice,
                       void *reg, size_t tamanyo, const t_fecha *fecha_proceso,
                       int (*parsear)(const char *, void *),
                       int (*validar)(void *, const t_contexto *, char *),
                       void (*rechazar)(const void *, const char *, const char *, const t_fecha *),
                       long (*obtener_clave)(const void *),
                       unsigned *aceptados, unsigned *rechazados)
{
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return leer_archivo_texto(path_txt, arch_bin, indice, reg, tamanyo, fecha_proceso, parsear, validar, rechazar, obtener_clave, aceptados, rechazados);
}

/******************************************************************************
Descripcion:    compara dos t_reg_indice por su clave id.
Parametros:     a, b: punteros a t_reg_indice.
Retorno:        Negativo si a < b, 0 si son iguales, positivo si a > b.
Observaciones:  Se usa como cmp en todas las operaciones del TDA indice.
******************************************************************************/
int compara_reg_indice_ALU(const void *a, const void *b)
{
    // Casteamos los parametros para que apunten a t_reg_indice
    const t_reg_indice *reg1 = (const t_reg_indice*) a;
    const t_reg_indice *reg2 = (const t_reg_indice*) b;

    // Comparamos por ID
    if(reg1->id < reg2->id)
        return -1;
    if(reg1->id > reg2->id)
        return 1;

    return 0;
}

/******************************************************************************
Descripcion:    construye los indices de socios y de titulos a partir de los
                archivos binarios existentes (solo registros activos).
Parametros:     ind_socios: indice de socios.
                ind_titulos: indice de titulos.
Retorno:        1 si ambos binarios existen y se cargaron, 0 en caso
                contrario.
Observaciones:  -
******************************************************************************/
int cargar_indices_ALU(t_indice *ind_socios, t_indice *ind_titulos)
{
    int res_socios;
    int res_titulos;

    /* 1. Cargamos el indice de socios desde socios.dat */
    res_socios = indice_cargar_ALU(ARCH_SOCIOS_BIN,
                               ind_socios,
                               sizeof(t_socio),
                               clave_socio,
                               activo_socio,
                               compara_reg_indice_ALU);

    /* 2. Cargamos el indice de titulos desde titulos.dat */
    res_titulos = indice_cargar_ALU(ARCH_TITULOS_BIN,
                                ind_titulos,
                                sizeof(t_titulo),
                                clave_titulo,
                                activo_titulo,
                                compara_reg_indice_ALU);

    /* 3. Retornamos 1 (OK) solo si ambos archivos existían y cargaron bien */
    if (res_socios == OK && res_titulos == OK)
    {
        return OK;
    }

    return ERROR;
}

/******************************************************************************
Descripcion:    opcion 1 del menu: lee Socios.csv y Titulos.csv con la funcion
                generica de lectura, genera socios.dat y titulos.dat
                (sobrescribiendolos) y reconstruye los indices desde los
                binarios generados.
Parametros:     ind_socios: indice de socios.
                ind_titulos: indice de titulos.
                fecha_proceso: fecha de proceso.
Retorno:        1 si los indices quedaron cargados, 0 en caso contrario.
Observaciones:  Los rechazos de socios se auditan en errores_socios.txt; los
                de titulos solo se cuentan.
******************************************************************************/
int generar_binarios_ALU(t_indice *ind_socios, t_indice *ind_titulos, const t_fecha *fecha_proceso)
{
    FILE *fp_socios, *fp_titulos;
    t_socio socio_buffer;
    t_titulo titulo_buffer;
    unsigned aceptados_s = 0, rechazados_s = 0;
    unsigned aceptados_t = 0, rechazados_t = 0;

    // Se vacian los indices previos
    indice_vaciar_ALU(ind_socios);
    indice_vaciar_ALU(ind_titulos);

    // Se sobreescriben los archivos
    fp_socios = fopen(ARCH_SOCIOS_BIN, "w+b");
    if(fp_socios == NULL)
    {
        puts("Error al crear el archivo binario de socios.\n");
        return ERROR;
    }

    fp_titulos = fopen(ARCH_TITULOS_BIN, "w+b");
    if(fp_titulos == NULL)
    {
        fclose(fp_socios);
        puts("Error al crear el archivo binario de titulos.\n");
        return ERROR;
    }

    // Se procesan ambos .csv a traves de la funcion generica de lectura
    if(leer_archivo_texto_ALU(ARCH_SOCIOS_CSV,
                              fp_socios,
                              ind_socios,
                              &socio_buffer,
                              sizeof(t_socio),
                              fecha_proceso,
                              texto_a_socio_ALU,
                              valida_socio_ALU,
                              rechaza_socio,
                              clave_socio,
                              &aceptados_s,
                              &rechazados_s) == ERROR)
    {
        fclose(fp_socios);
        fclose(fp_titulos);
        return ERROR;
    }

    if(leer_archivo_texto_ALU(ARCH_TITULOS_CSV,
                              fp_titulos,
                              ind_titulos,
                              &titulo_buffer,
                              sizeof(t_titulo),
                              fecha_proceso,
                              texto_a_titulo_ALU,
                              valida_titulo_ALU,
                              rechaza_titulo,
                              clave_titulo,
                              &aceptados_t,
                              &rechazados_t) == ERROR)
    {
        fclose(fp_socios);
        fclose(fp_titulos);
        return ERROR;
    }

    // Cerramos los archivos binarios ya cargados
    fclose(fp_socios);
    fclose(fp_titulos);

    // Reconstruimos los indices
    if(cargar_indices_ALU(ind_socios, ind_titulos) == ERROR)
    {
        puts("Error al reconstruir los indices tras la generacion.\n");
        return ERROR;
    }

    // Reporte final
    printf("Socios:  %u aceptados, %u rechazados (ver archivos/%s)\n", aceptados_s, rechazados_s, ARCH_ERRORES);
    printf("Titulos: %u aceptados, %u rechazados\n", aceptados_t, rechazados_t);

    return OK;
}
