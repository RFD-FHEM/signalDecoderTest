/*
*   Example Sketch for testing the signalDecoder lib and some sublibs
*   Copyright (C) 2014  .Butzek
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   This Sketch will only run complete on a Arduino Mega or in a Simulator which has enouth ram
*/
#define UNITTEST 1


#include <ArduinoUnit.h>
#define PROGNAME               "signalDecoderTest"
#define PROGVERS               "0.1"
//#define DEBUGDETECT 4
//#define DEBUGDECODE 2
#define BAUDRATE               115200
#define DEBUG				   1

#include "output.h"
#include "FastDelegate.h"
#include "signalDecoder.h"
#include "bitstore.h"


//#define B12;

//Decoder
SignalDetectorClass  ooDecode;
ManchesterpatternDecoder mcdecoder(&ooDecode);			// Init Manchester Decoder class

testing(basic_tol)
{
	assertFalse(ooDecode.inTol(-1061, 908, 212));
	assertTrue(ooDecode.inTol(1061, 908, 212));
	
	pass();
}

testing(basic_findpatt)
{
	ooDecode.pattern[0] = 908;
	ooDecode.patternLen = 1;
	int8_t idx =ooDecode.findpatt(-1061);

	assertLess(idx,0);

	idx = ooDecode.findpatt(900);
	assertMoreOrEqual(idx, 0);

	pass();
}





testing(basic_samesign)
{
	bool state;
	uint16_t i = 0;

	int16_t pulse = -500;


	for (i=0; i < 20; i++)
	{
		state = ooDecode.decode(&pulse);
	}
	pulse = -32001;
	for (i = 0; i < 10; i++)
	{
		state = ooDecode.decode(&pulse);
	}
	pulse = 1000;
	state = ooDecode.decode(&pulse);
	//ooDecode.printOut();


	assertEqual(ooDecode.patternLen, 2);
	assertFalse(state);
	assertEqual(ooDecode.messageLen, 2);
	
	pass();

}



testing(speed_calibrate)
{
	long now = micros();
	yield();
	long after = micros();
	assertLessOrEqual(after - now, 4);
	pass();
}


testing(speed_findpatt)
{
	ooDecode.reset();
	ooDecode.pattern[0] = 100;
	ooDecode.pattern[1] = -100;
	ooDecode.pattern[2] = -908;
	ooDecode.pattern[3] = 908;

	ooDecode.patternLen = 4;


	long now = micros();
	int idx = ooDecode.findpatt(900);	
	long after = micros();
	assertLessOrEqual(after - now, 32);

	pass();
}


testing(speed_addData)
{
	ooDecode.reset();


	long now = micros();
	ooDecode.addData(1);
	long after = micros();
	assertLessOrEqual(after - now, 12);

	pass();
}


testing(speed_decode)
{
	ooDecode.reset();
	int pulse = 500;
	
	unsigned long now = micros();
	bool state=ooDecode.decode(&pulse);
	unsigned long after = micros();
	assertLessOrEqual(after - now, 44);

	pulse = -pulse;
	now = micros();
	state = ooDecode.decode(&pulse);
	after = micros();
	assertLessOrEqual(after - now, 45);

	now = micros();
	ooDecode.calcHisto();
	after = micros();
	assertLessOrEqual(after - now, 40);





	pass();
}


testing(speed_calcHisto)
{
	ooDecode.reset();
	int pulse = 500;

	for (uint8_t i = 0; i < 127; i++)
	{
		unsigned long now = micros();
		ooDecode.calcHisto();
		unsigned long after = micros();

		assertLessOrEqual(after - now, 19*(i+1));

		DigitalSimulate(-(pulse));
		if (i % 2)
			DigitalSimulate(pulse * 2);
		else
			DigitalSimulate(pulse * 4);
	}


	unsigned long now = micros();
	ooDecode.calcHisto();
	unsigned long after = micros();
	assertLessOrEqual(after - now, 2196);

	pass();
}

testing(speed_processMessage)
{
	ooDecode.reset();
	int pulse = 500;

	for (uint8_t i = 0; i < 127; i++)
	{
		DigitalSimulate(-(pulse));
		if (i % 2)
			DigitalSimulate(pulse * 2);
		else
			DigitalSimulate(pulse * 4);
	}

	int endpulse = -32001;

	unsigned long now = micros();
	state = ooDecode.decode(&endpulse);
	unsigned long after = micros();
	assertLessOrEqual(after - now, 24600);
	

	pass();
}

testing(speed_compressPattern)
{
	ooDecode.reset();
	int pulse = 500;

	for (uint8_t i = 0; i < 127; i++)
	{
		DigitalSimulate(-(pulse));
		if (i % 2)
			DigitalSimulate(pulse * 2);
		else
			DigitalSimulate(pulse * 4);
	}
	ooDecode.pattern[2] = 600; 


	unsigned long now = micros();
	ooDecode.compress_pattern();
	unsigned long after = micros();
	assertLessOrEqual(after - now, 2260);

	pass();
}

testing(mc_decode_osv2)
{
	bool state;
	int pData[6] = {
		2000,392,908,-1061,-569,-32000
	};

	uint8_t s_Stream[200] = {
		0, 1, 1, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 4, 1, 3, 1, 4, 2, 4, 1, 3, 1, 4, 2, 4, 1, 3, 1, 4, 2, 3, 2, 3, 2, 4, 1, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 1, 4, 2, 4, 1, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 4, 1, 3, 1, 4, 2, 3, 2, 3, 2, 4, 1, 3, 2, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 1, 4, 2,
	};

	uint16_t len = sizeof(s_Stream) / sizeof(s_Stream[0]);

	uint16_t i = 0;
	*ooDecode.last = pData[s_Stream[i]];  // Fill in at last

	for (i++; i < len; i++)
	{
		state = ooDecode.decode(&pData[s_Stream[i]]);
	}
	//state = ooDecode.decode(&pData[5]);

	
	assertEqual(ooDecode.patternLen, 4);
	assertFalse(state);
	assertEqual(ooDecode.message[0], 0);
	assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);
	
	pass();
	
	//assertTrue(state);
}

testing(mc_isManchester_osv2)
{
	if checkTestDone(mc_decode_osv2) {
		//assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		pass();
	}
}

testing(mc_doDecode_osv2)
{
	if checkTestDone(mc_isManchester_osv2) {
		bool result = mcdecoder.doDecode();
#ifndef B12
		assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
#endif
		assertTrue(result);
		assertEqual(mcdecoder.ManchesterBits.bytecount, 20);  // 0-20
		assertEqual(mcdecoder.ManchesterBits.valcount, 167);
		pass();
	}
}

testing(mc_getHexStr_osv2)
{
	if checkTestDone(mc_doDecode_osv2) {

		String mcStr;
		mcdecoder.getMessageHexStr(&mcStr);

		String base;
		base = "AAAAAAAA666A5A595595555A556996555566A565A6"; // "AAAAAAAB332B4B4D54D5554B552CD3555532B534B2";
		assertEqual(mcStr, base); // may not compile or give warning
		pass();
	}
}

testing(mc_MCBits)
{

	if checkTestDone(mc_getHexStr_osv2) {

		assertEqual(mcdecoder.ManchesterBits.bytecount, 20);
		assertEqual(mcdecoder.ManchesterBits.valcount, 167);
		pass();
	}
}

