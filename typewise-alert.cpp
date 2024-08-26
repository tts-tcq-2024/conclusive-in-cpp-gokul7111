#include "typewise-alert.h"
#include <stdio.h>

const TemperatureLimits SetTemperatureLimits[] = {
{0, 35},  //limits for passive cooling
{0, 45},  // limits for High active cooling
{0, 40}   // limits for Med active cooling
};

const email SetEmailMsg[] = {
  {" Temperature is too low", "The temperature is too low "},
  {" Temperature is too high", "The temperature is too high "},
  {" Temerature is normal", " "}
};

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
    TemperatureLimits localTemperatureLimit = SetTemperatureLimits[coolingType];
  return inferBreach(temperatureInC, localTemperatureLimit.lowerlimit, localTemperatureLimit.upperlimit);
}


void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) 
{
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  switch(alertTarget) 
  {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  printf("To: %s\n", recepient);
  printf(" %s \n", SetEmailMsg[breachType].mailbody);
}