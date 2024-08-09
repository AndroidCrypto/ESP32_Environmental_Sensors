# ESP32 Environmental Sensors
This is the accompanying repository for all source code and additional material for the article series "Environmental Sensor usage with ESP32", published on medium.com.

### NOTE: this is a repository that will be updated very frequently in the next weeks, so please be patient if a specific sensor sketch is not available for now !

Parts of the tutorial:
- Overview: coming soon
- Base Application: coming soon
## Temperature, Humidity or Air Pressure Sensors
- **AHT10** Sensor (Temperature and Humidity): coming soon
- **AHT20 + BMP280** Sensor module (Temperature, Humidity and Air Pressure): coming soon
- **DHT11** Sensor (Temperature and Humidity): coming soon
- **DHT22** Sensor (Temperature and Humidity): coming soon
- **BME280** Sensor (Temperature, Humidity and Air Pressure): coming soon
- **BME280** Sensor Bonus Calculate the Altitude of the sensor: coming soon
- **BMP280** Sensor (Temperature and Air Pressure): coming soon
- **BME680** Sensor (Temperature, Humidity, Air Pressure and Air Quality Data): coming soon
- **DS18B20** Sensor (Temperatur in Waterproof cover): coming soon
## Air Quality Sensors
- **AGS02MA** Sensor (Total volatile organic compounds "TVOC" for Air Quality): coming soon

- Bonus - using ESP-NOW to retrieve the data from remote places: coming soon

For each tutorial you will find a subfolder with all material needed to follow the tutorial.

All available data sheets of the sensors are available in the subfolder "datasheets".

| Sensor Name | Values retrieved | Interface | Address | Pricing |
|-------------|:-----------------|:----------|---------|---------|
| | **Basic Sensors** | | | | EUR |
| AHT10 | Temperature, Humidity | I2C | 0x38 (alternative 0x39) | 1.40 € |
| AHT20 + BMP280 | Temperature, Humidity, Air Pressure | I2C | AHT20: 0x38 , BMP280: 0x77 | 1.40 € |
| BME280 | Temperature, Humidity, Air Pressure | I2C | 0x77 (alternative 0x76) | 3.40 € | 
| BMP280 | Temperature, Air Pressure | I2C | 0x76 (alternative 0x77) | 1.70 € | 
| DHT11 | Temperature, Humidity | GPIO | P 18 | 1.20 € |
| DHT22 | Temperature, Humidity | GPIO | P 18 | 2.10 € |
| DS18B20 | Temperature (waterproof) | GPIO | P 18 | 2.00 € (Set) |
| | **Specialised Sensors** | |
| AGS02MA | Total volatile organic compounds "TVOC" | I2C | 0x1A (changable) | 2.00 € |