testing(mc_doDecode_osv2_append)
{
	if (checkTestDone(mc_MCBits))
	{
		ooDecode.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;
		bool state;
		int pData[6] = {
			2000,392,908,-1061,-569,-32000
		};

		uint8_t s_Stream[200] = {
			3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 4, 1, 3, 1, 4, 2, 4, 1, 3, 1, 4, 2, 4, 1, 3, 1, 4, 2, 3, 2, 3, 2, 4, 1, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 1, 4, 2, 4, 1, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 1, 4, 2, 4, 1, 3, 1, 4, 2, 3, 2, 3, 2, 4, 1, 3, 2, 3, 2, 3, 1, 4, 2, 4, 1, 3, 2, 3, 1, 4, 2, 3, 2, 4, 1, 3, 1, 4, 2,
		};

		uint16_t len = sizeof(s_Stream) / sizeof(s_Stream[0]);
		uint16_t i = 0;

		for (uint8_t j = 1; j < 3; j++)
		{
			i = 0;
			for (; i < len; i++)
			{
				state = ooDecode.decode(&pData[s_Stream[i]]);
			}
			//state = ooDecode.decode(&pData[5]);
		}

		//ooDecode.printOut();

		assertEqual(ooDecode.patternLen, 4);
		assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen -1]], pData[s_Stream[i - 1]]);
		state = ooDecode.decode(&pData[5]);
		assertFalse(state);

		const int pulse = -4000;
		state = ooDecode.decode(&pulse);

		assertTrue(state);
		ooDecode.reset();
		
		
		/*
		for (uint8_t j = 1; j < 3; j++)
		{
			i = 0;
			for (; i < len; i++)
			{
				state = ooDecode.decode(&pData[s_Stream[i]]);
			}
		}
		state = ooDecode.decode(&pData[5]);
		assertEqual(ooDecode.patternLen, 5);
		assertFalse(state);

		state = ooDecode.decode(&pulse);
		assertTrue(state);
		ooDecode.reset();

		*/



		pass();
	}
}

testing(mc_finished)
{
	if (checkTestDone(mc_doDecode_osv2_append))
		pass();
}

testing(ms_dodecode_NCWS)
{
	if (checkTestDone(mc_finished))
	{
		bool state;
		
		//s5FA80C43C000
	    //	   MS; P0 = -3886; P1 = 481; P2 = -1938; P3 = -9200; D = 13121012101010101010121012101212121212121210101212121012121212101010101212; CP = 1; SP = 3; O; 
		ooDecode.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;


		int pData[] = {
			-9200,481,200,-400,-1938,-3886
		};

		uint8_t s_Stream[] = {
			1,0,1,4,1,5,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,4,1,5,1,4,1,5,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,4,1,4,1,4,1,5,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,4,1,4, 
		};

		uint16_t len = sizeof(s_Stream) / sizeof(s_Stream[0]);

		uint16_t i = 0;
		bool decoded;
		for (uint8_t j = 1 , i = 5; j < 5; j++)
		{
			
			for (; i < len; i++)
			{
				state = ooDecode.decode(&pData[s_Stream[i]]);
				if (state)
					decoded = true;
			}
			if (j < 2) {
				assertEqual(ooDecode.patternLen, 3);
				assertFalse(state);
			}
			i = 0;
		}
		//assertTrue(decoded);

		//ooDecode.printOut();
		assertFalse(state);
		pass();
	}
}

testing(mu_dodecode_TX3)
{
	if (checkTestDone(ms_dodecode_NCWS))
	{
		bool state;

		//	TXAE07540540
		//	MU; P0 = -27698; P1 = -180; P2 = 1274; P3 = -1037; P4 = 505; D = 12323232343234323434343232323232323434343234323432343232323232323234323432343232323232340232323234323432343434323232323232343434323432343234323232323232323432343234323232323234023232323432343234343432323232323234343432343234323432323232323232343234323432; CP = 4; O;
		//  MU;P0=1274;P1=-1037;P2=505;P3=-27698;                          D=0101010121012101212121010101010101212121012101210121010101010101012101210121010101010123010101012101210121212101010101010121212101210121012101010101010101210121012101010101012301010101210121012121210101010101012121210121012101210101010101010121012101210;CP=2;
		ooDecode.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;


		int pData[] = {
			-27698,180,1274,-1037,505
		};

		uint8_t s_Stream[] = {
			1,2,3,2,3,2,3,2,3,4,3,2,3,4,3,2,3,4,3,4,3,4,3,2,3,2,3,2,3,2,3,2,3,2,3,4,3,4,3,4,3,2,3,4,3,2,3,4,3,2,3,4,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,4,3,2,3,4,3,2,3,4,3,2,3,2,3,2,3,2,3,2,3,4,0,2,3,2,3,2,3,2,3,4,3,2,3,4,3,2,3,4,3,4,3,4,3,2,3,2,3,2,3,2,3,2,3,2,3,4,3,4,3,4,3,2,3,4,3,2,3,4,3,2,3,4,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,4,3,2,3,4,3,2,3,4,3,2,3,2,3,2,3,2,3,2,3,4,0,2,3,2,3,2,3,2,3,4,3,2,3,4,3,2,3,4,3,4,3,4,3,2,3,2,3,2,3,2,3,2,3,2,3,4,3,4,3,4,3,2,3,4,3,2,3,4,3,2,3,4,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,4,3,2,3,4,3,2,3,4,3,2, 
		};

		uint16_t len = sizeof(s_Stream) / sizeof(s_Stream[0]);

		uint16_t i = 0;

		for (uint8_t j = 1, i = 5; j < 5; j++)
		{

			for (; i < len; i++)
			{
				state = ooDecode.decode(&pData[s_Stream[i]]);
			}
			if (j < 2) {
				assertEqual(ooDecode.patternLen, 4);
				assertFalse(state);
			}
			i = 0;
		}

		//ooDecode.printOut();

		//assertEqual(ooDecode.message[0], 0);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);

		pass();

		//assertTrue(state);
	}
}

testing(mc_long_1)
{
	if (checkTestDone(mu_dodecode_TX3))
	{

		bool state;
		ooDecode.reset();
		mcdecoder.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		// Ein paar Pule um das Signal zu füllen
		/*
		DigitalSimulate(10000);
		DigitalSimulate(-10000);
		DigitalSimulate(10000);
		DigitalSimulate(-10000); */

		String dstr2(F("0B0F9FFA555AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAA"));
		state = import_mcdata(&dstr2, 0, dstr2.length(), 450);
		//ooDecode.printOut();
		assertFalse(mcdecoder.isManchester());
		assertEqual(253, ooDecode.messageLen);

		ooDecode.calcHisto();
		ooDecode.printOut();

		Serial.println("part one");
		assertTrue(mcdecoder.isManchester());
		assertFalse(state);

		bool result = mcdecoder.doDecode();
		assertEqual(226, mcdecoder.ManchesterBits.valcount-1);
		assertEqual(253, ooDecode.messageLen);
		assertFalse(mcdecoder.pdec->mcDetected);
		assertTrue(result);
	
		String mcStr;
		String base;
		mcdecoder.getMessageHexStr(&mcStr);
		base = "0B0F9FFA555AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAA";  //56 Hex digits (56*4=224)
		assertEqual(mcStr, base); // may not compile or give warning
		//mcdecoder.reset();
		assertEqual(1, mcdecoder.ManchesterBits[220]);
		assertEqual(0, mcdecoder.ManchesterBits.getValue(221));
		assertEqual(1, mcdecoder.ManchesterBits.getValue(222));
		Serial.println("part two ");


		// Part two funktioniert nicht, da der mcdecoder die Daten entfernt
		/*
		state = import_mcdata(&dstr2, 0, 10, 450);
		assertEqual(0, mcdecoder.ManchesterBits.getValue(223));

		dstr2 = "";
		ooDecode.calcHisto();
		assertTrue(mcdecoder.isManchester());
		assertEqual(59, ooDecode.messageLen);
		DigitalSimulate(10000);
		ooDecode.printOut();

		assertEqual(60, ooDecode.messageLen);

		result = mcdecoder.doDecode();

		assertEqual(262, mcdecoder.ManchesterBits.valcount-1);

		mcStr = "";
		mcdecoder.getMessageHexStr(&mcStr);
		
		base = "0B0F1FFCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAA0B0F1FFCAA";
		assertEqual(mcStr, base); // may not compile or give warning

		*/
		pass();
	}
	//assertTrue(state);
}


