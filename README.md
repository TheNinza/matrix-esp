# Matrix Animation for ESP32

A Matrix-style digital rain animation for ESP32 with SSD1306 OLED display. This project creates the iconic Matrix "falling code" effect on a small OLED screen using the ESP32 microcontroller.

<p align="center">
  <img height="400" src="https://github.com/TheNinza/matrix-esp/blob/main/images/matrix-demo.gif?raw=true" alt="Matrix Animation Demo">
</p>

## Features

- Smooth Matrix-style digital rain animation
- Customizable characters and animation parameters
- Optimized for 128x64 SSD1306 OLED displays
- Easy to set up and customize

## Hardware Requirements

- ESP32 development board
- SSD1306 OLED display (128x64 pixels)
- Jumper wires
- Breadboard (optional)

## Wiring Instructions

Connect your SSD1306 OLED display to the ESP32 as follows:

| OLED Display | ESP32   |
| ------------ | ------- |
| VCC          | 3.3V    |
| GND          | GND     |
| SCL          | GPIO 22 |
| SDA          | GPIO 21 |

## Software Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- Required libraries:
  - Adafruit SSD1306
  - Adafruit GFX Library

## PlatformIO Setup Instructions

### Installing PlatformIO

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Open VS Code and install the PlatformIO extension from the marketplace

### Setting up the Project

1. Clone this repository:

   ```
   git clone https://github.com/yourusername/matrix-esp.git
   cd matrix-esp
   ```

2. Open the project in VS Code with PlatformIO:

   - Launch VS Code
   - Click on the PlatformIO icon in the sidebar
   - Select "Open Project"
   - Navigate to the cloned repository and open it

3. PlatformIO will automatically install all dependencies specified in the `platformio.ini` file

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
