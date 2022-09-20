/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <IRLibAll.h>
#include <avr/pgmspace.h>

IRrecvPCI myReceiver(2); //create receiver and pass pin number
IRdecode myDecoder;   //create decoder
IRsendNECx mySender;

#define DEBUG false

#define keys 25
bool keyfound[keys];
const unsigned int keymap[][36] PROGMEM = {
  {194,914,194,1734,194,774,194,2830,194,1322,194,1326,194,1182,198,2690,194,13018,194,910,194,2694,194,774,198,774,194,774,194,910,198,774,194,774,194},//1
  {194,910,194,1734,194,778,194,2830,194,1322,194,1322,194,1186,194,2694,194,13014,194,914,194,2558,190,778,194,774,194,774,218,1026,194,774,194,774,198},//2
  {194,910,194,1734,194,774,194,2830,198,1318,198,1322,194,1186,194,2694,194,13014,194,910,198,2418,194,774,194,774,198,774,194,1186,194,774,194,774,194},//3
  {194,910,194,1738,190,778,194,2830,218,1298,194,1326,214,1162,194,2694,194,13018,194,910,194,2282,194,774,194,778,194,774,194,1322,194,778,190,778,194},//4
  {194,910,218,1714,190,778,190,2834,194,1322,194,1322,194,1186,194,2694,194,13018,190,914,194,2146,194,774,194,774,194,778,190,1462,194,778,190,778,194},//5
  {194,910,194,1734,194,778,194,2830,190,1326,194,1322,194,1190,190,2694,190,13022,194,910,194,2010,190,778,194,774,194,774,194,1598,194,774,194,774,194},//6
  {194,910,194,1734,194,778,190,2834,190,1326,194,1322,194,1186,194,2694,218,12994,190,914,194,1870,194,774,198,774,194,774,194,1734,194,774,194,774,198},//7
  {194,914,190,1734,194,778,190,2834,194,1322,194,1322,194,1186,194,2694,194,13018,190,914,194,1734,194,778,190,778,190,778,194,1870,194,774,194,778,194},//8
  {194,914,190,1738,190,778,190,2834,190,1326,194,1326,190,1186,194,2698,190,13018,194,910,194,1598,194,774,218,750,194,778,194,2006,194,774,194,778,194},//9
  {190,914,194,1734,194,774,194,2830,194,1326,190,1326,190,1190,194,2694,190,13018,194,914,190,2834,190,778,194,774,194,774,194,778,194,774,194,774,194},//0
  {194,910,194,1734,194,774,194,2830,194,1326,194,1322,194,1186,194,2694,194,13014,194,910,198,2006,194,774,194,774,198,1458,194,910,198,774,194,774,194},//<
  {194,910,194,1734,194,774,198,2826,198,1318,198,1322,194,1186,194,2694,194,13014,194,914,194,2142,222,750,218,750,222,2526,222,1846,218,750,218,750,222},//OK->ENTER
  {222,886,218,1710,218,750,218,2806,218,1298,222,1294,222,1158,222,2666,222,12990,218,886,222,1706,222,746,222,750,218,1022,222,1570,222,746,222,750,218},//I
  {194,910,194,1734,194,774,194,2834,190,1326,194,1322,194,1186,194,2694,194,13014,194,914,194,1046,194,778,194,774,194,774,194,2558,194,774,194,774,194},//C+
  {190,918,190,1762,162,782,190,2834,190,1326,190,1326,218,1162,194,2694,218,12994,214,890,214,894,190,778,190,778,190,778,218,2670,218,750,194,774,194},//C-
  {194,910,222,1706,194,774,222,2802,222,1298,194,1322,218,1162,194,2694,218,12990,194,914,218,1162,222,746,222,746,222,1022,222,2118,222,746,222,746,222},//EXIT
  {194,914,194,1734,194,774,194,2830,194,1322,194,1322,198,1182,194,2694,194,13018,194,910,194,1598,194,774,194,778,194,1046,194,1734,194,778,194,774,194},//GUIDE
  {194,914,194,1734,194,774,194,2830,194,1322,194,1322,194,1190,190,2694,194,13018,194,910,194,1462,194,774,194,774,194,1050,194,1870,194,778,194,774,194},//PG+->UP
  {198,910,194,1734,194,774,194,2830,194,1326,194,1322,194,1186,194,2690,198,13014,194,914,194,1322,194,774,194,774,194,1050,194,2006,198,774,194,774,194},//PG-->DOWN
  {194,910,194,1734,194,778,194,2830,194,1322,194,1322,194,1186,194,2694,194,13018,194,910,194,2558,194,774,194,774,194,1050,194,774,194,774,194,774,198},//XFINITY->HOME
  {190,894,190,1738,190,778,190,2834,190,1326,190,1354,162,1194,190,2718,166,13022,214,890,190,2150,190,778,190,802,166,1054,190,1194,186,782,190,778,214},//LEFT
  {190,942,162,1738,218,754,190,2830,194,1350,162,1354,166,1190,214,2674,214,13018,190,918,166,2014,162,802,194,778,190,1078,186,1306,190,778,190,806,162},//RIGHT
  {190,918,162,1766,162,806,186,2838,166,1326,214,1326,166,1218,162,2726,186,13022,162,942,190,2426,162,806,162,806,166,1078,186,922,186,778,166,806,166},//UP
  {190,918,162,1790,162,782,190,2834,162,1354,186,1334,162,1214,190,2698,190,12994,190,942,162,2314,162,810,158,810,162,1082,186,1054,162,810,186,782,162},//DOWN
  {190,918,186,1742,186,782,162,2862,190,1302,190,1354,162,1218,162,2726,162,13046,186,918,166,1766,186,782,162,806,162,1194,186,1494,186,782,162,806,186}//REC->MENU
};