testing(mc_long_2) //Maverick et733
{
	if (checkTestDone(mc_long_1))
	{
		assertFalse(ooDecode.inTol(-500, 5000, 550));



		bool state;
		ooDecode.reset();
		mcdecoder.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;
		const int pause = 5000;
		const int pulse = -230;
	                   	
		String dstr2(F("AA9995595555595999A9A9A669"));
		for (uint8_t r = 0; r < 4; r++)
		{
			Serial.print("repeat="); Serial.print(r); Serial.println(">");
			for (uint8_t i = 0; i < 8; i++)
			{
				DigitalSimulate(pause);
				DigitalSimulate(pulse);
			}
			DigitalSimulate(pause);

			state = import_mcdata(&dstr2, 0, dstr2.length(), 250);
			ooDecode.printOut();
			Serial.print("<repeat="); Serial.print(r); Serial.println(";");

		}



		ooDecode.calcHisto(0,1);
		assertFalse(mcdecoder.isManchester());
		//assertEqual(130,ooDecode.messageLen);

		ooDecode.calcHisto();
		ooDecode.printOut();

		Serial.println("import done");
		assertTrue(mcdecoder.isManchester());

		assertFalse(state);

		bool result = mcdecoder.doDecode();
		assertEqual(103, mcdecoder.ManchesterBits.valcount);
		assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
		//assertFalse(result);




		String mcStr;
		String base;


		mcdecoder.getMessageHexStr(&mcStr);
		base = "AA9995595555595999A9A9A668";  // eigentlich AA9995595555595999A9A9A669, aber es fehlt der letzte Pulse
		assertEqual(mcStr, base); // may not compile or give warning



		pass();
	}
	//assertTrue(state);
}

testing(mc_maverick_1)
{
	
	if (checkTestDone(mc_long_2))
	{
		ooDecode.reset();
		mcdecoder.reset();
		
	   String dstr(F("MU;P0=-4913;P1=228;P2=361;P3=-632;P4=-382;P5=153;P6=106;D=0101010101010101023232323245354245354245323232323542463232323232323232323236424;"));
	   import_sigdata(&dstr);
	   ooDecode.compress_pattern();
	   ooDecode.printOut();
	   pass();
	}

}

bool DigitalSimulate(const int pulse)
{
	bool state = false;
	static int duration = 0;
	if (duration != 0 && (pulse ^ duration) < 0) // true if a and b have opposite signs
	{
		state = ooDecode.decode(&duration);
		duration = 0;
	} 
	duration += pulse;
	return state;
}

bool import_mcdata(String *cmdstring, const uint8_t startpos, const uint8_t endpos, const int16_t clock)
{
	int8_t b;
	char c;
	uint8_t bit;
	bool state =false;
	unsigned long stoptime = micros();
	int pwidth = 0;
	
	for (uint8_t i = startpos; i < endpos; i++) {
		c = cmdstring->charAt(i);
		b = ((byte)c) - (c <= '9' ? 0x30 : 0x37);

		for (bit = 0x8; bit>0; bit >>= 1) {
			for (byte i = 0; i <= 1; i++) {
				if ((i == 0 ? (b & bit) : !(b & bit))) {
					pwidth = -1*clock;
				}
				else {
					pwidth = clock;
				}
				state = DigitalSimulate(pwidth);
			}

		}

	}

	return state;
	// MSG_PRINTLN("");
}


bool import_sigdata( String *cmdstring)
{

	String msg_part;
	int16_t startpos = 0;
	int16_t endpos = 0;
	int buckets[8] = {};
	bool state;
	while (true)
	{
		startpos = endpos + 1;    // Set startpos to endpos to extract next part
		endpos = cmdstring->indexOf(";", startpos);     			 // search next   ";"
		if (endpos == -1 || startpos == -1) break;


		if (cmdstring->charAt(startpos) == 'P' && cmdstring->charAt(startpos+2) == '=') // Do some basic detection if data matches what we expect
		{
			uint8_t counter = cmdstring->substring(startpos+1, startpos+2).toInt(); // extract the pattern number
			buckets[counter] = cmdstring->substring(startpos+3,endpos).toInt();

		}
		else if (cmdstring->charAt(startpos) == 'D') {
			for (int i=startpos+2; i < endpos; i++)
			{
				//state = DigitalSimulate(buckets[cmdstring->substring(i, i + 1).toInt()]);
				state = ooDecode.decode(&buckets[cmdstring->substring(i, i + 1).toInt()]);
			}

		}
		
	}
	return state;


}

void random_import()
{
	//Random Data
	String dstr(F("MU;P0=-2096;P1=4020;P2=-5081;P3=-10090;P4=887;D=012431;"));

	import_sigdata(&dstr);
	dstr = "";

}

testing(mc_osv3_a)
{
	if (checkTestDone(mu_dodecode_TX3))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();

		String dstr(F("MU;P0=-296;P1=420;P2=-581;P3=-1090;P4=887;D=01212121212121212121212121212121212121212134343431243124213121212124312121212124212121312431212121212121212121212121212121212121242131212121;"));
		state = import_sigdata(&dstr);
		dstr="";
		
		
		//state = ooDecode.decode(&pData[5]);
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		//ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);


		bool result = mcdecoder.doDecode();
#ifndef B12
		assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
#endif
		assertTrue(result);
		assertEqual(mcdecoder.ManchesterBits.bytecount, 9);  
		assertEqual(mcdecoder.ManchesterBits.valcount, 79);
	
		String mcStr;
		String base;


		mcdecoder.getMessageHexStr(&mcStr);
		base = "0000055B3EFC37FFFF9E";
		assertEqual(mcStr, base); // may not compile or give warning
		
		
		mcStr = "";
		mcdecoder.getMessagePulseStr(&mcStr);
		base = "LL=-1090;LH=887;SL=-581;SH=420;";
		assertEqual(mcStr, base); // may not compile or give warning

		mcStr = "";
		mcdecoder.getMessageLenStr(&mcStr);
		base = "L=79;";
		assertEqual(mcStr, base); // may not compile or give warning

		pass();

	}
}

testing(mc_osv3_b)
{
	if (checkTestDone(mu_dodecode_TX3))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();

		String dstr2(F("MU;P0=-542;P1=434;P2=-1040;P3=911;D=01010101010101010101012323210103012101010103012101010101010321010323210301210101030101210101032301012101010301010101010101010101232321010301210101010301210101010101032101032321030121010103010121010103230101210101030101010101010101010123232101030121010101;"));
		state = import_sigdata(&dstr2);
		//ooDecode.printOut();
		dstr2="";


		//state = ooDecode.decode(&pData[5]);
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);

		bool result = mcdecoder.doDecode();
#ifndef B12
		assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertTrue(mcdecoder.pdec->mcDetected);
