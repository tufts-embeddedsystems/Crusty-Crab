# README

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
