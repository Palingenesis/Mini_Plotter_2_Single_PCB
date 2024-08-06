
/*
	Name:       Tims I2C Memory Writer Firmware
	Created:	27/02/2019 16:24:59
	Edited:		August 2024
	Author:     Tim Jackson.1960

	More info about this can be found at my Blog: https://tims-mini-plotter2.blogspot.com/2019/01/tims-mini-plotter-2-this-is-new-project.html
	Also the New version at: https://www.youtube.com/watch?v=YSE00Mr2BqY&t=1783s

	This is the bit of code I use to test and write data to 512 I2C EEPROM
	It has a sister program that is a windows aplication. https://drive.google.com/open?id=19ci9kac4xSghIvizUF5hXIfjpHqR-iEv
	It is code to store text as I ran out of memory on a NANO making a large menu system.


	Creadits:
		I have only used the Wire libray with this one. https://www.arduino.cc/

	ATMega328p (Arduino NANO)
	Sketch uses 6368 bytes (20%) of program storage space. Maximum is 30720 bytes.
	Global variables use 523 bytes (25%) of dynamic memory, leaving 1525 bytes for local variables. Maximum is 2048 bytes.


*/


#include <Wire.h>	/*	For I2C	*/

#define VERSION		1.1      /*	 My firmware version	*/
#define MAX_BUF		(64)     /*	 What is the longest message Arduino can store? The serial receive buffer holds 64 bytes.	*/
#define EEPROM_ADD	0x50    /*	 512 I2C EEPROM address (64k bytes) 	*/
#define MAX_I2C_PAGE_WRITE 64 

/*	SERIAL MESSAGES	*/
char  buffer[MAX_BUF];  /*	 message	*/
unsigned int no_data = 0;
unsigned int sofar;/*	 size of buffer	*/
bool procsessingString = false;
bool isSetting = false;

/*	MEMORY ADDRESSES	*/
char memWord[MAX_I2C_PAGE_WRITE];
byte memNumber = 0;
byte compData[3];
long MenuAddress = 0;/*	Mem 1 only	*/
long PlotNameAddress = 500;/*	Mem 1 only	*/
long dataStartAddress = 3000; /*	3000 for Mem 1, 0 for others.	*/
int lengthOfData = 3;


/*	WORDS USED IN MENUE and COMUNICATION (uses first mem chip, allocate first 500 bytes (0 to 499) to this)	*/
char* menuWords[] = { "MAIN", "MANUAL DRAW", "CHOOSE PLOT", "SETTINGS", "INFO", "RESET", "MOVE X", "DRAW X", "MOVE Y", "DRAW Y", "<<== BACK",
						"SPEED", "TOOL", "--", "Tim's Mini Plotter", "ok", "=== Motors Off ===", "Version: ", "Basic Gcode Commands",
						"M03 - Tool On/Down", "M05 - Tool Off/Up", "M18 - Motors Off", "System Reset", "M100 - this Info message",
						"R01 - Reset Plotter", "By Tim Jackson.1960", "================", "          ", "test A", "" };
int menuWordsLength = 29;

/*	 RULES FOR PLOT NAMES	*/

/*

	make all plot names 10 Characters to simplify menu system.
	will need 1 byte for Memory IC number. (needs to hold number 0 to 2)
	will need 2 bytes for start address. (needs to hold max number 32000)
	will need 2 bytes for length of plot data. (needs to hold max number 32000)

	Going to allocate 15 bytes to each plot.
	As I have managed to compress each command to 3 bytes, and I am using three 32k byte chips, I will probably have room for a lot of plots.
	So I am going to set aside the next 2500 bytes to the plot names. (500 to 2999)
	This is enough for 2500/15 = 166 plot names. (going to add the alphabet (74), abcdefghijklmnopqrstuvwxyxABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/*-+=()&%$£!)
	Actual plots will be a max of 92.

	As 166 plot names only use up to address 2990, I am going us 1 byte at address 2991 for storing the number of plot files saved. (needs to hold 0 to 166)

	I may alocate IC 3 to the alphabet at a later date after I see how it goes.

*/

/*	WORDS USED IN FILE MENU (old way i did it)*/
char* plotNames[] = { "Cube      ", "Snowflake ", "Wolf      ", "Links     ", "Witch     ", "Unicorn 1 ", "Butterfly ", "Spider    ", "Triangles ",
						"Skull     ", "Tim       ", "Alien     ", "Agent     ", "5 P Star  ", "9 P Star  ", "18 P Star ", "Reaper    ", "Octopus   " };
int plotNamesLength = 18;

