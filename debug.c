#include "ws2812.pio.h"

void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

void put_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t mask = (red << 16) | (green << 8) | (blue << 0);
    //uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
    put_pixel(mask);
}

void init_rgb(){
	PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    uint8_t cnt = 0;
    ws2812_program_init(pio, sm, offset, 22, 800000, true);
    
}