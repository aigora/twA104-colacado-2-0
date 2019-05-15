//                                                                              SIM�N DICE
#define _CRT_SECURE_NO_DEPRECATE
#include<stdlib.h>
#include<LiquidCrystal.h>
#define N 3
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD
LiquidCrystal lcd(A0, A1, 9, 8, 7, 6);
//Recuerda, despues lo de las rondas 
//Prototipos de funciones.

	///FUNCIONES PRINCIPALES
void seleccion_nivel(int*, int*, int*);				// La funci�n "seleccion_nivel" determinar� el grado de dificultad de la partida.
void crear_enunciado(int*, int*);					// La funci�n "crear_enunciado" crear� el vector aleatorio a adivinar.
void enunciado_secuencia(int*, int*);				// La funci�n "enunciado_secuencia" mostrar� por los 4 leds la secuencia creada aleatoriamente.
void leer_secuencia(int*, int*);					// La funci�n "leer_secuencia" leer� la secuencia introducida por el usuario mediante los botones.
int comparar_secuencia(int, int*, int*, int*, int*);// La funci�n "comparar_secuencia" comparar� el vector secuencia y pulso para comprobar si hay alg�n error.

	///FUNCIONES SECUNDARIAS
void inicio_juego();					// La funci�n "inicio_juego" indicar� que se ha iniciado el juego.
void game_over(int, int*, int*, int*);	// La funci�n "game_over" indicar� si el jugador ha ganado o ha perdido la partida
void acierto_fallo(int, int*);			// La funci�n "acierto_fallo" indicar� si has acertado o has fallado al introducir la secuencia.
void fin_secuencia();					// La funci�n "fin_secuencia" indicar� el fin de la secuencia mostrada o introducida por los leds.
void paso_ronda(int*);					// La funci�n "paso_ronda" mostrar� en que ronda nos encontramos.
void mostrar_fallos(int*, int*, int*);	// La funci�n "mostrar_fallo" dir� en que bot�n has fallado y cual deber�as haber pulsado.

	///FUNCIONES PARA ANIMACONES DE LEDS
void esperaI();
void esperaII();
void esperaIII();
void apagar_led();
void encender_led();

//Declaro una estructura con los pines de entrada y salida.
typedef struct {
	int led[4] = { 2, 3, 4, 5 };
	int boton[4] = { 10, 11, 12, 13 };
}PIN;

void setup() {
	Serial.begin(9600);
	int i;
	PIN pines;
	// Inicializo los pines de entrada y salida
	for (i = 0; i < 4; i++)
	{
		pinMode(pines.led[i], OUTPUT);
		pinMode(pines.boton[i], INPUT);
	}
	// Inicializo las columnas y las filas del LCD
	lcd.begin(COLS, ROWS);
}