#endif
		//assertTrue(result);
		assertEqual(mcdecoder.ManchesterBits.bytecount, 19);
		assertEqual(mcdecoder.ManchesterBits.valcount, 155);

		String mcStr;
		String base;


		mcdecoder.getMessageHexStr(&mcStr);
		base = "000AE7CFEEB3C7A3C00573E7F759E3D1E002B9E";
		assertEqual(mcStr, base); // may not compile or give warning


		mcStr = "";
		mcdecoder.getMessagePulseStr(&mcStr);
		base = "LL=-1040;LH=911;SL=-542;SH=434;";
		assertEqual(mcStr, base); // may not compile or give warning
		
		mcStr = "";
		mcdecoder.getMessageLenStr(&mcStr);
		base = "L=155;";
		assertEqual(mcStr, base); // may not compile or give warning
		
		pass();

	}
}

testing(mc_osv3_c)
{
	if (checkTestDone(mu_dodecode_TX3))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();

		String dstr2(F("MU;P0=-1198;P1=1676;P2=-541;P3=433;P4=-160;P5=124;P7=905;D=012345432323232323232323232323232323232323232323070723232323032327070323232707032327230323272323230323272323032723032323232323232323232327032703270327230703270323232707032;CP=3;"));
		state = import_sigdata(&dstr2);
		//ooDecode.printOut();
		dstr2 = "";


		//state = ooDecode.decode(&pData[5]);
		assertFalse(mcdecoder.isManchester());
		mcdecoder.reset();

		ooDecode.calcHisto();
		ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);

		bool result = mcdecoder.doDecode();
#ifndef B12
		assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
#endif
		assertTrue(result);
		String mcStr;
		String base;


		mcdecoder.getMessageHexStr(&mcStr);
		base = "000005075EB9C38CFFF6D96F5";
		assertEqual(mcStr, base); // may not compile or give warning


		mcStr = "";
		mcdecoder.getMessagePulseStr(&mcStr);
		base = "LL=-1198;LH=905;SL=-541;SH=433;";
		assertEqual(mcStr, base); // may not compile or give warning

		mcStr = "";
		mcdecoder.getMessageLenStr(&mcStr);
		base = "L=100;";
		assertEqual(mcStr, base); // may not compile or give warning

		pass();

	}
}

testing(mc_somfy_a)
{
	if (checkTestDone(mu_dodecode_TX3))
	{
		bool state;
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		ooDecode.reset();
		mcdecoder.reset();
		
		

		int s_Data[] = {
			 2608, -2552, 2592, -2572, 2432, -2572, 2584, -2572, 2580, -2584, 2572, -2576, 2576, -2584, 4844, -1224, 1364, -1224, 1204, -756, 604, -616, 592, -760, 608, -620, 592, -1376, 1204, -1220, 756, -624, 1192, -608, 760, -604, 604, -616, 600, -1368, 1204, -1368, 600, -628, 1200, -772, 584, -612, 604, -616, 748, -1220, 1208, -1372, 604, -604, 1364, -608, 604, -628, 584, -760, 612, -612, 592, -768, 608, -1212, 1352, -1236, 1200, -1360, 1216, -620, 748, -612, 600, -608, 604, -1360, 1212, -612, 764, -1208, 1208, -768, 604, -608, 596, -628, 736, -624, 588, -624, 592, -760, 608, -608, 600, -620, 740, -32000,
			//-2304, 2640, -2532, 4856, -1412, 1156, -1428, 1148, -712, -836, 304, -672, 548, -856, 348, -1340, 1252, -1308, 512, -640, 1324, -688, 524, -700, 668, -692, 528, -1264, 1300, -1284, 540, -820, 1156, -644, 580, -800, 552, -652, 568, -1424, 1148, -1284, 696, -656, 1164, -644, 720, -656, 556, -664, 552, -824, 540, -636, 584, -1388, 1176, -1416, 1172, -1236, 1340, -636, 572, -656, 708, -644, 572, -1252, 1328, -644, 572, -1388, 1184, -648, 568, -784, 580, -652, 564, -636, 728, -648, 568, -656, 552, -804, 564, -648, 556, -32001

		};

		uint16_t len = sizeof(s_Data) / sizeof(s_Data[0]);

		uint16_t i = 0;

		for (uint8_t j = 1, i = 0; j < 5; j++)
		{

			for (; i < len; i++)
			{
				state = ooDecode.decode(&s_Data[i]);
			}
			if (j < 2) {
				//assertEqual(ooDecode.patternLen, 4);
				//assertFalse(state);
			}
			i = 0;
		}


		//state = ooDecode.decode(&pData[5]);
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.compress_pattern();
		ooDecode.printOut();
		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);

		pass();

	}
}


testing(mu_decode_dooya)
{
	if (checkTestDone(ms_dodecode_NCWS))
	{
		bool state;
		// MU;P0=-776;P1=323;P2=679;P3=-417;P4=-10548;P5=4904;P6=-1636;D=01023232310101023101010101010101023102323101010232310102310231010102310101024561010102323231010102310101010101010102310232310101023231010231023101010231010102456101010232323101010231010101010101010231023231010102323101023102310101023101010245610101023232;CP=1;O;
		
		ooDecode.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		/*

		*/


		int16_t signal_raw[] = {
			 4904, -1636, 324, -772, 324, -772, 324, -780, 676, -412, 680, -416, 680, -416, 316, -776, 320, -800, 324, -772, 680, -416, 320, -772, 324, -772, 324, -772, 320, -780, 320, -772, 324, -800, 320, -772, 320, -780, 680, -412, 320, -776, 680, -412, 680, -416, 320, -776, 320, -800, 320, -776, 680, -416, 676, -412, 324, -776, 320, -780, 672, -420, 316, -776, 680, -440, 320, -772, 324, -772, 320, -780, 676, -420, 316, -776, 320, -772, 324, -776, 680, -10548, 
		
		};

		uint16_t len = sizeof(signal_raw) / sizeof(signal_raw[0]);

		uint16_t i = 0;

		for (uint8_t j = 1, i = 5; j < 5; j++)
		{

			for (; i < len; i++)
			{
				state = ooDecode.decode(&signal_raw[i]);
			}
			if (j < 2) {
				assertEqual(ooDecode.patternLen, 5);
				assertFalse(state);
				ooDecode.printOut();

			}
			i = 0;
		}

		const int16_t pause_pulse = 32160;
		state = ooDecode.decode(&pause_pulse);;
		ooDecode.printOut();
		assertEqual(ooDecode.patternLen, 8);

		//ooDecode.printOut();

		//assertEqual(ooDecode.message[0], 0);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);

		pass();
		
		//assertTrue(state);
	}

}



