
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansOblique9pt7b.h>
#include <RotaryEncoder.h>

// 128 by 32
#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 32 // OLED display height
#define PWM_pin 7        // fan pwm

#define ROT_CLK 5
#define ROT_DT 6

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int speed = 0; // fanspeed
int state_CLK; // CLK (A) state of the rotary encoder
int state_DT;  // DT (B) state of the rotary encoder
int pstate_CLK = 0;

RotaryEncoder encoder(ROT_CLK, ROT_DT, RotaryEncoder::LatchMode::TWO03);
void setup()
{
  Serial.begin(115200);

  pinMode(PWM_pin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.setFont(&FreeSansOblique9pt7b);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  display.println("Success!");
  display.display();
}

void loop()
{
  encoder.tick();
  state_CLK = encoder.getPosition();
  if (state_CLK != pstate_CLK)
  {
    if (encoder.getDirection() == RotaryEncoder::Direction::CLOCKWISE)
    {
      speed++;
    }
    else
    {
      speed--;
    }
    // bound speed between 0 and 5
    if (speed > 5)
    {
      speed = 0;
    }
    else if (speed < 0)
    {
      speed = 5;
    }

    pstate_CLK = state_CLK;
  }

  display.clearDisplay();
  // Indicator 5 steps
  display.drawRect(0, 0, 128, 20, WHITE);
  for (int i = 0; i < speed; i++)
  {
    display.fillRect(118 / 5 * i + 5, 5, 128 / 5, 10, WHITE);
  }

  analogWrite(PWM_pin, 255 / 5 * speed); // Adjust fan speed
  display.display();
}