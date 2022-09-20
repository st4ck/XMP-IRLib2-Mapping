# XMP-IRLib2-Mapping - Arduino
Using Arduino and IRLib2 library I decoded XMP protocol of remote Xfinity XR15v2-UQ and remapped each command using NECx protocol of Samsung TVs (in this case using AA59-00741A codes).

The goal of this project is to be able to use Xfinity remotes with other devices as universal remote. Xfinity remotes can be reprogrammed for each TV but only few buttons will work (Power,Volume+/-,Mute,TV Input).

In practice, I use Arduino as a Remote controller gateway able to translate one IR Code (XMP) to another (NECx). Feel free to improve the code, add other protocols or just test it with your controller.

For the key "Home" I had on my source Samsung 2.4GHz remote controller (RMCSPR1AP1), I've not found the right NECx code online, so I've implemented as a timed combination of MENU+EXIT that gives the same exact result.

The other keystrokes are mapped 1-to-1.

A debug version can be activated to print the Xfinity sequence to implement new keys/update them or simply as debugging purpose.




For testing I used as IR Receiver a VS1838B and as IR Transmitter a common 940nm IR Led. The Arduino board I used is a Arduino Duemilanove (I had it in a drawer).

The IR Receiver LED is attached to PIN2 (with a resistor on power pin 100ohm) and the IR transmitter led  is attached to PIN3 (with a resistor of 100ohm).

The transmitter LED is not so powerful (being only one and with that resistor) but it's not a problem in my case having placed the Arduino in a box below my Samsung TV. 

If you want to increase the power consider to have a powerful led or put them in series.
