// DESCIFRA EL CÓDIGO.

#define _CRT_SECURE_NO_DEPRECATE
#include<stdlib.h>
#define N 4

//FUNCIONES
void inicio_juego(); //Te muestra por pantalla INICIO DESCIFRA EL CÓDIGO.
void leer_secuencia(int*); //Lee la secuencia introducia por el usuario y te lo guarda en un vector.
int comparar_secuencia(int*, int*, int*); //Compara el vector creado aleatoriamente en el loop y lo compara con el creado en leer secuencia. Si son iguales ganas y sino tienes 3 intentos para acertar o pierdes.
void game_over(); //Te muestra por pantalla GAME OVER.
void acierto_fallo(int*); //Te dice si has acertado (Secuencia de leds), y si fallas dice que has fallado y te dice cuantos intentos te quedan.


//Se crea una estructura con un vector led para que localize el led en el arduino y un boton para que localize los botones en el arduino.
typedef struct {

	int led[4] = { 2, 3, 4, 5 };

	int boton[4] = { 10, 11, 12, 13 };

}PIN;


//Aqui se inicializan las variables.
void setup() {

	Serial.begin(9600);
	int i;
	PIN pines;

	//Inicializo los pines de entrada y salida

	for (i = 0; i < 4; i++) {

		pinMode(pines.led[i], OUTPUT);
		pinMode(pines.boton[i], INPUT);
	}
	
}


//Aqui se realiza el juego continuamente.
void loop() {

	int i, flagfail = 0;
	int secuencia[N], pulso[N], *psecuencia, *ppulso;
	psecuencia = &secuencia[0];
	ppulso = &pulso[0];

	inicio_juego();



	do
	{
		//Le asigno un valor aleatorio de 0 a 3 al vector secuencia, sin repetirse.
		for (i = 0; i < N; i++)
		{
			
			switch (i) {
			case 0:
				*(psecuencia + i) = rand() % (N);
			case 1:
				do
				{
					*(psecuencia + i) = rand() % (N);
				} while (*(psecuencia + i) == *(psecuencia + i - 1));
			case 2:
				do
				{
					*(psecuencia + i) = rand() % (N);
				} while (*(psecuencia + i) == *(psecuencia + i - 1) || *(psecuencia + i) == *(psecuencia + i - 2));
			case 3:
				do
				{
					*(psecuencia + i) = rand() % (N);
				} while (*(psecuencia + i) == *(psecuencia + i - 1) || *(psecuencia + i) == *(psecuencia + i - 2) || *(psecuencia + i) == *(psecuencia + i - 3));
			}
		}

		//ESTO MUESTRA LA CONTRASEÑA POR PANTALLA.
		/*for (i = 0; i < N; i++) {
			Serial.print(*(psecuencia + i));
		}*/
		
		Serial.println(" ");
		leer_secuencia(ppulso);
		delay(500);

		flagfail = comparar_secuencia(psecuencia, ppulso, &flagfail); //Te devuelve el flagfail, si es 0 has ganado y te pone una nueva contraseña, sino entra en el if y tienes dos intentos mas.
		acierto_fallo(&flagfail);

		
		//En caso de fallar tienes 2 intentos más. 
		if (flagfail != 0) {

			while (flagfail < 3 && flagfail != 0)  //Si la aciertas el falgfail es 0, sales de este bucle y te pone una nueva contraseña. En cambio si fallas 2 veces más a parte de la primera vez saldras de los dos bucles irás a la función game_over() que significa que has perdido.
			{
				leer_secuencia(ppulso);
				delay(500);
				flagfail = comparar_secuencia(psecuencia, ppulso, &flagfail);
				acierto_fallo(&flagfail);
			}
		}

	} while (flagfail < 3);


	game_over();
	delay(2500);
}


void inicio_juego() {

	Serial.println("\nINICIO DESCIFRA EL CÓDIGO.");

}