void setup()
{
	Serial.begin(115200);	/*	 Initialize the serial	*/
	Wire.begin();
	Wire.setClock(400000);

	while (!Serial) {
		/*	 wait for serial port to connect.	*/
	}

	Serial.flush();
	Serial.println("Connected");
	delay(5);
	Serial.println("ok");
	Serial.flush();

	/*	 RULES TO STORE PLOTS	*/

	/*

		I2C Memory
		Total memory = 32k Bytes.
		Buffer = 64 Bytes.
		Write cycle time: 5ms

		Work out file system:
		1 char = 1 Byte.
		Lets make G-Code as short as posible.
		lets have only straight lines.
		No ARC commands.
		Have only up/off or down/on.

		Our max distance is 40mm.(actual max movement my plotter has is 42mm. but im rounding it for reasions.)
		But i want it to 1 decimal place.
		As bytes (8 bits) are are 0 to 255 positive numbers i will multiply our distance by 10, then divide it by 10 later.
		So the largest number is going to 409.
		I could use 2 bytes (16 bits) and have a max of 65,535 but I dont need that. I want to compress the code as mutch as posible.

		So I am going to convert things to the minimum bits needed.

		So max number = 409. 9 bits should hold that. (it will hold 511)
		We have 2 numbers so we need 2 of them, 18 bits so far.

		We need 1 bit to say if it is up/off or down/on

		That meens I only need 19 bits to make a command.
		as memory is stored in Bytes we are going to use 3 Bytes (24 bits). 5 bits won't be used.

		So standard G-Code:
		typical draw command = G01 X10.0 Y10.0 (this would normaly take up 15 Bytes of memory)
		typical move command = G00 X10.0 Y10.0 (this would normaly take up 15 Bytes of memory)

		So:
		up/off or down/on is the first part of the command, 1 bit (1 or 0)
		X dimention (x10) is the second part of the command, 9 bits (0 to 511)
		Y dimention (x10) is the third part of the command, 9 bits (0 to 511)

		Example G01 X25.4 Y10.8

		G01 = 1
		X25.4 = 254 = 011111110 (remember we are using 9 bits)
		Y10.8 = 108 = 001101100 (remember we are using 9 bits)

		we put them into a 24 bit command
		000001011111110001101100 (5 bits are not used, so it will alwas start with 00000)

		= 392300 (this number can be held in 3 Bytes)

		When plotting this code we just do a revers convertion to a standard G-Code.

		By doing this we have reduced a command by 80%
		So we can fit 80% more plot commands on the memory.

		Lets say we take some for menue and plot names:
		Lets 500 bytes for menu names.
		Lets 500 bytes for Plot names. Plot names to be 10 charectors long (use spaces to extend). this gives room for 50 plot names.
		Leaving 31k byte for plots.

		So 31000/3 = 10333 plot commands.

		If I add more memory IC, each extra will have 32000/3 = 10666 plot commands.

		decided to use 2 of the spair bits to indicate start and end of a plot

		NOTE! Arduino does not recognize sent exstended ASCII, so send raw bits.

	*/


	/*	===		EEPROM Checks	===	*/

	readWords_Mini_Plotter_2();	/*	512 I2C EEPROM	1	*/
	//ReadCharectors();				/*	512 I2C EEPROM	3	*/

	/*	===		Workings	===	*/

	//ClearMemory(0, 1000);
	//ReadMemory(0, 900);

	//MemTest32KB(0, 499, 32);

	//writeMenuData();
	//writePlotNames();
	//readWords_Cybot_IR();
	//ReadPlotFiles_Mem_1();
	//ReadPlotFiles_Mem_2();
	//ReadPlotFiles_Mem_1_compressed(3000, 156);/*	 cube				*/
	//ReadPlotFiles_Mem_1_compressed(3157, 606);/*	 snowflake			*/
	//ReadPlotFiles_Mem_1_compressed(1296, 150);
	//ReadPlotFiles_Mem_1_compressed(4831, 615);/*	 Witch on a stick	*/
	//printStringArrays();


}

void loop()
{
	Serial.flush();

	if (!procsessingString) {
		ReadSerial();

	}



}
void ReadSerial() {

	char p;
	char c;
	/*	Read characters if we have them.	*/
	if (Serial.available() > 0) {
		no_data = 0;
		p = Serial.peek();
		c = Serial.read();
		/*	Newlines are ends of commands.	*/
		if (!(c == '\r' && p == '\n') || sofar <= lengthOfData) {

			buffer[sofar] = c;
			sofar++;
		}
	}
	else {

		/*	If data count is grater than Zero AND eather char is a Carage Return, char is a New Line or Timed Out.	*/
		if (sofar && (c == '\n' || c == '\r' || no_data > 100)) {

			while (sofar < MAX_BUF) {
				c = Serial.read();
				buffer[sofar] = 0;
				sofar++;
			}

			if (*buffer != '\n' && *buffer != '\r') {

				procsessingString = true;
				if (*buffer == '$' && buffer[1] == 's') { changeSettings(); }
				else { writeCommand(); }

			}
			delay(30);
			init_process_string();	/*	Clear Command.	*/
		}
		no_data++;
	}
}
void init_process_string() {

	/*
		Init our command.
		Clean buffer and send ready "ok".
	*/
	for (byte i = 0; i < MAX_BUF; i++) { buffer[i] = 0; }
	sofar = 0;
	no_data = 0;
	Serial.println("ok");
	delay(20);
	Serial.flush();
	procsessingString = false;
}
void writeCommand() {

	//Serial.print("Writing Command. Data Length ");	/*	 don't use unless needed	*/
	//delay(20);
	//Serial.println(lengthOfData);	/*	 don't use unless needed	*/
	//delay(20);

	delay(30);

	for (byte i = 0; i < lengthOfData; i++) {

		Wire.beginTransmission(EEPROM_ADD);
		delay(2);
		Wire.write((int)(dataStartAddress >> 8));
		delay(6);
		Wire.write((int)(dataStartAddress & 0xFF));
		delay(6);

		Wire.write(byte(buffer[i]));
		delay(6);

		Wire.endTransmission();
		delay(2);

		dataStartAddress++;


	}

	delay(40);
	//Serial.print("Command Written");	/*	 Don't use unless needed.	*/
	//delay(20);

}
void changeSettings() {

	delay(30);

	char* ptr = buffer;				/*	 Start at the beginning of buffer.							*/

	ptr = strchr(ptr, ' ') + 1;		/*	 Take a step from here to the char. after the next space.	*/
	dataStartAddress = atof(ptr);

	ptr = strchr(ptr, ' ') + 1;		/*	 Take a step from here to the char. after the next space.	*/
	lengthOfData = atof(ptr);

	Serial.print("Start Address: ");
	delay(20);
	Serial.println(dataStartAddress);
	delay(20);
	//Serial.flush();
	Serial.print("Page Length: ");
	delay(20);
	Serial.println(lengthOfData);
	delay(20);
	//Serial.flush();

}

	/*	===		Workings	===	*/
