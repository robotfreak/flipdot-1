// - - - - -
// A demo Application for our little Arduino Library. Please copy the FLIPDOT folder into your Arduino/libraries folder to use the lib
//
//
// Copyright (C) 2016 fluepke, kryptokommunist <fabian.luepke@student.hpi.de>,<marcus.ding@student.hpi.de>
// This work is licensed under a GNU style license.
//
// Last change: kryptokommunist
//
// Documentation and samples are available at https://github.com/nerdmanufaktur/flipdot
// - - - -

#include "FLIPDOT.h"

FLIPDOT *board = new FLIPDOT();

void setup() {
  Serial.begin(115200);
  board->init();
}

uint16_t framebuffer[25] = {0,0b0000001110000000,0b0000011111000000,0b0000111111100000,0b0001111111110000,0b0011111111111000,0b0011111111111000,0b0011111111111000,
0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,0b0011111111111000,
0b0011111111111000,0b0001111111110000,0b0000111111100000,0b0000011111000000,0b0000001110000000,0};

uint16_t framebuffer2[25] = {0,0,0,0,0,0b1111111111111111,0b1111111111111111,0b1111111111111111,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b0000001110000000,0b1111111111111111,0b1111111111111111,0b1111111111111111,0,0,0,0,0};

void loop() {
for(int i = 0; i < 20; i++) {
    board->render_frame(framebuffer);
    delay(i*5);
    board->render_frame(framebuffer2);
    delay(i*5);
}
board->scroll_string("Hello World!\0",40);
delay(500);
board->scroll_string("I LOVE FLIP-DOTS MORE THAN $$!!\0",0);
delay(1000);
board->render_string("13:42",2);
delay(1000);
board->all_off();
delay(100);
board->render_string("13:43",2);
delay(1000);
board->all_off();
delay(100);
board->render_string("13:43",2);
delay(1000);
board->all_off();
delay(100);
board->render_string("12:00",2);
delay(1000);
board->all_off();
delay(100);
board->render_string("66:00",2);
delay(1000);
}

