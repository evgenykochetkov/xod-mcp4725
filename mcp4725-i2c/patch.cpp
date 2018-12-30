/*
This is an adaptation of Adafruit_MCP4725 library(https://github.com/adafruit/Adafruit_MCP4725).
Tested and works great with the Adafruit MCP4725 Breakout Board ------> http://www.adafruit.com/products/935
Adafruit invests time and resources providing that open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

License: BSD
*/

{{#global}}
#include <Wire.h>
{{/global}}

struct State {
    bool begun;
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    State* state = getState(ctx);
    if (!state->begun) {
        Wire.begin();
        state->begun = true;
    }

    uint8_t addr = getValue<input_ADDR>(ctx);
    uint16_t output = 4095 * getValue<input_VAL>(ctx);

#ifdef TWBR
    uint8_t twbrback = TWBR;
    TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz
#endif
    Wire.beginTransmission(addr);
    if (getValue<input_WE>(ctx)) {
        // Writes data to the DAC and the EEPROM (persisting the assigned value after reset)
        Wire.write(0x60);
    } else {
        // Writes data to the DAC
        Wire.write(0x40);
    }
    Wire.write(output / 16);        // Upper data bits (D11.D10.D9.D8.D7.D6.D5.D4)
    Wire.write((output % 16) << 4); // Lower data bits (D3.D2.D1.D0.x.x.x.x)
    Wire.endTransmission();
#ifdef TWBR
    TWBR = twbrback;
#endif
}