void loop() {
	// Variables
	int i, ronda = 1, rondamax = 0, fallos = 0, *pronda, *prondamax, *pfallos, flagfail = 0;
	int *psecuencia, *ppulso;
	pronda = &ronda;
	prondamax = &rondamax;
	pfallos = &fallos;
	// "ronda"	variable entera que indica en que ronda nos encontramos dentro de la partida, y tambi�n, el n�mero de leds a adivinar.
	// "rondamax"	variable entera tendr� el valor de el n�mero m�ximo de rondas. Variar� dependiendo del nivel en el que estemos.
	// "fallos"	variable entera que aumentar� a medida que se encuentren los fallos en un 
	// "flagfail"	variable entera me servir� para saber si hay alg�n fallo en la secuencia introducida por los botones. 
	// Si su valor es distinto de 0 siginifica que hemos perdido.
	// "*psecuencia" y "*ppulso" punteros para la creaci�n de los vectores din�micos.

	lcd.clear();
	inicio_juego();

	// Determino el nivel de la partida.
	seleccion_nivel(prondamax, psecuencia, ppulso);

	// Inicializo los vectores din�micos.
	psecuencia = (int*)malloc((*prondamax) * sizeof(int));
	ppulso = (int*)malloc((*prondamax) * sizeof(int));
	if (psecuencia == NULL)
	{
		Serial.println("Error de memoria en VECTOR SECUENCIA.\n");
		exit(1);	/// Si no hay espacio en memoria din�mica salir del programa.
	}
	if (ppulso == NULL)
	{
		Serial.println("Error de memoria en VECTOR PULSO.\n");
		exit(1);	/// Si no hay espacio en memoria din�mica salir del programa.
	}

	// Inicializo la secuencia a adivinar.
	crear_enunciado(psecuencia, prondamax);

			// FLUJO PRINCIPAL DEL JUEGO.
	// Inicializo la secuencia a adivinar.
	crear_enunciado(psecuencia, prondamax);

	// Mientras que no fallemos y no lleguemos al l�mite de rondas, seguiremos jugando.
	while ((flagfail == 0) && (ronda <= rondamax))
	{
		paso_ronda(pronda);
		enunciado_secuencia(pronda, psecuencia);
		leer_secuencia(pronda, ppulso);
		delay(500);
		flagfail = comparar_secuencia(ronda, &ronda, psecuencia, ppulso, pfallos);
		acierto_fallo(flagfail, pfallos);
		delay(500);
	}
	// En caso de perder o llegar al final de las rondas m�ximas.
	game_over(flagfail, psecuencia, ppulso, pronda);

	free(psecuencia);
	free(ppulso);

	apagar_led();
	delay(2500);	/// Tiempo hasta nuevo juego (2.5 segs)
}


// Funciones PRINCIPALES.

void seleccion_nivel(int *y, int *vectI, int *vectII)
{
	int i;

	/// Muestro los mensajes por el monitor seria y por el LCD.
	Serial.println("\nSELECCIONA LA DIFICULTAD:\n 0-->Facil (5 rondas)\n 1-->Medio (10 rondas)\n 2-->Dificil (15 rondas)\n 3-->Extremo (30 rondas)");
	lcd.setCursor(1, 0);
	lcd.print("Elige el nivel");
	lcd.setCursor(5, 1);
	lcd.print("3 2 1 0");

	/// Espero a que el usuario pulse alg�n bot�n 
	while ((digitalRead(10) == 0) && (digitalRead(11) == 0) && (digitalRead(12) == 0) && (digitalRead(13) == 0))
	{
		esperaI();
		digitalRead(10);
		digitalRead(11);
		digitalRead(12);
		digitalRead(13);
		esperaII();
	}

	if (digitalRead(10) == HIGH)		/// F�CIL
	{
		*y = 5; /// Numero de rondas m�ximas.

		/// Muestro los mensajes por el monitor serial y el LCD.
		Serial.println("FACIL");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("FACIL");
		lcd.setCursor(0, 1);
		lcd.print("5 rondas");

		digitalWrite(2, HIGH); /// Confirmaci�n de lectura del bot�n. 
		delay(200);
		digitalWrite(2, LOW);

		apagar_led();
		delay(2250);
	}
	else if (digitalRead(11) == HIGH)	/// MEDIO
	{
		*y = 10;

		Serial.println("MEDIO");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("MEDIO");
		lcd.setCursor(0, 1);
		lcd.print("10 rondas");

		digitalWrite(3, HIGH);
		delay(200);
		digitalWrite(3, LOW);

		apagar_led();
		delay(2250);
	}
	else if (digitalRead(12) == HIGH)	/// DIF�CIL
	{
		*y = 15;

		Serial.println("DIFICIL");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DIFICIL");
		lcd.setCursor(0, 1);
		lcd.print("15 rondas");

		digitalWrite(4, HIGH);
		delay(200);
		digitalWrite(4, LOW);

		apagar_led();
		delay(2250);
	}
	else if (digitalRead(13) == HIGH)	/// EXTREMO
	{
		*y = 30;

		Serial.println("EXTREMO");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("EXTREMO");
		lcd.setCursor(0, 1);
		lcd.print("30 rondas");

		digitalWrite(5, HIGH);
		delay(200);
		digitalWrite(5, LOW);

		apagar_led();
		delay(2250);
	}

	Serial.println();
	lcd.clear();
}


