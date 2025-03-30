#include "matrix.h"
#include <Arduino.h>

Matrix::Matrix(Adafruit_SSD1306 &disp, uint8_t height, uint8_t width)
	: display(disp), displayHeight(height), displayWidth(width) {

	numRows = height / CHAR_HEIGHT;
	columnSpacing = CHAR_WIDTH / 3;
	numColumns = width / (CHAR_WIDTH + columnSpacing);

	characters = new char *[numRows];
	for (uint8_t row = 0; row < numRows; row++) {
		characters[row] = new char[numColumns];
	}

	shadows = new Shadow[numColumns];
	lastFrameTime = millis();

	reset();
}

Matrix::~Matrix() {
	for (uint8_t row = 0; row < numRows; row++) {
		delete[] characters[row];
	}
	delete[] characters;
	delete[] shadows;
}

void Matrix::reset() {
	display.setTextColor(SSD1306_WHITE);

	// Initialize characters
	for (uint8_t i = 0; i < numRows; i++) {
		for (uint8_t j = 0; j < numColumns; j++) {
			characters[i][j] = random(33, 126);
		}
	}

	// Initialize shadows with staggered positions
	for (uint8_t col = 0; col < numColumns; col++) {
		shadows[col].col = col;
		shadows[col].length = random(CHAR_HEIGHT, displayHeight * 0.9);

		// Stagger starting positions
		int staggerType = col % 3;
		if (staggerType == 0) {
			shadows[col].posY = -shadows[col].length + CHAR_HEIGHT;
		} else if (staggerType == 1) {
			shadows[col].posY = -shadows[col].length;
		} else {
			shadows[col].posY = -shadows[col].length - CHAR_HEIGHT * 2;
		}

		shadows[col].speed = random(40, 80);
		shadows[col].shouldReset = false;
		shadows[col].positionAccumulator = 0.0f;
		shadows[col].lastCharChangePos = shadows[col].posY;
	}
}

void Matrix::update() {
	unsigned long currentTime = millis();
	unsigned long timeDelta;

	if (currentTime < lastFrameTime) {
		// Handle millis() overflow
		timeDelta = (0xFFFFFFFF - lastFrameTime) + currentTime + 1;
	} else {
		timeDelta = currentTime - lastFrameTime;
	}

	// If the time delta is less than the frame time, don't update
	if (timeDelta < (1000 / ANIMATION_FPS)) {
		return;
	}

	display.clearDisplay();

	resetShadows();
	drawCharacters();
	updateShadows(timeDelta);

	display.display();
	lastFrameTime = currentTime;
}

void Matrix::drawCharacters() {
	for (uint8_t i = 0; i < numRows; i++) {
		for (uint8_t j = 0; j < numColumns; j++) {
			uint8_t x = j * (CHAR_WIDTH + columnSpacing);
			uint8_t y = i * CHAR_HEIGHT;
			display.drawChar(x, y, characters[i][j], SSD1306_WHITE,
							 SSD1306_BLACK, 1, 1);
		}
	}
}

void Matrix::resetShadows() {
	checkShadowVisibility();
	resetMarkedShadows();
}

void Matrix::checkShadowVisibility() {
	static unsigned long lastCheckTime = 0;
	unsigned long currentTime = millis();

	// Periodically check if shadows are visible
	if (currentTime - lastCheckTime > 1000) {
		bool anyVisible = false;

		for (uint8_t col = 0; col < numColumns; col++) {
			if (shadows[col].posY > -shadows[col].length * 2) {
				anyVisible = true;
				break;
			}
		}

		// Force reset some shadows if none are visible
		if (!anyVisible) {
			for (uint8_t col = 0; col < numColumns; col += 2) {
				shadows[col].shouldReset = true;
			}
		}

		lastCheckTime = currentTime;
	}
}

void Matrix::resetMarkedShadows() {
	// Reset shadows that need it
	for (uint8_t col = 0; col < numColumns; col++) {
		if (!shadows[col].shouldReset) {
			continue;
		}

		shadows[col].length = random(CHAR_HEIGHT, displayHeight * 0.9);
		shadows[col].posY = -shadows[col].length;
		shadows[col].speed = random(40, 80);
		shadows[col].shouldReset = false;
		shadows[col].positionAccumulator = 0.0f;
		shadows[col].lastCharChangePos = shadows[col].posY;
	}
}

void Matrix::updateShadows(unsigned long &timeDelta) {
	const float timeSeconds = timeDelta / 1000.0f;

	for (uint8_t col = 0; col < numColumns; col++) {
		updateShadowPosition(shadows[col], timeSeconds);
		drawShadowAndUpdateCharacters(shadows[col]);
	}
}

void Matrix::updateShadowPosition(Shadow &shadow, float timeSeconds) {
	// Update shadow position
	shadow.positionAccumulator += shadow.speed * timeSeconds;
	int16_t pixelsToMove = (int16_t)shadow.positionAccumulator;

	if (pixelsToMove > 0) {
		shadow.posY += pixelsToMove;
		shadow.positionAccumulator -= pixelsToMove;
	}

	// Check if shadow moved off screen
	if (shadow.posY >= displayHeight) {
		shadow.shouldReset = true;
	}
}

void Matrix::drawShadowAndUpdateCharacters(Shadow &shadow) {
	// Calculate boundaries
	const uint8_t colStart = shadow.col * (CHAR_WIDTH + columnSpacing);
	const uint8_t colEnd = colStart + CHAR_WIDTH;
	const int16_t shadowTop = shadow.posY;
	const int16_t shadowBottom = shadowTop + shadow.length;

	// Skip shadows that aren't visible
	if (shadowBottom < 0 || shadowTop >= displayHeight) {
		return;
	}

	// Calculate visible region
	const uint8_t startRow = (shadowTop < 0) ? 0 : (uint8_t)shadowTop;
	const uint8_t endRow =
		(shadowBottom > displayHeight) ? displayHeight : (uint8_t)shadowBottom;

	// Draw the shadow
	for (uint8_t row = startRow; row < endRow; row++) {
		for (uint8_t x = colStart; x < colEnd; x++) {
			if (x < displayWidth) {
				display.drawPixel(x, row, SSD1306_BLACK);
			}
		}
	}

	// Update characters within the shadow
	uint8_t charRowStart = startRow / CHAR_HEIGHT;
	uint8_t charRowEnd = ((endRow - 1) / CHAR_HEIGHT + 1 < numRows)
							 ? (endRow - 1) / CHAR_HEIGHT + 1
							 : numRows;

	for (uint8_t charRow = charRowStart; charRow < charRowEnd; charRow++) {
		int16_t charRowY = charRow * CHAR_HEIGHT;

		// Update character if shadow is entering this row for the first
		// time
		if (shadow.lastCharChangePos < charRowY && shadowTop >= charRowY) {
			characters[charRow][shadow.col] = random(33, 126);
		}
	}

	// Track shadow position
	if (shadowTop > shadow.lastCharChangePos) {
		shadow.lastCharChangePos = shadowTop;
	}
}
