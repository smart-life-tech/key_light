#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// Pin Definitions
#define TOUCH_SENSOR_PIN 2
#define BUZZER_PIN 9

// Neopixel Rings
#define RING1_PIN 6
#define RING2_PIN 7
#define RING3_PIN 8
#define NUM_PIXELS_RING1 16 // Adjust to the number of LEDs in Ring 1
#define NUM_PIXELS_RING2 24 // Adjust to the number of LEDs in Ring 2
#define NUM_PIXELS_RING3 24 // Adjust to the number of LEDs in Ring 3

// DFPlayer Mini
#define DFPLAYER_RX 10
#define DFPLAYER_TX 11

// Initialize Neopixel
Adafruit_NeoPixel ring1 = Adafruit_NeoPixel(NUM_PIXELS_RING1, RING1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring2 = Adafruit_NeoPixel(NUM_PIXELS_RING2, RING2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring3 = Adafruit_NeoPixel(NUM_PIXELS_RING3, RING3_PIN, NEO_GRB + NEO_KHZ800);

// Initialize DFPlayer Mini
SoftwareSerial dfplayerSerial(DFPLAYER_RX, DFPLAYER_TX);

void setup()
{
    pinMode(TOUCH_SENSOR_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize Neopixels
    ring1.begin();
    ring2.begin();
    ring3.begin();
    ring1.show(); // Initialize all pixels to 'off'
    ring2.show();
    ring3.show();

    // Initialize DFPlayer Mini
    dfplayerSerial.begin(9600);
    mp3_set_serial(dfplayerSerial); // Set serial for DFPlayer
    mp3_set_volume(20);             // Set volume (0 to 30)
}

void loop()
{
    // Check if the touch sensor is triggered
    if (digitalRead(TOUCH_SENSOR_PIN) == LOW)
    {
        // Play sound
        mp3_play(1); // Play the first sound file on the SD card

        // Turn on Buzzer
        digitalWrite(BUZZER_PIN, HIGH);

        // Change LED colors to green
        setRingColor(ring1, ring1.Color(0, 255, 0)); // Green
        setRingColor(ring2, ring2.Color(0, 255, 0)); // Green
        setRingColor(ring3, ring3.Color(0, 255, 0)); // Green
        delay(2000);                                 // Hold green for 2 seconds

        // Strobe effect for the lower RGB light
        for (int i = 0; i < 5; i++)
        {
            setRingColor(ring3, ring3.Color(255, 255, 255)); // White
            delay(100);
            setRingColor(ring3, ring3.Color(0, 0, 0)); // Off
            delay(100);
        }

        // Turn off Buzzer
        digitalWrite(BUZZER_PIN, LOW);
    }
    else
    {
        // Pulse whitish-blue when not triggered
        pulseEffect(ring1, 150, 150, 255, 0.02); // Adjust colors and speed as needed
        pulseEffect(ring2, 150, 150, 255, 0.02);
        pulseEffect(ring3, 150, 150, 255, 0.02);
    }
}

void setRingColor(Adafruit_NeoPixel &ring, uint32_t color)
{
    for (int i = 0; i < ring.numPixels(); i++)
    {
        ring.setPixelColor(i, color);
    }
    ring.show();
}

void pulseEffect(Adafruit_NeoPixel &ring, int red, int green, int blue, float speed)
{
    for (int brightness = 0; brightness < 256; brightness++)
    {
        setRingColor(ring, ring.Color(red * (brightness / 255.0), green * (brightness / 255.0), blue * (brightness / 255.0)));
        delay(speed * 10);
    }
    for (int brightness = 255; brightness >= 0; brightness--)
    {
        setRingColor(ring, ring.Color(red * (brightness / 255.0), green * (brightness / 255.0), blue * (brightness / 255.0)));
        delay(speed * 10);
    }
}
