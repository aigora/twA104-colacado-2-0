//                                                                              SIMÓN DICE
#define _CRT_SECURE_NO_DEPRECATE
#include<stdlib.h>
#include<LiquidCrystal.h>
#define N 3
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD
LiquidCrystal lcd(A0, A1, 9, 8, 7, 6);

//Prototipos de funciones.

	///FUNCIONES PRINCIPALES
void seleccion_nivel(int*, int*, int*);		//La función "seleccion_nivel" se encargará de determinar el grado de dificultad de la partida.
void enunciado_secuencia(int*, int*);				//La función "enunciado_secuencia" mostrará por los 4 leds la secuencia creada aleatoriamente.
void leer_secuencia(int*, int*);						//La función "leer_secuencia" se encargará de leer la secuencia  introducida por el usuario mediante los botones.
int comparar_secuencia(int, int*, int*, int*, int*);		//La función "comparar_secuencia" comparará el vector secuencia y pulso para comprobar si hay algún error.
void mostrar_fallos(int*, int*, int*);				//La función "mostrar_fallo" te dirá en que botón has fallado.

	///FUNCIONES SECUNDARIAS
void inicio_juego();								//La función "inicio_juego" muestra que se ha iniciado el juego.
void game_over(int, int*, int*, int*);								//La función "game_over" indiga si el jugador ha ganado o a perdido.
void acierto_fallo(int, int*);							//La función "acierto_fallo" te dice si has acertado o has fallado.
void fin_secuencia();								//La función "fin_secuencia" enciende y apaga 2 veces los leds para indicar el fin de la secuencia mostrada por los leds.
void paso_ronda(int*);								//La función "paso_ronda" muestra por el monitor serial en que ronda nos encontramos.

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
	//Inicializo los pines de entrada y salida
	for (i = 0; i < 4; i++)
	{
		pinMode(pines.led[i], OUTPUT);
		pinMode(pines.boton[i], INPUT);
	}

	// Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
	lcd.begin(COLS, ROWS);
}

void loop() {
	//Variables
	int i, ronda = 1, rondamax = 0, fallos = 0, *pronda, *prondamax, *pfallos, flagfail = 0;
	int *psecuencia, *ppulso;
	pronda = &ronda;
	prondamax = &rondamax;
	pfallos = &fallos;
	/// "ronda" indica en que nivel estamos y el número de leds que se encenderán.
	/// "flagfail" me servirá para saber si hay algún fallo en la secuencia introducida por los botones.
	/// "nivel" indicará el grado de dificultad de la partida.
	/// "rondamax" variará dependiendo del nivel en el que estemos
	/// Variables para la creación de los vectores dinámicos.

	lcd.clear();
	inicio_juego();

//Determino el nivel de la partida.
	seleccion_nivel(prondamax, psecuencia, ppulso);

	//Inicializo los vectores dinámicos
	psecuencia = (int*)malloc((*prondamax) * sizeof(int));
	ppulso = (int*)malloc((*prondamax) * sizeof(int));
	if (psecuencia == NULL)
	{
		Serial.println("Error de memoria en VECTOR SECUENCIA.\n");
		exit(1);	///Si no hay espacio en memoria dinámica salir del programa.
	}
	if (ppulso == NULL)
	{
		Serial.println("Error de memoria en VECTOR PULSO.\n");
		exit(1);	///Si no hay espacio en memoria dinámica salir del programa.
	}

	///Inicializo la secuencia a adivinar.
	for (i = 0; i < (*prondamax); i++)
	{
		*(psecuencia + i) = rand() % (N + 1);		///Le asigno un valor aleatorio de 0 a 3 al vector psecuencia.
	}

	//FLUJO PRINCIPAL DEL JUEGO.
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
	game_over(flagfail, psecuencia, ppulso, pronda);

	free(psecuencia);
	free(ppulso);
	apagar_led();
	delay(2500);	///Tiempo hasta nuevo juego (2.5 segs)
}


// Funciones PRINCIPALES.

