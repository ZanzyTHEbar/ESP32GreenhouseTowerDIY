#include "ph_grav.h"       
#include "orp_grav.h"
#include "do_grav.h"

Gravity_pH pH = Gravity_pH(A0);
Gravity_ORP ORP = Gravity_ORP(A1);
Gravity_DO DO = Gravity_DO(A2);

uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];


void parse_cmd(char* string) {
  strupr(string);
  String cmd = String(string);
  if(cmd.startsWith("ORP,CAL")){
    int index = cmd.indexOf(',');
    if(index != -1){
      String param = cmd.substring(index+1, cmd.length());
      int index = param.indexOf(',');
      if(index != -1){
        String param2 = param.substring(index+1, param.length());
        if(param2.equals("CLEAR")){
          ORP.cal_clear();
          Serial.println("CALIBRATION CLEARED");
        }
        else{
          int cal_param = param2.toInt();
          ORP.cal(cal_param);
          Serial.println("ORP CALIBRATED");
        }
      }
    }
  }
  else if (strcmp(string, "PH,CAL,7") == 0) {       
    pH.cal_mid();                                
    Serial.println("MID CALIBRATED");
  }
  else if (strcmp(string, "PH,CAL,4") == 0) {            
    pH.cal_low();                                
    Serial.println("LOW CALIBRATED");
  }
  else if (strcmp(string, "PH,CAL,10") == 0) {      
    pH.cal_high();                               
    Serial.println("HIGH CALIBRATED");
  }
  else if (strcmp(string, "PH,CAL,CLEAR") == 0) { 
    pH.cal_clear();                              
    Serial.println("CALIBRATION CLEARED");
  }
  else if(cmd.startsWith("DO,CAL,CLEAR")){
    DO.cal_clear();
    Serial.println("CALIBRATION CLEARED");
  }
  else if(cmd.startsWith("DO,CAL")){
    DO.cal();
    Serial.println("DO CALIBRATED");
  
  }
}

void setup() {
  Serial.begin(9600);
  delay(200);
  Serial.println(F(" - For pH, use commands \"PH,CAL,7\", \"PH,CAL,4\", and \"PH,CAL,10\" to calibrate the circuit to those respective values"));
  Serial.println(F("   Use command \"PH,CAL,CLEAR\" to clear the calibration"));
  Serial.println();
  Serial.println(F(" - For ORP, use command \"ORP,CAL,xxx\" to calibrate the circuit to the value xxx \n   \"ORP,CAL,CLEAR\" clears the calibration"));
  Serial.println();
  Serial.println(F(" - For DO, use command \"CAL\" to calibrate the circuit to 100% saturation in air\n   \"DO,CAL,CLEAR\" clears the calibration"));
  Serial.println();
  
  if (pH.begin()) {                                     
    Serial.println("Loaded pH EEPROM");
  }
  if(ORP.begin()){
    Serial.println("Loaded ORP EEPROM");
  }
  if(DO.begin()){
    Serial.println("Loaded DO EEPROM");
  }
  Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
  }

  if (user_bytes_received) {
    parse_cmd(user_data);
    user_bytes_received = 0;
    memset(user_data, 0, sizeof(user_data));
  }

  Serial.print("pH: ");
  Serial.print(pH.read_ph());
  Serial.print(", ORP: ");
  Serial.print((int)ORP.read_orp());
  Serial.print(", DO: ");
  Serial.println(DO.read_do_percentage());
  delay(1000);
}