void printStringArrays() {

	for (size_t i = 0; i < menuWordsLength; i++) {

		Serial.println(menuWords[i]);
		Serial.flush();

	}

	for (size_t i = 0; i < plotNamesLength; i++) {

		Serial.println(plotNames[i]);
		Serial.flush();

	}


}
void readWords_Mini_Plotter_2() {

	Serial.println();
	Serial.println();
	Serial.flush();

	/*	Menu :	*/
	readWord(0, 4);		/*	1 MAIN						*/
	readWord(4, 11);	/*	2 MANUAL DRAW				*/
	readWord(15, 11);	/*	3 CHOOSE PLOT				*/
	readWord(26, 8);	/*	4 SETTINGS					*/
	readWord(34, 4);	/*	5 INFO						*/
	readWord(38, 5);	/*	6 RESET						*/
	readWord(43, 6);	/*	7 MOVE X					*/
	readWord(49, 6);	/*	8 DRAW X					*/
	readWord(55, 6);	/*	9 MOVE Y					*/
	readWord(61, 6);	/*	10 DRAW Y					*/
	readWord(67, 9);	/*	11 <<= = BACK				*/
	readWord(76, 5);	/*	12 SPEED					*/
	readWord(81, 4);	/*	13 TOOL						*/
	readWord(85, 2);	/*	14 --						*/
	readWord(87, 18);	/*	15 Tim's Mini Plotter		*/
	readWord(105, 2);	/*	16 ok						*/
	readWord(107, 18);	/*	17 == = Motors Off == =		*/
	readWord(125, 9);	/*	18 Version:					*/
	readWord(134, 20);	/*	19 Basic Gcode Commands		*/
	readWord(154, 18);	/*	20 M03 - Tool On/Down		*/
	readWord(172, 17);	/*	21 M05 - Tool Off/Up		*/
	readWord(189, 16);	/*	22 M18 - Motors Off			*/
	readWord(205, 12);	/*	23 System Reset				*/
	readWord(217, 24);	/*	24 M100 - this Info message	*/
	readWord(241, 19);	/*	25 R01 - Reset Plotter		*/
	readWord(260, 19);	/*	26 By Tim Jackson.1960		*/
	readWord(279, 16);	/*	27 ================			*/
	readWord(295, 10);	/*	28 (10 spaces)				*/
	readWord(305, 12);	/*	29 TYPE OF PLOT				*/
	readWord(317, 7);	/*	30 OUTLINE					*/
	readWord(324, 4);	/*	31 TEXT						*/
	readWord(328, 14);	/*	32 CHOOSE OUTLINE			*/
	readWord(342, 3);	/*	33 A - Z					*/
	readWord(345, 3);	/*	34 a - z					*/
	readWord(348, 3);	/*	35 0 - 9					*/
	readWord(351, 7);	/*	36 SYMBOLS					*/
	readWord(358, 10);	/*	37 CHOOSE A - Z				*/
	readWord(368, 10);	/*	38 CHOOSE a - z				*/
	readWord(378, 10);	/*	39 CHOOSE 0 - 9				*/
	readWord(388, 14);	/*	40 CHOOSE SYMBOLS			*/
	readWord(402, 16);	/*	41 CHOOSE CHARACTER			*/
	readWord(418, 5);	/*	42 Servo					*/
	readWord(423, 5);	/*	42 Laser					*/
	readWord(428, 6);	/*	42 EndEND					*/

	Serial.println();
	Serial.println();
	Serial.flush();

	/*	Number of plots	*/
	readWord(2991, 1);					/*	char of byte number of plots.	*/
	Serial.print("Number of plots = ");
	Serial.println(memNumber);			/*	byte number of plots.			*/
	Serial.flush();


	/*	 Plot Names + location:	*/
	Serial.println();
	Serial.println("=== Plot List ===");
	Serial.println();



	//for (size_t i = 500; i < 3000; i += 15) {	/*	 all	*/
	for (size_t i = 500; i < int(500 + ((memNumber - 1) * 15)); i += 15) {

		readPlotNames(i);

	}




}
void readWords_Cybot_IR() {

	Serial.println();
	Serial.println();
	Serial.flush();

	/*	Menu Words:	*/

	readWord(0, 10);	/*	<10 Spaces>				*/
	readWord(10, 5);	/*	GAMES					*/
	readWord(15, 5);	/*	CYBOT					*/
	readWord(15, 13);	/*	CYBOT TRAINER			*/
	readWord(28, 11);	/*	CYBOT RACER				*/
	readWord(39, 8);	/*	MUNCHBOT				*/
	readWord(47, 5);	/*	ROBOT					*/
	readWord(47, 12);	/*	ROBOT SELECT			*/
	readWord(59, 4);	/*	MODE					*/
	readWord(63, 7);	/*	PC LINK					*/
	readWord(70, 5);	/*	RESET					*/
	readWord(75, 5);	/*	Cybot					*/
	readWord(80, 3);	/*	Tom						*/
	readWord(83, 7);	/*	ROBOT 3					*/
	readWord(90, 9);	/*	<<== Back				*/
	readWord(99, 13);	/*	CYBOT CHANNEL			*/
	readWord(112, 3);	/*	TOM						*/
	readWord(112, 11);	/*	TOM CHANNEL				*/
	readWord(123, 5);	/*	SONAR					*/
	readWord(123, 8);	/*	SONAR(T)				*/
	readWord(131, 5);	/*	LIGHT					*/
	readWord(136, 11);	/*	FOLLOW LINE				*/
	readWord(147, 14);	/*	REMOTE CONTROL			*/
	readWord(147, 17);	/*	REMOTE CONTROL(T)		*/
	readWord(164, 14);	/*	RUN PROGRAM(T)			*/
	readWord(178, 15);	/*	TRACK IR TAG(T)			*/
	readWord(193, 6);	/*	CYBALL					*/
	readWord(199, 2);	/*	AI						*/
	readWord(201, 13);	/*	HIDE AND SEEK			*/
	readWord(214, 10);	/*	AVOID FAST				*/
	readWord(214, 13);	/*	AVOID FAST(T)			*/
	readWord(227, 10);	/*	AVOID SLOW				*/
	readWord(237, 6);	/*	FOLLOW					*/
	readWord(237, 9);	/*	FOLLOW(T)				*/
	readWord(246, 11);	/*	FOLLOW FAST				*/
	readWord(257, 11);	/*	FOLLOW SLOW				*/
	readWord(268, 5);	/*	SETUP					*/
	readWord(268, 8);	/*	SETUP(T)				*/
	readWord(276, 9);	/*	ACTIVATED				*/
	readWord(285, 8);	/*	BUTTON A				*/
	readWord(293, 8);	/*	BUTTON B				*/
	readWord(301, 8);	/*	BUTTON C				*/
	readWord(309, 8);	/*	BUTTON D				*/
	readWord(317, 14);	/*	FLASH AMTENNAE			*/
	readWord(331, 13);	/*	FLASH LEDS(T)			*/
	readWord(344, 15);	/*	AMTENNAE ON/OFF			*/
	readWord(359, 14);	/*	LEDS ON/OFF(T)			*/
	readWord(373, 13);	/*	BEEP(T)/SHOOT			*/
	readWord(386, 8);	/*	CHIME(T)				*/
	readWord(394, 8);	/*	AUDIO(T)				*/
	readWord(402, 12);	/*	RECORD SOUND			*/
	readWord(414, 8);	/*	DOWNLOAD				*/
	readWord(422, 12);	/*	CLEAR MEMORY			*/
	readWord(434, 15);	/*	PLAY FLASH FILE			*/
	readWord(449, 20);	/*	VOICE RECOGNITION(T)	*/
	readWord(469, 21);	/*	SETUP USER COMMAND(T)	*/
	readWord(490, 7);	/*	LIBRARY					*/
	readWord(497, 6);	/*	YES/NO					*/
	readWord(503, 14);	/*	03/04/VR ON(T)			*/
	readWord(517, 17);	/*	03/04 ACTIVATE(T)		*/
	readWord(523, 11);	/*	ACTIVATE(T)				*/
	readWord(534, 10);	/*	PROGRAM 01				*/
	readWord(544, 10);	/*	PROGRAM 02				*/
	readWord(554, 16);	/*	1 PLAYER (CYBOT)		*/
	readWord(570, 16);	/*	2 PLAYER (CYBOT)		*/
	readWord(586, 14);	/*	1 PLAYER (TOM)			*/
	readWord(600, 20);	/*	TOM VS CYBOT (SMART)	*/
	readWord(620, 8);	/*	4 PLAYER				*/
	readWord(628, 12);	/*	UNSUPERVISED			*/
	readWord(640, 10);	/*	SUPERVISED				*/
	readWord(650, 11);	/*	CYBOT SEEKS				*/
	readWord(661, 9);	/*	TOM SEEKS				*/
	readWord(670, 4);	/*	<==>					*/
	readWord(674, 3);	/*	USB						*/
	readWord(677, 1);	/*	-						*/



	Serial.println();
	Serial.println();
	Serial.flush();

	/*	Plot Names + location:	*/

	////for (size_t i = 500; i < 3000; i += 15) {	/*	all	*/
	//for (size_t i = 500; i < 770; i += 15) {

	//	readPlotNames(i);


	//}

	//readWord(2991, 1);			/*	char of byte number of plots	*/
	//Serial.println(memNumber);	/*	 byte number of plots			*/
	//Serial.flush();



}
void ReadCharectors() {

	/*	 Charector Plots Stored on EEPROM 3	*/

	Serial.println();
	Serial.println();
	Serial.flush();

	/*	 Plot Charectors + location:	*/

	//for (size_t i = 0; i < (52 * 6); i += 6) {	/*	 all	*/
	for (size_t i = 0; i < (88 * 6); i += 6) {

		readPlotCharectors(i);

	}

}
void ReadPlotFiles_Mem_1() {

	/*
		To Write file:
		Set dataStartAddress to Start Address
		File lenth is determined by File Sender

		To read file:
		Use command:
		readCommand(dataStartAddress, Length of file commands);
		ie. readCommand(1000, 238);

	*/

	//readWord(1000, 15);	/*	start address, number of bytes	*/
	//readWord(1015, 15);	/*	start address, number of bytes	*/
	//readWord(1030, 15);	/*	start address, number of bytes	*/
	//readWord(1045, 15);	/*	start address, number of bytes	*/
	//readWord(1060, 15);	/*	start address, number of bytes	*/
	//readWord(1075, 15);	/*	start address, number of bytes	*/
	//readWord(1090, 15);	/*	start address, number of bytes	*/
	//readWord(1105, 15);	/*	start address, number of bytes	*/
	//readWord(1120, 15);	/*	start address, number of bytes	*/
	//readWord(1135, 15);	/*	start address, number of bytes	*/
	//readWord(1150, 15);	/*	start address, number of bytes	*/
	//readWord(1165, 15);	/*	start address, number of bytes	*/
	//readWord(1180, 15);	/*	start address, number of bytes	*/
	//readWord(1195, 15);	/*	start address, number of bytes	*/
	//readWord(1210, 15);	/*	start address, number of bytes	*/
	//readWord(1225, 15);	/*	start address, number of bytes	*/
	//readWord(1240, 15);	/*	start address, number of bytes	*/



	/*	 ALL (Cube","Snowflake", "Wolf", "Links","Witch","Unicorn 1" ,"Butterfly", "Spider" ,"Triangles")	*/
	//readCommand(1000, 20);	/*	(1000,2066)	*/


	/*	Cube	*/
	Serial.println();
	Serial.println("Cube");
	Serial.println();
	Serial.flush();
	readCommand(1015, 6);

	/*	Snowflake	*/
	Serial.println();
	Serial.println("Snowflake");
	Serial.println();
	Serial.flush();
	readCommand(2230, 6);

	/*	Wolf Howling	*/
	Serial.println();
	Serial.println("Wolf Howling");
	Serial.println();
	Serial.flush();
	readCommand(5725, 6);

	/*	Links	*/
	Serial.println();
	Serial.println("Links");
	Serial.println();
	Serial.flush();
	readCommand(8890, 6);

	/*	Witch on a Stick	*/
	Serial.println();
	Serial.println("Witch on a Stick");
	Serial.println();
	Serial.flush();
	readCommand(12280, 6);

	/*	Unicorn 1	*/
	Serial.println();
	Serial.println("Unicorn 1");
	Serial.println();
	Serial.flush();
	readCommand(15820, 6);

	/*	Butterfly	*/
	Serial.println();
	Serial.println("Butterfly");
	Serial.println();
	Serial.flush();
	readCommand(19900, 6);

	/*	Spider	*/
	Serial.println();
	Serial.println("Spider");
	Serial.println();
	Serial.flush();
	readCommand(24745, 6);

	/*	Rotating Triangles	*/
	Serial.println();
	Serial.println("Rotating Triangles");
	Serial.println();
	Serial.flush();
	readCommand(29080, 6);



}
void ReadPlotFiles_Mem_2() {

	/*
		To Write file:
		Set dataStartAddress to Start Address
		File lenth is determined by File Sender

		To read file:
		Use command:
		readCommand(dataStartAddress, Length of file commands);
		ie. readCommand(1000, 238);

	*/

	/*
		"Skull"			515		15
		"Tim"			544		7710
		"Alien"			129		15870
		"Agent"			348		17805
		"5 P Star"		15		23025
		"9 P Star"		23		23250
		"18 P Star"		41		23595
		"Reaper"		145		24210
		"Octopus"		367		26385

	*/

	/*	 ALL ("Skull","Tim", "Alien", "Agent", "5 P Star", "9 P Star", "18 P Star", "Reaper", "Octopus")	*/
	//delay(100);
	//readCommand(0, 2133);

	/*	"Skull"	*/
	Serial.println();
	Serial.println("Skull");
	Serial.println();
	Serial.flush();
	readCommand(15, 6);
	//readCommand(15, 515);


	/*	 "Tim"	*/
	Serial.println();
	Serial.println("Tim");
	Serial.println();
	Serial.flush();
	readCommand(7710, 6);
	//readCommand(7710, 544);

	/*	 "Alien"	*/
	Serial.println();
	Serial.println("Alien");
	Serial.println();
	Serial.flush();
	readCommand(15870, 6);
	//readCommand(15870, 129);

	/*	 "Agent"	*/
	Serial.println();
	Serial.println("Agent");
	Serial.println();
	Serial.flush();
	readCommand(17805, 6);
	//readCommand(17805, 348);

	/*	 "5 P Star"	*/
	Serial.println();
	Serial.println("5 P Star");
	Serial.println();
	Serial.flush();
	readCommand(23025, 6);
	//readCommand(23025, 15);

	/*	 "9 P Star"	*/
	Serial.println();
	Serial.println("9 P Star");
	Serial.println();
	Serial.flush();
	readCommand(23250, 6);
	//readCommand(23250, 23);

	/*	 "18 P Star"	*/
	Serial.println();
	Serial.println("18 P Star");
	Serial.println();
	Serial.flush();
	readCommand(23595, 6);
	//readCommand(23595, 41);

	/*	 "Reaper"	*/
	Serial.println();
	Serial.println("Reaper");
	Serial.println();
	Serial.flush();
	readCommand(24210, 6);
	//readCommand(24210, 145);

	/*	 "Octopus"	*/
	Serial.println();
	Serial.println("Octopus");
	Serial.println();
	Serial.flush();
	readCommand(26385, 6);
	//readCommand(26385, 367);


}
void ReadPlotFiles_Mem_1_compressed(int startAddress, int numberCommands) {

	Serial.println();
	Serial.println();
	Serial.flush();

	//readCompressed(1000);	/*	 start address (number of bytes is always 3)	*/

	bool penIsUp = true;
	String commandString = "\tM05";
	Serial.println(commandString);
	Serial.flush();


	for (size_t i = startAddress; i < startAddress + numberCommands; i += 3) {

		Serial.println(i);	/*	 Address	*/
		Serial.flush();

		readCompressed(i);	/*	 Function put 3 bytes in compressed data array <compData>	*/

		/*	 seperate the bytes	*/
		byte firstByte = compData[0];
		byte secondByte = compData[1];
		byte thirdByte = compData[2];

		delay(50);

		unsigned long tempPart = 0L;		/*	 Used to hold tempary binary.		*/
		unsigned long commandBinary = 0L;	/*	 Used to hold recontructed binary.	*/

		/*	Recontrution	*/
		/*	 XXXXXXXX XXXXXXXX XXXXXXXX = recived	*/
		for (size_t i = 0; i < 8; i++) {
			bitWrite(commandBinary, i + 16, bitRead(firstByte, i));
			bitWrite(commandBinary, i + 8, bitRead(secondByte, i));
			bitWrite(commandBinary, i, bitRead(thirdByte, i));
		}
		/*	 000 X X X XXXXXXXXX XXXXXXXXX = Command	*/

		/*	 000 X 0 0 000000000 000000000 = (1)start of plot		  	*/
		/*	 000 0 X 0 000000000 000000000 = (1)end of plot		  	*/
		/*	 000 0 0 X 000000000 000000000 = (0)Up/Off (1)Down/On	  	*/
		/*	 000 0 0 0 XXXXXXXXX 000000000 = (0)dimention X		  	*/
		/*	 000 0 0 0 000000000 XXXXXXXXX = (0)dimention Y		  	*/

		/*	 222 2 1 1 111111119 876543210				  	*/
		/*	 321 0 9 8 76543210							  	*/

		commandString = "";

		boolean MovePen = false;

		if (bitRead(commandBinary, 19)) { Serial.println("Start of Plot"); }
		if (bitRead(commandBinary, 20)) { Serial.println("End of Plot"); }
		Serial.flush();

		/*	 Check for a change, no need to move pen every command.	*/
		if (bitRead(commandBinary, 18)) {
			commandString = "G01";
			if (penIsUp) { MovePen = true; }
			penIsUp = false;
		}
		else {
			commandString = "G00";
			if (!penIsUp) { MovePen = true; }
			penIsUp = true;
		}

		/*	 If pen state changes.	*/
		if (MovePen) {
			if (commandString == "G00") { Serial.println("\tM05"); }
			else { Serial.println("\tM03"); }
			Serial.flush();
		}

		/*	 Get dimention X.	*/
		int dimentionX = 0;
		for (size_t i = 0; i < 9; i++) { bitWrite(dimentionX, i, bitRead(commandBinary, i + 9)); }

		/*	 Get dimention Y.	*/
		int dimentionY = 0;
		for (size_t i = 0; i < 9; i++) { bitWrite(dimentionY, i, bitRead(commandBinary, i)); }

		/*	 Change to 1 decimal place.	*/
		float floatDimX = (float)dimentionX / 10;
		float floatDimY = (float)dimentionY / 10;


		commandString = commandString + " X" + String(floatDimX, 1) + " Y" + String(floatDimY, 1);



		//Serial.println();
		Serial.println("\t" + String(commandString));
		//Serial.println();
		Serial.flush();



	}

}
char readCommand(int eAddress, int lines) {

	int thisAddress = eAddress;
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear command in memWord.	*/

	for (size_t l = 0; l < lines; l++) {

		byte byteCount = 0;

		Wire.beginTransmission(EEPROM_ADD);
		delay(1);
		Wire.write((int)(thisAddress >> 8));
		delay(6);
		Wire.write((int)(thisAddress & 0xFF));
		delay(6);
		Wire.endTransmission();
		delay(1);

		Wire.requestFrom(EEPROM_ADD, lengthOfData);	/*	 Ask for a command length of data	*/
		delay(1);

		while (Wire.available()) {

			byte c = Wire.read();
			delay(1);

			memWord[byteCount] = char(c);
			byteCount++;
		}

		Serial.print("Address start = ");
		Serial.println(thisAddress);

		Serial.println(memWord);	/*	 Command has return new line	*/
		Serial.flush();
		thisAddress += lengthOfData;
	}

}
void writeMenuData() {

	Serial.print("Writing Menu: ");
	Serial.println();
	Serial.flush();

	for (size_t w = 0; w < menuWordsLength; w++) {

		String thisWord = { menuWords[w] };
		byte thisWordLength = thisWord.length();

		Serial.print(w + 1);
		Serial.print(" ");
		Serial.print(thisWord);
		Serial.flush();
		byte spaces = 50 - thisWordLength;
		for (size_t ii = 0; ii < spaces; ii++) { Serial.print(" "); }
		Serial.print("readWord(");
		Serial.print(MenuAddress);
		Serial.print(", ");
		Serial.print(thisWordLength);
		Serial.println(");");
		Serial.flush();

		for (size_t i = 0; i < thisWordLength; i++) {

			Wire.beginTransmission(EEPROM_ADD);
			delay(1);
			Wire.write((int)(MenuAddress >> 8));
			delay(6);
			Wire.write((int)(MenuAddress & 0xFF));
			delay(6);

			int bytesSent(Wire.write(byte(thisWord.charAt(i))));
			delay(6);

			Wire.endTransmission();
			delay(1);

			MenuAddress++;
		}
	}
}
void writePlotNames() {

	Serial.print("Writing Plot Names: ");
	Serial.println();
	Serial.flush();

	for (size_t w = 0; w < plotNamesLength; w++) {

		String thisWord = { plotNames[w] };
		byte thisWordLength = thisWord.length();

		Serial.print(w + 1);
		Serial.print(" ");
		Serial.print(thisWord);
		Serial.flush();
		byte spaces = 50 - thisWordLength;
		for (size_t ii = 0; ii < spaces; ii++) { Serial.print(" "); }
		Serial.print("readWord(");
		Serial.print(PlotNameAddress);
		Serial.print(", ");
		Serial.print(thisWordLength);
		Serial.println(");");
		Serial.flush();

		for (size_t i = 0; i < thisWordLength; i++) {

			Wire.beginTransmission(EEPROM_ADD);
			delay(1);
			Wire.write((int)(PlotNameAddress >> 8));
			delay(6);
			Wire.write((int)(PlotNameAddress & 0xFF));
			delay(6);

			int bytesSent(Wire.write(byte(thisWord.charAt(i))));
			delay(6);

			Wire.endTransmission();
			delay(1);

			PlotNameAddress++;
		}
	}
}
char readWord(int dataAddress, int dataLength)
{
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)(dataAddress >> 8));
	delay(6);
	Wire.write((int)(dataAddress & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, dataLength);	/*	 Ask for a length of data.	*/

	while (Wire.available() < dataLength);

	for (size_t i = 0; i < dataLength; i++)
	{
		memNumber = Wire.read();
		memWord[i] = char(memNumber);
	}

	Serial.println(memWord);
	Serial.flush();

}
char readPlotNames(int eAddress)
{
	byte byteCount = 0;
	String plotName = "";
	byte firstnumber = 0;
	int secondNumber = 0;
	int thirdNumber = 0;
	byte firstPartOfnumber = 0;
	byte secondPartOfnumber = 0;



	/*	 NAME (10 bytes)	*/
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)(eAddress >> 8));
	delay(6);
	Wire.write((int)(eAddress & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 10);	/*	 Ask for Plot Name.	*/
	delay(1);

	while (Wire.available()) {

		byte c = Wire.read();
		delay(1);
		memWord[byteCount] = char(c);
		byteCount++;
	}
	plotName = memWord;

	Serial.print(plotName);
	Serial.print("     \t");
	Serial.flush();

	/*	 FIRST NUMBER (1 byte)	*/
	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)((eAddress + 10) >> 8));
	delay(6);
	Wire.write((int)((eAddress + 10) & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 1);	/*	 Ask for first number.	*/
	delay(1);

	while (Wire.available()) {

		firstnumber = Wire.read();
		delay(1);
	}

	Serial.print(firstnumber);
	Serial.print("\t");
	Serial.flush();

	/*	 SECOND NUMBER (2 bytes)	*/
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)((eAddress + 11) >> 8));
	delay(6);
	Wire.write((int)((eAddress + 11) & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 2);	/*	 Ask for second number.	*/
	delay(1);

	while (Wire.available()) {

		firstPartOfnumber = Wire.read();
		delay(1);
		secondPartOfnumber = Wire.read();
		delay(1);
	}

	secondNumber = (firstPartOfnumber << 8) + secondPartOfnumber;

	Serial.print(secondNumber);
	Serial.print("\t");
	Serial.flush();

	/*	 THIRD NUMBER (2 bytes)	*/
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)((eAddress + 13) >> 8));
	delay(6);
	Wire.write((int)((eAddress + 13) & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 2);	/*	 Ask for third number.	*/
	delay(1);

	while (Wire.available()) {

		firstPartOfnumber = Wire.read();
		delay(1);
		secondPartOfnumber = Wire.read();
		delay(1);
	}

	thirdNumber = (firstPartOfnumber << 8) + secondPartOfnumber;

	Serial.print(thirdNumber);
	Serial.println();
	Serial.flush();

}
char readPlotCharectors(int eAddress)
{
	byte byteCount = 0;
	String plotName = "";
	byte firstnumber = 0;
	int secondNumber = 0;
	int thirdNumber = 0;
	byte firstPartOfnumber = 0;
	byte secondPartOfnumber = 0;



	/*	 CHARECTOR	*/
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)(eAddress >> 8));
	delay(6);
	Wire.write((int)(eAddress & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 1);	/*	 Ask for Plot Name.	*/
	delay(1);

	while (Wire.available()) {

		byte c = Wire.read();
		delay(1);
		memWord[byteCount] = char(c);
		byteCount++;
	}
	plotName = memWord;

	Serial.print(plotName);
	Serial.print("     \t");
	Serial.flush();

	/*	 FIRST NUMBER	*/
	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)((eAddress + 1) >> 8));
	delay(6);
	Wire.write((int)((eAddress + 1) & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 1);	/*	 Ask for first number.	*/
	delay(1);

	while (Wire.available()) {

		firstnumber = Wire.read();
		delay(1);
	}

	Serial.print(firstnumber);
	Serial.print("\t");
	Serial.flush();

	/*	 SECOND NUMBER	*/
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)((eAddress + 2) >> 8));
	delay(6);
	Wire.write((int)((eAddress + 2) & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 2);	/*	 Ask for second number.	*/
	delay(1);

	while (Wire.available()) {

		firstPartOfnumber = Wire.read();
		delay(1);
		secondPartOfnumber = Wire.read();
		delay(1);
	}

	secondNumber = (firstPartOfnumber << 8) + secondPartOfnumber;

	Serial.print(secondNumber);
	Serial.print("\t");
	Serial.flush();

	/*	 THIRD NUMBER	*/
	for (byte i = 0; i < MAX_I2C_PAGE_WRITE; i++) { memWord[i] = 0; }	/*	 Clear memWord.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)((eAddress + 4) >> 8));
	delay(6);
	Wire.write((int)((eAddress + 4) & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 2);	/*	Ask for third number.	*/
	delay(1);

	while (Wire.available()) {

		firstPartOfnumber = Wire.read();
		delay(1);
		secondPartOfnumber = Wire.read();
		delay(1);
	}

	thirdNumber = (firstPartOfnumber << 8) + secondPartOfnumber;

	Serial.print(thirdNumber);
	Serial.println();
	Serial.flush();

}
char readCompressed(int eAddress) {

	for (byte i = 0; i < 3; i++) { compData[i] = 0; }	/*	Clear compData.	*/

	Wire.beginTransmission(EEPROM_ADD);
	delay(1);
	Wire.write((int)(eAddress >> 8));
	delay(6);
	Wire.write((int)(eAddress & 0xFF));
	delay(6);
	Wire.endTransmission();
	delay(1);

	Wire.requestFrom(EEPROM_ADD, 3);	/*	Ask for a compressed command.	*/
	delay(1);

	byte byteCount = 0;

	while (Wire.available()) {

		byte c = Wire.read();
		delay(1);

		compData[byteCount] = c;	/*	Put 3 bytes in compressed data array.	*/
		byteCount++;
	}

}

