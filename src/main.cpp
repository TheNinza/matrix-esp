#include "matrix.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

// Display constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

// Display instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Matrix instance
Matrix *matrix = nullptr;

void cleanup() {
	if (matrix != nullptr) {
		delete matrix;
		matrix = nullptr;
	}
}

void setup() {
	// Initialize serial communication
	Serial.begin(115200);

	// Initialize the display
	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
		Serial.println("SSD1306 allocation failed");
		for (;;)
			; // Don't proceed, loop forever
	}

	// Setup display parameters
	display.setRotation(2); // 180 degree rotation
	display.setTextSize(1);

	// Seed the random number generator with noise from analog input
	randomSeed(analogRead(0));

	// Cleanup any existing matrix instance and create a new one
	cleanup();
	matrix = new Matrix(display, SCREEN_HEIGHT, SCREEN_WIDTH);
}

void loop() {
	if (matrix != nullptr) {
		matrix->update();
	}

	// a small delay of 4ms to prevent excessive CPU usage.
	// note this must be smaller than 1000 / FPS ms.
	delay(4);
}
