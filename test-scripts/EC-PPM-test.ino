/*
  ElCheapo Arduino EC-PPM measurments Calibration
 
  This Script is used for calibration of the sensor and fine tuning of the Cell Constant K
  Submerge the sensor and temperature probe in the calibration solution and leave for a while so the temperature probe can settle
  Change the value of the calibration solution to suit the solutiton strength
  Stir the probe to make sure the solution is well mixed and upload the code to the arduino
  Open the terminal for an update of the estimated Cell Constant K [should be around 3] and use this new value in the main EC code.
 
 
  28/8/2015  Michael Ratcliffe  Mike@MichaelRatcliffe.com
 
 
          This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see .
 
    Parts:
    -Arduino - Uno/Mega
    -Standard American two prong plug
    -1 kohm resistor
    -DS18B20 Waterproof Temperature Sensor
 
 
    See www.MichaelRatcliffe.com/Projects for a Pinout and user guide or consult the Zip you got this code from
 
*/
 
 
//************************** Libraries Needed To Compile The Script [See Read me In Download] ***************//
// Both below Library are custom ones [ SEE READ ME In Downloaded Zip If You Dont Know how To install Use them or add a pull up resistor to the temp probe
 
 
#include <OneWire.h>
#include <DallasTemperature.h>
 
 
 
 
 
 
//************************* User Defined Variables ********************************************************//
 
 
float CalibrationEC=1.38; //EC value of Calibration solution is s/cm
 
 
 
 
//##################################################################################
//-----------  Do not Replace R1 with a resistor lower than 300 ohms    ------------
//##################################################################################
 
 
int R1= 1000;
int Ra=25; //Resistance of powering Pins
int ECPin= A0;
int ECGround=A1;
int ECPower =A4;
 
 
//*************Compensating for temperature ************************************//
//The value below will change depending on what chemical solution we are measuring
//0.019 is generaly considered the standard for plant nutrients [google "Temperature compensation EC" for more info
float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring
 
 
 
 
//************ Temp Probe Related *********************************************//
#define ONE_WIRE_BUS 10          // Data wire For Temp Probe is plugged into pin 10 on the Arduino
const int TempProbePossitive =8;  //Temp Probe power connected to pin 9
const int TempProbeNegative=9;    //Temp Probe Negative connected to pin 8
 
 
 
 
//***************************** END Of Recomended User Inputs *****************************************************************//
 
 
OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.
 
 
float TemperatureFinish=0;
float TemperatureStart=0;
float EC=0;
int ppm =0;
 
 
float raw= 0;
float Vin= 5;
float Vdrop= 0;
float Rc= 0;
float K=0;
 
 
 
 
int i=0;
float buffer=0;
 
 
//*********************************Setup - runs Once and sets pins etc ******************************************************//
void setup()
{
  Serial.begin(9600);
  pinMode(TempProbeNegative , OUTPUT ); //seting ground pin as output for tmp probe
  digitalWrite(TempProbeNegative , LOW );//Seting it to ground so it can sink current
  pinMode(TempProbePossitive , OUTPUT );//ditto but for positive
  digitalWrite(TempProbePossitive , HIGH );
  pinMode(ECPin,INPUT);
  pinMode(ECPower,OUTPUT);//Setting pin for sourcing current
  pinMode(ECGround,OUTPUT);//setting pin for sinking current
  digitalWrite(ECGround,LOW);//We can leave the ground connected permanantly
 
  delay(100);// gives sensor time to settle
  sensors.begin();
  delay(100);
  //** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
  // Consule Read-Me for Why, or just accept it as true
  R1=(R1+Ra);
 
  Serial.println("ElCheapo Arduino EC-PPM measurments Calibration");
  Serial.println("By: Michael Ratcliffe  Mike@MichaelRatcliffe.com");
  Serial.println("Free software: you can redistribute it and/or modify it under GNU ");
  Serial.println("");
  Serial.println("Make sure Probe and Temp Sensor are in Solution and solution is well mixed");
  Serial.println("");
  Serial.println("Starting Calibration: Estimated Time 60 Seconds:");
 
 
 
};
//******************************************* End of Setup **********************************************************************//
 
 
 
 
//************************************* Main Loop - Runs Forever ***************************************************************//
//Moved Heavy Work To subroutines so you can call them from main loop without cluttering the main loop
void loop()
{
 
 
  i=1;
  buffer=0;
sensors.requestTemperatures();// Send the command to get temperatures
TemperatureStart=sensors.getTempCByIndex(0); //Stores Value in Variable
 
 
//************Estimates Resistance of Liquid ****************//
while(i<=10){
 
 
 
digitalWrite(ECPower,HIGH);
raw= analogRead(ECPin);
raw= analogRead(ECPin);// This is not a mistake, First reading will be low
digitalWrite(ECPower,LOW);
buffer=buffer+raw;
i++;
delay(5000);
};
raw=(buffer/10);
 
 
 
 
sensors.requestTemperatures();// Send the command to get temperatures
TemperatureFinish=sensors.getTempCByIndex(0); //Stores Value in Variable
 
 
//*************Compensating For Temperaure********************//
EC =CalibrationEC*(1+(TemperatureCoef*(TemperatureFinish-25.0))) ;
 
//***************** Calculates R relating to Calibration fluid **************************//
Vdrop= (((Vin)*(raw))/1024.0);
Rc=(Vdrop*R1)/(Vin-Vdrop);
Rc=Rc-Ra;
K= 1000/(Rc*EC);
 
 
 
 
Serial.print("Calibration Fluid EC: ");
Serial.print(CalibrationEC);
Serial.print(" S  ");  //add units here
Serial.print("Cell Constant K");
Serial.print(K);
 
 
if (TemperatureStart==TemperatureFinish){
  Serial.println("  Results are Trustworthy");
  Serial.println("  Safe To Use Above Cell Constant in Main EC code");
 
}
else{
  Serial.println("  Error -Wait For Temperature To settle");
 
}
 
 
}
//************************************** End Of Main Loop **********************************************************************//
