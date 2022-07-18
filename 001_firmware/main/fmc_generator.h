// Este generado de pulsos se usa para simular pulsos a la entrada del front-end
// su uso principal esta asociado a la etapa de desarrollo y debugger
//

#ifndef FMC_GENERATOR_H
#define FMC_GENERATOR_H

#include "esp_timer.h"

#define GENERATOR_CYCLES_PERPETUAL 0XFFFFFFFF // Salida perpetua hasta orden de esto

esp_timer_handle_t generator_init();
void generator_start(esp_timer_handle_t , uint32_t , uint32_t );


#endif