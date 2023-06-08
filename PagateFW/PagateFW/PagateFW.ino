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

//Device Input Variable -> As of now a photoresistor with 1kΩ potential divider
int inputLuz = A3;

//Object to measure length
double objLength;


//Lap Measuring Variables
 int lapCounter = 1;
 double lapStart;

//Variables for Initial and final time control and Δt
 double timeInterval;
 unsigned int passthroughStart;
 double passthroughEnd;

//State Variables to detect interruption on light sensor and first start
bool isInterrupted = false;
bool raceStart = false;

void setup()
{
  pinMode(inputLuz, INPUT);

  objLength = 0.1;
  
  Serial.begin(9600);
}

void outputLapStatsAndSpeed(double avgSpeed, double lapTime, unsigned int lapCounter){
  //Output lap data -> lapNumber|lapTime|InstantSpeed
  Serial.print(String(lapCounter)+",");
  Serial.print(String(lapTime, 3)+",");
  Serial.println(String(3.6 * avgSpeed, 2));
}

double calculateInstantSpeed(double objLength, double passthroughStart, double passthroughEnd){
  //Avarage Speed [Instant] = Δs/Δt | [m]/[s] 	
  double timeInterval = (passthroughEnd - passthroughStart);
  return objLength / (timeInterval / 1000);
}


void loop()
{  
  if(analogRead(inputLuz) < 200 && !isInterrupted){
  	passthroughStart = millis();
    isInterrupted = true;
  }
  
  if(analogRead(inputLuz) > 200 && isInterrupted){
    if(!raceStart){
      lapStart = millis();
      raceStart = true;
    }

    passthroughEnd = millis();
 	  
    double avgSpeed = calculateInstantSpeed(objLength, passthroughStart, passthroughEnd);
    double lapTime = (passthroughEnd - lapStart)/1000;

    outputLapStatsAndSpeed(avgSpeed, lapTime, lapCounter);

    lapCounter++;
    lapStart = passthroughEnd;
    
    isInterrupted = false;
    passthroughStart = 0;
    passthroughEnd = 0; 
    timeInterval = 0;
  }
}