//                                                       SIMÓN DICE
//Prototipos de funciones.
void inicio_juego();        //La función "inicio_juego" muestra que se ha iniciado el juego.
void game_over();        //La función "game_over" muestra que se has perdido el juego.
void fin_secuencia();       //La función "fin_secuencia" enciende y apaga 2 veces los leds para indicar el fin de la secuencia mostrada por los leds.
void enunciado_secuencia(int, int[]);     //La función "enunciado_secuencia" mostrará por los 4 leds la secuencia creada aleatoriamente.
void leer_secuencia(int);                 //La función "leer_secuencia" se encargará de leer la secuencia  introducida por el usuario mediante los botones.
void mostrar_fallos();                    //La función "mostrar_fallo" te dirá en que botón has fallado.


//Variables
int led[4]={2, 3, 4, 5};
int boton[4]={10, 11, 12, 13};
int i, ronda=8, secuencia[8]={0, 1, 2, 3, 3, 0, 2, 1}, pulso[8], flagfail=0;
    // "ronda" indica en que nivel estamos y el número de leds que se encenderán.
    // "secuencia" es un vector creado aleatoriamente que contendrá la posicón en el vector del led que se tiene que encender.
    // "pulso" es un vector donde guardaré los botones que pulsará el usuario en su turno de juego para después compararlo con la secuencia.
    // "flagfail" me servirá para saber si hay algún fallo en la secuencia introducida por los botones.
    
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

//  while(ronda<=8 && flagfail==0) //Bucle en el que se dearrolla el juego. Saldré de el en caso de fallar (flagfail=1) o en caso de que se me acabe el vector secuencia.
//  {
    enunciado_secuencia(ronda, secuencia);
    delay(3000);
    leer_secuencia(ronda);
    delay(3000);
    
//    for(i=0;i<ronda;i++) //Comprobación de si he acertado o no en la ronda. (Comparo si los vectores de secuencia y pulso son iguales).
//    {
//      if(secuencia[i] != pulso[i]) //En caso de fallar, activo el flagfail a 1 
//      {
//        flagfail=1;
//      }
//    }
  
  game_over();
}




// FUNCIONES.

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
  
  for(i=0;i<ronda;i++) //Muestro los botones pulsados (test de comprobación)
  {
  Serial.print(secuencia[i]);
  }
    
  fin_secuencia();
}



void leer_secuencia(int x) // "x" es la ronda. "pulso[]" es el vector que iré creando a medida que vaya pulsando
{
  int i;

  
  for(i=0;i<x;i++) // "x" es la ronda. "pulso[]" es el vector donde se guarda las pulsaciones del usuario. 
  {
    int i;
    Serial.print("Introduce pulso ");
    Serial.println(i);
    // ¡¡¡OJO!!! Para que se rellene el vector pulso hay que mantener pulsado el boton hasta que parpadee la luz (confirmación de lectura).
    
    digitalRead(10);
    digitalRead(11);
    digitalRead(12);
    digitalRead(13);
    delay(3000);
    
    if(digitalRead(10)==HIGH)
    {
      pulso[i]=0;
      digitalWrite(2, HIGH); //CONFIRMACIÓN DE LECTURA.
      delay(100);
      digitalWrite(2, LOW);
    }
    else if(digitalRead(11)==HIGH)
    {
      pulso[i]=1;
      digitalWrite(3, HIGH);
      delay(100);
      digitalWrite(3, LOW);
    }
    else if(digitalRead(12)==HIGH)
    {
      pulso[i]=2;
      digitalWrite(4, HIGH);
      delay(100);
      digitalWrite(4, LOW);
    }
    else if(digitalRead(13)==HIGH)
    {
      pulso[i]=3;
      digitalWrite(5, HIGH);
      delay(100);
      digitalWrite(5, LOW);
    }
    delay(1000);
  }
  
  for(i=0;i<ronda;i++) //Muestro los botones pulsados
  {
  Serial.print(pulso[i]);
  }
  Serial.println("\n");
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
  Serial.println("\nINICIO SIMÓN DICE.");
}



void game_over()
{
  Serial.println("\nGAME OVER SIMÓN DICE.\n");
}