/*
// OSV2 Data hex : DADC539E18277055        //-1116, 840, -1104, 848, -1112, 352, -628, 836, -1124, 828, -644, -1112, 840, -1124, 832, -1116, -624, 840, -1120, 832, -1120, 836, -636, -1124, 832, -1116, -628, 840, -624, 352, -1124, 832, -1120, 836, -1116, 840, -1108, 844, -1104, 852, -1104, 364, -608, 856, -1100, 852, -1108, 848, -624, 352, -1092, 376, -604, 860, -624, 356, -1108, 356, -608, 860, -624, 352, -1092, 376, -604, 860, -604, 372, -1096, 368, -604, 864, -608, 368, -1104, 852, -1092, 372, -612, 852, -608, 368, -1092, 376, -612, 852, -3568,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          | Detector ends here
int sample_OSV2_data[] = {  863, -1080, 872, -1080, 876, -1088, 864, -1088, 868, -1092, 856, -1096, 860, -1084, 872, -1084, 868, -1100, 856, -1084, 868, -1096, 860, -1084, 868, -1080, 872, -1092, 864, -1088, 864, -1092, 864, -608, 368, -1088, 380, -592, 872, -600, 376, -1080, 388, -600, 860, -1092, 864, -600, 376, -1092, 372, -604, 864, -1088, 868, -600, 372, -1092, 864, -1084, 380, -600, 868, -1084, 868, -1088, 868, -596, 380, -1092, 376, -596, 868, -1088, 868, -1088, 864, -1092, 860, -620, 356, -1088, 868, -1088, 380, -604, 860, -608, 368, -1104, 360, -600, 868, -596, 384, -1084, 864, -1100, 364, -608, 860, -1092, 860, -1096, 860, -1096, 856, -600, 380, -1088, 860, -1092, 376, -616, 852, -612, 364, -1084, 872, -1084, 868, -1088, 376, -624, 844, -1088, 868, -596, 376, -1088, 868, -1084, 868, -1084, 384, -608, 856, -1096, 856, -1092, 864, -604, 372, -1088, 868, -1076, 392, -596, 864, -592, 388, -1084, 868, -1092, 860, -1092, 864, -1096, 860, -1092, 860, -1096, 368, -596, 868, -1088, 868, -1084, 868, -608, 372, -1092, 372, -600, 868, -604, 372, -1088, 376, -600, 864, -612, 368, -1088, 376, -608, 856, -600, 376, -1088, 380, -604, 864, -604, 372, -1080, 872, -1096, 372, -596, 868, -600, 376, -1084, 380, -604, 861 };

const uint8_t rand_data_size = 30;

int random_data[rand_data_size];  // Array for some Random Data

int sample_onoff_data[] =
// Logilink:
//{ 512, -9200, 576, -1920, 516, -3880, 512, -1924, 512, -3880, 516, -1936, 512, -3880, 512, -3880, 516, -3876, 516, -3880, 512, -1924, 512, -1920, 528, -3880, 516, -3876, 516, -3880, 512, -1924, 512, -3880, 512, -1936, 512, -1928, 508, -1924, 524, -1924, 508, -3884, 512, -3880, 512, -3880, 512, -1940, 512, -1920, 512, -1924, 524, -3880, 512, -3880, 516, -1920, 512, -3880, 512, -1936, 512, -1924, 512, -1936, 512, -1924, 508, -3880, 512, -1940, 508 };//, -9184, 576, -1936, 512, -3876, 516, -1920, 512, -3880, 528, -1920, 516, -3876, 512, -3880, 512, -3880, 528, -3876, 516, -1920, 512, -1924, 524, -3880, 512, -3880, 512, -3880, 512, -1920, 528, -3880, 512, -1920, 512, -1940, 508, -1924, 508, -1940, 508, -3884, 508, -3880, 512, -3880, 512, -1936, 512, -1924, 508, -1940, 508, -3880, 512, -3880, 512, -1936, 512, -3880, 512, -1920, 512, -1936, 512, -1924, 508, -1940, 508, -3880};
// ITTX Data:
//{-1012, -3556, -2364, 152, -108, -1448, -5604, -3944, 188, -3304, -768, -1212, -2156, -568, -636, -460, -704, 100, -496, -1324, -1052, 112, -5852, -1484, -716, -300, -2692, -380, 1264, -1012, 1260, -1004, 1256, -1016, 1240, -1032, 468, -1020, 1252, -1020, 488, -1024, 1232, -1032, 1236, -1020, 1236, -1036, 1236, -1036, 1228, -1040, 460, -1028, 1240, -1036, 1232, -1032, 1228, -1040, 1232, -1044, 1220, -1040, 460, -1032, 472, -1036, 1224, -1040, 464, -1044, 1224, -1036, 468, -1048, 1212, -1040, 1224, -1032, 476, -1036, 1232, -1024, 1236, -1044, 456, -1052, 460, -1036, 1228, -1044, 1220, -1040, 460, -1036, 1240, -1024, 468, -1032, 1248, -1024, 1232, -1028, 476, -1028, 1232, -1032, 476, -1036, 1224, -1048, 1216, -1040, 468, 464, -843};
//Einhell
{ 0, -364, -480, -312, -312, 348, 428, -548, -444, -308, -624, -596, -500, -540, -328, -412, 352, -308, -708, 428, 408, 440, -352, -704, 440, -300, -352, -492, 384, -384, 304, 400, 312, -644, -372, -688, 352, -668, -604, -352, -324, 304, -444, -532, 308, -384, 384, 308, 428, 300, -524, -392, -420, -404, 356, -432, -364, -520, 356, 324, 328, -504, -340, 308, -440, -308, -544, -348, -400, -500, 384, -356, 320, 316, -528, -440, -372, -452, 348, -500, 564, -300, -488, -312, -384, -516, -380, -368, -360, 576, -524, -444, -444, -312, 324, -368, -444, -588, -432, -428, -324, -416, -324, 324, -536, -504, -324, -300, 332, -320, -664, -368, -308, -536, 336, 376, -320, 336, -316, -308, -404, -328, -340, -304, -364, -332, 324, -528, -392, -304, 312, -376, -368, 376, 424, -340, -528, -332, -388, -304, 396, 484, -344, 356, -320, 328, -316, 488, -508, -328, -304, -580, -392, -504, -384, -556, 304, -424, -592, -388, 352, 328, -352, 364, 360, -364, -324, -396, -432, 304, 448, 476, 332, -340, -404, 336, -492, -392, -300, -688, 400, 572, 324, -388, 356, -316, 556, -348, -368, -364, -344, 320, -300, 300, -380, 568, -344, -360, -352, 360, 332, -544, -324, -368, -352, 428, -376, -340, -304, -400, -400, 472, -304, -368, -304, -320, -908, -428, 308, 500, -332, -524, 376, 364, -368, -360, -324, -324, -320, 416, -340, 404, -456, 348, 344, -412, -352, 412, -616, -384, -340, 572, -448, -376, -736, -304, -316, -356, 340, 304, 412, -316, -468, -372, -380, -304, -324, -356, -332, -368, -308, -320, -392, 376, -372, 392, -308, -380, -472, -360, -300, 352, -300, -384, -572, -364, 364, 348, -308, 412, -380, -316, 316, -352, 408, 364, -412, -356, -320, -424, 368, 340, -628, -408, -548, -452, 376, 316, -344, -388, 304, -392, 512, 1276, -340, 1236, -356, 1220, -376, 1200, -1188, 444, -356, 1192, -396, 1184, -1208, 440, -352, 1164, -1228, 408, -1196, 396, -1200, 396, -1204, 392, -1208, 384, -420, 1112, -1296, 352, -1252, 320, -1288, 308, -500, 1044, -1340, -1300, 300, -1292, 304, -1288, 312, -488, 1064, -512, 1068, -516, 1072, -512, 1076, -512, 1076, -504, 1080, -508, 1080, -500, 1088, -1300, 332, -20556, 392, -408, 1164, -436, 1136, -464, 1124, -460, 1124, -1264, 372, -428, 1120, -460, 1128, -1260, 392, -400, 1128, -1260, 372, -1224, 376, -1220, 376, -1220, 380, -1224, 368, -420, 1132, -1256, 396, -1196, 380, -1216, 380, -416, 1136, -1252, 384, -1216, 380, -1216, 380, -1212, 388, -416, 1132, -444, 1136, -448, 1140, -448, 1136, -452, 1136, -448, 1140, -448, 1136, -452, 1136, -1248, 384, -20528, 412, -388, 1184, -412, 1164, -428, 1160, -428, 1156, -1232, 404, -396, 1152, -436, 1152, -1232, 416, -376, 1152, -1232, 404, -1196, 400, -1200, 400, -1192, 404, -1196, 400, -396, 1156, -1232, 416, -1176, 400, -1196, 404, -392, 1156, -1232, 404, -1196, 400, -1196, 400, -1196, 404, -392, 1156, -424, 1156, -432, 1156, -432, 1152, -436, 1152, -432, 1156, -428, 1156, -436, 1152, -1232, 400, -20520, 420, -380, 1192, -404, 1172, -416, 1172, -420, 1164, -1224, 408, -388, 1164, -416, 1168, -1224, 428, -364, 1164, -1220, 412, -1188, 412, -1188, 408, -1184, 416, -1184, 412, -384, 1164, -1224, 428, -1164, 412, -1192, 404, -388, 1164, -1224, 408, -1188, 408, -1188, 412, -1184, 412, -384, 1164, -424, 1160, -420, 1164, -424, 1164, -420, 1164, -424, 1164, -420, 1164, -424, 1164, -1224, 408, -20512, 428, -376, 1196, -396, 1180, -416, 1172, -412, 1176, -1212, 420, -376, 1172, -416, 1172, -1216, 432, -360, 1168, -1220, 416, -1180, 416, -1188, 412, -1180, 416, -1188, 408, -380, 1168, -1220, 432, -1160, 416, -1180, 420, -384, 1164, -1224, 408, -1184, 416, -1184, 412, -1184, 412, -384, 1168, -416, 1164, -424, 1164, -416, 1168, -420, 1164, -424, 1164, -420, 1168, -420, 1168, -1220, 412, -20512, 428, -372, 1200, -392, 1184, -412, 1176, -408, 1176, -1212, 424, -372, 1176, -412, 1176, -1212, 436, -356, 1172, -1216, 420, -1180, 416, -1180, 416, -1184, 416, -1176, 420, -376, 1172, -1220, 432, -1160, 416, -1180, 416, -380, 1172, -1216, 416, -1184, 416, -1180, 416, -1180, 416, -384, 1168, -412, 1168, -420, 1164, -424, 1164, -420, 1168, -416, 1168, -424, 1164, -420, 1164, -1228, 408, -20508, 432, -372, 1200, -396, 1180, -412, 1176, -404, 1180, -1208, 428, -372, 1176, -416, 1168, -1212, 440, -356, 1172, -1220, 416, -1176, 420, -1176, 424, -1176, 420, -1180, 416, -376, 1172, -1216, 436, -1164, 412, -1184, 412, -380, 1172, -1212, 420, -1184, 416, -1180, 416, -1184, 412, -384, 1168, -412, 1168, -416, 1172, -416, 1168, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -1220, 412, -20512, 432, -368, 1204, -392, 1184, -408, 1176, -408, 1180, -1212, 420, -376, 1176, -408, 1176, -1216, 436, -356, 1172, -1212, 424, -1176, 420, -1176, 420, -1180, 420, -1176, 420, -376, 1172, -1216, 436, -1156, 420, -1180, 416, -380, 1172, -1216, 416, -1180, 420, -1176, 420, -1180, 416, -380, 1172, -412, 1168, -416, 1172, -412, 1172, -420, 1168, -420, 1164, -424, 1164, -424, 1160, -1224, 412, -20516, 424, -372, 1200, -396, 1184, -408, 1176, -408, 1180, -1208, 424, -376, 1172, -416, 1172, -1216, 436, -352, 1176, -1212, 420, -1176, 424, -1176, 420, -1176, 420, -1180, 416, -376, 1176, -1216, 436, -1156, 420, -1176, 420, -380, 1172, -1216, 416, -1180, 416, -1180, 420, -1180, 416, -380, 1168, -416, 1168, -416, 1168, -416, 1172, -416, 1172, -416, 1168, -420, 1168, -420, 1164, -1220, 416, -20512, 428, -372, 1200, -396, 1184, -408, 1176, -408, 1180, -1212, 420, -372, 1180, -408, 1176, -1216, 436, -356, 1172, -1212, 424, -1176, 420, -1176, 420, -1180, 420, -1176, 420, -376, 1172, -1220, 432, -1156, 420, -1180, 416, -380, 1172, -1216, 416, -1180, 420, -1180, 416, -1180, 416, -380, 1172, -412, 1168, -420, 1168, -420, 1164, -416, 1172, -420, 1168, -420, 1164, -420, 1168, -1220, 412, -20512, 432, -368, 1204, -392, 1184, -404, 1184, -408, 1176, -1212, 424, -372, 1176, -408, 1180, -1208, 440, -356, 1172, -1216, 420, -1176, 420, -1176, 424, -1176, 420, -1180, 416, -376, 1176, -1212, 436, -1160, 416, -1180, 420, -380, 1168, -1224, 412, -1180, 416, -1180, 420, -1180, 416, -376, 1172, -412, 1172, -424, 1160, -420, 1168, -416, 1172, -416, 1168, -424, 1164, -420, 1168, -1220, 412, -20512, 432, -372, 1200, -392, 1184, -404, 1184, -408, 1176, -1212, 424, -372, 1176, -408, 1180, -1216, 432, -352, 1176, -1216, 420, -1184, 416, -1176, 420, -1176, 420, -1180, 416, -376, 1176, -1212, 440, -1156, 420, -1176, 420, -380, 1172, -1212, 420, -1180, 416, -1180, 420, -1176, 420, -380, 1172, -412, 1168, -416, 1172, -416, 1168, -416, 1172, -416, 1168, -420, 1168, -420, 1168, -1220, 412, -20512, 432, -372, 1200, -392, 1184, -408, 1180, -408, 1176, -1212, 424, -376, 1172, -408, 1180, -1212, 436, -356, 1172, -1216, 420, -1176, 420, -1180, 420, -1176, 420, -1180, 416, -376, 1176, -1212, 440, -1160, 416, -1180, 416, -376, -1216, 416, -1180, 420, -1176, 420, -1188, 408, -380, 1172, -408, 1172, -416, 1172, -416, 1168, -420, 1168, -416, 1172, -420, 1164, -420, 1168, -1220, 412, -20512, 432, -372, 1200, -392, 1184, -408, 1180, -408, 1180, -1208, 424, -372, 1180, -408, 1176, -1212, 440, -352, 1176, -1216, 420, -1176, 420, -1180, 416, -1176, 424, -1172, 424, -380, 1168, -1216, 436, -1156, 420, -1180, 416, -376, 1176, -1220, 416, -1176, 420, -1180, 416, -1184, 416, -380, 1168, -408, 1176, -416, 1168, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -416, 1168, -1220, 416, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -404, 1180, -1216, 420, -372, 1176, -408, 1180, -1212, 440, -352, 1176, -1212, 420, -1176, 424, -1176, 420, -1176, 424, -1176, 420, -376, 1172, -1216, 436, -1156, 420, -1180, 416, -384, 1168, -1212, 420, -1180, 420, -1180, 416, -1180, 416, -384, 1168, -412, 1172, -412, 1172, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -416, 1168, -1224, 412, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -408, 1176, -1216, 420, -372, 1176, -412, 1176, -1212, 436, -356, 1176, -1212, 420, -1176, 424, -1176, 420, -1176, 424, -1176, 420, -376, 1172, -1216, 436, -1156, 420, -1180, 416, -380, 1172, -1220, 416, -1176, 420, -1180, 416, -1184, 416, -376, 1172, -412, 1172, -412, 1172, -416, 1172, -416, 1172, -420, 1164, -420, 1168, -420, 1168, -1220, 412, -20516, 428, -372, 1200, -396, 1184, -404, 1180, -412, 1176, -1212, 420, -372, 1180, -408, 1180, -1208, 440, -360, 1168, -1212, 424, -1176, 420, -1176, 424, -1180, 416, -1180, 416, -376, 1176, -1216, 436, -1160, 416, -1176, 420, -376, 1172, -1216, 420, -1180, 420, -1180, 416, -1180, 416, -380, 1172, -412, 1168, -420, 1168, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -420, 1164, -1224, 412, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -408, 1180, -1208, 424, -372, 1180, -408, 1176, -1212, 440, -352, 1176, -1216, 420, -1176, 420, -1176, 420, -1180, 420, -1176, 420, -376, 1176, -1212, 436, -1160, 416, -1180, 420, -380, 1168, -1220, 416, -1180, 420, -1176, 420, -1180, 416, -376, 1176, -408, 1172, -416, 1172, -420, 1164, -420, 1168, -420, 1168, -416, 1168, -420, 1168, -1220, 416, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -408, 1180, -1208, 424, -376, 1170 };

// AS Data
//SB:10110011
//B1:00000110  =0x06
//B2:10000000  =0x80
//B3:11001000  =0xc8
//B4:10000000  =0x80

/* ITv1
MU; P0 = -5802; P1 = 579; P2 = -171; P3 = 192; P4 = -558; D = 0121212121212121212123434341234123412341234123434301212121212121212121234343412341234123412341234343012121212121212121212343434123412341234123412343430121212121212121212123434341234123412341234123434301212121212121212121234343412341234123412341234343012; CP = 3; O; 
*/
/*
//                                        | Start                                                                                                                                                                                                                                                                                                                                                                                         |Ends
int sample_AS_data[] = { -764, -524, -536, 1776, -1508, 956, -684, 1712, -784, 844, -1568, 924, -720, 1712, -808, 828, -812, 816, -820, 812, -824, 808, -1608, 880, -756, 1652, -1620, 1656, -832, 796, -844, 788, -844, 784, -844, 792, -844, 788, -844, 788, -1632, 864, -772, 1636, -856, 780, -1628, 1636, -856, 772, -860, 776, -1636, 1632, -864, 772, -860, 772, -864, 772, -860, 768, -860, 776, -860, 776, -860, 772, -780, 776, -3908, 1640, -1628, 864, -768, 1640, -852, 784, -1624, 864, -772, 1676, -848, 788, -844, 792, -844, 784, -848, 788, -1620, 868, -772, 1636, -1632, 1644, -848, 780, -852, 784, -852, 780, -848, 784, -852, 784, -844, 788, -1632, 864, -768, 1640, -852, 784, -1624, 1640, -852, 776, -852, 788, -1636, 1632, -852, 784, -852, 776, -856, 780, -852, 784, -848, 784, -852, 780, -856, 776, -776, 780 };


// This array can be filled with output from signalduino
//id=738, channel=2, temp=12.3 :  unknown Protocol
//uint8_t signal_Stream[]= {  0,3,0,2,0,1,0,2,0,2,0,2,0,1,0,1,0,1,0,2,0,1,0,1,0,2,0,1,0,1,0,1,0,1,0,1,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3};
//int patternData[]=  { 476, -980, -1956, -4016,100  };//{-100,-988,488,-1960,-4016};

// Kaku Swirtch Protocol bits: 00111000111000111111111010001001
//uint8_t signal_Stream[]={1,5,1,0,1,3,1,0,1,3,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,2,1,0,1,2,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,3,1,0,1,3,1,0,1,2,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,3,1,2,1,0,1,0,1,3,1,0,1,3,1,0,1,3,1,2,1,0,1,0,1,3,1,0,1,3,1,2,1,0,1,4};
//int patternData[]={-232,308,-904,-1092,-9464,-2548,1589};

// Demo Signal like from einhell example
//int patternData[]={390,-1184,-401,1122,-20012};
//uint8_t signal_Stream []={0,2,3,2,3,2,3,1,0,1,0,2,3,2,3,1,0,2,3,1,0,1,0,1,0,1,0,1,0,2,3,2,3,2,3,1,0,1,0,2,3,1,0,1,0,1,0,2,3,2,3,2,3,2,3,2,3,2,3,1,0,2,0,4};

// Demo Signal like from RF803E

//int patternData[]={291,-506,0,-926,695,-1758};
//uint8_t signal_Stream []={0,1,0,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,4,3,4,1,0,1,0,3,4,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,5,0,1,0,1,0,3,4,3,4,3,0,1,4,3,0,1,0,1,4,3,0,1,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,0,1,4,1,0,1,0,1,0,1,0,3,4,1,0,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,4,3,4,1,0,1,0,3,4,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,5,0,1,0,1,0,3,4,3,4,3,0,1,4,3,0,1,0,1,4,3,0,1};

// Demo Signal like from Oregon NR868
/*
int patternData[] = {
	1139,-1494,-504,596,
};

uint8_t signal_Stream[] = {
	0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,0,

};
*/