void setup() {
  for(int i=0;i<keys;i++)
        keyfound[i] = true;
        
  if (DEBUG) Serial.begin(115200);
  //delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.setFrameTimeout(13800);
  myReceiver.enableIRIn(); // Start the receiver
  if (DEBUG) Serial.println(F("Ready to receive IR signals"));

  if (DEBUG) pinMode(13, OUTPUT);
}



void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {

    //myDecoder.decode();           //Decode it
    //myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    if (recvGlobal.recvLength != 36) {
      if (DEBUG) {
        Serial.println(recvGlobal.recvLength);
        Serial.println("Not 36");
        myDecoder.decode();           //Decode it
        myDecoder.dumpResults(true);  //Now print results. Use false for less detail
      }
      myReceiver.enableIRIn();
      return;
    }

    
    unsigned int offset = recvGlobal.recvBuffer[1];
      for(bufIndex_t i=2;i<36;i++) {
        int x = recvGlobal.recvBuffer[i]-offset;
        if (x<0) { x=0; }

         

        for (int j=0; j<keys; j++) {
          if (!keyfound[j]) { continue; }

          unsigned int val = pgm_read_word(&keymap[j][i-1]);
          unsigned int offs = pgm_read_word(&keymap[j][0]);
          
          int cmp = val - offs;
          if (cmp<0) { cmp = 0; }
  
          if (abs(x-cmp) >= 100) {
            keyfound[j] = false;
          }
        }
      }

      bool f = false;
      for(int i=0;i<keys;i++) {
        if (keyfound[i]) {
          if (DEBUG) {
            Serial.print("OK ");
            Serial.println(i);
          }
          f=true;

          switch (i) {
            case 0:
             mySender.send(0xE0E020DF);
             break;
            case 1:
             mySender.send(0xE0E0A05F);
             break;
            case 2:
             mySender.send(0xE0E0609F);
             break;
            case 3:
             mySender.send(0xE0E010EF);
             break;
            case 4:
             mySender.send(0xE0E0906F);
             break;
            case 5:
             mySender.send(0xE0E050AF);
             break;
            case 6:
             mySender.send(0xE0E030CF);
             break;
            case 7:
             mySender.send(0xE0E0B04F);
             break;
            case 8:
             mySender.send(0xE0E0708F);
             break;
            case 9:
             mySender.send(0xE0E08877);
             break;
            case 10:
             mySender.send(0xE0E01AE5);
             break;
            case 11:
             mySender.send(0xE0E016E9);
             break;
            case 12:
             mySender.send(0xE0E0F807);
             break;
            case 13:
             mySender.send(0xE0E048B7);
             break;
            case 14:
             mySender.send(0xE0E008F7);
             break;
            case 15:
             mySender.send(0xE0E0B44B);
             break;
            case 16:
             mySender.send(0xE0E0F20D);
             break;
            case 17:
            case 22:
             mySender.send(0xE0E006F9);
             break;
            case 18:
            case 23:
             mySender.send(0xE0E08679);
             break;
            case 19:
             mySender.send(0xE0E058A7);
             delay(200);
             mySender.send(0xE0E0B44B);
             break;
            case 20:
             mySender.send(0xE0E0A659);
             break;
            case 21:
             mySender.send(0xE0E046B9);
             break;
            case 24:
             mySender.send(0xE0E058A7);
             break;
          }

          if (DEBUG) {
            digitalWrite(13, HIGH); // sets the digital pin 13 on
            delay(100);            // waits for a second
            digitalWrite(13, LOW);  // sets the digital pin 13 off
            delay(100);            // waits for a second
          }
        }
      }

      if (!f) {
        for(bufIndex_t i=1;i<recvGlobal.recvLength;i++) {
          unsigned int x = recvGlobal.recvBuffer[i];
          if (DEBUG) {
            Serial.print(x,DEC);
            Serial.print(F(","));
          }
        }
        if (DEBUG) Serial.println("");
      }
      

      for(int i=0;i<keys;i++)
        keyfound[i] = true;

      myReceiver.enableIRIn(); 
  }
}
