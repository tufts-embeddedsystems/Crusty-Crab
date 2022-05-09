# Crusty Crab

Contributors: Luan Banh, Taswar Mahbub, Char McFarland, Jessica Nordlund, Huyen Pham (and Ben McDermott)

## Goal
```
Design, build, and program 10 temperature sensor nodes to measure temperature once an hour. 
Nodes should last 6 months in the field. 
```
Note: due to software limitation, node will wake up every half and hour but will only measure temperature and report it once an hour.

## Parts
```
Microcontroller : ESP32-C3
Sensor          : G-NIMO-005
Thermistor      : TDK B57350V2104F460
```

## Roles
```
Communication/Manager : Luan Banh
Electrical Engineers  : Huyen Pham, Jessica Nordlund, and Taswar Mahbub
Logistics             : Huyen Pham and Jessica Nordlund
Software Engineers    : Luan Banh and Char McFarland
Enclosure Design      : Char McFarland (and Ben McDermott)
Documentation         : Jessica Nordlund
```

## Basic Program Flow
```
.----> Deep Sleep <----------------.
|          |                       |
|        Wakeup                    |
|          |                       |
|     Boot count++                 |
|          |                       |
|    Set wakeup timer              |
|          |                       | 
|    Check boot count              |
|          |                       |  
|  (Is count 1 or 3?) ---- <No> ---|           
|          |                       |
|        <Yes>                     |
|          |                       |
|    Boot count = 2                |
|          |                       |
|    Setup ADC chars               |
|          |                       |
|    Take readings                 |
|          |                       |                      
|      Retry = 0                   |
|          |                       |
|     (Is retry 3?) ---- <Yes> ----'
|          |      ^----------------.
|         <No>                     |
|          |                       |
|    Connect to Wifi            Retry++
|          |                       | 
|     (Connected?) ----- <No> -----'
|          |
|        <Yes>    
|          |
|   Send MQTT message
|          |
|__________|          

```


### Crusty_Crab_Temperature_Sensor_Node
```
This folder contains the implementation for the temperature sensor node.
Download the folder and open it using PlatformIO.
```

### main.c
```
Run the system based on flow chart
Change the topic name of the MQTT to the correct <node_id>
```

### main_debug.c
```
Use to debug a node to see which pin is not connected, change offset, etc.
```

### sensors_config.h
```
Configuration file for the temperature sensor and thermistor
Change the SENSOR_OFFSET and THERMISTOR_OFFSET to calibrate
Battery voltage dies at 1.4V for AA battery
```

### main_config.h
```
Configuration file for the node
Change the MQTT server name and wireless network id and pwd as needed
```
