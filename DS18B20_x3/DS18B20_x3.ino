// Librerías necesarias
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// Pines de conexión para los sensores
#define ONE_WIRE_BUS_1 10
#define ONE_WIRE_BUS_2 11
#define ONE_WIRE_BUS_3 12

// Configuración para comunicarse con cualquier otro dispositivo OneWire
OneWire oneWire1(ONE_WIRE_BUS_1);
OneWire oneWire2(ONE_WIRE_BUS_2);
OneWire oneWire3(ONE_WIRE_BUS_3);

// Pasa nuestras referencias de oneWire a los sensores Dallas.
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);
  
// Arrays para almacenar las direcciones de los dispositivos
DeviceAddress insideThermometer;
DeviceAddress sensor2Address;
DeviceAddress sensor3Address;
/*
 * Funciones básicas del programa
 */

// Imprimir la dirección de los dispositivos
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

//#################################################################################
// Función para imprimir temp1
//#################################################################################
void printTemperature(DeviceAddress deviceAddress)
{
  // Temperatura en unidad Fahrenheit
  float tempF = sensors1.getTempF(deviceAddress);
  if (tempF == DEVICE_DISCONNECTED_F)
  {
    Serial.println("Error: No se puede leer la temperatura");
    Serial.println("");
    return;
  }

  // Imprimir la temperatura en grados Fahrenheit
  Serial.print(tempF);

  // Enviar la temperatura al servidor a través del puerto serie
  sendTemperature(tempF);
}

//#################################################################################
// Función para imprimir temp2
//#################################################################################
void printTemperature2(DeviceAddress deviceAddress)
{
  // Temperatura en unidad Fahrenheit
  float tempF = sensors2.getTempF(deviceAddress);
  if (tempF == DEVICE_DISCONNECTED_F)
  {
    Serial.println("Error: No se puede leer la temperatura");
    Serial.println("");
    return;
  }

  // Imprimir la temperatura en grados Fahrenheit
  Serial.print(tempF);

  // Enviar la temperatura al servidor a través del puerto serie
  sendTemperature(tempF);
}

//#################################################################################
// Función para imprimir temp3
//#################################################################################
void printTemperature3(DeviceAddress deviceAddress)
{
  // Temperatura en unidad Fahrenheit
  float tempF = sensors3.getTempF(deviceAddress);
  if (tempF == DEVICE_DISCONNECTED_F)
  {
    Serial.println("Error: No se puede leer la temperatura");
    Serial.println("");
    return;
  }

  // Imprimir la temperatura en grados Fahrenheit
  Serial.print(tempF);

  // Enviar la temperatura al servidor a través del puerto serie
  sendTemperature(tempF);
}

//#################################################################################
// Función para enviar la temperatura al servidor a través del puerto serie
//#################################################################################
void sendTemperature(String sensorName, float temperature)
{
  Serial.print(sensorName);
  Serial.print(":");
  Serial.println(temperature);
}

//#################################################################################
// Función para imprimir todas las temperaturas
//#################################################################################
void printAllTemperatures()
{
  sensors1.requestTemperatures();
  float temp1 = sensors1.getTempF(insideThermometer);
  sendTemperature("temp1", temp1);

  sensors2.requestTemperatures();
  float temp2 = sensors2.getTempF(sensor2Address);
  sendTemperature("temp2", temp2);

  sensors3.requestTemperatures();
  float temp3 = sensors3.getTempF(sensor3Address);
  sendTemperature("temp3", temp3);
}

// Función para enviar la temperatura al servidor a través del puerto serie
void sendTemperature(float temperature)
{
  Serial.print(" ----------> ");
  Serial.print(temperature);
  Serial.println("");
}

/*
 * Función principal. Pregunta las temperaturas a los sensores
 */
void setup()
{
  // Configuración del monitor serie
  Serial.begin(9600);

  sensors1.begin();
  sensors2.begin();
  sensors3.begin();

  if (!sensors1.getAddress(insideThermometer, 0))
  {
    Serial.println("No se encontró la dirección del Sensor 1");
  }

  if (!sensors2.getAddress(sensor2Address, 0))
  {
    Serial.println("No se encontró la dirección del Sensor 2");
  }

  if (!sensors3.getAddress(sensor3Address, 0))
  {
    Serial.println("No se encontró la dirección del Sensor 3");
  }

  // Imprimir las direcciones de los dispositivos
  Serial.println("");
  Serial.print("Dirección dispositivo 1: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.print("Dirección dispositivo 2: ");
  printAddress(sensor2Address);
  Serial.println();

  Serial.print("Dirección dispositivo 3: ");
  printAddress(sensor3Address);
  Serial.println();


  // Establecer la resolución de los sensores a 9 bits
  sensors1.setResolution(insideThermometer, 9);
  sensors2.setResolution(sensor2Address, 9);
  sensors3.setResolution(sensor3Address, 9);
}

void loop()
{
  // Enviar una petición a los sensores para que nos devuelvan la temperatura
  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  sensors3.requestTemperatures();

  // Imprimir todas las temperaturas
  printAllTemperatures();

  delay(2000);
}
