# a proper algorithm would be as follows

0. set boolean humLow & resDry flag to false;

1. read humidity sensor to sensor1Value;
2. check if sensor1Value is low:
   1. yes, set humLow to true
   2. no, set humLow to false
3. read water level sensor into resval
4. check if  resval is dry
   1. yes, set resDry to true
   2. no, set resDry to false
5. check if humLow or resDry are true:
   1. yes, turn on the pump.
6. check if humLow and resDry are false:
   1. yes, turn off the pump.
