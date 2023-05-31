/*
______                     _         ______  _    _ 
| ___ \                   | |        |  ___|| |  | |
| |_/ /__ _   __ _   __ _ | |_  ___  | |_   | |  | |
|  __// _` | / _` | / _` || __|/ _ \ |  _|  | |/\| |
| |  | (_| || (_| || (_| || |_|  __/ | |    \  /\  /
\_|   \__,_| \__, | \__,_| \__|\___| \_|     \/  \/ 
              __/ |                                 
             |___/                                  
*/

/*Variável input Fototransistor com divisor de tensao de 1kΩ

Leituras analógicas de 0 - 471
Entendimento de interrupção:
-Interrupção = Sinal al < 200
-Sem obstrução = Sinal al > 200
*/
int inputLuz = A3;

/*Inicio do timer->
Possível uso para sincronização com outras unidades
*/
unsigned int startTime;

//Variavel para armazenar o comprimento do corpo que está sendo aferido
double comprimObjeto;

//Variaveis para controle de tempo inicial, final e Δt
double intervaloInterrupcao;
unsigned int inicioInterrupcao;
unsigned int fimInterrupcao;

//Variavel para determinar se o estado atual é interrompido, ou seja, tem algum objeto interrompendo o photogate
bool isInterrupted;

void setup()
{
  pinMode(inputLuz, INPUT);
  
  //Para sincronizar: utilizar um 
  startTime = millis();
  
  isInterrupted = false;
  //Comprim Obj em m -> sequencia de calibragem
  comprimObjeto = 0.1;
  
  //output em console
  Serial.begin(9600);
}

void loop()
{  
  if(analogRead(inputLuz) < 200 && !isInterrupted){
  	inicioInterrupcao = millis();
    isInterrupted = true;
  }
  
  if(analogRead(inputLuz) > 200 && isInterrupted){
    fimInterrupcao = millis();

 	  //Velocidade media [Instantânea] = Δs/Δt | [m]/[s] 	
    intervaloInterrupcao = (fimInterrupcao - inicioInterrupcao);
    double velocidadeMedFim = comprimObjeto / (intervaloInterrupcao / 1000);
    
    Serial.println("==============================================================");
    Serial.println("Velocidade aferida: "
                   + String(velocidadeMedFim, 2) + "m/s" +
                  " | " + String(3.6 *velocidadeMedFim, 2) + "Km/h");

    Serial.println("\t\tTempo Final" + String(fimInterrupcao, 4) + " | Tempo Inicio" + String(inicioInterrupcao, 4));
    Serial.println("\t\tTempo Intervalo em ms" + String(intervaloInterrupcao, 4));
    Serial.print("==============================================================");
    
    isInterrupted = false;
    inicioInterrupcao = 0;
    fimInterrupcao = 0; 
    intervaloInterrupcao = 0;
  }
}