testing(mc_somfy_b)
{
	if (checkTestDone(mc_somfy_a))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();

		String dstr(F("MU;P0=387;P1=-420;P2=788;P3=-822;P4=-31734;P5=2719;P6=-2726;P7=4748;D=01230121010101010323232101010321032101010101010101045656565656565671232101010301230121010301230121010301230121010101010323232101010321032101010101010101045656565656565671232101010301230121010301230121010301230121010101010323232101010321032101010101010101;CP=0;O;"));
		//String dstr(F("MU;P0=4850;P1=645;P2=-654;P3=383;P4=-1276;P5=1234;P6=-2501;P7=2504;D=01234523452343254125454545412121212125212121212121212121412121212125216767676767676045452141254121252141212121212521412121254541252121214125214521452141254125454545412121212125212121212121212145212121212145676767676767604545214125412125214121212121252141;CP=1;R=247;O;"));

		state = import_sigdata(&dstr);
		dstr = "";


		//state = ooDecode.decode(&pData[5]);
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);

		//ooDecode.printOut();
		bool result = mcdecoder.doDecode();
		assertFalse(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertTrue(mcdecoder.pdec->m_truncated);
		assertTrue(result);
		//result = mcdecoder.doDecode();

#ifndef B12
		//assertTrue(mcdecoder.mc_start_found);
		//assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
#endif
		assertTrue(result);
		//assertEqual(mcdecoder.ManchesterBits.bytecount, 9);
		//assertEqual(mcdecoder.ManchesterBits.valcount, 79);

		String mcStr;
		String base;


		mcdecoder.getMessageHexStr(&mcStr);
		base = "D8150900"; // früher 
		     
		        
		assertEqual(mcStr, base); // may not compile or give warning


		mcStr = "";
		mcdecoder.getMessagePulseStr(&mcStr);
		base = "LL=-822;LH=788;SL=-420;SH=387;";
		assertEqual(mcStr, base); // may not compile or give warning

		mcStr = "";
		mcdecoder.getMessageLenStr(&mcStr);
		base = "L=32;";
		assertEqual(mcStr, base); // may not compile or give warning

		pass();

	}
}
testing(mc_somfy_c)
{
	if (checkTestDone(mc_somfy_b))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();

		String dstr(F("MU;P0=-2544;P1=4755;P2=-674;P3=603;P4=-1319;P5=1242;P6=-26783;P7=2453;D=22323232523232345234523232343232325232345432323252345234325452345234523432523436707070707070701454543232323252323234523452323234323232523234543232325234523432545234523452343252343670707070707070145454323232325232323452345232323432323252323454323232523452;CP=3;O"));
		state = import_sigdata(&dstr);                                                         
		dstr = "";

		
		ooDecode.printOut();
		//state = ooDecode.decode(&pData[5]);
		//assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		//ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);


		bool result = mcdecoder.doDecode();
#ifndef B12
	//	assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
#endif
		assertTrue(result);
		//assertEqual(mcdecoder.ManchesterBits.bytecount, 9);
		//assertEqual(mcdecoder.ManchesterBits.valcount, 79);

		String mcStr;
		String base;


		mcdecoder.getMessageHexStr(&mcStr);
		base = "F090F17934932";
		assertEqual(mcStr, base); // may not compile or give warning


		mcStr = "";
		mcdecoder.getMessageLenStr(&mcStr);
		base = "L=51;";
		assertEqual(mcStr, base); // may not compile or give warning

		ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);
		result = mcdecoder.doDecode();
		mcdecoder.getMessageHexStr(&mcStr);
		base = "L=51;F090F179349335F090F17934932";
		assertEqual(mcStr, base); // may not compile or give warning



		pass();

	}
}

