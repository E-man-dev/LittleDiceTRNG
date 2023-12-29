# LittleDiceTRNG
Have a nerd friend who always complains about the dice being rigged? Don't trust the dice yourself? With the right hardware, you can solve all that! This uses an Adafruit ESP32-S2 Reverse TFT Feather, a 50 mm STEMMA QT / Qwiic JST SH 4-Pin Cable, and an Adafruit Infineon Trust M Breakout Board, as well as a little PLA and TPU for the case.

This uses the Infineon's Arduino library (https://github.com/Infineon/arduino-optiga-trust-m) for the TRNG and Espressif's Arduino library (https://github.com/espressif/arduino-esp32) for the ESP32-S2. It is all written in Arduino. I followed a lot of Adafruit's guide for getting the Feather all set up, and I advise you to do the same. You will need to follow the README for the trust-m gitub in order to set up the trng, but keep in mind the library is called OPTIGA Trust M, not arduino-optiga-trust-m.

From there, build the code, load it onto the code, and you are good to go! Please keep in mind that I made the case designs with an educational license copy of Fusion 360, so any restrictions imposed by the code I have built upon and the software I have used cannot be violated, even if my licensing is permissive.

If you are interested in building the case, I recommend using PLA for the top and bottom and TPU for the side. It looks nice with shiny orange PLA and black TPU, but filaments these days are good enough where what color you pick shouldn't affect print quality. The little hole in the corner is in case you want to put it on a keychain ring.
