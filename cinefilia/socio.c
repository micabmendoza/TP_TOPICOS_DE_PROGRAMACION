#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "socio.h"
#include "cadena.h"
#include "util.h"
#include "auditoria.h"
#include "config.h"
#include "menu.h"

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/

/******************************************************************************
Descripcion:    convierte una linea CSV de 10 campos en t_socio. Los campos
                con formato invalido quedan con valores que haran fallar la
                validacion correspondiente (dni = 0, fecha 0/0/0, sexo '?',
                etc.). El estado se fija en 'A'.
Parametros:     linea: linea CSV.
                reg: puntero a t_socio destino.
Retorno:        OK si la cantidad de campos es correcta, ERROR en caso
                contrario.
Observaciones:  Se pasa por puntero como parsear a leer_archivo_texto.
******************************************************************************/
int texto_a_socio_ALU(const char *linea, void *reg)
{
    char linea_aux[TAM_LINEA];
    char *campos[CAMPOS_SOCIO];
    t_socio *socio = (t_socio *)reg;
    int cant_campos;

    if (!linea || !reg)
    {
        return ERROR;
    }

    copiar_cadena(linea_aux, linea, TAM_LINEA);

    cant_campos = separar_campos_ALU(linea_aux, campos, CAMPOS_SOCIO);

    if (cant_campos != CAMPOS_SOCIO)
    {
        return ERROR;
    }

    if (sscanf(campos[0], "%ld", &socio->dni) != 1)
    {
        socio->dni = 0;
    }

    copiar_cadena(socio->cuil, campos[1], TAM_CUIL);

    copiar_cadena(socio->apellidos_nombres, campos[2], TAM_APYN);

    if (texto_a_fecha(campos[3], &socio->fecha_nacimiento) != OK)
    {
        socio->fecha_nacimiento.dia = 0;
        socio->fecha_nacimiento.mes = 0;
        socio->fecha_nacimiento.anio = 0;
    }

    if (cadena_vacia(campos[4]))
    {
        socio->sexo = '?';
    }
    else
    {
        socio->sexo = campos[4][0];
    }

    if (texto_a_fecha(campos[5], &socio->fecha_afiliacion) != OK)
    {
        socio->fecha_afiliacion.dia = 0;
        socio->fecha_afiliacion.mes = 0;
        socio->fecha_afiliacion.anio = 0;
    }

    copiar_cadena(socio->categoria, campos[6], TAM_CATEGORIA);

    if (texto_a_fecha(campos[7], &socio->fecha_ultima_cuota) != OK)
    {
        socio->fecha_ultima_cuota.dia = 0;
        socio->fecha_ultima_cuota.mes = 0;
        socio->fecha_ultima_cuota.anio = 0;
    }

    socio->estado = 'A';

    copiar_cadena(socio->plan, campos[8], TAM_PLAN);

    copiar_cadena(socio->email_tutor, campos[9], TAM_EMAIL);

    return OK;
}