void crear_enunciado(int *vect, int *x) /// Introduzco el vector del enunciado y el n�mero de rondas.
{
	int i;
	for (i = 0; i < (*x); i++)
	{
		/// Le asigno un valor aleatorio de 0 a 3 al vector psecuencia.
		*(vect + i) = rand() % (N + 1);
	}
}


void enunciado_secuencia(int *x, int *vect)
{	/// "x" es la ronda. "*vect" es el vector psecuencia, aleatorio con los leds a encender, es decir la secuencia.
	int i, posicion, luz;
	PIN pines;

	for (i = 0; i < *x; i++)
	{
		posicion = *(vect + i);				/// Leo el vector de la secuencia que me indica el led a encender.
		luz = pines.led[posicion];			/// Determino el pin que ha que encender.

		digitalWrite(luz, HIGH);
		delay(500);
		digitalWrite(luz, LOW);
		delay(500);
	}

	for (i = 0; i < *x; i++)					///Muestro los botones a pulsar
	{
		Serial.print(*(vect + i));
	}

	fin_secuencia();
}


void leer_secuencia(int *x, int *vect) // "x" es la ronda. "*vect" es el vector 'ppulso' que ir� creando a medida que vaya pulsando
{
	int i, cont = 1; //cont sirve mostrar en que turno vas de pulsado

	for (i = 0; i < *x; i++)
	{
		Serial.print("Introduce pulso ");
		Serial.print(cont);
		Serial.print(" --> ");

		lcd.setCursor(0, 1);
		lcd.print("PULSO ");
		lcd.print(cont);
		lcd.print("-->      ");
		/// ���OJO!!! Para que se rellene el vector hay que dejar que parpadee la luz (confirmaci�n de lectura).

		///Esperar a que el usuario pulse un boton.
		while ((digitalRead(10) == 0) && (digitalRead(11) == 0) && (digitalRead(12) == 0) && (digitalRead(13) == 0))
		{
			digitalRead(10);
			digitalRead(11);
			digitalRead(12);
			digitalRead(13);
		}

		if (digitalRead(10) == HIGH)
		{
			*(vect + i) = 0;

			lcd.setCursor(12, 1);
			lcd.print("0");

			digitalWrite(2, HIGH); ///CONFIRMACI�N DE LECTURA.
			delay(200);
			digitalWrite(2, LOW);
		}
		else if (digitalRead(11) == HIGH)
		{
			*(vect + i) = 1;

			lcd.setCursor(12, 1);
			lcd.print("1");

			digitalWrite(3, HIGH);
			delay(200);
			digitalWrite(3, LOW);
		}
		else if (digitalRead(12) == HIGH)
		{
			*(vect + i) = 2;

			lcd.setCursor(12, 1);
			lcd.print("2");

			digitalWrite(4, HIGH);
			delay(200);
			digitalWrite(4, LOW);
		}
		else if (digitalRead(13) == HIGH)
		{
			*(vect + i) = 3;

			lcd.setCursor(12, 1);
			lcd.print("3");

			digitalWrite(5, HIGH);
			delay(200);
			digitalWrite(5, LOW);
		}
		delay(250);

		cont++;
		Serial.println(*(vect + i)); ///Muestro el boton que se pulsa en cada turno por monitor serial.
	}


	for (i = 0; i < *x; i++) /// Muestro los botones pulsados por monitor serial.
	{
		Serial.print(*(vect + i), DEC);
	}
	Serial.println("\n");

}


