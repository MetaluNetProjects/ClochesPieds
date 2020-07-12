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

#include <fraise.h>
#include <analog.h>
#include <switch.h>

unsigned char PERIOD=255;
unsigned char t,t2=0;	
unsigned int InputsConfig=0;

void LowInterrupts() {}
void HighInterrupts() {}

void ConfigInputs() 
{
	Analog_Init();
	Switch_Init();
	
	if(InputsConfig&1) Analog_Select(0,12); //analog 0 = B0
	if(InputsConfig&2) SWITCH_SELECT_(0,B,1); // sw 0 = B1
	if(InputsConfig&4) SWITCH_SELECT_(1,B,2); // sw 1 = B1
	if(InputsConfig&8) SWITCH_SELECT_(2,B,3); // sw 2 = B1
	if(InputsConfig&16) SWITCH_SELECT_(3,B,4); // sw 3 = B1
	if(InputsConfig&32) SWITCH_SELECT_(4,B,5); // sw 4 = B1
	if(InputsConfig&64) SWITCH_SELECT_(5,B,6); // sw 5 = B1
	if(InputsConfig&128) SWITCH_SELECT_(6,B,7); // sw 6 = B1
	if(InputsConfig&256) SWITCH_SELECT_(7,A,0); // sw 7 = A0 ? not working ?
	if(InputsConfig&512) SWITCH_SELECT_(8,A,1); // sw 8 = A1
	if(InputsConfig&1024) SWITCH_SELECT_(9,A,2); // sw 9 = A2
}

void main(void)
{
	unsigned long t3;
	
	//SET_PIN_ANSEL_(A,0,0);
	
	FraiseInit();
		
	Analog_Init();
	//Analog_Select(0,12);
	/*Analog_Select(13,4);
	Analog_Select(11,3);
	Analog_Select(12,2);
	Analog_Select(16,1);
	Analog_Select(9,0);*/

	/*ANALOG_SELECT(0,K4);
	ANALOG_SELECT(1,K5);
	ANALOG_SELECT(2,K6);
	ANALOG_SELECT(3,K7);
	ANALOG_SELECT(4,K10);*/
	
	Switch_Init();
	
	/*SWITCH_SELECT_(0,B,1);
	SWITCH_SELECT_(1,B,2);
	SWITCH_SELECT_(2,B,3);
	SWITCH_SELECT_(3,B,4);
	SWITCH_SELECT_(4,B,5);*/
	/*SWITCH_SELECT(0,K1); 
	SWITCH_SELECT(1,K8);*/

	/*SWITCH_SELECT(3,MA1);
	SWITCH_SELECT(4,MA2);
	SWITCH_SELECT(5,MB1);
	SWITCH_SELECT(6,MB2);

	SetPinDigiOut(Led1); DigitalWrite(Led1,1);
	SetPinDigiOut(Led2); DigitalWrite(Led2,1);
	SetPinDigiOut(Led3); DigitalWrite(Led3,1);*/
	
	EEreadMain();
	ConfigInputs();
	
	t=PERIOD;
	t3=GetTime();
	
	while(1){
		FraiseService();
		Analog_Service();
		Switch_Service();
		if(Elapsed(t3)>Micros(5000UL)) {
			t3=GetTime();
			if(Switch_Send()==0) 
				Analog_Send(0);
			if(!t--){
				t=PERIOD;
				t2++;
				//if(t2==0) Analog_printmin();
				//else if(t2==1) Analog_printmax();
				//else 
				printf("C t2 %d \n", t2);//%d %d\n",t2,PORTB,PORTE);
			}
		}
	}

}

void CharBroadcast()
{
	/*unsigned char c;
	
	printf("Brdcst: ");
	for(c=0;c<FrRXout_len;c++) printf("%c",FrRXgetchar());
	putchar('\n');*/
}

void Broadcast()
{
	/*unsigned char c;
	
	printf("brdcst: ");
	for(c=0;c<FrRXout_len;c++) printf("%d ",FrRXgetchar());
	putchar('\n');*/
}

void CharInput()
{
	unsigned char c;
	
	c=FrRXgetchar();
	//printf("Cchar in:%c %c\n",c,c2);
	/*if(c=='L'){	
		c=FrRXgetchar()-'0';
		DigitalWrite(Led1,(c&1)==0); 
		DigitalWrite(Led2,(c&2)==0); 
		DigitalWrite(Led3,(c&4)==0); 
	}
	else*/ if(c=='E') {
		printf("C");
		for(c=1;c<FrRXout_len;c++) printf("%c",FrRXgetchar());
		putchar('\n');
	}
	else if(c=='S') { //analog scaling
		c=FrRXgetchar()-'0';
		Analog_Scaling(c!=0);
		if(c==0) EEwriteMain();
	}	
	else if(c=='W') { //write eeprom
		c=FrRXgetchar()-'0';
		if(c==0) EEwriteMain();
	}	
}

void Input()
{
	unsigned char c;//,c2;
	//unsigned int i;

	c=FrRXgetchar();

	switch(c) {
		PARAM_CHAR(1,t2); break;
		PARAM_CHAR(2,PERIOD); break;
		PARAM_INT(3,InputsConfig); ConfigInputs(); break;
	}
}

void EEdeclareMain()
{
	EEsetaddress(EEUSER);
	
	Analog_declareEE();
	EEdeclareInt(&InputsConfig);
}
