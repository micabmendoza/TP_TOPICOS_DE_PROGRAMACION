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
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return auditoria_registrar(path, dni_texto, tipo_error, fecha_proceso);
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
    /* TODO: reemplazar esta llamada a la funcion de la catedra por su propia
       implementacion. En la entrega no puede quedar ninguna llamada a una
       funcion de la biblioteca que tenga version _ALU. */
    return auditoria_mostrar(path);
}
