
![License](https://img.shields.io/github/license/amrathesh/GPS-Tracker)

## GPS Tracker
GPS Tracker using Neo 6M GPS sensor and GSM module.

This repo documents code for Arduino implementation of GPS tracking and provides example PHP script for server to receive location data sent by Arduino client.
## Methodology
* Wait for GPS module to get a fix (i.e., get the location).
* Sample GPS data via UART connection with Neo 6M GPS sensor.
* Use AT commands to talk to GSM module via UART to setup a GPRS connection. (I believe there is a dedicated library to do this but was curious about AT commands, hence used this approach)
* Pack the data to a JSON object.
* Send the data to server.
* PHP script captures the data in server database, which can be used for various applications.

## Contributing
Pull requests are welcome.