testing(mc_somfy_d)
{
	if (checkTestDone(mc_somfy_c))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();


		String dstr(F("MU;P0=577;P1=-26798;P2=2411;P3=-2576;P4=4731;P5=-1348;P6=1211;P7=-700;D=23234565656707050707670707050707076707056705670705076707070707056565076567070705656565012323232323232345656567070507076707070507070767070567056707050767070707070565650765670707056565650123232323232323456565670705070767070705070707670705670567070507670707;"));
		state = import_sigdata(&dstr);
		dstr = "";


		ooDecode.printOut();
		//state = ooDecode.decode(&pData[5]);
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		//ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);


		bool result = mcdecoder.doDecode();
#ifndef B12
		//	assertTrue(mcdecoder.mc_start_found);
		assertTrue(mcdecoder.mc_sync);
		assertFalse(mcdecoder.pdec->mcDetected);
#endif
		assertTrue(result);
		//assertEqual(mcdecoder.ManchesterBits.bytecount, 9);
		//assertEqual(mcdecoder.ManchesterBits.valcount, 79);

		String mcStr;
		String base;

		/*
		mcdecoder.getMessageHexStr(&mcStr);
		base = "F090F17934932";
		assertEqual(mcStr, base); // may not compile or give warning

		
		mcStr = "";
		mcdecoder.getMessageLenStr(&mcStr);
		base = "L=51;";
		assertEqual(mcStr, base); // may not compile or give warning

		ooDecode.printOut();

		assertTrue(mcdecoder.isManchester());
		assertFalse(state);
		//assertEqual(ooDecode.pattern[ooDecode.message[ooDecode.messageLen - 1]], pData[s_Stream[i - 1]]);
		result = mcdecoder.doDecode();
		mcdecoder.getMessageHexStr(&mcStr);
		base = "F090F17934932AF84878BC9A499";
		assertEqual(mcStr, base); // may not compile or give warning

*/

		pass();

	}
}
testing(mc_tfa_a)
{
	if (checkTestDone(mc_somfy_d))
	{

		bool state;
		ooDecode.reset();
		mcdecoder.reset();

		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		String dstr(F("MU;P0=-124;P1=1505;P2=-863;P3=561;P6=-1508;D=01232323232321232121212123232321212321212123212121212323232123232323232323232321212123212321632323232323232321232121212123232321212321212123212121212323232123232323232323232321212123212321632323232323232321232121212123232321212321212123212121212323232123;CP=3;O;"));
		state = import_sigdata(&dstr);
		dstr = "";


		ooDecode.printOut();
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.getClock();
		ooDecode.getSync();

		ooDecode.printOut();

		assertFalse(mcdecoder.isManchester());
		assertFalse(state);




		pass();
	}

}