/******************************************************************************
Descripcion:    aplica las validaciones del punto 6.1 en orden (DNI, Sexo,
                CUIL, Apellidos y Nombres, Fecha de nacimiento, Fecha de
                afiliacion, Categoria, Fecha de ultima cuota, Plan, Email
                tutor). Normaliza apellidos y nombres. Se detiene en el primer
                error.
Parametros:     reg: puntero a t_socio a validar (se modifica al normalizar).
                ctx: contexto con indice, archivo binario, fecha de proceso e
                id_modificando.
                tipo_error: salida; nombre de la validacion que fallo.
Retorno:        OK si el socio es valido, ERROR en caso contrario.
Observaciones:  Si ctx->id_modificando == dni no se considera duplicado y se
                excluye ese DNI al verificar unicidad del email. Cumple con el
                puntero validar de leer_archivo_texto.
******************************************************************************/
int valida_socio_ALU(void *reg, const t_contexto *ctx, char *tipo_error)
{
    t_socio *socio = (t_socio *)reg;
    t_reg_indice reg_busc;
    char cuil_calc[TAM_CUIL];
    int edad_socio;

    if (socio->dni <= DNI_MIN || socio->dni >= DNI_MAX)
    {
        strcpy(tipo_error, "DNI");
        return ERROR;
    }

    if (ctx->id_modificando != socio->dni)
    {
        reg_busc.id = socio->dni;
        if (indice_buscar_ALU(ctx->indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) != NO_EXISTE)
        {
            strcpy(tipo_error, "DNI");
            return ERROR;
        }
    }

    if (socio->sexo != 'H' && socio->sexo != 'M')
    {
        strcpy(tipo_error, "SEXO");
        return ERROR;
    }

    calcular_cuil_ALU(socio->dni, socio->sexo, cuil_calc);
    if (strcmp(socio->cuil, cuil_calc) != 0)
    {
        strcpy(tipo_error, "CUIL");
        return ERROR;
    }

    arreglar_apyn_ALU(socio->apellidos_nombres);
    if (cadena_vacia(socio->apellidos_nombres))
    {
        strcpy(tipo_error, "APELLIDOS_NOMBRES");
        return ERROR;
    }

    if (!fecha_correcta(&socio->fecha_nacimiento))
    {
        strcpy(tipo_error, "FECHA_NACIMIENTO");
        return ERROR;
    }

    edad_socio = edad(&socio->fecha_nacimiento, ctx->fecha_proceso);

    if (edad_socio < EDAD_MINIMA)
    {
        strcpy(tipo_error, "FECHA_NACIMIENTO");
        return ERROR;
    }

    if (!fecha_correcta(&socio->fecha_afiliacion))
    {
        strcpy(tipo_error, "FECHA_AFILIACION");
        return ERROR;
    }

    if (compara_fechas(&socio->fecha_afiliacion, &socio->fecha_nacimiento) <= 0 ||
        compara_fechas(&socio->fecha_afiliacion, ctx->fecha_proceso) > 0)
    {
        strcpy(tipo_error, "FECHA_AFILIACION");
        return ERROR;
    }

    if (strcmp(socio->categoria, CAT_MENOR) != 0 && strcmp(socio->categoria, CAT_ADULTO) != 0)
    {
        strcpy(tipo_error, "CATEGORIA");
        return ERROR;
    }

    if (edad_socio < EDAD_ADULTO && strcmp(socio->categoria, CAT_MENOR) != 0)
    {
        strcpy(tipo_error, "CATEGORIA");
        return ERROR;
    }

    if (edad_socio >= EDAD_ADULTO && strcmp(socio->categoria, CAT_ADULTO) != 0)
    {
        strcpy(tipo_error, "CATEGORIA");
        return ERROR;
    }

    if (!fecha_correcta(&socio->fecha_ultima_cuota))
    {
        strcpy(tipo_error, "FECHA_ULTIMA_CUOTA");
        return ERROR;
    }

    if (compara_fechas(&socio->fecha_ultima_cuota, &socio->fecha_afiliacion) < 0 ||
        compara_fechas(&socio->fecha_ultima_cuota, ctx->fecha_proceso) > 0)
    {
        strcpy(tipo_error, "FECHA_ULTIMA_CUOTA");
        return ERROR;
    }

    if (plan_indice(socio->plan) < 0)
    {
        strcpy(tipo_error, "PLAN");
        return ERROR;
    }

    if (strcmp(socio->categoria, CAT_MENOR) == 0)
    {
        if (cadena_vacia(socio->email_tutor))
        {
            strcpy(tipo_error, "EMAIL_TUTOR");
            return ERROR;
        }
        if (!email_valido_ALU(socio->email_tutor))
        {
            strcpy(tipo_error, "EMAIL_TUTOR");
            return ERROR;
        }
        if (socio_email_existe(ctx->arch_bin, ctx->indice, socio->email_tutor, ctx->id_modificando))
        {
            strcpy(tipo_error, "EMAIL_TUTOR");
            return ERROR;
        }
    }
    else
    {
        if (!cadena_vacia(socio->email_tutor))
        {
            strcpy(tipo_error, "EMAIL_TUTOR");
            return ERROR;
        }
    }

    return OK;
}

