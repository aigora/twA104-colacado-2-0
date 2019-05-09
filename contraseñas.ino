// DESCIFRA EL CÓDIGO.

#define _CRT_SECURE_NO_DEPRECATE
#include<stdlib.h>
#define N 4

void inicio_juego();
void leer_secuencia(int*);
int comparar_secuencia(int*, int*);
void game_over();
void acierto_fallo(int*);

typedef struct {

	int led[4] = { 2, 3, 4, 5 };

	int boton[4] = { 10, 11, 12, 13 };

}PIN;


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

void loop() {

	int i, flagfail = 0;
	int secuencia[N], pulso[N], *psecuencia, *ppulso;
	psecuencia = &secuencia[0];
	ppulso = &pulso[0];
	for (i = 0; i < N; i++)
	{
		///Le asigno un valor aleatorio de 0 a 3 al vector secuencia, sin repetirse.
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
	//ESTO MUESTRA LA CONTRASEÑA POR PANTALLA
	for (i = 0; i < N; i++) {
		Serial.println(*(psecuencia + i));
	}

	inicio_juego();

	while (flagfail == 0) {

		leer_secuencia(ppulso);
		delay(500);
		flagfail = comparar_secuencia(psecuencia, ppulso); //(secuencia,pulso)
		acierto_fallo(&flagfail);


	}

	game_over();

}

void inicio_juego() {

	Serial.println("\nINICIO CONTRA.");

}

void leer_secuencia(int* vect) {

	int i;
	for (i = 0; i < N; i++) {
		while ((digitalRead(10) == 0) && (digitalRead(11) == 0) && (digitalRead(12) == 0) && (digitalRead(13) == 0)) {

			digitalRead(10);

			digitalRead(11);

			digitalRead(12);

			digitalRead(13);

		}

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
	}

	delay(250);
}

int comparar_secuencia(int *x, int *y) {
	int i, flag = 0, v1[N], v2[N];

	for (i = 0; i < N; i++) {
		*(x + i);
		v1[i] = *(x + i);
	}
	for (i = 0; i < N; i++) {
		*(y + i);
		v2[i] = *(y + i);
	}
  
  //ILUMINA LAS QUE HAS FALLADO.
	if (v1[0] != v2[0]) {
		digitalWrite(2, HIGH); 

		delay(200);

		digitalWrite(2, LOW);
	}
	if (v1[1] != v2[1]) {
		digitalWrite(3, HIGH);

		delay(200);

		digitalWrite(3, LOW);
	}
	if (v1[2] != v2[2]) {
		digitalWrite(4, HIGH);

		delay(200);

		digitalWrite(4, LOW);
	}
	if (v1[3] != v2[3]) {
		digitalWrite(5, HIGH);

		delay(200);

		digitalWrite(5, LOW);
	}

	if (v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2] || v1[3] != v2[3]) //En caso de fallar, activo el flagfail a 1
	{
		flag = 1;
	}
	// Si son el mismo vector, eso es que se ha acertado y el valor de flag seguirá siendo 0;
	else
	{
		flag = 0;
	}


	
	return flag;


}

void acierto_fallo(int* x) //x es el valor de flagfail
{
	if (*x == 0)//Si hemos acertado todo.
	{
		Serial.println("Acertaste");
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
	}
	else
	{
		Serial.println("Fallaste");
		//Encontrar la forma de decirle al usuario que botones ha acertado.
    }
}
void game_over() {
	Serial.println("\nGAME OVER");
}
