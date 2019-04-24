//                                                       SIMÓN DICE
//Prototipos de funciones.
void inicio_juego();						//La función "inicio_juego" muestra que se ha iniciado el juego.
void acierto_fallo(int);					//La función "acierto_fallo" te dice si has acertado o has fallado.
void game_over();						//La función "game_over" muestra que se has perdido el juego.
void fin_secuencia();						//La función "fin_secuencia" enciende y apaga 2 veces los leds para indicar el fin de la secuencia mostrada por los leds.
void paso_ronda(int);						//La función "paso_ronda" muestra por el monitor serial en que ronda nos encontramos.
void enunciado_secuencia(int, int[]);				//La función "enunciado_secuencia" mostrará por los 4 leds la secuencia creada aleatoriamente.
void leer_secuencia(int, int[]);				//La función "leer_secuencia" se encargará de leer la secuencia  introducida por el usuario mediante los botones.
int comparar_secuencia(int, int*, int[], int[]);		//La función "comparar_secuencia" comparará el vector secuencia y pulso para comprobar si hay algún error.
//void mostrar_fallos();					//La función "mostrar_fallo" te dirá en que botón has fallado.

//Variables globales (Las utilizo para no tener que declararlas siempre en todas las funciones.)
int led[4] = { 2, 3, 4, 5 };
int boton[4] = { 10, 11, 12, 13 };


void setup() {
	Serial.begin(9600);
	int i;
	//Inicializo los pines de entrada y salida
	for (i = 0; i < 4; i++)
	{
		pinMode(led[i], OUTPUT);
		pinMode(boton[i], INPUT);
	}
}

void loop() {
	//Variables
	int i, ronda = 1, secuencia[8] = { 0, 1, 2, 3, 3, 0, 2, 1 }, pulso[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }, flagfail = 0;
	// "ronda" indica en que nivel estamos y el número de leds que se encenderán.
	// "secuencia" es un vector creado aleatoriamente que contendrá la posicón en el vector del led que se tiene que encender.
	// "pulso" es un vector donde guardaré los botones que pulsará el usuario en su turno de juego para después compararlo con la secuencia.
	// "flagfail" me servirá para saber si hay algún fallo en la secuencia introducida por los botones.
    

	//FLUJO PRINCIPAL
    inicio_juego();
	while (ronda <= 8 && flagfail == 0)
	{
		paso_ronda(ronda);
		enunciado_secuencia(ronda, secuencia);
		leer_secuencia(ronda, pulso);
		flagfail = comparar_secuencia(ronda, &ronda, secuencia, pulso);
		acierto_fallo(flagfail); //En caso de acertar
		delay(1000);
	}
	game_over();
	delay(3000); //Tiempo hasta nuevo juego (3 segs)
}



// Funciones PRINCIPALES del juego.

void enunciado_secuencia(int x, int vect[])  // "x" es la ronda. "vect[]" es el vector aleatorio con los leds a encender, es decir la secuencia.
{
	int i, posicion, luz;

	for (i = 0; i < x; i++)
	{
		posicion = vect[i];       // Leo el vector de la secuencia que me indica el led a encender
		luz = led[posicion];   // Determino el pin que ha que encender

		digitalWrite(luz, HIGH);
		delay(1000);
		digitalWrite(luz, LOW);
		delay(1000);
	}

	for (i = 0; i < x; i++) //Muestro los botones pulsados
	{
		Serial.print(vect[i]);
	}

	fin_secuencia();
}


void leer_secuencia(int x, int vect[]) // "x" es la ronda. "pulso[]" es el vector que iré creando a medida que vaya pulsando
{
	int i, cont = 1;//cont sirve mostrar en que turno vas de pulsado

	for (i = 0; i < x; i++) // "x" es la ronda. "pulso[]" es el vector donde se guarda las pulsaciones del usuario. 
	{
		Serial.print("Introduce pulso ");
		Serial.print(cont);
		Serial.print(" --> ");
		// ¡¡¡OJO!!! Para que se rellene el vector hay que mantener pulsado el boton hasta que parpadee la luz (confirmación de lectura).
		
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
			vect[i] = 0;
			digitalWrite(2, HIGH); //CONFIRMACIÓN DE LECTURA.
			delay(100);
			digitalWrite(2, LOW);
		}
		else if (digitalRead(11) == HIGH)
		{
			vect[i] = 1;
			digitalWrite(3, HIGH);
			delay(100);
			digitalWrite(3, LOW);
		}
		else if (digitalRead(12) == HIGH)
		{
			vect[i] = 2;
			digitalWrite(4, HIGH);
			delay(100);
			digitalWrite(4, LOW);
		}
		else if (digitalRead(13) == HIGH)
		{
			vect[i] = 3;
			digitalWrite(5, HIGH);
			delay(100);
			digitalWrite(5, LOW);
		}
		delay(250);

		cont++;
		Serial.println(vect[i]); //Muestro el boton que se pulsa en cada turno.
	}


	for (i = 0; i < x; i++) //Muestro los botones pulsados
	{
		Serial.print(vect[i], DEC);
	}
	Serial.println("\n");


}


int comparar_secuencia(int w, int *r, int x[], int y[]) // "x" será el vector secuencia, e "y" el vector pulso. "w" es la ronda
{
	int i, flag = 0;

	for (i = 0; i < w; i++)//Comprobación de si he acertado o no en la ronda. (Comparo si los vectores de secuencia y pulso son iguales).
	{
		if (x[i] != y[i]) //En caso de fallar, activo el flagfail a 1 
		{
			flag = 1;
		}
		// Si son el mismo vector, eso es que se ha acertado y el valor de flag seguirá siendo 0;
		else
		{
			*r = w + 1; //Si acertamos, aumento una ronda. (A lo que apunta r(ronda), le doy el valor de w(ronda) mas una unidad).
		}
	}
	return flag;
}




//Funciones SECUNDARIAS del juego.

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


void acierto_fallo (int x)
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

	//Juego de luces final
	//for (i = 0; i < 3; i++)
	//{
	//	for (j = 2; j <= 5; j++)
	//	{
	//		switch (j)
	//		{
	//		case 2:
	//			digitalWrite(2, HIGH);
	//			digitalWrite(3, LOW);
	//			digitalWrite(4, LOW);
	//			digitalWrite(5, LOW);
	//			break;
	//		case 3:
	//			digitalWrite(2, LOW);
	//			digitalWrite(3, HIGH);
	//			digitalWrite(4, LOW);
	//			digitalWrite(5, LOW);
	//			break;
	//		case 4:
	//			digitalWrite(2, LOW);
	//			digitalWrite(3, LOW);
	//			digitalWrite(4, HIGH);
	//			digitalWrite(5, LOW);
	//			break;
	//		case 5:
	//			digitalWrite(2, LOW);
	//			digitalWrite(3, LOW);
	//			digitalWrite(4, LOW);
	//			digitalWrite(5, HIGH);
	//			break;
	//		}
	//	}
	//}

	//for (j = 2; j <= 5; i++) // Apago los leds
	//{
	//	digitalWrite(i, LOW);
	//}
}

void paso_ronda(int x)
{
	Serial.print("\nRONDA: ");
	Serial.println(x + 1);
}
