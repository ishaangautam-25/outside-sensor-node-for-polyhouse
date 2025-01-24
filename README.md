# Sensor Node for monitoring the outside environment of polyhouse

This project involves creating a **sensor node outside a polyhouse** to monitor and compare environmental parameters outside and inside the polyhouse for optimal climate control in controlled climate agriculture. The system ensures efficient data collection, transmission, and real-time environment regulation using multiple sensors and a NodeMCU microcontroller.  

## Table of Contents  

1. [Introduction](#introduction)  
2. [Working Principle](#working-principle)  
3. [Components Used](#components-used)  
4. [Description of Components](#description-of-components)  
5. [Sensor Integration and Implementation](#sensor-integration-and-implementation)  
6. [Environmental Control Features](#environmental-control-features)  
7. [Usage Instructions](#usage-instructions)  


---

## Introduction  

The sensor node collects environmental data outside the polyhouse using multiple sensors. It compares these with the internal polyhouse conditions to optimize irrigation, lighting, CO2 levels, and temperature inside the polyhouse. Real-time data is sent to a **ThingSpeak server** for visualization and remote access.  

---

## Working Principle  

1. **Data Collection**: Sensors measure parameters such as temperature, humidity, CO2, light intensity, and water flow.  
2. **Data Processing**: A NodeMCU microcontroller formats the data and prepares it for transmission.  
3. **Data Transmission**: Data is sent wirelessly to a central control system for analysis and logged to a ThingSpeak server.  
4. **Environment Regulation**: Automated systems use the collected data to make adjustments to maintain optimal growing conditions.  

---

## Components Used  

| S.No. | Component                  | Model      | Quantity |  
|-------|----------------------------|------------|----------|  
| 1     | Dallas temperature sensor  | DS18B20    | 2        |  
| 2     | Temperature & Humidity     | SHT20      | 1        |  
| 3     | CO2 Sensor                 | MH-Z19C    | 1        |  
| 4     | Ambient Light Sensor       | TSL2591    | 1        |  
| 5     | Water Flow Sensor          | YF-S201    | 2        |  
| 6     | I2C Analog Multiplexer     | PCA9548A   | 1        |  
| 7     | LEDs                       | -          | 2        |  

---

## Description of Components  

### 1. **Dallas Temperature Sensor (DS18B20)**  
- **Interface**: 1-Wire  
- **Voltage**: 3V - 5.5V  
- **Temperature Range**: -55°C to +125°C  
- **Accuracy**: ±0.5°C  

### 2. **Temperature and Humidity Sensor (SHT20)**  
- **Interface**: I2C  
- **Voltage**: 2.1V - 3.6V (Recommended: 3V)  
- **Temperature Accuracy**: ±0.3°C  
- **Humidity Accuracy**: ±3% RH  

### 3. **CO2 Sensor (MH-Z19C)**  
- **Interface**: UART  
- **Voltage**: 5V  
- **CO2 Detection Range**: 400~10000 ppm  

### 4. **Ambient Light Sensor (TSL2591)**  
- **Interface**: I2C  
- **Voltage**: 3V - 5V  
- **LUX Detection Range**: 0.1 to 40,000 lux  

### 5. **Water Flow Sensor (YF-S201)**  
- **Voltage**: 3V - 18V  
- **Flow Rate**: 1 to 30 Liters/Minute  

### 6. **I2C Multiplexer (PCA9548A)**  
The 8-channel multiplexer connects multiple I2C sensors to the NodeMCU using a single SDA and SCL pin pair.  

### 7. **LEDs**  
- **Power LED**: Indicates system power.  
- **Data Logging LED**: Blinks when data is sent to ThingSpeak.  

---

## Sensor Integration and Implementation  

- **Microcontroller**: NodeMCU with inbuilt Wi-Fi capability.  
- **Communication**: Sensors communicate via I2C or UART interfaces. Data is logged to ThingSpeak for remote monitoring.  
- **I2C Multiplexer**: Used to handle multiple I2C sensors with a single NodeMCU.  

---

## Environmental Control Features  

1. **Temperature Regulation**: Activates foggers if external temperatures rise.  
2. **Lighting Adjustment**: Increases artificial lighting inside the polyhouse during low light conditions outside.  
3. **Water Management**: Ensures optimal irrigation by monitoring water temperature and flow rate.  
4. **CO2 Regulation**: Maintains CO2 levels for effective photosynthesis.  
5. **Energy Conservation**: Optimizes system efficiency to reduce energy consumption.  

---

## Usage Instructions  

1. **Connect the Sensors**: Wire the sensors to the NodeMCU as per their respective pin configurations.  
2. **Upload the Code**: Use the Arduino IDE to upload the code to the NodeMCU.  
3. **Power the System**: Supply power via USB or a connected power source.  
4. **Access Data**: View real-time data on ThingSpeak or through the NodeMCU’s serial monitor.  
5. **Analyze and Control**: Use collected data to regulate the polyhouse environment.  


