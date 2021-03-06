#include <SensorManager.h>
#include <SensorData.h>

String DETECTED_STR = "Threat Detected: ";
String AVERAGE_STR = "Average: ";
String THREAT_VAL_STR = "Threat Val: ";
String CALIBRATING_STR = "Calibrating: ";
String DATA_STR = "Data: ";
String STD_DEV_STR = "Std Dev: ";
String START_STR = "Starting: ";
String DONE_SETUP_STR = "Done Calibrating";

/**
 * Written by Gideon Pfeffer
 */

/*
 * ms delay between reading the sensors when calibration
 * should be 1000 (1 second) or higher in my opinion
 */
#define CALIBRATION_DELAY 100

/*
 * ms delay between reading sensors in loop
 */
#define LOOP_DELAY 1000

/*
 * defines the number of loops a sensor will sit at
 * "threated" before being reset and recalibrated
 */
#define LOOPS_BETWEEN_RESETS 5

/**
 * Defines how many multiples of the standard deviation
 * are needed for a sensor value to be considered a threat
 * 
 * 2.0 = 95% of normal distribution
 */
#define THRESHOLD 2.0

//number of sensors in use
#define NUM_SENSORS 4

/**
 * amount of data point a sensor data will hold at one time. 
 * Should be maxed out in terms of space for string
 */
#define SENSOR_CAPACITY 25

/**
 * The class responsible for holding all of the data
 */
SensorManager<double>* manager = new SensorManager<double>
  (LOOPS_BETWEEN_RESETS, THRESHOLD, NUM_SENSORS, SENSOR_CAPACITY);

/**
 * Defines a function call for doubles
 */
typedef double (* dFunctionCall)();

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  Serial.println(START_STR);
  Serial.println();
  calibrateSensors();
  Serial.println(DONE_SETUP_STR);
  Serial.println();
  //Any other initializations you need here...
  /**
   * Setting pins ot output for example
   */
}


void loop() {
  recalibrateWhereNecessary();
  updateValues();
  logThreats();
  incrementThreatCounters();
  delay(LOOP_DELAY);
  /**
   * PROCESS NRF COMMANDS HERE IF APPLICABLE
   */
}

//pushes new values to the data
void updateValues(){
  manager -> getSensor(0) -> updateData(readSensor0());
  manager -> getSensor(1) -> updateData(readSensor1());
  manager -> getSensor(2) -> updateData(readSensor2());
  manager -> getSensor(3) -> updateData(readSensor3());
}

//logs threats that have not yet been logged
void logThreats(){
  if(manager -> getSensor(0) -> shouldBeLogged()){
    logSensor0();
    manager -> getSensor(0) -> setLogged();
  }
  if(manager -> getSensor(1) -> shouldBeLogged()){
    logSensor1();
    manager -> getSensor(1) -> setLogged();
  }
  if(manager -> getSensor(2) -> shouldBeLogged()){
    logSensor2();
    manager -> getSensor(2) -> setLogged();
  }
  if(manager -> getSensor(3) -> shouldBeLogged()){
    logSensor3();
    manager -> getSensor(3) -> setLogged();
  }
}

void incrementThreatCounters(){
  manager -> incrementThreats();
}

void recalibrateWhereNecessary(){
  if(!(manager -> getSensor(0) ->isCalibrated())){
    calibrateSensor0();
  }
  if(!(manager -> getSensor(1) ->isCalibrated())){
    calibrateSensor1();
  }
  if(!(manager -> getSensor(2) ->isCalibrated())){
    calibrateSensor2();
  }
  if(!(manager -> getSensor(3) ->isCalibrated())){
    calibrateSensor3();
  }
}

void calibrateSensors(){
  calibrateSensor0();
  calibrateSensor1();
  calibrateSensor2();
  calibrateSensor3();
}

/**
 * Will calibrate sensor 0
 */
void calibrateSensor0()
{
  calibrateSensor("Sensor 0", manager -> getSensor(0), (dFunctionCall)readSensor0);
}

/**
 * Will calibrate sensor 1
 */
void calibrateSensor1()
{
  calibrateSensor("Sensor 1", manager -> getSensor(1), (dFunctionCall)readSensor1);
}


/**
 * Will calibrate sensor 2
 */
void calibrateSensor2()
{
  calibrateSensor("Sensor 2", manager -> getSensor(2), (dFunctionCall)readSensor2);
}


/**
 * Will calibrate sensor 3
 */
void calibrateSensor3()
{
  calibrateSensor("Sensor 3", manager -> getSensor(3), (dFunctionCall)readSensor3);
}

void calibrateSensor(String sensorName, SensorData<double>* sensor, dFunctionCall readFunct){
  Serial.print(CALIBRATING_STR);
  Serial.println(sensorName);
  Serial.println();
  while(!(sensor -> isCalibrated()))
  { 
    sensor -> updateData(readFunct());
    delay(CALIBRATION_DELAY);
  }
}


//PLACE WHERE YOU SHOULD START IMPLEMENTING


/**
 * THIS IS WHERE YOU IMPLEMENT THE CODE FOR EACH SENSOR
 */
double readSensor0(){
  //for example, return the acceleromator value
  return random(1, 25);
}

double readSensor1(){
  return random(1, 10);
}

double readSensor2(){
  return random(1, 50);
}

double readSensor3(){
  return random(1, 15);
}



//IMPLEMENT CODE HERE TO LOG THE SENSOR

/*
 * Calls:
 * 
 * Average: manager -> getSensor(0) -> getAverage();
 * StdDev: manager -> getSensor(0) -> getStdDev();
 * Data as String: manager -> getSensor(0) -> dataToString();
 * Value That Caused Threat: manager -> getSensor(0) -> getThreatCausingVal();
 */
   
void logSensor0(){
  logSensor(manager -> getSensor(0), "Sensor 0");
}

void logSensor1(){
  logSensor(manager -> getSensor(1), "Sensor 1");
}

void logSensor2(){
  logSensor(manager -> getSensor(2), "Sensor 2");
}

void logSensor3(){
  logSensor(manager -> getSensor(3), "Sensor 3");
}

void logSensor(SensorData<double>* sensor, String name){
  double average = sensor -> getAverage();
  double stdDev = sensor -> getStdDev();
  String dataStr = sensor -> dataToString();
  double threatVal = sensor -> getThreatCausingVal();

  /**
   * LOG SOMETHING HERE
   */
   Serial.print(DETECTED_STR);
   Serial.println(name);
   Serial.print(AVERAGE_STR);
   Serial.println(average);
   Serial.print(STD_DEV_STR);
   Serial.println(stdDev);
   Serial.print(THREAT_VAL_STR);
   Serial.println(threatVal);
   Serial.print(DATA_STR);
   Serial.println(dataStr);
   Serial.println();
}

