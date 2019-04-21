//                                                       SIMÓN DICE
//Prototipos de funciones.
void inicio_juego();        //La función "inicio_juego" muestra que se ha iniciado el juego.
void game_over();        //La función "game_over" muestra que se has perdido el juego.
void fin_secuencia();       //La función "fin_secuencia" enciende y apaga 2 veces los leds para indicar el fin de la secuencia mostrada por los leds.
void enunciado(int, int[]); //La función "enunciado" mostrará por los 4 leds la secuencia creada aleatoriamente.


//Variables
int led[4]={2, 3, 4, 5};
int boton[4]={10, 11, 12, 13};
int i, ronda=8, secuencia[8]={0, 1, 2, 3, 3, 0, 2, 1};
    // "ronda" indica en que nivel estamos y el número de leds que se encenderán.
    // "secuencia" es un vector creado aleatoriamente que contendrá la posicón en el vector del led que se tiene que encender.



    
void setup() {
  //Inicializo los pines de entrada y salida
  for(i=0;i<4;i++)
  {
    pinMode(led[i], OUTPUT);
    pinMode(boton[i], INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  inicio_juego();

  
  enunciado(ronda, secuencia);
  delay(3000);

  

  game_over();
}






// Funciones.

void enunciado(int x, int vect[])  // "x" es la ronda. "vect[]" es el vector aleatorio con los leds a encender, es decir la secuencia.
{
  int i, posicion, luz;

  for(i=0;i<x;i++)
  {
    posicion=vect[i];       // Leo el vector de la secuencia que me indica el led a encender
    luz = led[posicion];   // Determino el pin que ha que encender

    digitalWrite(luz, HIGH);
    delay(250);
    digitalWrite(luz, LOW);
    delay(250);
  }
  fin_secuencia();
}

void fin_secuencia()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(75);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(75);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(75);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(75);
  Serial.println("Fin de secuencia.");
}
void inicio_juego()
{
  Serial.println("INICIO SIMÓN DICE.");
}
void game_over()
{
  Serial.println("GAME OVER SIMÓN DICE.\n");
}
