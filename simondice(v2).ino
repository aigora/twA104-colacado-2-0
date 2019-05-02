//                                                                              SIMÓN DICE
#define _CRT_SECURE_NO_DEPRECATE
#include<stdlib.h>
#define N 3
//Prototipos de funciones.
void inicio_juego();								//La función "inicio_juego" muestra que se ha iniciado el juego.
void game_over();									//La función "game_over" muestra que se has perdido el juego.
void acierto_fallo(int);							//La función "acierto_fallo" te dice si has acertado o has fallado.
void fin_secuencia();								//La función "fin_secuencia" enciende y apaga 2 veces los leds para indicar el fin de la secuencia mostrada por los leds.
void paso_ronda(int);								//La función "paso_ronda" muestra por el monitor serial en que ronda nos encontramos.
void enunciado_secuencia(int, int*);				//La función "enunciado_secuencia" mostrará por los 4 leds la secuencia creada aleatoriamente.
void leer_secuencia(int, int*);						//La función "leer_secuencia" se encargará de leer la secuencia  introducida por el usuario mediante los botones.
int comparar_secuencia(int, int*, int*, int*);		//La función "comparar_secuencia" comparará el vector secuencia y pulso para comprobar si hay algún error.
//void mostrar_fallos();							//La función "mostrar_fallo" te dirá en que botón has fallado.

//Variables globales (Las utilizo para no tener que declararlas siempre en todas las funciones.)
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
}

void loop() {
	//Variables
	int i, ronda = 1, flagfail = 0;
		/// "ronda" indica en que nivel estamos y el número de leds que se encenderán.
		/// "flagfail" me servirá para saber si hay algún fallo en la secuencia introducida por los botones.
		///
	//Variables para la creación de los vectores dinámicos.
	int *psecuencia, *ppulso;				///Estos serán los vectores a comparar
	

	//Inicializo los vectores dinámicos
	psecuencia = (int*)malloc(1 * sizeof(int));
	ppulso = (int*)malloc(1 * sizeof(int));
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
		///Asigno el primer valor aleatorio al vector de la secuencia.
	*(psecuencia) = rand() % (N + 1);		///ronda-1 porque he inicializado las rondas a 1. 

	//FLUJO PRINCIPAL
	inicio_juego();
	while (flagfail == 0)
	{
		paso_ronda(ronda);
		enunciado_secuencia(ronda, psecuencia);
		leer_secuencia(ronda, ppulso);
		flagfail = comparar_secuencia(ronda, &ronda, psecuencia, ppulso);
		acierto_fallo(flagfail);
		delay(500);
	}
	game_over();
	free(psecuencia);
	free(ppulso);
	delay(2000);	///Tiempo hasta nuevo juego (2 segs)
}




// Funciones PRINCIPALES del juego.

void enunciado_secuencia(int x, int *vect)
{	/// "x" es la ronda. "*vect" es el vector psecuencia, aleatorio con los leds a encender, es decir la secuencia.
	int i, posicion, luz;
	PIN pines;

	for (i = 0; i < x; i++)
	{
		posicion = *(vect + i);				/// Leo el vector de la secuencia que me indica el led a encender.
		luz = pines.led[posicion];			/// Determino el pin que ha que encender.

		digitalWrite(luz, HIGH);
		delay(500);
		digitalWrite(luz, LOW);
		delay(500);
	}

	//vect = &vect[0];						///Reinicio el puntero al inicio del vector psecuencia.

	for (i = 0; i < x; i++)					///Muestro los botones a pulsar
	{
		Serial.print(*(vect + i));
	}

	fin_secuencia();
}




