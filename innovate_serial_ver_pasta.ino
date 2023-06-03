/*
innovate serial monitor   2022/10/31

LILYGO RP2040 + Tdisplay(7789V)     RS232 to TTL    φ2.5　Stereo
                          GND ->    GND         ->  GND
                          13  ->    RXD         ->  Second
                                                    Top     (NoConnect)
                          3V3 ->    VCC
*/

#include <TFT_eSPI.h>

#define TFT_BL 4
#define BOTTON1 6
#define BOTTON2 7
#define PWR_ON 22
#define RedLED 25

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

int d[5];                                       //bufa
int c = 0;                                      //serial read
int ct;                                         //count
int af;                                         //ex.147
int af1;                                        //ex. 14.
int af2;                                        //ex.    7


void setup() {

  pinMode(PWR_ON, OUTPUT);    digitalWrite(PWR_ON, 1);
  pinMode(TFT_BL, OUTPUT);    analogWrite(TFT_BL, 0);
  pinMode(RedLED, OUTPUT);

//  Serial1.setTX(12);
  Serial1.setRX(13);
  Serial1.begin(19200);

  tft.init();
  tft.setRotation(1);                     // usb right
  // tft.setRotation(3);                  // usb left
  // tft.setRotation(0);                  // usb under

  delay(25);
  analogWrite(TFT_BL, 255);               //Back Light

  img.createSprite(270, 135);             //lcd　横
  //  img.createSprite(135, 240);         //lcd　縦
  img.fillSprite(TFT_BLACK);
  img.pushSprite(0, 0);
  img.setCursor (0, 0);
  img.setTextSize(2);
  img.setTextFont(4);
  img.setTextColor(0xc618);

//-----------------------------------initial
    img.setCursor (0, 20);
    img.println("You");
    img.pushSprite(0, 0);
    delay (800);    
    
    img.setCursor (0, 20);
    img.println("You already ");
    img.pushSprite(0, 0);
    delay (800);
    
    img.setCursor (110, 80);
      img.setTextColor(random(TFT_WHITE));
    img.println("died !");
    img.pushSprite(0, 0);
      img.setTextColor(0xc618);
    delay (4000);
    
    img.fillSprite(TFT_BLACK);
    img.pushSprite(0, 0);

    delay (3000);

    img.setCursor (0, 20);
    img.println("You");
    img.pushSprite(0, 0);    
    delay (800); 

    img.setCursor (0, 20);
    img.println("You = ");
    img.pushSprite(0, 0);  
    delay (800); 


    delay (1500);
    img.setCursor (60, 80);
      img.setTextColor(random(TFT_WHITE));
    img.println("shock!!");
    img.pushSprite(0, 0);
    delay (3000);
    
    img.setTextColor(0xc618);
    img.fillSprite(TFT_BLACK);


//----------------------------------------
  if (c == 0) {
    img.setCursor (0, 20);
    img.println("Ready to");
    img.println("   connect");
    img.pushSprite(0, 0);
    delay (500);
    img.fillSprite(TFT_BLACK);
  }

}


void loop() {


  if (!digitalRead(BOTTON1)) {                        //IO6 botton
    img.fillSprite(TFT_BLACK);
    img.pushSprite(0, 0);
    img.setCursor (0, 0);
  }

  if (!digitalRead(BOTTON2)) {                        //IO7 botton
    img.setTextColor(random(TFT_WHITE));              //color change
  }

  //------------------------------------- serial read
  if ( Serial1.available() )
  {
    c = Serial1.read();

    if (ct == 4) {
      d[4] = c;                           //ex. 116   deta4
      ct = 5;
    }
    if (ct == 3) {
      d[3] = c;                           //ex. 3     deta3
      ct = 4;
    }
    if (ct == 2) {
      d[2] = c;                           //ex. 19    deta2
      ct = 3;
    }
    if (ct == 1) {
      d[1] = c;                           //ex. 67    deta1
      ct = 2;
    }
    if (c == 130) {
      ct = 1;                            //          header
    }
  }




  //------------------------------------------------- calc
  if (ct == 5) {

    af = (((128 * d[3] + d[4]) + 500) * (128 + 19)) / 1000;             //ex.147 
                                                                        //gasoline d[1]67 d[2]19
    if (af > 222) {
      af = 222;
    }
    if (af < 74) {
      af = 74;
    }

    af1 = af / 10;                                  //ex.14
    af2 = af - (af1 * 10);                          //ex.  7

    if (d[1]==70 or d[1]==71) {                                  //lean
      af1=22;af2=2;
    }


    //---------------------------------------------------- disp


    if (d[1]==83 or d[1]==87) {                         //warm up
      while (Serial1.available())Serial1.read();        //waste serial data
      img.fillSprite(TFT_BLACK);
      img.setTextSize(2);
      img.setTextFont(4);
      img.setCursor (0, 20);
      img.println("Warm up");
      img.println("       now");
      img.pushSprite(0, 0);
      delay (700);
      img.fillSprite(TFT_BLACK);
      img.pushSprite(0, 0);
      delay (400);
      goto NEXT;
    }

    if (d[1]==67 or d[1]==70 or d[1]==71) {           //display af
      img.fillSprite(TFT_BLACK);
      img.setTextFont(7);                             //only number
      img.setTextSize(2);
      img.setCursor (203, 40);
      img.print(af2);                                 //ex .7

      img.setTextSize(3);
      img.setCursor (-22, -3);
      if (af1 < 10) {
        img.setCursor (74, -3);
      }                                               //init
      img.print(String(af1) + ".");                   //ex 14.
      img.pushSprite(-22, -3);
      goto NEXT;
    }


    /*------------------------------------ test
      img.fillSprite(TFT_BLACK);
      img.setCursor (0, 0);
      img.setTextSize(1);
      img.setTextFont(2);
      img.println((String)d[1] + " " + d[2] + " " + d[3] + " " + d[4]+ " "+d[5]);
      img.pushSprite(0, 0);
    *///------------------------------------

    if (d[1]!=67) {                                   //error
      while (Serial1.available())Serial1.read();      //waste serial data
      img.fillSprite(TFT_BLACK);
      img.setTextSize(2);
      img.setTextFont(4);
      img.setCursor (0, 20);
      img.println("error");
      img.println((String)" "+d[1] + " " + d[2]);
      img.pushSprite(0, 0);
      delay (500);
      img.fillSprite(TFT_BLACK);
      img.pushSprite(0, 0);
      delay (300);
    }

    
NEXT:
    ct = 0;
    digitalWrite(RedLED, !digitalRead(RedLED));
  }

}
