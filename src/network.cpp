#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "network.h"
#include "secrets.h"
#include "hit_detector.h"

// Networking settings
static const char* ssid = SECRET_SSID;
static const char* password = SECRET_PASS;
static const char* targetIP = SECRET_TARGET_IP; // currently to Vivobook S14
static const int udpPort = 12345;
static WiFiUDP udp;

void network_init(void) {
    // Initializing serial monitor
	Serial.begin(9600);

	// Wifi setup
	WiFi.mode(WIFI_STA); // Station mode (can declare as Access Point (AP) if needed)
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
    	delay(1000);
    	Serial.println("Connecting to WiFi...");
	}
	Serial.print("WiFi connected. IP Address: ");
	Serial.println(WiFi.localIP());

	// UDP setup
	udp.begin(udpPort);
}

bool network_send_rfid(unsigned long tag) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot send UDP.");
        return false;
    }

    // UDP integration to send packets on local network
	udp.beginPacket(targetIP, udpPort);
	udp.printf("RFID_TAG:%lu\n", tag);

	if (udp.endPacket() == 1) {
		Serial.println("UDP packet sent sucessfully");
        return true;
	} else {
		Serial.println("UDP transmission failed");
        return false;
	}
}

bool network_send_breakbeam(bool broken) {
	if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot send UDP.");
        return false;
    }

    // UDP integration to send packets on local network
	udp.beginPacket(targetIP, udpPort);
	udp.printf("BREAKBEAM STATUS:%s\n", broken ? "BROKEN" : "UNBROKEN");

	if (udp.endPacket() == 1) {
		Serial.println("UDP packet sent sucessfully");
        return true;
	} else {
		Serial.println("UDP transmission failed");
        return false;
	}
}

bool network_send_hitDetector(bool hit) {
	if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Cannot send UDP.");
        return false;
    }

    // UDP integration to send packets on local network
	udp.beginPacket(targetIP, udpPort);
	udp.printf("HIT DETECTED:%s\n", hit ? "YES" : "NO");

	if (udp.endPacket() == 1) {
		Serial.println("UDP packet sent sucessfully");
        return true;
	} else {
		Serial.println("UDP transmission failed");
        return false;
	}
}