void leer_secuencia(int x, int *vect) // "x" es la ronda. "*vect" es el vector 'ppulso' que iré creando a medida que vaya pulsando
{
	int i, cont = 1; //cont sirve mostrar en que turno vas de pulsado

	for (i = 0; i < x; i++) 
	{
		Serial.print("Introduce pulso ");
		Serial.print(cont);
		Serial.print(" --> ");
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
			digitalWrite(2, HIGH); //CONFIRMACIÓN DE LECTURA.
			delay(200);
			digitalWrite(2, LOW);
		}
		else if (digitalRead(11) == HIGH)
		{
			*(vect + i) = 1;
			digitalWrite(3, HIGH);
			delay(200);
			digitalWrite(3, LOW);
		}
		else if (digitalRead(12) == HIGH)
		{
			*(vect + i) = 2;
			digitalWrite(4, HIGH);
			delay(200);
			digitalWrite(4, LOW);
		}
		else if (digitalRead(13) == HIGH)
		{
			*(vect + i) = 3;
			digitalWrite(5, HIGH);
			delay(200);
			digitalWrite(5, LOW);
		}
		delay(250);

		cont++;
		Serial.println(*(vect + i)); //Muestro el boton que se pulsa en cada turno.
	}
	vect = &vect[0]; //Reinicio el puntero al inicio del vector pulso.

	for (i = 0; i < x; i++) //Muestro los botones pulsados
	{
		Serial.print(*(vect + i), DEC);
	}
	Serial.println("\n");
	vect = &vect[0]; //Reinicio el puntero al inicio del vector pulso.
}




int comparar_secuencia(int w, int *r, int *x, int *y) // "x" será el vector secuencia, e "y" el vector pulso. "w" es la ronda. "*r" es un puntero que cambia el valor de la ronda en caso de acertar.
{
	int i, flag = 0, *aux;//aux es un 

	for (i = 0; i < w; i++)//Comprobación de si he acertado o no en la ronda. (Comparo si los vectores de secuencia y pulso son iguales).
	{
		if (*(x + i) != *(y + i)) //En caso de fallar, activo el flagfail a 1 
		{
			flag = 1;
		}
		// Si son el mismo vector, eso es que se ha acertado y el valor de flag seguirá siendo 0;
		else
		{
			//Si acertamos, aumento una ronda. (A lo que apunta r(ronda), le doy el valor de w(ronda) mas una unidad).
			*r = w + 1;
		}
	}
		//Reinicio punteros al inicio de los vectores.
	x = &x[0];
	y = &y[0];

	//En caso de acertar también habrá que aumentar el tamaño de los vectores.
	if (flag == 0)
	{
		aux = (int*)malloc((w) * sizeof(int));  //Copio en vector de secuencia en un nuevo vector. OJO! LA w YA ESTA CAMBIADA
		for (i = 0; i < (w); i++)
		{
			*(aux + i) = *(x + i);
			Serial.print(*(aux + i));
		}
		Serial.print(" <-- aux\n");
			//Aumento el tamaño de los vectores. (El tamaño que tenian antes mas un nuevo espacio.
		x = (int*)realloc(x, (w) * sizeof(int));
		y = (int*)realloc(y, (w) * sizeof(int));
			//Vuelvo acrear el vector de secuencia coiandole el auxiliar.
		for (i = 0; i < w; i++)
		{
			*(x + i) = *(aux + i);
			Serial.print(*(x + i));
		}
			//Asigno un nuevo valor aleatorio al nuevo espacio del vector secuencia, con un valor entre 0 y 4 == rand()%(4+1).
		*(x + w) = rand() % (N + 1); //w-1 porque las rondas están inicializadas en 1.
		Serial.print(*(x + w));
		Serial.print(" <-- x\n");


			//Reinicio el vector de los pulso a 0 
		//y = &y[0];	///Pongo el puntero al inicio del vector.
		for (i = 0; i < w; i++)
		{
			*(y + i) = 0;
		}

		if (x == NULL)
		{
			Serial.println("Error de memoria al aumentar vector psecuencia.\n");
			//exit(1);	///Si no hay espacio en memoria dinámica salir del programa.
		}
		if (y == NULL)
		{
			Serial.println("Error de memoria al aumentar vector ppulso.\n");
			//exit(1);	///Si no hay espacio en memoria dinámica salir del programa.
		}
		free(aux);
	}
	return flag;
}











//Funciones SECUNDARIAS del juego.

void paso_ronda(int x)
{
	Serial.print("\nRONDA: ");
	Serial.println(x);
}


void fin_secuencia() //2 parpadeos
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


void inicio_juego()
{
	Serial.println("\nINICIO SIMON DICE.");
}


void acierto_fallo(int x)
{
	if (x == 0) //Si acierta
	{
		Serial.println("CORRECTO!");
	}
	else
	{
		Serial.println("FALLASTE!");
	}

}

void game_over()
{
	int i, j;

	Serial.println("GAME OVER SIMON DICE.\n\n");
	//Hacer juego de luces para final.
}