/******************************************************************************
Descripcion:    lee el registro nro_reg del archivo binario de socios.
Parametros:     arch: archivo abierto en modo binario.
                nro_reg: numero de registro.
                s: socio destino.
Retorno:        OK si se pudo leer, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_leer_ALU(FILE *arch, unsigned nro_reg, t_socio *s)
{
    if(fseek(arch, nro_reg * sizeof(t_socio), SEEK_SET) != 0)
    {
        return ERROR;
    }

    if(fread(s, sizeof(t_socio), 1, arch) != 1)
    {
        return ERROR;
    }

    return OK;
}

/******************************************************************************
Descripcion:    escribe el socio en la posicion nro_reg del archivo binario y
                hace flush.
Parametros:     arch: archivo abierto en modo de escritura binaria.
                nro_reg: numero de registro.
                s: socio a grabar.
Retorno:        OK si se pudo grabar, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_escribir_ALU(FILE *arch, unsigned nro_reg, const t_socio *s)
{
    if(fseek(arch, nro_reg * sizeof(t_socio), SEEK_SET) != 0)
    {
        return ERROR;
    }

    if(fwrite(s, sizeof(t_socio), 1, arch) != 1)
    {
        return ERROR;
    }

    fflush(arch);
    return OK;
}

/******************************************************************************
Descripcion:    aplica la regla del Anexo A.4: el socio esta al dia si la
                ultima cuota es del mes de la fecha de proceso, o del mes
                anterior cuando el dia de la fecha de proceso es <= 10.
Parametros:     ultima_cuota: fecha de ultima cuota paga.
                fecha_proceso: fecha de proceso.
Retorno:        1 si esta al dia, 0 en caso contrario.
Observaciones:  -
******************************************************************************/
int cuota_al_dia_ALU(const t_fecha *ultima_cuota, const t_fecha *fecha_proceso)
{
        int mes_ant, anio_ant;

    if (ultima_cuota->anio == fecha_proceso->anio && ultima_cuota->mes == fecha_proceso->mes)
    {
        return 1;
    }

    if (fecha_proceso->dia <= DIA_LIMITE_CUOTA)
    {
        mes_ant = fecha_proceso->mes - 1;
        anio_ant = fecha_proceso->anio;

        if (mes_ant == 0) {
            mes_ant = 12;
            anio_ant--;
        }

        if (ultima_cuota->anio == anio_ant && ultima_cuota->mes == mes_ant) {
            return 1;
        }
    }

    return 0;
}

