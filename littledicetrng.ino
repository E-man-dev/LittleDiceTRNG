#include <OPTIGATrustM.h>
#include <fprint.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define RND_MAXLENGTH     256
#define SUPPRESSCOLLORS
#define optiga_rng_type   OPTIGA_RNG_TYPE_TRNG

const int buttonPin0 = 0; //High by default, near top left corner of screen, back
int buttonState0 = 1;
int lastButtonState0 = 1;
unsigned long lastDebounceTime0 = 0;
const int buttonPin1 = 1; //Low by default, mid screen, get
int buttonState1 = 0;
int lastButtonState1 = 0;
unsigned long lastDebounceTime1 = 0;
const int buttonPin2 = 2; //Low by default, bottom of screen, next
int buttonState2 = 0;
int lastButtonState2 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;

unsigned int stateMachineIsh = 1;
unsigned int biggestState = 6;
unsigned int valueRange[] = {4,6,8,10,12,20,100};
bool badNum = false;

String myString = "";
int thePrintyNum = 0;

// Use dedicated hardware SPI pins
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;




void setup(void) {
  uint32_t ret = 0;

  Serial.begin(9600);
  Serial.print(F("Hello! Feather TFT Test"));

  // turn on backlight
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  Serial.println(F("Initialized"));

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(400);

  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  /*
   * Initialise an OPTIGA™ Trust X Board
   */
  tft.setTextSize(2);
  tft.println("Begin to trust ... ");
  ret = trustM.begin();
  if (ret) {
    tft.println("Failed");
    while (true);
  }
  tft.println("OK");

  /*
   * Speedup the board (from 6 mA to 15 mA)
   */
  tft.println("Setting current limit to 15 mA ... ");
  ret = trustM.setCurrentLimit(8);
  if (ret) {
    tft.println("Failed");
    while (true);
  }
  tft.println("OK");
  
  delay(80);

  // a single pixel
  tft.drawPixel(tft.width() / 2, tft.height() / 2, ST77XX_GREEN);
  delay(80);

  Serial.println("done");
  delay(100);

  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}





void buttonlabels() {
  // 
  tft.fillScreen(ST77XX_BLACK);
  //if (buttonState0 == LOW) {
  //  tft.fillTriangle(1, 10, 20, 1, 20, 20, ST77XX_RED);
  //}
  //else {
  //  tft.fillTriangle(1, 10, 20, 1, 20, 20, ST77XX_WHITE);
  //}
  if (buttonState1 == HIGH) {
    tft.fillCircle(10, 68, 10, ST77XX_RED);
  }
  else {
    tft.fillCircle(10, 68, 10, ST77XX_WHITE);
  }
  //valueRange[];
  if (buttonState2 == HIGH) {
    tft.fillTriangle(1, 115, 1, 135, 20, 125, ST77XX_RED);
  }
  else {
    tft.fillTriangle(1, 115, 1, 135, 20, 125, ST77XX_WHITE);
  }
  tft.setCursor(22, 64);
  tft.print(valueRange[stateMachineIsh]);
  tft.setCursor(22, 120);
  if (stateMachineIsh < biggestState) {
    tft.print(valueRange[stateMachineIsh+1]);
  }
  else {
    tft.print(valueRange[0]);
  }
}





