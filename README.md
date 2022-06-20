# 28BYJ-48 steppers stuff
A recopilation of **28BYJ-48** stepper motor + **ULN2003** driver *experiments*, *checks*, *analysis*, *documentation*, *models* and *data*.

**WIP**

## CODE

### PORTx vs digitalWrite()

To be able to set the four coils simultaneously we can use the [**PORTx registers**](https://web.archive.org/web/20211130201930/https://www.arduino.cc/en/Reference/PortManipulation). Also they are a lot faster (2.6x times!) than the [**digitalWrite()**](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/) counterpart. It's true that the code is more difficult to maintain.

Take a look at the `2X-stepper-portx.ino` and `2X-stepper-digitalwrite.ino` Arduino programs for the comparison:

    Stepper motors (ULN2003 + 28BYJ-48) driving using PORTx registers
    Maximun setCoils() time: 0
    Mode: WAVE  Delay: 2250
    Maximun setCoils() time: 28

    Stepper motors (ULN2003 + 28BYJ-48) driving using digitalWrite()
    Maximun setCoils() time: 0
    Mode: WAVE  Delay: 2250
    Maximun setCoils() time: 72


### WAVE vs FULL vs HALF driving models

* **WAVE** to save energy
* **FULL** for the strongest torque
* **HALF** for the finest resolution


## 3D models

Go take a look at my [FreeCAD repository](https://github.com/mgesteiro/FreeCAD-models) for these models:

![28BYJ-48](https://github.com/mgesteiro/FreeCAD-models/blob/master/28BYJ-48/28BYJ-48.png)
![ULN2003](https://github.com/mgesteiro/FreeCAD-models/blob/master/ULN2003-driver-board/ULN2003-driver-board.png)


## Fritzing Parts

Go take a look at my [Fritzing repository](https://github.com/mgesteiro/fritzing-parts) for these parts:

![28BYJ-48](https://github.com/mgesteiro/fritzing-parts/blob/main/28BYJ-48-motor/28BYJ-48-motor.png)
![ULN2003](https://github.com/mgesteiro/fritzing-parts/blob/main/28BYJ-48-driver/28BYJ-48-driver.png)


## LICENSE

This work is licensed under the [GNU General Public License v3.0](LICENSE-GPLV30). All media and data files that are not source code are licensed under the [Creative Commons Attribution 4.0 BY-SA license](LICENSE-CCBYSA40).

More information about licenses in [Opensource licenses](https://opensource.org/licenses/) and [Creative Commons licenses](https://creativecommons.org/licenses/).