/******************************************************************************
Descripcion:    opcion Alta: carga un socio por teclado, lo valida con
                valida_socio, lo agrega al final de socios.dat con estado 'A'
                y lo inserta en el indice. Si es rechazado lo audita.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
                fecha_proceso: fecha de proceso.
Retorno:        OK si el socio fue agregado, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_alta_manual_ALU(t_indice *indice, const char *path, const t_fecha *fecha_proceso)
{
    t_socio socio;
    char tipo_error[TAM_TIPO_ERROR];
    char buffer[TAM_LINEA], buffer_dni[20];
    t_contexto ctx;
    t_reg_indice nuevo_reg;
    FILE *arch;
    long pos;

    printf("DNI: ");
    leer_linea(buffer_dni, sizeof(buffer_dni));

    if (sscanf(buffer_dni, "%ld", &socio.dni) != 1)
    {
        socio.dni = 0;
    }

    printf("CUIL (XY-NNNNNNNN-Z): ");
    leer_linea(socio.cuil, TAM_CUIL);

    printf("Apellidos y nombres: ");
    leer_linea(socio.apellidos_nombres, TAM_APYN);

    printf("Fecha de nacimiento (DD/MM/AAAA): ");
    leer_linea(buffer, TAM_LINEA); texto_a_fecha(buffer, &socio.fecha_nacimiento);

    printf("Sexo (H/M): ");
    leer_linea(buffer, TAM_LINEA); socio.sexo = buffer[0];

    printf("Fecha de afiliacion (DD/MM/AAAA): ");
    leer_linea(buffer, TAM_LINEA); texto_a_fecha(buffer, &socio.fecha_afiliacion);

    printf("Categoria (MENOR/ADULTO): ");
    leer_linea(socio.categoria, TAM_CATEGORIA);

    printf("Fecha de ultima cuota paga (DD/MM/AAAA): ");
    leer_linea(buffer, TAM_LINEA); texto_a_fecha(buffer, &socio.fecha_ultima_cuota);

    printf("Plan (BASIC/PREMIUM/VIP/FAMILY): ");
    leer_linea(socio.plan, TAM_PLAN);

    printf("Email tutor (vacio si ADULTO, '-' para borrar): ");
    leer_linea(socio.email_tutor, TAM_EMAIL);
    if (strcmp(socio.email_tutor, "-") == 0) socio.email_tutor[0] = '\0';

    socio.estado = 'A';

    arch = fopen(path, "r+b");
    if (!arch) arch = fopen(path, "w+b");

    ctx.indice = indice;
    ctx.arch_bin = arch;
    ctx.fecha_proceso = fecha_proceso;
    ctx.id_modificando = 0;
    if (valida_socio_ALU(&socio, &ctx, tipo_error) != OK)
    {
        printf("Socio RECHAZADO. Validacion fallida: %s\n", tipo_error);
        sprintf(buffer_dni, "%ld", socio.dni);
        auditoria_registrar_ALU(ARCH_ERRORES, buffer_dni, tipo_error, fecha_proceso);
        fclose(arch);
        return ERROR;
    }

    fseek(arch, 0, SEEK_END);
    pos = ftell(arch) / sizeof(t_socio);
    socio_escribir_ALU(arch, pos, &socio);
    fclose(arch);

    nuevo_reg.id = socio.dni;
    nuevo_reg.nro_reg = pos;
    indice_insertar_ALU(indice, &nuevo_reg, sizeof(t_reg_indice), compara_reg_indice_ALU);

    printf("Socio agregado (registro %ld).\n", pos);
    return OK;
}

/******************************************************************************
Descripcion:    opcion Modificacion: pide el DNI, lo busca en el indice (un
                socio dado de baja no se puede modificar), muestra los datos
                actuales, pide los nuevos (Enter conserva), valida y
                sobrescribe el registro. El DNI y el estado no se modifican.
                Si es rechazado lo audita.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
                fecha_proceso: fecha de proceso.
Retorno:        OK si se modifico, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socio_modificacion_manual_ALU(t_indice *indice, const char *path, const t_fecha *fecha_proceso)
{
    long dni;
    t_reg_indice reg_busc;
    t_socio socio;
    t_fecha fecha_aux; /* Variable temporal para capturar fechas */
    char tipo_error[TAM_TIPO_ERROR];
    char buffer[TAM_LINEA], buffer_dni[20];
    t_contexto ctx;
    FILE *arch;
    int pos_idx;

    printf("DNI del socio a modificar: ");
    leer_linea(buffer_dni, sizeof(buffer_dni));

    if (cadena_vacia(buffer_dni) || sscanf(buffer_dni, "%ld", &dni) != 1)
    {
        puts("DNI invalido.");
        return ERROR;
    }

    reg_busc.id = dni;
    pos_idx = indice_buscar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU);
    if (pos_idx == NO_EXISTE)
    {
        printf("El socio no existe o esta dado de baja.\n");
        return ERROR;
    }

    arch = fopen(path, "r+b");
    if (!arch || socio_leer_ALU(arch, reg_busc.nro_reg, &socio) != OK)
    {
        printf("No se pudo leer el registro.\n");
        if (arch) fclose(arch);
        return ERROR;
    }

    /* Uso de la función prehecha de la cátedra para el formato tabular */
    printf("\nDatos actuales:\n");
    socio_mostrar(&socio);

    printf("\n(Enter para conservar el valor actual)\n");

    printf("CUIL (XY-NNNNNNNN-Z) [%s]: ", socio.cuil);
    leer_linea(buffer, TAM_LINEA);
    if (!cadena_vacia(buffer)) copiar_cadena(socio.cuil, buffer, TAM_CUIL);

    printf("Apellidos y nombres [%s]: ", socio.apellidos_nombres);
    leer_linea(buffer, TAM_LINEA);
    if (!cadena_vacia(buffer)) copiar_cadena(socio.apellidos_nombres, buffer, TAM_APYN);

    /* Uso de lee_fecha para manejar nativamente el input o el Enter vacío */
    printf("Fecha de nacimiento [%02d/%02d/%04d]: ", socio.fecha_nacimiento.dia, socio.fecha_nacimiento.mes, socio.fecha_nacimiento.anio);
    if (lee_fecha(&fecha_aux) == OK)
    {
        socio.fecha_nacimiento = fecha_aux;
    }

    printf("Sexo (H/M) [%c]: ", socio.sexo);
    leer_linea(buffer, TAM_LINEA);
    if (!cadena_vacia(buffer)) socio.sexo = toupper(buffer[0]);

    printf("Fecha de afiliacion [%02d/%02d/%04d]: ", socio.fecha_afiliacion.dia, socio.fecha_afiliacion.mes, socio.fecha_afiliacion.anio);
    if (lee_fecha(&fecha_aux) == OK)
    {
        socio.fecha_afiliacion = fecha_aux;
    }

    printf("Categoria (MENOR/ADULTO) [%s]: ", socio.categoria);
    leer_linea(buffer, TAM_LINEA);
    if (!cadena_vacia(buffer)) copiar_cadena(socio.categoria, buffer, TAM_CATEGORIA);

    printf("Fecha de ultima cuota paga [%02d/%02d/%04d]: ", socio.fecha_ultima_cuota.dia, socio.fecha_ultima_cuota.mes, socio.fecha_ultima_cuota.anio);
    if (lee_fecha(&fecha_aux) == OK)
    {
        socio.fecha_ultima_cuota = fecha_aux;
    }

    printf("Plan (BASIC/PREMIUM/VIP/FAMILY) [%s]: ", socio.plan);
    leer_linea(buffer, TAM_LINEA);
    if (!cadena_vacia(buffer)) copiar_cadena(socio.plan, buffer, TAM_PLAN);

    printf("Email tutor (vacio si ADULTO, '-' para borrar) [%s]: ", socio.email_tutor);
    leer_linea(buffer, TAM_LINEA);
    if (!cadena_vacia(buffer))
    {
        if (strcmp(buffer, "-") == 0) socio.email_tutor[0] = '\0';
        else copiar_cadena(socio.email_tutor, buffer, TAM_EMAIL);
    }

    ctx.indice = indice;
    ctx.arch_bin = arch;
    ctx.fecha_proceso = fecha_proceso;
    ctx.id_modificando = socio.dni;

    if (valida_socio_ALU(&socio, &ctx, tipo_error) != OK)
    {
        printf("\nModificacion RECHAZADA. Validacion fallida: %s\n", tipo_error);
        sprintf(buffer_dni, "%ld", socio.dni);
        auditoria_registrar_ALU(ARCH_ERRORES, buffer_dni, tipo_error, fecha_proceso);
        fclose(arch);
        return ERROR;
    }

    socio_escribir_ALU(arch, reg_busc.nro_reg, &socio);
    fclose(arch);

    printf("\nSocio modificado.\n");
    return OK;
}

