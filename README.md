# AES3/EBU tech 3250 Digital Audio Output on ESP32
This library will utilize the SPI output of the ES32[^1] chip to form a digital audio interface according to EBU tech 3250[^2]. The code has been testet on the
LilyGO TTGO-T display[^3] and the ESP32 D1 R32 UNO from Wemos.

## Development Environment
The code has been developed in Microsoft Visual Studio VS2019 with the Visual Micro[^4] extension but it is fully compatible with the Arduino IDE[^5].
Installation of arduino IDE and the TTGO-T can be done by following the procedure described at Joen's code snippets pages[^3] and the development board can be bought from various sites on the internet.
Visual Studio can be downloaded directly from Microsoft's homepage[^VS];

## Library Installation Guide

## Pin Configuration

## Examples
In the example folder

## Testing

The sinetone generator has been tested with a FFT Analyzer[^FFT] on the DK-Technohogies[^DK] DKT7.

## ToDo List
* Channel Status
* 
## Contribution

Please contact me if you like to contribute[^Contact].


[^1]: API reference ESP32 [ESPRESSIF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/index.html).
[^2]: EBU Standard [Tech 3250](https://tech.ebu.ch/docs/tech/tech3250.pdf).
[^3]: Application example on Joen's code snippets [TTGo-T](https://sites.google.com/site/jmaathuis/arduino/lilygo-ttgo-t-display-esp32).
[^4]: Visual Micro (https://www.visualmicro.com/).
[^5]: Arduino IDE (https://www.arduino.cc/en/software).
[^VS]: Visual Studio (https://visualstudio.microsoft.com/downloads).
[^FFT]:  Fast Fourier transform (https://en.wikipedia.org/wiki/Fast_Fourier_transform).
[^DK]: DK-Technologies (https://www.hansens.dk/);
[^Contact]: Contact: Karsten Hervoe Hansen khh@hansens.dk