int comparar_secuencia(int w, int *r, int *x, int *y, int *cont) /// "x" ser� el vector secuencia, e "y" el vector pulso. "w" es la ronda. "*r" es un puntero que cambia el valor de la ronda en caso de acertar.
{
	int i, flagfallo = 0;

	for (i = 0; i < w; i++)//Comprobaci�n de si he acertado o no en la ronda. (Comparo si los vectores de secuencia y pulso son iguales).
	{
		///En caso de fallar, activo el flagfail a 1.
		if (*(x + i) != *(y + i))
		{
			flagfallo = 1;
			(*cont)++;
		}
			/// Si son el mismo vector, eso es que se ha acertado y el valor de flag seguir� siendo 0;
		else
		{
				///Si acertamos, aumento una ronda. (A lo que apunta r(ronda), le doy el valor de w(ronda) mas una unidad).
			*r = w + 1;
		}
	}
	return flagfallo;
}


void mostrar_fallos(int *x, int *y, int *num) ///Los vectores de los par�metros son el pulso y la secuencia. Los ir� comparando para ver que valores son diferentes. Mostrar� por pantalla los fallos.
{
	int i;
	for (i = 0; i < *num; i++)
	{
		if (*(x + i) != *(y + i))
		{
			Serial.print("\nHas fallado en el pulso ");
			Serial.println(i + 1);
			Serial.print("Tenias que pulsar ");
			Serial.println(*(x + i));
			Serial.print("Has pulsado ");
			Serial.println(*(y + i));
		}
	}
	/// Muestro la secuencia de leds que deber�a haber pulsado.
	enunciado_secuencia(num, x);

}


// Funciones SECUNDARIAS.

///Indico el inicio de la partida.
void inicio_juego()
{
	Serial.println("\nINICIO SIMON DICE.");

	lcd.setCursor(0, 0);
	lcd.print("SIMON DICE");
	delay(2000);
	lcd.clear();
}

///Muestro en la ronda en la que nos encontramos.
void paso_ronda(int *x)
{
	Serial.print("\nRONDA: ");
	Serial.println(*x);

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Ronda ");
	lcd.print(*x);
	lcd.setCursor(0, 1);
	lcd.print("Mira leds");
	delay(2000);
}

void fin_secuencia() ///2 parpadeos de todos los leds al terminar la secuncia de entrada o salida.
{
	int i;

	for (i = 2; i <= 5; i++)
	{
		digitalWrite(i, HIGH);
	}
	delay(75);
	for (i = 2; i <= 5; i++)
	{
		digitalWrite(i, LOW);
	}
	delay(75);
	for (i = 2; i <= 5; i++)
	{
		digitalWrite(i, HIGH);
	}
	delay(75);
	for (i = 2; i <= 5; i++)
	{
		digitalWrite(i, LOW);
	}
	delay(75);
	Serial.println("\nFin de secuencia.");
}

///Con esta funci�n se informa de si has acertado o has perdido. Tambi�n te dice por el LCD cuantos fallos has cometido y por el serial especifica cuales.
void acierto_fallo(int x, int *fallos)
{
	int i;
	if (x == 0) /// Si acierta.
	{
		Serial.println("CORRECTO!");

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("CORRECTO!");

		for (i = 2; i <= 5; i++)
		{
			digitalWrite(i, HIGH);
		}
		delay(75);
		for (i = 2; i <= 5; i++)
		{
			digitalWrite(i, LOW);
		}
		delay(75);
		for (i = 2; i <= 5; i++)
		{
			digitalWrite(i, HIGH);
		}
		delay(75);
		for (i = 2; i <= 5; i++)
		{
			digitalWrite(i, LOW);
		}
		delay(75);
	}
	else
	{
		Serial.println("FALLASTE!");

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("FALLASTE!");
		lcd.setCursor(0, 1);
		lcd.print("Por ");
		lcd.print(*fallos);
		lcd.print(" fallos");
		delay(2000);
	}

}