/******************************************************************************
Descripcion:    baja logica de socio: busca el DNI en el indice, pone estado
                'B' en el registro del archivo y lo elimina del indice.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
Retorno:        OK si se dio de baja, ERROR si no existe o ya estaba dado de
                baja.
Observaciones:  -
******************************************************************************/
int socio_baja_ALU(t_indice *indice, const char *path)
{
    long dni;
    t_reg_indice reg_busc;
    t_socio socio;
    FILE *arch;
    char buffer[TAM_LINEA];

    printf("Datos del socio con DNI que sera dado de baja: ");
    leer_linea(buffer, sizeof(buffer));

    // Si ingresa un Enter vacio o texto, se rechaza
    if (cadena_vacia(buffer) || sscanf(buffer, "%ld", &dni) != 1)
    {
        printf("DNI invalido.\n");
        return ERROR;
    }

    reg_busc.id = dni;

    if(indice_buscar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU) == NO_EXISTE)
    {
        printf("El socio no existe o ya esta dado de baja.\n");
        return ERROR;
    }

    arch = fopen(path, "r+b");
    if (!arch)
    {
        printf("No se pudo abrir %s.\n", path);
        return ERROR;
    }

    if(socio_leer_ALU(arch, reg_busc.nro_reg, &socio) != OK)
    {
        fclose(arch);
        return ERROR;
    }

    socio.estado = 'B';
    if(socio_escribir_ALU(arch, reg_busc.nro_reg, &socio) != OK)
    {
        fclose(arch);
        return ERROR;
    }

    fclose(arch);

    indice_eliminar_ALU(indice, &reg_busc, sizeof(t_reg_indice), compara_reg_indice_ALU);
    printf("Socio %ld (%s) dado de baja.\n", socio.dni, socio.apellidos_nombres);
    return OK;
}

