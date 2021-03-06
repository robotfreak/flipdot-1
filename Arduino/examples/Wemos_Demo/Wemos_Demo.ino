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
#include <ESP8266WiFi.h>
#include <FLIPDOT.h>
#include <ESPTime.h>
#include <Snake.h>
#include <ESP8266HTTPClient.h>

#define SSID "Mettigel24.de | Get off my LAN"
#define PASSWORD "N00bznet?NoWay!"
//change config in FLipdot.h if this changes!
#define BOARD_WIDTH 95
#define BOARD_HEIGHT 16
FLIPDOT board = FLIPDOT();
ESPTime timer = ESPTime();

/*
const int xPin = A0; //X attach to A0
const int yPin = D1; //Y attach to A1
const int btPin = D0; //Bt attach to digital 7
*/
Snake game = Snake(BOARD_WIDTH,BOARD_HEIGHT);

void setup() {
  Serial.begin(115200);
  WiFi.hostname("Flip-Dot");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  board.init(); //should come after wifi is connected on ESP8266
  board.render_string_small("My IP:");
  delay(1000);
  char ip[15];
  WiFi.localIP().toString().toCharArray(ip,15);
  board.scroll_string_small(ip);
  delay(1500);
}

enum {SHOW_SNAKE, SNAKE_INIT, SHOW_CLOCK, SHOW_SHUTTER, SHOW_TWEET};
int mode = SHOW_CLOCK;

void loop() {
  switch(mode)
  {
    case SNAKE_INIT:
        snake_start_animations();
        break;
    case SHOW_SNAKE:
        play_snake_game();
        break;
    case SHOW_CLOCK:
        show_time();
        break;
    case SHOW_SHUTTER:
        shutter_animation();
        break;
    case SHOW_TWEET:
        show_tweet();
        break;
    default:;
  }
}

/*
void clock_mode() {
  while(true) {
    if(timer.get_second() == 0){
      board.all_off();
    }
    board.render_string_small(timer.getFormattedTime(),0,1, FLIPDOT::ZERO_NONE);
    render_seconds_pixel();
  }
}


void render_seconds_pixel(){
  uint8_t sec = timer.get_second();
  uint8_t row = sec/12; //12 pixel per row
  uint8_t pos_in_row = (sec % 13)*2;
  board.draw_in_internal_framebuffer(1,pos_in_row,10+row);
}*/

void show_time() {
  const char* date = timer.getFormattedDate();//strdup(hour() + ":" + minute() + ":" + second() + " " + day() + "." + month() + "." + year());
  board.scroll_string(date );
  delay(500);
  board.scroll_string_small("Oh it's");
  delay(1000);
  board.render_string_small(timer.getFormattedTime());
  delay(1800);
  board.all_on();
  delay(200);
  board.all_off();
  for (float a = 0; a <= 2 * PI; a += 0.2) {
    int x = 1 + 8 * cos(a);
    int y = 4 + 8 * sin(a);
    board.render_string_small(timer.getFormattedTime(), x, y, FLIPDOT::ZERO_ALL);
    delay(10);
  }
  for (float a = 2 * PI; a >= 0; a -= 0.2) {
    int x = 1 + 8 * cos(a);
    int y = 4 + 8 * sin(a);
    board.render_string_small(timer.getFormattedTime(), x, y, FLIPDOT::ZERO_ALL);
    delay(10);
  }
  board.all_off();
float deg = 0;
float len = strlen(timer.getFormattedTime()) * CHAR_OFFSET_SMALL;
  for (float a = -len; a <= BOARD_WIDTH; a++) {
    board.render_string_small(timer.getFormattedTime(), a,5.5+5.5*sin(deg), FLIPDOT::ZERO_NONE);
    deg += 6;
    delay(50);
  }
  for (float a = BOARD_WIDTH; a >= -len; a-=1) {
    board.render_string_small(timer.getFormattedTime(), a, 5.5+5.5*sin(deg));
    deg -= 6;
    delay(50);
  }
 
  mode = SHOW_TWEET;
}

void play_snake_game(){
  if(game.step(Snake::AUTO)){
    //game.render_frame_buffer();
    board.reset_internal_framebuffer();
    for(unsigned int x = 0; x < BOARD_WIDTH; x++){
      for(unsigned int y = 0; y < BOARD_HEIGHT; y++){
        if(game.pixel_is_set(x,y)){
          board.draw_in_internal_framebuffer(1,(uint8_t)x,(uint8_t)y);
        }
      }
    }
    board.render_internal_framebuffer();
    delay(100);
  } else {
    snake_show_score();
  }
}

void snake_show_score(){
    board.render_string_small("Game",3,1, FLIPDOT::ZERO_ALL);
    board.render_string_small("Over!",1,8, FLIPDOT::ZERO_NONE);
    delay(1500);
    char highscore[5];
    itoa(game.score, highscore, 10);
    board.render_string_small("Score:",0,0, FLIPDOT::ZERO_ALL);
    delay(1000);
    board.render_string_small(highscore,8,8, FLIPDOT::ZERO_NONE);

    mode = SHOW_CLOCK;
}

void snake_start_animations() {
  board.render_string_small("SNAKE",0,4, FLIPDOT::ZERO_ALL);
    delay(1000);
    board.scroll_string("ON FLIPDOT");
    board.all_on();
    delay(300);
    board.all_off();
    delay(300);
    board.all_on();
    delay(100);
    board.all_off();
    board.render_string_small("Let's",0,4, FLIPDOT::ZERO_ALL);
    delay(700);
    board.all_on();
    delay(100);
    board.all_off();
    board.render_string("GO", 3);
    delay(600);
    board.all_on();
    delay(150);
    board.all_off();
    delay(500);
    game.start_game();

    mode = SHOW_SNAKE;
}

void shutter_animation() {
  for (int i = 100; i > 0; i -= 5) {
    board.all_on();
    delay(i);
    board.all_off();
    delay(i);
  }

  board.scroll_string("HPI MAKERKLUB");

  mode = SNAKE_INIT;
}


void show_tweet() {
  /*
  if(WiFi.status() == WL_CONNECTED) {
        
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://blog.fefe.de"); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                delay(500);
                Serial.println("HEY");
                String payload = http.getString();
                Serial.println("NEY");
                int firstNews = payload.indexOf('</image>\n<item>\n<title>');
                int endFirstNews = payload.indexOf('</title>', firstNews + 1 );
                Serial.println(payload);
                payload = payload.substring(firstNews+13, endFirstNews-1);
                delay(500);
                Serial.println(payload.substring(0,50));
                //board.scroll_string(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }*/
    mode = SNAKE_INIT;
}

