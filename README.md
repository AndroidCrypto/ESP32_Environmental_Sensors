# ESP32 Environmental Sensors
This is the accompanying repository for all source code and additional material for the article series "Environmental Sensor usage with ESP32", published on medium.com.

## NOTE: this is just a stub repository with no content. It will be filled in the next weeks, so please be patient !

Parts of the tutorial:
- Overview:
- Base Application: coming soon
- AHT10 Sensor(Temperature): coming soon
- DHT11 Sensor (Temperature and Humidity): coming soon
- DHT22 Sensor (Temperature and Humidity): coming soon

- BME280 Sensor (Temperature, Humidity and Air Pressure): coming soon
- BMP280 Sensor (Temperature and Air Pressure): coming soon
- BME680 Sensor (Temperature, Humidity, Air Pressure and Air Quality Data): coming soon
- DS18B20 Sensor (Temperatur, Waterproof): coming soon
- AGS02MA Sensor (Total volatile organic compounds "TVOC" for Air Quality): coming soon

- Bonus - using ESP-NOW to retrieve the data from remote places: coming soon

For each tutorial you will find a subfolder with all material needed to follow the tutorial.

All available data sheets of the sensors are available in the subfolder "datasheets".

| Sensor Name | Values retrieved | Interface | Address |
|-------------|:-----------------|:----------|---------|
| AHT10 | Temperature, Humidity | I2C | 0x38 (alternative 0x39) |
| DHT11 | Temperature, Humidity | GPIO | P 18 |
| DHT22 | Temperature, Humidity | GPIO | P 18 |
| | Specialised Sensors | |
| AGS02MA | Total volatile organic compounds "TVOC" | I2C | 0x1A (changable) |