/*	MEMORY CLEAR (Start address, Number of Bytes)	*/
void ClearMemory(int memoryStart, int memoryLength) {
	Serial.print("Mem Clear Start at: ");
	Serial.print(memoryStart);
	Serial.println();
	Serial.flush();

	int _Line_Length = 32;
	int _Bit_count = 0;
	int thisAddress = memoryStart;

	/*	WRITE	*/
	for (size_t i = memoryStart; i < memoryStart + memoryLength; i++) {

		Wire.beginTransmission(EEPROM_ADD);
		delay(1);
		Wire.write((int)(thisAddress >> 8));
		delay(6);
		Wire.write((int)(thisAddress & 0xFF));
		delay(6);


		Wire.write(0);
		delay(6);

		Wire.endTransmission();
		delay(1);
		Serial.print(0);
		Serial.flush();

		thisAddress++;
		_Bit_count++;
		if (_Bit_count >= 32) {
			_Bit_count = 0;
			Serial.println();
		}
	}
	Serial.println();
	Serial.println("Mem Clear End");
	Serial.print("Current Address = ");
	Serial.println(thisAddress);
	Serial.flush();

}
/*	Memory Start Address, Lenth of Memory to Read, Size of Page Read	*/
void ReadMemory(int memoryStart, int memoryLength) {

	Serial.print("Mem Read Start at: ");
	Serial.print(memoryStart);
	Serial.println();
	Serial.flush();

	int _Line_Length = 32;
	int _Bit_count = 0;
	int thisAddress = memoryStart;

	for (size_t i = memoryStart; i < memoryStart + memoryLength; i++) {

		Wire.beginTransmission(EEPROM_ADD);
		delay(1);
		Wire.write((int)(thisAddress >> 8));
		delay(6);
		Wire.write((int)(thisAddress & 0xFF));
		delay(6);
		Wire.endTransmission();


		Wire.requestFrom(EEPROM_ADD, 1);
		delay(1);

		if (_Bit_count == 0) {
			Serial.print("Start Address = ");
			Serial.println(i);
			delay(40);
		}

		while (Wire.available())	/*	Slave may send less than requested.	*/
		{
			byte c = Wire.read();
			delay(1);
			Serial.print(c, DEC);
			if (_Bit_count < _Line_Length - 1) {
				Serial.print(", ");
				delay(40);

			}
		}

		thisAddress++;
		_Bit_count++;
		if (_Bit_count >= 32) {
			_Bit_count = 0;
			Serial.println();
		}
	}
	Serial.println();
	Serial.println("Mem Read End");
	Serial.print("Current Address = ");
	Serial.println(thisAddress);
	Serial.flush();



}
/*	Memory Start Address, Lenth of Memory to Read, Size of Page Read	*/
void MemTest32KB(int memoryStart, int memoryLength, int pageSize) {
	Serial.println("Mem Test Start");	/*	 start	*/
	Serial.flush();

	int count = 0;
	byte bitCount = 0;
	//int memoryStart = 1000;
	//int memoryLength = 500;
	//int pageSize = 25;

	/*	WRITE	*/
	for (size_t i = memoryStart; i < memoryStart + memoryLength; i++) {

		Serial.print("Writing Byte ");
		Serial.print(i);
		Serial.print(" = ");
		Serial.println(bitCount);
		Serial.flush();

		Wire.beginTransmission(EEPROM_ADD);
		delay(1);
		Wire.write((int)(i >> 8));
		delay(6);
		Wire.write((int)(i & 0xFF));
		delay(6);

		Wire.write(bitCount);
		delay(6);

		Wire.endTransmission();
		delay(1);

		bitCount++;
	}

	/*	READ	*/
	for (size_t i = memoryStart; i < memoryStart + memoryLength; i += pageSize) {

		Wire.beginTransmission(EEPROM_ADD);
		delay(1);
		Wire.write((int)(i >> 8));
		delay(6);
		Wire.write((int)(i & 0xFF));
		delay(6);
		Wire.endTransmission();
		delay(1);

		Wire.requestFrom(EEPROM_ADD, pageSize);	/*	Ask for a length of data.	*/
		delay(1);

		Serial.print("Start Address = ");
		Serial.println(i);
		delay(50);

		byte byteCount = 0;

		while (Wire.available())	/*	Slave may send less than requested.	*/
		{
			byte c = Wire.read();
			//char c = Wire.read();
			delay(1);
			Serial.print(c, DEC);
			//Serial.print(c);
			Serial.print("\t");
			delay(50);

			memWord[byteCount] = char(c);
			byteCount++;
		}

		Serial.println();
		Serial.println(memWord);
		delay(50);

	}

	Serial.println("Mem Test End");/*	 End	*/
	Serial.flush();

}