void leer_secuencia(int *vect) {

	int i;

	for (i = 0; i < N; i++)
	{
		Serial.print("pulso ");
		Serial.print(i + 1);
		Serial.print("--> ");

		//Este bucle while sirve para esperar a que el usuario pulse un boton.
		while ((digitalRead(10) == 0) && (digitalRead(11) == 0) && (digitalRead(12) == 0) && (digitalRead(13) == 0)) {

			digitalRead(10);
			digitalRead(11);
			digitalRead(12);
			digitalRead(13);

		}

		//Usuario crea el vector.
		if (digitalRead(10) == HIGH) {

			*(vect + i) = 0;

			digitalWrite(2, HIGH); //CONFIRMACIÓN DE LECTURA.
			delay(200);
			digitalWrite(2, LOW);

		}

		else if (digitalRead(11) == HIGH) {

			*(vect + i) = 1;

			digitalWrite(3, HIGH);
			delay(200);
			digitalWrite(3, LOW);

		}

		else if (digitalRead(12) == HIGH) {

			*(vect + i) = 2;

			digitalWrite(4, HIGH);
			delay(200);
			digitalWrite(4, LOW);

		}

		else if (digitalRead(13) == HIGH) {

			*(vect + i) = 3;
			digitalWrite(5, HIGH);
			delay(200);
			digitalWrite(5, LOW);
		}
		delay(100);
		Serial.println(*(vect + i));
	}

	delay(250);
}


int comparar_secuencia(int *x, int *y, int *flag) {
	int i, v1[N], v2[N];
	PIN pines;
	int aux;

	//Pone en v1 en vector creado aleatoriamente y en v2 el creado por el usuario.
	for (i = 0; i < N; i++) {
		v1[i] = *(x + i);
	}
	for (i = 0; i < N; i++) {
		v2[i] = *(y + i);
	}


	

	for (i = 0; i < N; i++)
	{
		if (*(x + i) != *(y + i)) //Si fallas te dice que botones has fallado.
		{
			Serial.print("\nHas fallado en el pulso ");

			Serial.println(i + 1);

			//Sirven para ver el boton que tenias que haber pulsado y el que has pulsado. 

			/*Serial.print("Tenias que pulsar ");

			Serial.println(*(x + i));

			Serial.print("Has pulsado ");

			Serial.println(*(y + i));*/
		}
	}

	//Compara los vectores.
	if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2] && v1[3] == v2[3])
	{
		*flag = 0;
	}

	if (v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2] || v1[3] != v2[3])
	{
		*flag = *flag + 1;
	}


	return *flag;
}

void acierto_fallo(int *x) //x es el valor de flagfail
{
	if (*x == 0)//Si hemos acertado todo.
	{
		Serial.println("Acertaste");
		Serial.println(" ");
		digitalWrite(2, HIGH);
		delay(200);
		digitalWrite(2, LOW);
		delay(100);
		digitalWrite(3, HIGH);
		delay(200);
		digitalWrite(3, LOW);
		delay(100);
		digitalWrite(4, HIGH);
		delay(200);
		digitalWrite(4, LOW);
		delay(100);
		digitalWrite(5, HIGH);
		delay(200);
		digitalWrite(5, LOW);

		digitalWrite(2, HIGH);
		digitalWrite(3, HIGH);
		delay(200);
		digitalWrite(2, LOW);
		digitalWrite(3, LOW);
		delay(100);
		digitalWrite(4, HIGH);
		digitalWrite(5, HIGH);
		delay(200);
		digitalWrite(4, LOW);
		digitalWrite(5, LOW);


	}
	else //Si has fallado, y te dice cuantos intentos te quedan.
	{
		Serial.println("Fallaste");
		Serial.print("Te quedan ");
		Serial.print(3 - *x);
		Serial.println(" intentos.");
	}
}

void game_over() {
	Serial.println("\nGAME OVER");

	digitalWrite(2, HIGH);
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	digitalWrite(5, HIGH);
	
	delay(1000);

	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
}
