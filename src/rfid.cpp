#include <Arduino.h>
#include "rfid.h"

// Hardware pins
const int RFID_PIN = 33; // GPIO33; D33 pin for input (tx)

// RFID sensor settings
/*==========================================================================
// RFID Code Original Author : Handson Technology
// Description : RDM6300 125KHz RFID Card Reader Module
// Originally designed for Arduino Uno, modified for ESP32 use with hardware ports
//========================================================================== */
const int BUFFER_SIZE = 14;		 // RFID DATA FRAME FORMAT: 1byte head (value: 2), 10byte data (2byte version + 8byte tag), 2byte checksum, 1byte tail (value: 3)
const int DATA_SIZE = 10;		 // 10byte data (2byte version + 8byte tag)
const int DATA_VERSION_SIZE = 2; // 2byte version (actual meaning of these two bytes may vary)
const int DATA_TAG_SIZE = 8;	 // 8byte tag
const int CHECKSUM_SIZE = 2;	 // 2byte checksum

static HardwareSerial ssrfid(1);	 // ESP32 UART channel 1
static uint8_t buffer[BUFFER_SIZE]; // used to store an incoming data frame
static int buffer_index = 0;

// Function Declarations
static unsigned extract_tag();
static long hexstr_to_value(char *str, unsigned int length);

void rfid_init(void) {
	// RFID sensor initialization
	ssrfid.begin(9600, SERIAL_8N1, RFID_PIN, -1); // Start HardwareSerial on RX=33, TX disabled (-1), with serial data = 8 bits, no parity, 1 stop bit
	delay(100);
	Serial.println("RFID INIT DONE");
}

bool rfid_scan(unsigned long *detectedTag) {
	if (ssrfid.available() > 0)
	{
		bool call_extract_tag = false;
		int ssvalue = ssrfid.read(); // read
		if (ssvalue == -1) { // no data was read
			return false;
		}
		if (ssvalue == 2) { // RDM630/RDM6300 found a tag => tag incoming
			buffer_index = 0;
		}
		else if (ssvalue == 3) { // tag has been fully transmitted
			call_extract_tag = true; // extract tag at the end of the function call
		}

		if (buffer_index >= BUFFER_SIZE) { // checking for a buffer overflow (It's very unlikely that an buffer overflow comes up!)
			Serial.println("Error: Buffer overflow detected!");
			return false;
		}

		buffer[buffer_index++] = ssvalue; // everything is alright => copy current value to buffer

		if (call_extract_tag == true) {
			if (buffer_index == BUFFER_SIZE) {
				unsigned long tag = extract_tag();
				buffer_index = 0;
				if (tag != 0) { // ensuring a valid tag read
					if (detectedTag) *detectedTag = tag;
					return true;
				}
				return false;
			} else { // something is wrong... start again looking for preamble (value: 2)
				buffer_index = 0;
				return false;
			}
		}
	}
	return false;
}


static unsigned extract_tag() { // From Handson Technology - RDM6300 module code
	uint8_t msg_head = buffer[0];
	// pointer arithmetic
	uint8_t *msg_data = buffer + 1; // 10 byte => data contains 2byte version + 8byte tag, starts at index 1 (&buffer[1])
	uint8_t *msg_data_version = msg_data; // same location as msg_data
	uint8_t *msg_data_tag = msg_data + 2; // buffer + 3 (&buffer[3])
	uint8_t *msg_checksum = buffer + 11; // 2 byte (&buffer[11])
	uint8_t msg_tail = buffer[13]; // tail

	// print message that was sent from RDM630/RDM6300 to Serial
	Serial.println("--------");
	Serial.print("Message-Head: ");
	Serial.println(msg_head);
	Serial.println("Message-Data (HEX): ");
	for (int i = 0; i < DATA_VERSION_SIZE; ++i) {
		Serial.print(char(msg_data_version[i]));
	}
	Serial.println(" (version)");
	for (int i = 0; i < DATA_TAG_SIZE; ++i) {
		Serial.print(char(msg_data_tag[i]));
	}
	Serial.println(" (tag)");
	Serial.print("Message-Checksum (HEX): ");
	for (int i = 0; i < CHECKSUM_SIZE; ++i) {
		Serial.print(char(msg_checksum[i]));
	}
	Serial.println("");
	Serial.print("Message-Tail: ");
	Serial.println(msg_tail);
	Serial.println("--");

	// checksum verification
	long checksum = 0;
	for (int i = 0; i < DATA_SIZE; i += CHECKSUM_SIZE) {
		long val = hexstr_to_value((char *)(msg_data + i), CHECKSUM_SIZE);
		checksum ^= val;
	}
	Serial.print("Extracted Checksum (HEX): ");
	Serial.print(checksum, HEX);
	if (checksum == hexstr_to_value((char *)msg_checksum, CHECKSUM_SIZE)) {	 // compare calculated checksum to retrieved checksum
		Serial.println(" (OK)"); // calculated checksum corresponds to transmitted checksum!
	}
	else {
		Serial.println(" (NOT OK)"); // checksums do not match
		return 0;
	}

	// Pointers must be explicitly cast to char* to ensure compatibility with ESP32 compiler
	long tag = hexstr_to_value((char *)msg_data_tag, DATA_TAG_SIZE);
	Serial.print("Extracted Tag: ");
	Serial.println(tag);
	Serial.println("--------");
	Serial.println("");
	
	return tag;
}

static long hexstr_to_value(char *str, unsigned int length) { // From Handson Technology - RDM6300 module code
	// converts a hexadecimal value (encoded as ASCII string) to a numeric value
	char *copy = (char *)malloc((sizeof(char) * length) + 1);
	memcpy(copy, str, sizeof(char) * length);
	copy[length] = '\0';
	// the variable "copy" is a copy of the parameter "str". "copy" has an additional '\0' element to make sure that "str" is null-terminated.
	long value = strtol(copy, NULL, 16); // strtol converts a null-terminated string to a long value
	free(copy);							 // clean up
	return value;
}