testing(mu_xt300_a)  //Opus XT 300
{
	if (checkTestDone(mc_somfy_d))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		String dstr(F("MU;P0=1363;P1=-935;P2=536;P3=-12304;P5=7276;D=0101210101212121212121212121010121212101232121212121212121012101210121012101010101012101210121010101210101212121212121212121010121212101215;CP=2;"));
		state = import_sigdata(&dstr);



		ooDecode.printOut();
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.getClock();
		ooDecode.getSync();
		int pulse = -120;
		state = ooDecode.decode(&pulse);
		state = ooDecode.decode(&pulse);

		ooDecode.printOut();

		assertFalse(mcdecoder.isManchester());
		assertFalse(state);

		pass();
	}
}


testing(mu_xt300_b)  //Opus XT 300
{
	if (checkTestDone(mu_xt300_a))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		String dstr(F("MU;P0=390;P1=-870;P2=420;P3=1260;P4=-6990;D=012121212121212131213121312131213131313131213121312131313121312121212121212121212131312121212134;"));
		state = import_sigdata(&dstr);



		ooDecode.printOut();
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.getClock();
		ooDecode.getSync();

		state = import_sigdata(&dstr);
		state = import_sigdata(&dstr);

		int pulse = -120;
		state = ooDecode.decode(&pulse);
		state = ooDecode.decode(&pulse);

		ooDecode.printOut();

		assertFalse(mcdecoder.isManchester());
		assertFalse(state);

		pass();
	}
}


testing(ms_fa20)
{
	if (checkTestDone(mc_somfy_d))
	{

		bool state;
		ooDecode.reset();
		mcdecoder.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;

		String dstr(F("MU;P0=108;P1=-1369;P2=795;P3=-2724;P4=-22182;P5=8157;P6=-900;D=012121212323232321212123212124562123212123212123232121212121232323232121212321212456212321212321212323212121212123232323212121232121245621232121232121232321212121212323232321212123212124;CP=2;"));

		state = import_sigdata(&dstr);

		ooDecode.printOut();
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.getClock();
		ooDecode.getSync();
		int pulse = -120;
		state = ooDecode.decode(&pulse);
		state = ooDecode.decode(&pulse);

		ooDecode.printOut();

		assertFalse(mcdecoder.isManchester());
		assertFalse(state);

		pass();

	}
}

testing(ms_nc)
{
	if (checkTestDone(ms_dodecode_NCWS))
	{
		bool state;
		ooDecode.reset();
		mcdecoder.reset();
		ooDecode.MSenabled = true;
		ooDecode.MCenabled = true;
		ooDecode.MUenabled = true;
		//String dstr(F("MU;P0=-4913;P1=228;P2=361;P3=-632;P4=-382;P5=153;P6=106;D=0101010101010101023232323245354245354245323232323542463232323232323232323236424;"));

		String dstr(F("MU;P0=-2876;P1=439;P2=-8005;P3=-2101;P4=-4112;D=0121212121212121212121212121212131313131314141314141313131314131314131413131314141313141414131314131313131413131413121213131313131414131414131313131413131413141313131414131314141413131413131313141313141312121313131313141413141413131313141313141314131313;"));
		state = import_sigdata(&dstr);
		
		
		assertFalse(mcdecoder.isManchester());
		ooDecode.calcHisto();
		ooDecode.getClock();
		ooDecode.getSync();
		ooDecode.printOut();
		for (uint8_t i=0;i<16;i++)
			ooDecode.processMessage();

		pass();
	}

}












uint8_t rssiCallback() { return 0; };	// Dummy return if no rssi value can be retrieved from receiver


void setup() {
	randomSeed(A0);
	Serial.begin(BAUDRATE);
	ooDecode.setRSSICallback(&rssiCallback);
	ooDecode.MSenabled = true;
	ooDecode.MCenabled = true;
	ooDecode.MUenabled = true;

	Serial.println("---- Start of ----");
	delay(400);


	Test::exclude("*"); //mc_long_2 mc_long_1
	//Test::include("mc_long_2");
	Test::include("speed*");


}

void loop() {
	
	Test::run();


}




