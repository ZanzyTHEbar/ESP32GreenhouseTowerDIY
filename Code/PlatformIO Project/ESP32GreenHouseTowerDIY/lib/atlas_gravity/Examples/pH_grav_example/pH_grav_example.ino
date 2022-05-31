// to use the Atlas gravity circuits with 
// the gravity isolator board's pulse output 
// uncomment line 8: #define USE_PULSE_OUT
// you can use any pins instead of just the analog ones
// but it must be recalibrated
// note that the isolator's analog output also provides isolation

// #define USE_PULSE_OUT

#ifdef USE_PULSE_OUT
  #include "ph_iso_grav.h"       
  Gravity_pH_Isolated pH = Gravity_pH_Isolated(A0);         
#else
  #include "ph_grav.h"             
  Gravity_pH pH = Gravity_pH(A0);   
#endif
                

uint8_t user_bytes_received = 0;                
const uint8_t bufferlen = 32;                   
char user_data[bufferlen];                     

void parse_cmd(char* string) {                   
  strupr(string);                                
  if (strcmp(string, "CAL,7") == 0) {       
    pH.cal_mid();                                
    Serial.println("MID CALIBRATED");
  }
  else if (strcmp(string, "CAL,4") == 0) {            
    pH.cal_low();                                
    Serial.println("LOW CALIBRATED");
  }
  else if (strcmp(string, "CAL,10") == 0) {      
    pH.cal_high();                               
    Serial.println("HIGH CALIBRATED");
  }
  else if (strcmp(string, "CAL,CLEAR") == 0) { 
    pH.cal_clear();                              
    Serial.println("CALIBRATION CLEARED");
  }
}

void setup() {
  Serial.begin(9600);                            
  delay(200);
  Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin()) {                                     
    Serial.println("Loaded EEPROM");
  }
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
  
  Serial.println(pH.read_ph());                                                      
  delay(1000);
}
