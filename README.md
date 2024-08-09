# ESP32 Environmental Sensors
This is the accompanying repository for all source code and additional material for the article series "Environmental Sensor usage with ESP32", published on medium.com.

### NOTE: this is a repository that will be updated very frequently in the next weeks, so please be patient if a specific sensor sketch is not available for now !

Parts of the tutorial:
- Overview: https://medium.com/@androidcrypto/environmental-sensor-usage-with-esp32-overview-d436343261c3
- Base Application: coming soon
## Temperature, Humidity or Air Pressure Sensors
- **AHT10** Sensor (Temperature and Humidity): https://medium.com/@androidcrypto/esp32-environmental-sensor-aht10-temperature-humidity-23b27a4b3cf8
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
- **AGS10** Sensor (Total volatile organic compounds "TVOC" for Air Quality): coming soon
- **BME680** Sensor (Indoor Air Quality "IAQ"): coming soon
 
**Bonus - using ESP-NOW** to retrieve the data from remote places: coming soon

For each tutorial you will find a subfolder with all material needed to follow the tutorial.

## Additional tutorials

ESP32 I2C-Bus Scanner: https://medium.com/@androidcrypto/esp32-i2c-bus-scanner-1ac88f6ffd86 (source code: https://github.com/AndroidCrypto/ESP32_I2C_Bus_Scanner)

ESP32 Add a long term data storage and visualization to your sketch (using ThingSpeak): https://medium.com/@androidcrypto/esp32-add-a-long-term-data-storage-and-visualization-to-your-sketch-using-thingspeak-49bf54f78360

ESP32 Environmental Sensor BME280 measures the Altitude: https://medium.com/@androidcrypto/esp32-environmental-sensor-bme280-measures-the-altitude-5a847073376a (source code: https://github.com/AndroidCrypto/ESP32_Environmental_Sensors/tree/main/Esp32_EnvironmentSensor_BME280_Altitude)

## Datasheets

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
| AGS10 | Total volatile organic compounds "TVOC" | I2C | 0x1A (changable) | 2.70 € |
| BME680 | Indoor Air Quality ("IAQ") | I2C | 0x77 (alternative 0x76) | 6.70 € |


