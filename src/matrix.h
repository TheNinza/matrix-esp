#ifndef MATRIX_H
#define MATRIX_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * Matrix - A class that creates a Matrix-style rain effect on an SSD1306 OLED
 * display
 *
 * This creates a grid of random ASCII characters with "shadows" moving down
 * each column, creating a Matrix-like digital rain effect.
 */
class Matrix {
  private:
	// Display character constants
	static const uint8_t CHAR_HEIGHT = 8;
	static const uint8_t CHAR_WIDTH = 6;
	static const uint8_t ANIMATION_FPS = 60;

	// Display reference and properties
	Adafruit_SSD1306 &display;
	uint8_t displayHeight;
	uint8_t displayWidth;

	// Character grid properties
	uint8_t numRows;
	uint8_t numColumns;
	uint8_t columnSpacing;

	// Character grid storage
	char **characters;

	// Shadow effect data structure
	struct Shadow {
		uint8_t col;			   // Column index
		int16_t posY;			   // Vertical position (can be negative)
		uint8_t length;			   // Length in pixels
		uint8_t speed;			   // Speed in pixels per second
		float positionAccumulator; // Sub-pixel position accumulator
		int16_t
			lastCharChangePos; // Last position where characters were updated

		bool shouldReset = false; // Flag for when shadow needs to be reset
	};

	// Array of shadows (one per column)
	Shadow *shadows;

	// Animation timing
	unsigned long lastFrameTime;

  public:
	/**
	 * Constructor
	 * @param disp Reference to the display
	 * @param height Height of the display in pixels
	 * @param width Width of the display in pixels
	 */
	Matrix(Adafruit_SSD1306 &disp, uint8_t height, uint8_t width);

	/**
	 * Destructor - frees allocated memory
	 */
	~Matrix();

	/**
	 * Update the matrix animation
	 * This handles timing, redrawing characters, and updating shadows
	 */
	void update();

	/**
	 * Reset the matrix to its initial state
	 * Initializes characters and shadows
	 */
	void reset();

  private:
	/**
	 * Draw all characters to the display
	 */
	void drawCharacters();

	/**
	 * Reset any shadows that have moved off screen
	 */
	void resetShadows();

	/**
	 * Check if any shadows are visible on screen and force reset if needed
	 */
	void checkShadowVisibility();

	/**
	 * Reset individual shadows that are marked for resetting
	 */
	void resetMarkedShadows();

	/**
	 * Update and draw the shadows based on elapsed time
	 * @param timeDelta Time in milliseconds since last update
	 */
	void updateShadows(unsigned long &timeDelta);

	/**
	 * Update position of a single shadow based on elapsed time
	 * @param shadow Reference to the shadow to update
	 * @param timeSeconds Time in seconds since last update
	 */
	void updateShadowPosition(Shadow &shadow, float timeSeconds);

	/**
	 * Draw a single shadow and update characters beneath it
	 * @param shadow Reference to the shadow to draw
	 */
	void drawShadowAndUpdateCharacters(Shadow &shadow);
};

#endif