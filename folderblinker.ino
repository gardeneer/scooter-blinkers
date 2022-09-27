/**
 * Folding blinker por scooter
 * @author: Felipe Martínez Rodríguez
 * @author: Ignacio Colino Cortizo <icolinocortizo@gmail.com>
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#include <Servo.h>
#endif
#define INT_D       2 // Output for right blinker
#define INT_I       9 // Output for left blinker
#define NUMPIXELS 16  // Number of pixels in LED panel
Adafruit_NeoPixel pixels(NUMPIXELS, INT_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUMPIXELS, INT_I, NEO_GRB + NEO_KHZ800);

#define TIME 50 // Blinker speed
#define TIME2 1 // Open speed

int INTD = 5;   // Input for right blinker activation
int INTI = 8;   // Input for left blinker activation
Servo INTDSERV;
Servo INTISERV;

struct color_t{ // RGB Color structure
	int red;
	int green;
	int blue;
};
                                                  // Color definition. All of them are orange with different intensity.
struct color_t LUZ_NARANJA_H = {200, 100, 0}; 
struct color_t LUZ_NARANJA_M= {100, 50, 0};
struct color_t LUZ_NARANJA_L = {10, 5, 0};
struct color_t LUZ_OFF{0, 0, 0};
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
}
;int CUATRICOLOR9[2][8] =
{
	{2, 3, 3, 3, 3, 3, 3, 3},
	{2, 3, 3, 3, 3, 3, 3, 3}
};
int CUATRICOLOR10[2][8] =
{
	{1, 2, 3, 3, 3, 3, 3, 3},
	{1, 2, 3, 3, 3, 3, 3, 3}
}
;int CUATRICOLOR11[2][8] =
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

void setup() 
{
#if defined(__AVR_ATtiny85__)
	if (F_CPU == 16000000) 
	{
		clock_prescale_set(clock_div_1);
	}
#endif
	pixels.begin();
	pixels2.begin(); 
	pinMode (INTD, INPUT);  // Define input for right blinker
	pinMode (INTI, INPUT); // Define input for left blinker
	INTDSERV.attach(3);  // link right servo with digital pin 3
	INTISERV.attach(10);  // link left servo with digital pin 10
}

void setMultiPanel(int panel[8][8], color_t colores[], Adafruit_NeoPixel &pixelsAux)      //Creamos un array que controla los cuadros de luces
{
  int numFilas = 8;
  int numColumnas = 8;
  for (int fila = 0; fila < numFilas; fila++)
  {
    for (int columna = 0; columna < numColumnas; columna++)
    {
      int value = panel[fila][columna];
      pixelsAux.setPixelColor(((numColumnas * fila) + columna), pixels.Color(colores[value].red, colores[value].green, colores[value].blue));
    }
  }
}

void loop()
{
	INTDSERV.write (5);
	INTISERV.write (123);
	if (digitalRead (INTD) == HIGH)   // Right blinker activation
	{
		INTDSERV.write (90);
		delay(250);
		{
			int aux;
			for (int i=0; i<6; i++)         //Número de intermitencias automáticas
			{
				struct color_t colores[4];        //Intensidad de colores utilizados
				colores[0] = LUZ_OFF;
				colores[1] = LUZ_NARANJA_L;
				colores[2] = LUZ_NARANJA_M;
				colores[3] = LUZ_NARANJA_H ;
				setMultiPanel(CUATRICOLOR0, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR1, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR2, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR3, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR4, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR5, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR6, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR7, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR8, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR9, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR10, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR11, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR12, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR13, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR14, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR15, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR16, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR17, colores, pixels);
				pixels.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR18, colores, pixels);
				pixels.show();
				delay(TIME);
				aux++;
			}
		}
	}
	else
	{
		if (digitalRead (INTI) == HIGH)   // Left blinker activation
		{
			INTISERV.write (30);
			delay(250);
			int aux;
			for (int i = 0; i < 6; i++)           // Automatic blinkers counter
			{
				struct color_t colores[4];
				colores[0] = LUZ_OFF;
				colores[1] = LUZ_NARANJA_L;
				colores[2] = LUZ_NARANJA_M;
				colores[3] = LUZ_NARANJA_H ;
				setMultiPanel(CUATRICOLOR0, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR1, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR2, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR3, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR4, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR5, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR6, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR7, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR8, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR9, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR10, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR11, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR12, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR13, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR14, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR15, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR16, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR17, colores, pixels2);
				pixels2.show();
				delay(TIME);
				setMultiPanel(CUATRICOLOR18, colores, pixels2);
				pixels2.show();
				delay(TIME);
				aux++;
			}
		}
	}
}
