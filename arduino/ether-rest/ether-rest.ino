// Present a "Will be back soon web page", as stand-in webserver.
// 2011-01-30 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php
 
#include <EtherCard.h>

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static BufferFiller bfill;

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char page[] PROGMEM = "HTTP/1.0 204 No Content\r\n";

void setup(){
  Serial.begin(9600);
  Serial.println("\nether-rest");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
}

void loop(){
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    // check if valid tcp data is received
    if (pos) {
        bfill = ether.tcpOffset();
        char* data = (char *) Ethernet::buffer + pos;
        if(strncmp(data, "PUT /", 5) == 0) {
          int i = 0;
          char cmd[50];
          while(data[5 + i] != ' ') i++;
          strncpy(cmd, data + 5, i);
          cmd[i] = 0;
          Serial.println(cmd);
        }
        // Serial.println(data);
        memcpy_P(ether.tcpOffset(), page, sizeof page);
        ether.httpServerReply(sizeof page - 1);
    }
}