/// Esta funci�n sirve tanto si ganas o si pierdes. Si ganas te felicita, si pierdes te muestra los fallos que has tenido confunciones anteriores.
void game_over(int x, int *vect1, int *vect2, int *r)	/// La x es el valor de flagfail. Si esta a 0 eso siginfica que has ganado, si no, perdiste.
{
	int i, j;
	if (x == 0)
	{
		Serial.println("HAS GANADO!!!\n\n");

		lcd.setCursor(0, 0);
		lcd.print("HAS GANADO!!!");

		esperaIII();
		esperaIII();
	}
	else
	{
		Serial.println("PERDISTE!!!\n\n");

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("HAS PERDIDO!!!");

		mostrar_fallos(vect1, vect2, r);
		///Juego de luces.
		for (i = 2; i <= 5; i++)
		{
			digitalWrite(i, HIGH);
		}
		delay(1000);
		for (i = 2; i <= 5; i++)
		{
			digitalWrite(i, LOW);
			delay(250);
		}
	}
}

// Funciones de ANIMACIONES DE LEDS.

void esperaI()
{
	int i, j;
	for (j = 0; j <= 1; j++)
	{
		for (i = 2; i <= 5; i++)
		{
			switch (i)
			{
			case 2:
				digitalWrite(2, HIGH);
				digitalWrite(3, LOW);
				digitalWrite(4, LOW);
				digitalWrite(5, LOW);
				break;
			case 3:
				digitalWrite(2, LOW);
				digitalWrite(3, HIGH);
				digitalWrite(4, LOW);
				digitalWrite(5, LOW);
				break;
			case 4:
				digitalWrite(2, LOW);
				digitalWrite(3, LOW);
				digitalWrite(4, HIGH);
				digitalWrite(5, LOW);
				break;
			case 5:
				digitalWrite(2, LOW);
				digitalWrite(3, LOW);
				digitalWrite(4, LOW);
				digitalWrite(5, HIGH);
				break;
			}
			delay(50);
		}
	}
}

void esperaII()
{
	int i;
	for (i = 5; i >= 2; i--)
	{
		switch (i)
		{
		case 2:
			digitalWrite(2, HIGH);
			digitalWrite(3, LOW);
			digitalWrite(4, LOW);
			digitalWrite(5, LOW);
			break;
		case 3:
			digitalWrite(2, LOW);
			digitalWrite(3, HIGH);
			digitalWrite(4, LOW);
			digitalWrite(5, LOW);
			break;
		case 4:
			digitalWrite(2, LOW);
			digitalWrite(3, LOW);
			digitalWrite(4, HIGH);
			digitalWrite(5, LOW);
			break;
		case 5:
			digitalWrite(2, LOW);
			digitalWrite(3, LOW);
			digitalWrite(4, LOW);
			digitalWrite(5, HIGH);
			break;
		}
		delay(50);
	}
}

void esperaIII()
{
	int i;
	for (i = 0; i <= 5; i++)
	{
		switch (i)
		{
		case 0:
			digitalWrite(2, HIGH);
			digitalWrite(3, LOW);
			digitalWrite(4, LOW);
			digitalWrite(5, LOW);
			break;
		case 1:
			digitalWrite(2, HIGH);
			digitalWrite(3, HIGH);
			digitalWrite(4, LOW);
			digitalWrite(5, LOW);
			break;
		case 2:
			digitalWrite(2, LOW);
			digitalWrite(3, HIGH);
			digitalWrite(4, HIGH);
			digitalWrite(5, LOW);
			break;
		case 3:
			digitalWrite(2, LOW);
			digitalWrite(3, LOW);
			digitalWrite(4, HIGH);
			digitalWrite(5, HIGH);
			break;
		case 4:
			digitalWrite(2, HIGH);
			digitalWrite(3, LOW);
			digitalWrite(4, LOW);
			digitalWrite(5, HIGH);
			break;
		case 5:
			digitalWrite(2, LOW);
			digitalWrite(3, LOW);
			digitalWrite(4, LOW);
			digitalWrite(5, HIGH);
			break;
		}
		delay(100);
	}
}

void encender_led()
{
	int i;
	for (i = 2; i <= 5; i++)
	{
		digitalWrite(i, HIGH);
	}
}

void apagar_led()
{
	int i;
	for (i = 2; i <= 5; i++)
	{
		digitalWrite(i, LOW);
	}
}