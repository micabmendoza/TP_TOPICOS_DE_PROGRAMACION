#ifndef AUDITORIA_H_INCLUDED
#define AUDITORIA_H_INCLUDED
#define TAM_TIPO_ERROR 25
#include "fecha.h"

int auditoria_registrar(const char *path, const char *dni_texto,
                        const char *tipo_error, const t_fecha *fecha_proceso);
int auditoria_mostrar(const char *path);

/*============================================================================*/
/*            FUNCIONES A IMPLEMENTAR POR EL ALUMNO (version _ALU)             */
/*============================================================================*/
typedef struct {
    char dni[25];
    char tipo_error[TAM_TIPO_ERROR];
    int cantidad;
} t_incidencia;

int auditoria_registrar_ALU(const char *path, const char *dni_texto,
                        const char *tipo_error, const t_fecha *fecha_proceso);
int auditoria_mostrar_ALU(const char *path);

#endif /* AUDITORIA_H_INCLUDED */
