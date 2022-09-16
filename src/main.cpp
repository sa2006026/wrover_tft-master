#include <Arduino.h>
#include "uFire_SHT20.h"
uFire_SHT20 sht20;
#include <SimpleKalmanFilter.h>
#include "Wire.h"
#include <TFT_eSPI.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;
/** TFT_eSPI class for display */
TFT_eSPI tft = TFT_eSPI();
/** Sprite needed for graph scrolling */
TFT_eSprite graph1 = TFT_eSprite(&tft);
/** Sprite needed for graph scrolling */
// TFT_eSprite graph2 = TFT_eSprite(&tft);
/** Value for graph1 */
// int graph1Val = 1;
/** Value for graph2 */
// int graph2Val = 1;
SimpleKalmanFilter kf = SimpleKalmanFilter(0.5, 0.5, 0.01);

// #define FRAME_TOPX    5
// #define FRAME_TOPY    25
// #define FRAME_WIDTH   230
// #define FRAME_HEIGHT  110
float HighY = 60;
float LowY = 40;

void setup() {
  Wire.begin();
  sht20.begin();
  tft.init();
  tft.fillScreen(TFT_BLACK);
  graph1.createSprite(200, 100);
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  // tft.fillRect(FRAME_TOPX-1, FRAME_TOPY-1, 
  //                 FRAME_WIDTH+2, FRAME_HEIGHT+2, TFT_WHITE);
  // tft.fillRect(FRAME_TOPX, FRAME_TOPY, 
  //                 FRAME_WIDTH, FRAME_HEIGHT, TFT_RED);


  //graph1.fillSprite(TFT_BLACK); // Optional, filled black by default
  //graph1.setScrollRect(0,0,200,100,TFT_); // Optional as these are defaults on creation of sprite

  //graph2.createSprite(128, 64);
  //graph2.fillSprite(TFT_BLACK); // Optional, filled black by default
  //graph2.setScrollRect(0, 0, 128, 64, TFT_BLACK); // Optional as these are defaults on creation of sprite

}

void loop() {
  float T = millis();
  float H = sht20.humidity();
  float F = kf.updateEstimate(H);
  float adc0 = ads.readADC_SingleEnded(0);
  // float adc1 = ads.readADC_SingleEnded(1);
  // float adc2 = ads.readADC_SingleEnded(2);
  // float adc3 = ads.readADC_SingleEnded(3);


  if(T>2000){

  

    if(F>HighY){
      
      // LowY = LowY +20;
      // if(F<HighY){
       
      graph1.scroll(-1,50);
      HighY = HighY +20;
      // LowY = LowY +20;
    }
    else{
        graph1.drawFastVLine(199,100-100*((F-LowY)/(HighY-LowY)),3,TFT_YELLOW);
        graph1.scroll(-1);
      }
    if(HighY == 80 && F<60){
    
        graph1.scroll(-1,-50);
        HighY = HighY -20;
      
    }
      
      // LowY = LowY +20;
      // if(F<HighY){
       

      // }
    
    graph1.pushSprite(20, 32);
     // Move sprite content 1 pixel left. Default dy is 0
    
  // graph2.scroll(1); // Move sprite content 1 pixel right. Default dy is 0
  // graph2.drawFastVLine(0,64-graph2Val,graph2Val,TFT_RED);
    
  }
  tft.drawString("Humidity",1, 5,2);
  tft.drawFloat(float(HighY),0,1, 32,1);
  tft.drawFloat(float(LowY),0,10, 132,1);
  // graph2.pushSprite(0, 96);
  tft.drawFloat(float(F), 1, 70, 180, 6);
  tft.drawFloat(float(adc0), 1, 70, 260, 1);
  

  tft.drawString("King's ",1, 250,2);
  tft.drawString("Technologies",1,270,1);
  tft.drawString("Phase",1,280,2);
  

  //graph1Val++;
  // graph2Val++;
  //if (graph1Val == 65) graph1Val = 1;
  // if (graph2Val == 65) graph2Val = 1;
  
}