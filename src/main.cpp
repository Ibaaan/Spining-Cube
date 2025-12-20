#include <stdio.h>
#include "pico/stdlib.h"
#include "st7735.h"

#define RK_UP 15
#define RK_DOWN 17
#define RK_RIGHT 10
#define RK_LEFT 11

#define DAS_MS 14
#define ARR_MS 1



extern "C"
{
#include "character.h"
}

Character player;

void on_rk_up_button_pressed()
{
    printf("rk_up");
}

int main()
{
    stdio_init_all();

    init_character(player);

    bool switch1 = false;

    // ButtonHandler bt_rk_up(RK_UP, on_rk_up_button_pressed);

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);
    while (true)
    {

        switch1 = switch1 ? false : true;
        sleep_ms(1000);
            printf("1");
        ST7735_Update();
        // bt_rk_up.update();
    }

    return 0;
}

