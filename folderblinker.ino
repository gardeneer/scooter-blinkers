/**
 * Folding blinker por scooter
 * @author: Felipe Martínez Rodríguez
 * @author: Ignacio Colino Cortizo <icolinocortizo@gmail.com>
 * @version 0.1.1
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#include <Servo.h>
#endif
#define INT_D       2 // Output for right blinker
#define INT_I       9 // Output for left blinker
#define NUMPIXELS 16  // Number of pixels in LED panel
Adafruit_NeoPixel pixelsDerecho(NUMPIXELS, INT_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsIzquierdo(NUMPIXELS, INT_I, NEO_GRB + NEO_KHZ800);

#define TIME 50 // Blinker speed
#define TIME2 1 // Open speed

int INTD = 5;   // Input for right blinker activation
int INTI = 8;   // Input for left blinker activation
Servo INTDSERV;
Servo INTISERV;

struct color_t { // RGB Color structure
  int red;
  int green;
  int blue;
};

// Color definition. All of them are orange with different intensity.
struct color_t LUZ_NARANJA_H = {200, 100, 0};
struct color_t LUZ_NARANJA_M = {100, 50, 0};
struct color_t LUZ_NARANJA_L = {10, 5, 0};
struct color_t LUZ_OFF = {0, 0, 0};

// Colors structure
struct color_t colores[4]; // Intensity of used colors

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
int numeroRepeticiones = 6;
// Number of elements of a sequence (from 0 to 18)
int elementosSecuencia = 19;

bool estadoIntermitenteDerecho;
bool estadoIntermitenteIzquierdo;

int frameIntermitenteDerecho;
int frameIntermitenteIzquierdo;

int bucleDerecho;
int bucleIzquierdo;

void setup()
{
#if defined(__AVR_ATtiny85__)
	if (F_CPU == 16000000)
	{
		clock_prescale_set(clock_div_1);
	}
#endif
	pixelsDerecho.begin();
	pixelsIzquierdo.begin();
	pinMode (INTD, INPUT);  // Define input for right blinker
	pinMode (INTI, INPUT); // Define input for left blinker
	INTDSERV.attach(3);  // link right servo with digital pin 3
	INTISERV.attach(10);  // link left servo with digital pin 10

	// Initialization of blinker state to FALSE
	estadoIntermitenteDerecho = false;
	estadoIntermitenteIzquierdo = false;
	
	// Frame de representacion
	frameIntermitenteDerecho = 0;
	frameIntermitenteIzquierdo = 0;
	
	// Loop
	bucleDerecho = 0;
	bucleIzquierdo = 0;
	
	// Color assignment
	colores[0] = LUZ_OFF;
	colores[1] = LUZ_NARANJA_L;
	colores[2] = LUZ_NARANJA_M;
	colores[3] = LUZ_NARANJA_H ;
	
	// Set servo to initial position
	INTDSERV.write (5);
	INTISERV.write (123);
}

void setMultiPanel(int panel[2][8], color_t colores[], Adafruit_NeoPixel &pixelsAux)      //Creamos un array que controla los cuadros de luces
{
	int numFilas = 2;
	int numColumnas = 8;
	for (int fila = 0; fila < numFilas; fila++)
	{
		for (int columna = 0; columna < numColumnas; columna++)
		{
			int value = panel[fila][columna];
			pixelsAux.setPixelColor(((numColumnas * fila) + columna), pixelsAux.Color(colores[value].red, colores[value].green, colores[value].blue));
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
			// Ambos intermitentes. Parada de emergencia.
			if (estadoIntermitenteDerecho)
			{
				if (estadoIntermitenteIzquierdo)
				{
					// Ya activa.
					if (frameIntermitenteDerecho == elementosSecuencia)
					{
						// Reiniciamos secuencia
						frameIntermitenteDerecho = 0;
					}
					if (frameIntermitenteIzquierdo == elementosSecuencia)
					{
						frameIntermitenteIzquierdo = 0;
					}
				}
				else
				{
					// Sólo estaba activo intermitente derecho
					frameIntermitenteDerecho = 0;
					frameIntermitenteIzquierdo = 0;
					estadoIntermitenteIzquierdo = true;
					bucleDerecho = 0;
					INTISERV.write(30);
					delay(250);
				}
			}
			else
			{
				estadoIntermitenteDerecho = true;
				if (estadoIntermitenteIzquierdo)
				{
					// Sólo estaba activo intermitente izquierdo
					frameIntermitenteDerecho = 0;
					frameIntermitenteIzquierdo = 0;
					bucleIzquierdo = 0;
					INTDSERV.write(90);
				}
				else
				{
					// No estaba activo ninguno
					frameIntermitenteDerecho = 0;
					frameIntermitenteIzquierdo = 0;
					estadoIntermitenteIzquierdo = true;
					INTDSERV.write(90);
					INTISERV.write(30);
				}
				delay(250);
			}
		}
		else
		{
			// Sólo se ha pulsado el intermitente derecho
			if (estadoIntermitenteDerecho)
			{
				if (estadoIntermitenteIzquierdo)
				{
					// Estábamos en parada de emergencia. Se deshabilita
					estadoIntermitenteDerecho = 0;
					estadoIntermitenteIzquierdo = 0;
					frameIntermitenteDerecho = 0;
					frameIntermitenteIzquierdo = 0;
					bucleDerecho = 0;
					bucleIzquierdo = 0;
					INTDSERV.write(5);
					INTISERV.write(123);
					setMultiPanel(CUATRICOLOR0, colores, pixelsDerecho);
					setMultiPanel(CUATRICOLOR0, colores, pixelsIzquierdo);
					delay(250);
				}
				else
				{
					// Estabamos en secuencia de derecho
					if (frameIntermitenteDerecho == elementosSecuencia)
					{
						bucleDerecho++;
						frameIntermitenteDerecho = 0;
					}
				}
			}
			else
			{
				// Queremos activar intermitentes derecho
				INTDSERV.write(90);
				estadoIntermitenteDerecho = true;
				bucleDerecho = 0;
				frameIntermitenteDerecho = 0;
				if (estadoIntermitenteIzquierdo)
				{
					// Estabamos en secuencia de izquierdo. Cambiamos a derecho
					INTISERV.write(123);
					estadoIntermitenteIzquierdo = false;
					frameIntermitenteIzquierdo = 0;
					bucleIzquierdo = 0;
					setMultiPanel(CUATRICOLOR0, colores, pixelsIzquierdo);
				}
				delay(250);
			}
		}
	}
	else
	{
		if (digitalRead(INTI) == HIGH)
		{
			// Queremos poner el intermitente izquierdo
			if (estadoIntermitenteIzquierdo)
			{
				if (estadoIntermitenteDerecho)
				{
					// Estábamos en parada de emergencia. Se deshabilita
					estadoIntermitenteDerecho = 0;
					estadoIntermitenteIzquierdo = 0;
					frameIntermitenteDerecho = 0;
					frameIntermitenteIzquierdo = 0;
					bucleDerecho = 0;
					bucleIzquierdo = 0;
					INTDSERV.write(5);
					INTISERV.write(123);
					setMultiPanel(CUATRICOLOR0, colores, pixelsDerecho);
					setMultiPanel(CUATRICOLOR0, colores, pixelsIzquierdo);
					delay(250);
				}
				else
				{
					// Ya estábamos en secuencia de izquierdo
					if (frameIntermitenteIzquierdo == elementosSecuencia)
					{
						frameIntermitenteIzquierdo = 0;
						bucleIzquierdo++;
					}
				}
			}
			else
			{
				// Queremos activar intermitente izquierdo
				INTISERV.write(30);
				estadoIntermitenteIzquierdo = true;
				frameIntermitenteIzquierdo = 0;
				bucleIzquierdo = 0;
				if (estadoIntermitenteDerecho)
				{
					// Cambiamos de intermitente
					estadoIntermitenteDerecho = false;
					frameIntermitenteDerecho = 0;
					bucleDerecho = 0;
					INTDSERV.write(5);
					setMultiPanel(CUATRICOLOR0, colores, pixelsDerecho);
				}
				delay(250);
			}
		}
		else
		{
			// No hay botones pulsados. Seguimos con las secuencias que haya
			if (estadoIntermitenteDerecho)
			{
				if (estadoIntermitenteIzquierdo)
				{
					// Estamos en emergencia
					if (frameIntermitenteDerecho >= elementosSecuencia)
					{
						frameIntermitenteDerecho = 0;
					}
					if (frameIntermitenteIzquierdo >= elementosSecuencia)
					{
						frameIntermitenteIzquierdo = 0;
					}
				}
				else
				{
					if (frameIntermitenteDerecho >= elementosSecuencia)
					{
						frameIntermitenteDerecho = 0;
						bucleDerecho++;
						if (bucleDerecho >= numeroRepeticiones)
						{
							// Hemos llegado al final de la secuencia
							bucleDerecho = 0;
							estadoIntermitenteDerecho = false;
							INTDSERV.write(5);
							setMultiPanel(CUATRICOLOR0, colores, pixelsDerecho);
							delay(250);
						}
					}
				}
			}
			else
			{
				if (estadoIntermitenteIzquierdo)
				{
					if (frameIntermitenteIzquierdo >= elementosSecuencia)
					{
						frameIntermitenteIzquierdo = 0;
						bucleIzquierdo++;
						if (bucleIzquierdo >= numeroRepeticiones)
						{
							// Hemos llegado al final de la secuencia
							bucleIzquierdo = 0;
							estadoIntermitenteIzquierdo = false;
							setMultiPanel(CUATRICOLOR0, colores, pixelsIzquierdo);
							INTISERV.write(123);
							delay(250);
						}
					}
				}
			}
		}
	}
	// Esperamos a que los intermitentes se abran o se cierren
	if (estadoIntermitenteDerecho)
	{
		switch(frameIntermitenteDerecho++)
		{
			case 0:
				setMultiPanel(CUATRICOLOR0, colores, pixelsDerecho);
				break;
			case 1:
				setMultiPanel(CUATRICOLOR1, colores, pixelsDerecho);
				break;
			case 2:
				setMultiPanel(CUATRICOLOR2, colores, pixelsDerecho);
				break;
			case 3:
				setMultiPanel(CUATRICOLOR3, colores, pixelsDerecho);
				break;
			case 4:
				setMultiPanel(CUATRICOLOR4, colores, pixelsDerecho);
				break;
			case 5:
				setMultiPanel(CUATRICOLOR5, colores, pixelsDerecho);
				break;
			case 6:
				setMultiPanel(CUATRICOLOR6, colores, pixelsDerecho);
				break;
			case 7:
				setMultiPanel(CUATRICOLOR7, colores, pixelsDerecho);
				break;
			case 8:
				setMultiPanel(CUATRICOLOR8, colores, pixelsDerecho);
				break;
			case 9:
				setMultiPanel(CUATRICOLOR9, colores, pixelsDerecho);
				break;
			case 10:
				setMultiPanel(CUATRICOLOR10, colores, pixelsDerecho);
				break;
			case 11:
				setMultiPanel(CUATRICOLOR11, colores, pixelsDerecho);
				break;
			case 12:
				setMultiPanel(CUATRICOLOR12, colores, pixelsDerecho);
				break;
			case 13:
				setMultiPanel(CUATRICOLOR13, colores, pixelsDerecho);
				break;
			case 14:
				setMultiPanel(CUATRICOLOR14, colores, pixelsDerecho);
				break;
			case 15:
				setMultiPanel(CUATRICOLOR15, colores, pixelsDerecho);
				break;
			case 16:
				setMultiPanel(CUATRICOLOR16, colores, pixelsDerecho);
				break;
			case 17:
				setMultiPanel(CUATRICOLOR17, colores, pixelsDerecho);
				break;
			case 18:
				setMultiPanel(CUATRICOLOR18, colores, pixelsDerecho);
				break;
			default:
				setMultiPanel(CUATRICOLOR0, colores, pixelsDerecho);
				break;
		}
	}
	if (estadoIntermitenteIzquierdo)
	{
		switch(frameIntermitenteIzquierdo++)
		{
			case 0:
				setMultiPanel(CUATRICOLOR0, colores, pixelsIzquierdo);
				break;
			case 1:
				setMultiPanel(CUATRICOLOR1, colores, pixelsIzquierdo);
				break;
			case 2:
				setMultiPanel(CUATRICOLOR2, colores, pixelsIzquierdo);
				break;
			case 3:
				setMultiPanel(CUATRICOLOR3, colores, pixelsIzquierdo);
				break;
			case 4:
				setMultiPanel(CUATRICOLOR4, colores, pixelsIzquierdo);
				break;
			case 5:
				setMultiPanel(CUATRICOLOR5, colores, pixelsIzquierdo);
				break;
			case 6:
				setMultiPanel(CUATRICOLOR6, colores, pixelsIzquierdo);
				break;
			case 7:
				setMultiPanel(CUATRICOLOR7, colores, pixelsIzquierdo);
				break;
			case 8:
				setMultiPanel(CUATRICOLOR8, colores, pixelsIzquierdo);
				break;
			case 9:
				setMultiPanel(CUATRICOLOR9, colores, pixelsIzquierdo);
				break;
			case 10:
				setMultiPanel(CUATRICOLOR10, colores, pixelsIzquierdo);
				break;
			case 11:
				setMultiPanel(CUATRICOLOR11, colores, pixelsIzquierdo);
				break;
			case 12:
				setMultiPanel(CUATRICOLOR12, colores, pixelsIzquierdo);
				break;
			case 13:
				setMultiPanel(CUATRICOLOR13, colores, pixelsIzquierdo);
				break;
			case 14:
				setMultiPanel(CUATRICOLOR14, colores, pixelsIzquierdo);
				break;
			case 15:
				setMultiPanel(CUATRICOLOR15, colores, pixelsIzquierdo);
				break;
			case 16:
				setMultiPanel(CUATRICOLOR16, colores, pixelsIzquierdo);
				break;
			case 17:
				setMultiPanel(CUATRICOLOR17, colores, pixelsIzquierdo);
				break;
			case 18:
				setMultiPanel(CUATRICOLOR18, colores, pixelsIzquierdo);
				break;
			default:
				setMultiPanel(CUATRICOLOR0, colores, pixelsIzquierdo);
				break;
		}
	}
	delay(TIME);
}
