#include <stdio.h>
#include "esp_log.h"
#include "fmc_board.h"
#include "fmc_debug.h"


void debug_log(fmc_data *ptr)
{   
    
    printf("TTL = %lld.%03lld\n", (ptr->ttl_volume)/1000, (ptr->ttl_volume)%1000);
    printf("ACM = %lld.%03lld\n", (ptr->acm_volume)/1000, (ptr->acm_volume)%1000);
    printf("RATE = %0.2f\n", (ptr->rate));
    printf("PULSE = %d\n\n", (ptr->ttl_pulse));
}