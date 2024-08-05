## This device will automaticly turn on or off your analog audio amplifier based on the audio output from your Chromecast or other audio player.


- ## Setup
1. Asamble the pcb 
2. Configure the settings:  
from line 25 to 30
```ino
const bool pwr_recover = false;// set true if amp remembers last state after power loss
const int treshold = 23;
const int time = 5;
const int btn = 4;
const int repeats = 4;
const int intputL = A1;
```
If your amp amp remembers last state after a power outage set pwr_recover to true.  
Set your cheomecast/media player to your normal lisening level and run the analogInSerialOut arduino example. Set treshold to the one of the lower values. Not recomended to set below 10.  
Time is after how manny seconds should it turn off the amp after an input below treshold is detected.  
"repeats" is the amount of consecutivr times it should send the pwoer command.  
"intputL" and "btn" are the inputs for the adc and learn button. Leave default if not using other pcb.  

3. compile and flash code.

4. connect the amp:  
It doesn' matter what jack is the input and pass trough, alternativley you can use your own splitter.  
Point the IR blaster to the front of the amp.  
Connect USB power cable while pressing the learn button.  
Press the powe button on your remote.  
Done  
<br />  
Todo:
- unspagerify void send()
- get rid of turno and turnoff
- stereo detection
- case