/*
Menu:
1 MAIN                                              readWord(1, 0, 4);
2 MANUAL DRAW                                       readWord(1, 4, 11);
3 CHOOSE PLOT                                       readWord(1, 15, 11);
4 SETTINGS                                          readWord(1, 26, 8);
5 INFO                                              readWord(1, 34, 4);
6 RESET                                             readWord(1, 38, 5);
7 MOVE X                                            readWord(1, 43, 6);
8 DRAW X                                            readWord(1, 49, 6);
9 MOVE Y                                            readWord(1, 55, 6);
10 DRAW Y                                           readWord(1, 61, 6);
11 <<== BACK                                        readWord(1, 67, 9);
12 SPEED                                            readWord(1, 76, 5);
13 TOOL                                             readWord(1, 81, 4);
14 --                                               readWord(1, 85, 2);
15 Tim's Mini Plotter                               readWord(1, 87, 18);
16 ok                                               readWord(1, 105, 2);
17 === Motors Off ===                               readWord(1, 107, 18);
18 Version:                                         readWord(1, 125, 9);
19 Basic Gcode Commands                             readWord(1, 134, 20);
20 M03 - Tool On/Down                               readWord(1, 154, 18);
21 M05 - Tool Off/Up                                readWord(1, 172, 17);
22 M18 - Motors Off                                 readWord(1, 189, 16);
23 System Reset                                     readWord(1, 205, 12);
24 M100 - this Info message                         readWord(1, 217, 24);
25 R01 - Reset Plotter                              readWord(1, 241, 19);
26 By Tim Jackson.1960                              readWord(1, 260, 19);
27 ================                                 readWord(1, 279, 16);
28													readWord(1, 295, 10);
29 TYPE OF PLOT										readWord(1, 305, 12);
30 OUTLINE											readWord(1, 317, 7);
31 TEXT												readWord(1, 324, 4);
32 CHOOSE OUTLINE									readWord(1, 328, 14);
33 A-Z												readWord(1, 342, 3);
34 a-z												readWord(1, 345, 3);
35 0-9												readWord(1, 348, 3);
36 SYMBOLS											readWord(1, 351, 7);
37 CHOOSE A-Z										readWord(1, 358, 10);
38 CHOOSE a-z										readWord(1, 368, 10);
39 CHOOSE 0-9										readWord(1, 378, 10);
40 CHOOSE SYMBOLS									readWord(1, 388, 14);
41 CHOOSE CHARACTER									readWord(1, 402, 16);
42 EndEND											readWord(1, 418, 6);



*/

