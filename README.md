# GPS Speedometer

Using our Arduino-compatible GPS module and TFT Screen, this project displays basic GPS information such as longitude, latitude, altitude, speed, and distance travelled from point A to B. Information are stored on an SD card which can be exported and viewed via Google Maps. Powered from a micro-USB for easy powering off your phone charger while you’re in the car.			

## Bill of Materials
| Qty | Product | Description |
| --- | ---- | --- |
|1 | [XC4430](https://jaycar.com.au/p/XC4430) | Duinotech Leonardo
|1 | [XC3712](https://jaycar.com.au/p/XC3712) | GPS Receiver module
|1 | [XC4630](https://jaycar.com.au/p/XC4630) | 2.8" TFT LCD screen.

#### Suggested Extras
* [WW4345](https://jaycar.com.au/p/WW4345) Wire wrap (use any colour)
  * Very thin wire that's perfect for small builds.
* [XC4983](https://jaycar.com.au/p/XC4983) 8GB SD card.
  * Use for logging GPS data.

## Required Software and Libraries
_Version numbers are at the time of this guide and might or might not work with later version._

| Library | Author |  Version |
| ---:|:--- | --- |
| NeoGps | john do| |

#### Notes

* **MCUFRIEND_kbv** needs to be modified to work with our TFT module.

* **SdFat** provides us with an interface to `FAT32` filesystems _as well as_ provides a **SoftwareSPI** bus. We use 'SoftSPI' due to the case that Leonardo does not have the SPI pins on 10~13 as traditional UNOs do. This library is perfect for our needs.


## Contribution

We welcome suggestions and improvements to the code, send in a pull request and we'll review it to merge into main branch.