static void output_result(char* tag, uint8_t* in, uint16_t in_len){
  tft.setCursor(0, 32);
  tft.print(in_len);
  tft.print(": ");
  myString = "";
  for (int i = 0; i < in_len; i++) {
    myString += (char)in[i];
  }
  tft.println(myString);
  badNum = true;
  unsigned int theNumberIPick = 0;
  uint8_t inFirst = in[theNumberIPick];
  while (badNum && theNumberIPick < in_len) {
    inFirst = in[theNumberIPick];
    if (valueRange[stateMachineIsh] == 6) {
      if (inFirst > 251) {
        tft.setCursor(0, 90);
        tft.println("Retrying...");
        badNum = true;
      }
      else {
        thePrintyNum = ((inFirst/42)+1);
        tft.setCursor(120, 90);
        tft.println(thePrintyNum);
        badNum = false;
      }
    }
    else if (valueRange[stateMachineIsh] == 8) {
      thePrintyNum = ((inFirst/32)+1);
      tft.setCursor(120, 90);
      tft.println(thePrintyNum);
      badNum = false;
    }
    else if (valueRange[stateMachineIsh] == 4) {
      thePrintyNum = ((inFirst/64)+1);
      tft.setCursor(120, 90);
      tft.println(thePrintyNum);
      badNum = false;
    }
    else if (valueRange[stateMachineIsh] == 12) {
      if (inFirst > 251) {
        tft.setCursor(0, 90);
        tft.println("Retrying...");
        badNum = true;
      }
      else {
        thePrintyNum = ((inFirst/21)+1);
        tft.setCursor(120, 90);
        tft.println(thePrintyNum);
        badNum = false;
      }
    }
    else if (valueRange[stateMachineIsh] == 20) {
      if (inFirst > 239) {
        tft.setCursor(0, 90);
        tft.println("Retrying...");
        badNum = true;
      }
      else {
        thePrintyNum = ((inFirst/12)+1);
        tft.setCursor(120, 90);
        tft.println(thePrintyNum);
        badNum = false;
      }
    }
    else if (valueRange[stateMachineIsh] == 10) {
      if (inFirst > 249) {
        tft.setCursor(0, 90);
        tft.println("Retrying...");
        badNum = true;
      }
      else {
        thePrintyNum = ((inFirst/25)+1);
        tft.setCursor(120, 90);
        tft.println(thePrintyNum);
        badNum = false;
      }
    }
    else if (valueRange[stateMachineIsh] == 100) {
      if (inFirst > 199) {
        tft.setCursor(0, 90);
        tft.println("Retrying...");
        badNum = true;
      }
      else {
        thePrintyNum = ((inFirst/2)+1);
        tft.setCursor(120, 90);
        tft.println(thePrintyNum);
        badNum = false;
      }
    }
    else {
      thePrintyNum = inFirst;
      tft.setCursor(120, 90);
      tft.println(thePrintyNum);
      badNum = false;
    }
    if (badNum) {
      theNumberIPick++;
    }
  }
  if (theNumberIPick == in_len) {
    tft.print(" failed!");
    delay(100);
  }
  
  //HEXDUMP(in, in_len);
}





static void outPrinty() {
  tft.setCursor(0, 32);
  tft.print(16);
  tft.print(": ");
  tft.println(myString);
  tft.setCursor(120, 90);
  tft.println(thePrintyNum);
}





/** The main loop*/
void loop() {

  uint32_t ret = 0;
  uint8_t  cntr = 10;
  uint8_t  *rnd = new uint8_t[RND_MAXLENGTH];
  uint32_t ts = 0;
  
  /* Initialise Memmory Area */
  memset(rnd, 0, RND_MAXLENGTH);
  
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);

  // Get readings
  int reading0 = digitalRead(buttonPin0);
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);

  // Button debouncing
  if (reading0 != lastButtonState0) {
    // reset the debouncing timer
    lastDebounceTime0 = millis();
  }
  if (reading1 != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime0 = millis();
  }
  if (reading2 != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime0 = millis();
  }

  if ((millis() - lastDebounceTime0) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading0 != buttonState0) {
      buttonState0 = reading0;

      // only toggle the LED if the new button state is HIGH
      if (buttonState0 == LOW) {
      //  ledState = !ledState;
        if (stateMachineIsh == 0) {
          stateMachineIsh = biggestState;
        }
        else {
          stateMachineIsh = stateMachineIsh - 1;
        }
      }
    }
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading1 != buttonState1) {
      buttonState1 = reading1;

      if (buttonState1 == HIGH) {
        ts = millis();
  
        ret = trustM.getRandom(16, rnd);//OPTIGA_RNG_TYPE_TRNG
        ts = millis() - ts;
        if (ret) {
          tft.println("Failed");
          while (true);
        }
        trustM.getRandom(16, rnd);//OPTIGA_RNG_TYPE_TRNG
        output_result("Random", rnd, 16);
      }
    }
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == HIGH) {
        if (stateMachineIsh == biggestState) {
          stateMachineIsh = 0;
        }
        else {
          stateMachineIsh++;
        }
      }
    }
  }

  buttonlabels();

  outPrinty();

  tft.setCursor(50, 60);
  delay(100);

  lastButtonState0 = reading0;
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
}