/******************************************************************************
Descripcion:    recorre el indice (ordenado por DNI y solo con activos) y
                lista los socios leyendo cada registro del archivo.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
Retorno:        OK si se pudo abrir el archivo, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socios_listar_ALU(const t_indice *indice, const char *path)
{
    FILE *fp;
    t_reg_indice *arreglo_indice;
    t_socio socio_leido;
    unsigned i;

    // Abrir socios.dat en modo de lectura binaria
    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        printf("\nError al intentar abrir el archivo binario %s.", path);
        return ERROR;
    }

    // Castear vindice, que apunta a void, para que apunte al tipo t_reg_indice
    arreglo_indice = (t_reg_indice *) indice->vindice;

    // Imprimir encabezado
    printf("\n");
    printf("%-9s %-14s %-32s %-10s %-1s %-10s %-7s %-10s %-1s %-8s %s\n",
           "DNI", "CUIL", "APELLIDOS Y NOMBRES",
           "NACIMIENTO", "S", "AFILIACION", "CATEG.",
           "ULT.CUOTA", "E", "PLAN", "EMAIL TUTOR");

    // Recorrer vindice completo
    for(i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        // Saltear al byte físico del archivo socios.dat, aprovechando la utilidad del índice
        fseek(fp, arreglo_indice[i].nro_reg * sizeof(t_socio), SEEK_SET);

        // Leer el registro
        fread(&socio_leido, sizeof(t_socio), 1, fp);

        // Imprimir el registro
        printf("%-9ld %-14s %-32s %02d/%02d/%04d %c %02d/%02d/%04d %-7s %02d/%02d/%04d %c %-8s %s\n",
               socio_leido.dni,
               socio_leido.cuil,
               socio_leido.apellidos_nombres,
               socio_leido.fecha_nacimiento.dia, socio_leido.fecha_nacimiento.mes, socio_leido.fecha_nacimiento.anio,
               socio_leido.sexo,
               socio_leido.fecha_afiliacion.dia, socio_leido.fecha_afiliacion.mes, socio_leido.fecha_afiliacion.anio,
               socio_leido.categoria,
               socio_leido.fecha_ultima_cuota.dia, socio_leido.fecha_ultima_cuota.mes, socio_leido.fecha_ultima_cuota.anio,
               socio_leido.estado,
               socio_leido.plan,
               socio_leido.email_tutor);
    }

    // Imprimir contador final de socios activos
    printf("\nTotal de socios activos: %u\n", indice->cantidad_elementos_actual);

    fclose(fp);
    return OK;
}

/******************************************************************************
Descripcion:    recorre el indice y exporta a un archivo binario los socios
                con plan PREMIUM o VIP y cuota al dia (el estado 'A' lo
                garantiza el indice). Sobrescribe el archivo de exportacion
                anterior.
Parametros:     indice: indice de socios.
                path: ruta de socios.dat.
                path_export: ruta de beta_testers.dat.
                fecha_proceso: fecha de proceso.
Retorno:        OK si se pudo exportar, ERROR en caso contrario.
Observaciones:  -
******************************************************************************/
int socios_exportar_ALU(const t_indice *indice, const char *path, const char *path_export,
                    const t_fecha *fecha_proceso)
{
    FILE *source, *target;
    t_reg_indice *arreglo_indice;
    t_socio socio;
    unsigned i, exportados = 0;

    source = fopen(path, "rb");
    target = fopen(path_export, "wb");

    if(source == NULL)
    {
        printf("Error al abrir el archivo\n");
        return ERROR;
    }

    if(target == NULL)
    {
        printf("Error al abrir el archivo\n");
        fclose(source);
        return ERROR;
    }

    arreglo_indice = (t_reg_indice *)indice->vindice;

    for(i = 0; i < indice->cantidad_elementos_actual; i++)
    {
        if(socio_leer_ALU(source, arreglo_indice[i].nro_reg, &socio) == OK)
        {
            if ((strcmp(socio.plan, "PREMIUM") == 0 || strcmp(socio.plan, "VIP") == 0) &&
                cuota_al_dia_ALU(&socio.fecha_ultima_cuota, fecha_proceso))
            {

                fwrite(&socio, sizeof(t_socio), 1, target);
                exportados++;
            }
        }
    }

    fclose(source);
    fclose(target);
    printf("Socios exportados a %s: %u\n", path_export, exportados);
    return OK;
}

/******************************************************************************
Descripcion:    opcion 2 del menu: submenu para elegir entre alta y
                modificacion manual de socio.
Parametros:     ind_socios: indice de socios.
                fecha_proceso: fecha de proceso.
Retorno:        No posee.
Observaciones:  -
******************************************************************************/
void abm_socio_ALU(t_indice *indice, const t_fecha *fecha_proceso)
{
    char opc;

    // No se limpia la pantalla para conservar el menu principal visible

    const char OPCIONES_ABM[][LENGTH_MENU] =
    {
        "AMV",
        "Alta",
        "Modificacion",
        "Volver"
    };

    // Se delega el formateo y la validacion de entrada a la funcion de la catedra
    printf("\n");
    opc = opcion(OPCIONES_ABM, "Alta / Modificacion de socio", "Ingrese opcion: ");

    if (opc == 'A')
    {
        socio_alta_manual_ALU(indice, ARCH_SOCIOS_BIN, fecha_proceso);
    }
    else if (opc == 'M')
    {
        socio_modificacion_manual_ALU(indice, ARCH_SOCIOS_BIN, fecha_proceso);
    }

    // Si es 'V' o invalida, retorna y el main se encarga de pausar y limpiar
}