void seleccion_nivel(int *y, int *vectI, int *vectII)
{
	int i;
	//Elección del nivel de dificultad.
	Serial.println("\nSELECCIONA LA DIFICULTAD:\n 0-->Facil (5 rondas)\n 1-->Medio (10 rondas)\n 2-->Dificil (15 rondas)\n 3-->Extremo (30 rondas)");
	lcd.setCursor(1, 0);
	lcd.print("Elige el nivel");
	lcd.setCursor(5, 1);
	lcd.print("3 2 1 0");

	while ((digitalRead(10) == 0) && (digitalRead(11) == 0) && (digitalRead(12) == 0) && (digitalRead(13) == 0))
	{
		esperaI();
		digitalRead(10);
		digitalRead(11);
		digitalRead(12);
		digitalRead(13);
		esperaII();
	}

	if (digitalRead(10) == HIGH)	///FÁCIL
	{
		*y = 5;//Numero de rondas

		Serial.println("FACIL");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("FACIL");
		lcd.setCursor(0, 1);
		lcd.print("5 rondas");

		digitalWrite(2, HIGH); ///CONFIRMACIÓN DE LECTURA.
		delay(200);
		digitalWrite(2, LOW);

		apagar_led();
		delay(2250);
	}
	else if (digitalRead(11) == HIGH)	///MEDIO
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
	else if (digitalRead(12) == HIGH)	///DIFÍCIL
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
	else if (digitalRead(13) == HIGH)	///EXTREMO
	{
		*y = 30;
		Serial.println("EXTREMO");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("EXTREMO");
		lcd.setCursor(0, 1);
		lcd.print("10 rondas");

		digitalWrite(5, HIGH);
		delay(200);
		digitalWrite(5, LOW);

		apagar_led();
		delay(2250);
	}

	Serial.println();
	lcd.clear();
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


void leer_secuencia(int *x, int *vect) // "x" es la ronda. "*vect" es el vector 'ppulso' que iré creando a medida que vaya pulsando
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
		lcd.print("--> ");
		// ¡¡¡OJO!!! Para que se rellene el vector hay que dejar que parpadee la luz (confirmación de lectura).

		//Este bucle while me sirve para esperar a que el usuario pulse un boton.
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

			digitalWrite(2, HIGH); //CONFIRMACIÓN DE LECTURA.
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
		Serial.println(*(vect + i)); //Muestro el boton que se pulsa en cada turno.
	}


	for (i = 0; i < *x; i++) //Muestro los botones pulsados
	{
		Serial.print(*(vect + i), DEC);
	}
	Serial.println("\n");

}


int comparar_secuencia(int w, int *r, int *x, int *y, int *cont) // "x" será el vector secuencia, e "y" el vector pulso. "w" es la ronda. "*r" es un puntero que cambia el valor de la ronda en caso de acertar.
{
	int i, flag = 0; 

	for (i = 0; i < w; i++)//Comprobación de si he acertado o no en la ronda. (Comparo si los vectores de secuencia y pulso son iguales).
	{
		if (*(x + i) != *(y + i)) //En caso de fallar, activo el flagfail a 1 
		{
			flag = 1;
			(*cont)++;
		}
		// Si son el mismo vector, eso es que se ha acertado y el valor de flag seguirá siendo 0;
		else
		{
			//Si acertamos, aumento una ronda. (A lo que apunta r(ronda), le doy el valor de w(ronda) mas una unidad).
			*r = w + 1;
		}
	}
	return flag;
}


void mostrar_fallos(int *x, int *y, int *num)
//Los vectores de los parámetros son el pulso y la secuencia. Los iré comparando para ver que valores son diferentes. Mostraré por pantalla los fallos.
{
	int i;
	for (i = 0; i < *num - 1; i++)
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
	//Muestro de nuevo la secuencia por los leds.
	enunciado_secuencia(num, x);

}


//Funciones SECUNDARIAS.

void inicio_juego()
{
	Serial.println("\nINICIO SIMON DICE.");

	lcd.setCursor(0, 0);
	lcd.print("SIMON DICE");
	delay(2000);
	lcd.clear();
}

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

void fin_secuencia() ///2 parpadeos de todos los leds.
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

void acierto_fallo(int x, int *fallos)
{
	int i;
	if (x == 0) /// Si acierta
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
