#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdbool.h>

// Inicializa el botón (GPIO, interrupción o timer si es necesario)
void init_button(void);

// Devuelve true si el botón fue presionado (flanco descendente)
bool button_pressed(void);

#endif // __BUTTON_H__