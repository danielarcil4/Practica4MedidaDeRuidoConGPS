#ifndef __BUTTON_H__
#define __BUTTON_H__


// Inicializa el botón (GPIO, interrupción o timer si es necesario)
void init_button(void);

// Llama periódicamente en el loop principal o por timer para hacer debounce
void button_update(void);

// Devuelve true si el botón fue presionado (flanco descendente)
void button_pressed(void);

#endif // __BUTTON_H__