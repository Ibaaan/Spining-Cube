#include "pico/stdlib.h"

typedef struct
{
    unsigned int pin;
    bool state;
    bool repeating;
    unsigned int pressedTime;
    unsigned int lastRepeatTime;
    void (*callback)();
} ButtonHandler;

void btn_init(int pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

ButtonHandler create_btn(int pin, void (*callback)()){
    ButtonHandler button_handler;
    button_handler.pin = pin;
    button_handler.callback = callback;
    button_handler.state = false;
    btn_init(pin);
    
    return button_handler;
}

// class ButtonHandler
// {
// public:
//     ButtonHandler(uint pin, void (*callback)())
//         : pin(pin), callback(callback), state(false),
//           pressedTime(0), lastRepeatTime(0), repeating(false)
//     {
//         gpio_init(pin);
//         gpio_set_dir(pin, GPIO_IN);
//         gpio_pull_up(pin);
//     }

//     void update()
//     {
//         bool isPressed = readButton();
//         uint64_t now = millis();

//         if (isPressed && !state)
//         {
//             // Button just pressed
//             state = true;
//             pressedTime = now;
//             lastRepeatTime = now;
//             repeating = false;
//             callback();
//         }
//         else if (isPressed && state)
//         {
//             // Button is being held
//             if (!repeating && (now - pressedTime >= DAS_MS))
//             {
//                 repeating = true;
//                 lastRepeatTime = now;
//                 callback();
//             }
//             else if (repeating && (now - lastRepeatTime >= ARR_MS))
//             {
//                 lastRepeatTime = now;
//                 callback(); // Subsequent repeats
//             }
//         }
//         else if (!isPressed && state)
//         {
//             // Button released
//             state = false;
//             repeating = false;
//         }
//     }

// private:
//     uint pin;
//     bool state;
//     bool repeating;
//     uint64_t pressedTime;
//     uint64_t lastRepeatTime;
//     void (*callback)();

//     uint64_t millis()
//     {
//         return time_us_64() / 1000;
//     }

//     bool readButton()
//     {
//         return gpio_get(pin) == 0;
//     }
// };