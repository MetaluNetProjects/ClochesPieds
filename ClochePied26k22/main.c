/*********************************************************************
 *
 *                ClochePied for Pied26k22
 *
 *********************************************************************
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Antoine Rousseau  march 2012     Original.
 ********************************************************************/

/*
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
*/

#define BOARD Pied26k22

#include <fruit.h>
#include <analog.h>
#include <switch.h>

unsigned char PERIOD=255;
unsigned char t,t2=0;	
unsigned int InputsConfig=0;
t_delay mainDelay;

void ConfigInputs() 
{
	analogInit();
	switchInit();
	
	if(InputsConfig&1) 		analogSelect(0, AN0); //analog 0 = B0
	if(InputsConfig&2) 		switchSelect(0, SW0); // sw 0 = B1
	if(InputsConfig&4) 		switchSelect(1, SW1); // sw 1 = B1
	if(InputsConfig&8) 		switchSelect(2, SW2); // sw 2 = B1
	if(InputsConfig&16) 	switchSelect(3, SW3); // sw 3 = B1
	if(InputsConfig&32) 	switchSelect(4, SW4); // sw 4 = B1
	if(InputsConfig&64) 	switchSelect(5, SW5); // sw 5 = B1
	if(InputsConfig&128) 	switchSelect(6, SW6); // sw 6 = B1
	if(InputsConfig&256) 	switchSelect(7, SW7); // sw 7 = A0 ? not working ?
	if(InputsConfig&512) 	switchSelect(8, SW8); // sw 8 = A1
	if(InputsConfig&1024) 	switchSelect(9, SW9); // sw 9 = A2
}

void setup(void)
{
	fruitInit();
	EEreadMain();
	ConfigInputs();
	delayStart(mainDelay, 5000); 	// init the mainDelay to 5 ms
}

void loop(void)
{
	t=PERIOD;
	fraiseService();
	analogService();
	switchService();
	if(delayFinished(mainDelay)) // when mainDelay triggers :
	{
		delayStart(mainDelay, 5000); 	// re-init mainDelay
		if(!switchSend()) analogSend();
		if(!t--){
			t=PERIOD;
			t2++;
			printf("C t2 %d \n", t2);//%d %d\n",t2,PORTB,PORTE);
		}
	}
}

void fraiseReceiveChar()
{
	unsigned char c;
	
	c=fraiseGetChar();
	if(c=='E') {
		printf("C");
		c = fraiseGetLen(); 			// get length of current packet
		while(c--) printf("%c",fraiseGetChar());// send each received byte
		putchar('\n');				// end of line
	}
	else if(c=='S') { //analog scaling
		c=fraiseGetChar()-'0';
		analogScaling(c!=0);
		if(c==0) EEwriteMain();
	}	
	else if(c=='W') { //write eeprom
		c=fraiseGetChar()-'0';
		if(c==0) EEwriteMain();
	}	
}

void fraiseReceive()
{
	unsigned char c;//,c2;
	//unsigned int i;

	c=fraiseGetChar();

	switch(c) {
		PARAM_CHAR(1,t2); break;
		PARAM_CHAR(2,PERIOD); break;
		PARAM_INT(3,InputsConfig); ConfigInputs(); break;
	}
}

void EEdeclareMain()
{
	analogDeclareEE();
	EEdeclareInt(&InputsConfig);
}
