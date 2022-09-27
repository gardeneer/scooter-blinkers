//**
 * Folding blinker por scooter
 *
 * @author: Felipe Martínez Rodríguez
 * @author: Ignacio Colino Cortizo <icolinocortizo@gmail.com>
 * @version 0.1.2
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#include <Servo.h>
#endif
#define INT_R       2 // Output for right blinker
#define INT_L       9 // Output for left blinker
#define NUMPIXELS 16  // Number of pixels in LED panel
Adafruit_NeoPixel rightPixels(NUMPIXELS, INT_R, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftPixels(NUMPIXELS, INT_L, NEO_GRB + NEO_KHZ800);

#define BLINKER_TIME 50 // Blinker speed
#define OPEN_SPEED 1 // Open speed

int INTD = 5;   // Input for right blinker activation
int INTI = 8;   // Input for left blinker activation
Servo INTRIGHTSERV;
Servo INTLEFTSERV;

struct color_t { // RGB Color structure
  int red;
  int green;
  int blue;
};

// Color definition. All of them are orange with different intensity.
struct color_t LIGHT_ORANGE_H = {200, 100, 0};
struct color_t LIGHT_ORANGE_M = {100, 50, 0};
struct color_t LIGHT_ORANGE_L = {10, 5, 0};
struct color_t LIGHT_OFF = {0, 0, 0};

// Colors structure
struct color_t colors[4]; // Intensity of used colors

// Light program
int CUATRICOLOR0[2][8] =
{
	{3, 0, 0, 0, 0, 0, 0, 0},
	{3, 0, 0, 0, 0, 0, 0, 0}
};
int CUATRICOLOR1[2][8] =
{
	{3, 3, 0, 0, 0, 0, 0, 0},
	{3, 3, 0, 0, 0, 0, 0, 0}
};
int CUATRICOLOR2[2][8] =
{
	{3, 3, 3, 0, 0, 0, 0, 0},
	{3, 3, 3, 0, 0, 0, 0, 0}
};
int CUATRICOLOR3[2][8] =
{
	{3, 3, 3, 3, 0, 0, 0, 0},
	{3, 3, 3, 3, 0, 0, 0, 0}
};
int CUATRICOLOR4[2][8] =
{
	{3, 3, 3, 3, 3, 0, 0, 0},
	{3, 3, 3, 3, 3, 0, 0, 0}
};
int CUATRICOLOR5[2][8] =
{
	{3, 3, 3, 3, 3, 3, 0, 0},
	{3, 3, 3, 3, 3, 3, 0, 0}
};
int CUATRICOLOR6[2][8] =
{
	{3, 3, 3, 3, 3, 3, 3, 0},
	{3, 3, 3, 3, 3, 3, 3, 0}
};
int CUATRICOLOR7[2][8] =
{
	{3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3}
};
int CUATRICOLOR8[2][8] =
{
	{3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3}
};
int CUATRICOLOR9[2][8] =
{
	{2, 3, 3, 3, 3, 3, 3, 3},
	{2, 3, 3, 3, 3, 3, 3, 3}
};
int CUATRICOLOR10[2][8] =
{
	{1, 2, 3, 3, 3, 3, 3, 3},
	{1, 2, 3, 3, 3, 3, 3, 3}
};
int CUATRICOLOR11[2][8] =
{
	{0, 1, 2, 3, 3, 3, 3, 3},
	{0, 1, 2, 3, 3, 3, 3, 3}
};
int CUATRICOLOR12[2][8] =
{
	{0, 0, 1, 2, 3, 3, 3, 3},
	{0, 0, 1, 2, 3, 3, 3, 3}
};
int CUATRICOLOR13[2][8] =
{
	{0, 0, 0, 1, 2, 3, 3, 3},
	{0, 0, 0, 1, 2, 3, 3, 3}
};
int CUATRICOLOR14[2][8] =
{
	{0, 0, 0, 0, 1, 2, 3, 3},
	{0, 0, 0, 0, 1, 2, 3, 3}
};
int CUATRICOLOR15[2][8] =
{
	{0, 0, 0, 0, 0, 1, 2, 3},
	{0, 0, 0, 0, 0, 1, 2, 3}
};
int CUATRICOLOR16[2][8] =
{
	{0, 0, 0, 0, 0, 0, 1, 2},
	{0, 0, 0, 0, 0, 0, 1, 2}
};
int CUATRICOLOR17[2][8] =
{
	{0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 1}
};
int CUATRICOLOR18[2][8] =
{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

// How many times the sequence is repeated
int numberOfRepetitions = 6;
// Number of elements of a sequence (from 0 to 18)
int elementsInSequence = 19;

// Blinkers state
bool rightBlinkerState;
bool leftBlinkerState;

// Blinkers frame
int rightBlinkerFrame;
int leftBlinkerFrame;

int rightLoop;
int leftLoop;

void setup()
{
#if defined(__AVR_ATtiny85__)
	if (F_CPU == 16000000)
	{
		clock_prescale_set(clock_div_1);
	}
#endif
	rightPixels.begin();
	leftPixels.begin();
	pinMode (INTD, INPUT);  // Define input for right blinker
	pinMode (INTI, INPUT); // Define input for left blinker
	INTRIGHTSERV.attach(3);  // link right servo with digital pin 3
	INTLEFTSERV.attach(10);  // link left servo with digital pin 10

	// Initialization of blinker state to FALSE
	rightBlinkerState = false;
	leftBlinkerState = false;
	
	// Frame de representacion
	rightBlinkerFrame = 0;
	leftBlinkerFrame = 0;
	
	// Loop
	rightLoop = 0;
	leftLoop = 0;
	
	// Color assignment
	colors[0] = LIGHT_OFF;
	colors[1] = LIGHT_ORANGE_L;
	colors[2] = LIGHT_ORANGE_M;
	colors[3] = LIGHT_ORANGE_H ;
	
	// Set servo to initial position
	INTRIGHTSERV.write (5);
	INTLEFTSERV.write (123);
}

/**
 * Create an array to control LED panel
 *
 * panel: image to display
 * colors: colors to use in display
 * pixelsAux: LED panel
 */
void setMultiPanel(int panel[2][8], color_t colors[], Adafruit_NeoPixel &pixelsAux)
{
	int rowNumber = 2;
	int columnNumber = 8;
	for (int row = 0; row < rowNumber; row++)
	{
		for (int column = 0; column < columnNumber; column++)
		{
			int value = panel[row][column];
			pixelsAux.setPixelColor(((columnNumber * row) + column), pixelsAux.Color(colors[value].red, colors[value].green, colors[value].blue));
		}
	}
	pixelsAux.show();
}

void loop()
{
	if (digitalRead (INTD) == HIGH)
	{
		if (digitalRead (INTI) == HIGH)
		{
			// Both blinkers. Emergency stop.
			if (rightBlinkerState)
			{
				if (leftBlinkerState)
				{
					// Already active.
					if (rightBlinkerFrame == elementsInSequence)
					{
						// Restart sequence
						rightBlinkerFrame = 0;
					}
					if (leftBlinkerFrame == elementsInSequence)
					{
						// Restart sequence
						leftBlinkerFrame = 0;
					}
				}
				else
				{
					// Only active right blinker
					rightBlinkerFrame = 0;
					leftBlinkerFrame = 0;
					leftBlinkerState = true;
					rightLoop = 0;
					INTLEFTSERV.write(30);
					delay(250);
				}
			}
			else
			{
				rightBlinkerState = true;
				if (leftBlinkerState)
				{
					// Only active left blinker
					rightBlinkerFrame = 0;
					leftBlinkerFrame = 0;
					leftLoop = 0;
					INTRIGHTSERV.write(90);
				}
				else
				{
					// No blinkers active
					rightBlinkerFrame = 0;
					leftBlinkerFrame = 0;
					leftBlinkerState = true;
					INTRIGHTSERV.write(90);
					INTLEFTSERV.write(30);
				}
				delay(250);
			}
		}
		else
		{
			// Only right blinker pressed
			if (rightBlinkerState)
			{
				if (leftBlinkerState)
				{
					// Emergency stop was actived. It is disabled.
					rightBlinkerState = 0;
					leftBlinkerState = 0;
					rightBlinkerFrame = 0;
					leftBlinkerFrame = 0;
					rightLoop = 0;
					leftLoop = 0;
					INTRIGHTSERV.write(5);
					INTLEFTSERV.write(123);
					setMultiPanel(CUATRICOLOR0, colors, rightPixels);
					setMultiPanel(CUATRICOLOR0, colors, leftPixels);
					delay(250);
				}
				else
				{
					// Right sequence active
					if (rightBlinkerFrame == elementsInSequence)
					{
						rightLoop++;
						rightBlinkerFrame = 0;
					}
				}
			}
			else
			{
				// Right blinker is set to active
				INTRIGHTSERV.write(90);
				rightBlinkerState = true;
				rightLoop = 0;
				rightBlinkerFrame = 0;
				if (leftBlinkerState)
				{
					// Left sequence was active. Change sequence to right.
					INTLEFTSERV.write(123);
					leftBlinkerState = false;
					leftBlinkerFrame = 0;
					leftLoop = 0;
					setMultiPanel(CUATRICOLOR0, colors, leftPixels);
				}
				delay(250);
			}
		}
	}
	else
	{
		if (digitalRead(INTI) == HIGH)
		{
			// Left blinker is set to active
			if (leftBlinkerState)
			{
				if (rightBlinkerState)
				{
					// Emergency stop active. It is disabled.
					rightBlinkerState = 0;
					leftBlinkerState = 0;
					rightBlinkerFrame = 0;
					leftBlinkerFrame = 0;
					rightLoop = 0;
					leftLoop = 0;
					INTRIGHTSERV.write(5);
					INTLEFTSERV.write(123);
					setMultiPanel(CUATRICOLOR0, colors, rightPixels);
					setMultiPanel(CUATRICOLOR0, colors, leftPixels);
					delay(250);
				}
				else
				{
					// Left sequence already active
					if (leftBlinkerFrame == elementsInSequence)
					{
						leftBlinkerFrame = 0;
						leftLoop++;
					}
				}
			}
			else
			{
				// Left blinker is changed to active
				INTLEFTSERV.write(30);
				leftBlinkerState = true;
				leftBlinkerFrame = 0;
				leftLoop = 0;
				if (rightBlinkerState)
				{
					// Change blinker
					rightBlinkerState = false;
					rightBlinkerFrame = 0;
					rightLoop = 0;
					INTRIGHTSERV.write(5);
					setMultiPanel(CUATRICOLOR0, colors, rightPixels);
				}
				delay(250);
			}
		}
		else
		{
			// No buttons pressed. Keep on active sequence.
			if (rightBlinkerState)
			{
				if (leftBlinkerState)
				{
					// Emergency stop active
					if (rightBlinkerFrame >= elementsInSequence)
					{
						rightBlinkerFrame = 0;
					}
					if (leftBlinkerFrame >= elementsInSequence)
					{
						leftBlinkerFrame = 0;
					}
				}
				else
				{
					if (rightBlinkerFrame >= elementsInSequence)
					{
						rightBlinkerFrame = 0;
						rightLoop++;
						if (rightLoop >= numberOfRepetitions)
						{
							// Sequence final reached
							rightLoop = 0;
							rightBlinkerState = false;
							INTRIGHTSERV.write(5);
							setMultiPanel(CUATRICOLOR0, colors, rightPixels);
							delay(250);
						}
					}
				}
			}
			else
			{
				if (leftBlinkerState)
				{
					if (leftBlinkerFrame >= elementsInSequence)
					{
						leftBlinkerFrame = 0;
						leftLoop++;
						if (leftLoop >= numberOfRepetitions)
						{
							// Sequence final reached
							leftLoop = 0;
							leftBlinkerState = false;
							setMultiPanel(CUATRICOLOR0, colors, leftPixels);
							INTLEFTSERV.write(123);
							delay(250);
						}
					}
				}
			}
		}
	}
	// Wait blinkers for opening or closing
	if (rightBlinkerState)
	{
		switch(rightBlinkerFrame++)
		{
			case 0:
				setMultiPanel(CUATRICOLOR0, colors, rightPixels);
				break;
			case 1:
				setMultiPanel(CUATRICOLOR1, colors, rightPixels);
				break;
			case 2:
				setMultiPanel(CUATRICOLOR2, colors, rightPixels);
				break;
			case 3:
				setMultiPanel(CUATRICOLOR3, colors, rightPixels);
				break;
			case 4:
				setMultiPanel(CUATRICOLOR4, colors, rightPixels);
				break;
			case 5:
				setMultiPanel(CUATRICOLOR5, colors, rightPixels);
				break;
			case 6:
				setMultiPanel(CUATRICOLOR6, colors, rightPixels);
				break;
			case 7:
				setMultiPanel(CUATRICOLOR7, colors, rightPixels);
				break;
			case 8:
				setMultiPanel(CUATRICOLOR8, colors, rightPixels);
				break;
			case 9:
				setMultiPanel(CUATRICOLOR9, colors, rightPixels);
				break;
			case 10:
				setMultiPanel(CUATRICOLOR10, colors, rightPixels);
				break;
			case 11:
				setMultiPanel(CUATRICOLOR11, colors, rightPixels);
				break;
			case 12:
				setMultiPanel(CUATRICOLOR12, colors, rightPixels);
				break;
			case 13:
				setMultiPanel(CUATRICOLOR13, colors, rightPixels);
				break;
			case 14:
				setMultiPanel(CUATRICOLOR14, colors, rightPixels);
				break;
			case 15:
				setMultiPanel(CUATRICOLOR15, colors, rightPixels);
				break;
			case 16:
				setMultiPanel(CUATRICOLOR16, colors, rightPixels);
				break;
			case 17:
				setMultiPanel(CUATRICOLOR17, colors, rightPixels);
				break;
			case 18:
				setMultiPanel(CUATRICOLOR18, colors, rightPixels);
				break;
			default:
				setMultiPanel(CUATRICOLOR0, colors, rightPixels);
				break;
		}
	}
	if (leftBlinkerState)
	{
		switch(leftBlinkerFrame++)
		{
			case 0:
				setMultiPanel(CUATRICOLOR0, colors, leftPixels);
				break;
			case 1:
				setMultiPanel(CUATRICOLOR1, colors, leftPixels);
				break;
			case 2:
				setMultiPanel(CUATRICOLOR2, colors, leftPixels);
				break;
			case 3:
				setMultiPanel(CUATRICOLOR3, colors, leftPixels);
				break;
			case 4:
				setMultiPanel(CUATRICOLOR4, colors, leftPixels);
				break;
			case 5:
				setMultiPanel(CUATRICOLOR5, colors, leftPixels);
				break;
			case 6:
				setMultiPanel(CUATRICOLOR6, colors, leftPixels);
				break;
			case 7:
				setMultiPanel(CUATRICOLOR7, colors, leftPixels);
				break;
			case 8:
				setMultiPanel(CUATRICOLOR8, colors, leftPixels);
				break;
			case 9:
				setMultiPanel(CUATRICOLOR9, colors, leftPixels);
				break;
			case 10:
				setMultiPanel(CUATRICOLOR10, colors, leftPixels);
				break;
			case 11:
				setMultiPanel(CUATRICOLOR11, colors, leftPixels);
				break;
			case 12:
				setMultiPanel(CUATRICOLOR12, colors, leftPixels);
				break;
			case 13:
				setMultiPanel(CUATRICOLOR13, colors, leftPixels);
				break;
			case 14:
				setMultiPanel(CUATRICOLOR14, colors, leftPixels);
				break;
			case 15:
				setMultiPanel(CUATRICOLOR15, colors, leftPixels);
				break;
			case 16:
				setMultiPanel(CUATRICOLOR16, colors, leftPixels);
				break;
			case 17:
				setMultiPanel(CUATRICOLOR17, colors, leftPixels);
				break;
			case 18:
				setMultiPanel(CUATRICOLOR18, colors, leftPixels);
				break;
			default:
				setMultiPanel(CUATRICOLOR0, colors, leftPixels);
				break;
		}
	}
	delay(BLINKER_TIME);
}
