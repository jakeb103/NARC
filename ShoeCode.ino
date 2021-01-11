#include <SPI.h>
#include <WiFi101.h>
char ssid[] = "Jake"; //  your network SSID (name) 
char pass[] = "Jake1";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
//thingspeak server
char server[] = "api.thingspeak.com";
WiFiClient client;
//API key for the Thingspeak ThingHTTP
const String apiKey = "S10G7T9TVU8A3RMK";
//the number the message should be sent to
const String sendNumber = "+18564899615";
const int fs = 3; 
int fr = 0; //used for force sensor values
const int x = 0;
const int y = 1;
const int z = 2;//used for accelerometer values
int Movement = 0;
const int minVal[3] = {200,200,200};
const int maxVal[3] = {700,700,700};

void setup() {
pinMode(fs, INPUT);
Serial.begin(9600);
pinMode(x, INPUT);
pinMode(y, INPUT);
pinMode(z,INPUT);
//set up WiFi:
 if (WiFi.status() == WL_NO_SHIELD) {
   Serial.println("WiFi shield not present"); 
   // don't continue:
   while(true);
 } 
 // attempt to connect to Wifi network:
 while (status != WL_CONNECTED) { 
   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);
   // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
   status = WiFi.begin(ssid, pass);

   // wait 10 seconds for connection:
   delay(10000);
 } 
 Serial.println("Connected to wifi");
 //printWifiStatus();


}


void sendSMS(String number,String message)
{
 // Make a TCP connection to remote host
 if (client.connect(server, 80))
 {


   //should look like this...
   //api.thingspeak.com/apps/thinghttp/send_request?api_key={api key}&number={send to number}&message={text body}


   client.print("GET /apps/thinghttp/send_request?api_key=");
   client.print(apiKey);
   client.print("&number=");
   client.print(number);
   client.print("&message=");
   client.print(message);
   client.println(" HTTP/1.1");
   client.print("Host: ");
   client.println(server);
   client.println("Connection: close");
   client.println();
 }
 else
 {
   Serial.println(F("Connection failed"));
 } 


 // Check for a response from the server, and route it
 // out the serial port.
 while (client.connected())
 {
   if ( client.available() )
   {
     char c = client.read();
     Serial.print(c);
   }      
 }
 Serial.println();
 client.stop();
}


String URLEncode(const char* msg)
{
 const char *hex = "0123456789abcdef";
 String encodedMsg = "";


 while (*msg!='\0'){
   if( ('a' <= *msg && *msg <= 'z')
     || ('A' <= *msg && *msg <= 'Z')
     || ('0' <= *msg && *msg <= '9') ) {
     encodedMsg += *msg;
   } 
   else {
     encodedMsg += '%';
     encodedMsg += hex[*msg >> 4];
     encodedMsg += hex[*msg & 15];
   }
   msg++;
 }
 return encodedMsg;
}


void printWifiStatus() {
 // print the SSID of the network you're attached to:
 Serial.print("SSID: ");
 Serial.println(WiFi.SSID());


 // print your WiFi shield's IP address:
 IPAddress ip = WiFi.localIP();
 Serial.print("IP Address: ");
 Serial.println(ip);


 // print the received signal strength:
 long rssi = WiFi.RSSI();
 Serial.print("signal strength (RSSI):");
 Serial.print(rssi);
 Serial.println(" dBm");
}




void loop() {
static boolean Fall = false;
static boolean msg_sent = false;







fr = analogRead(fs);
Serial.print("Force reading: ");
Serial.println(fr);
int xRead = analogRead(x);
int yRead = analogRead(y);
int zRead = analogRead(z);
Serial.print("x: ");
 Serial.print(xRead);
 Serial.print(" | y: ");
 Serial.print(yRead);
 Serial.print(" | z: ");
 Serial.println(zRead);
 delay(500);
 int xRead2 = analogRead(x);
int yRead2 = analogRead(y);
int zRead2 = analogRead(z);
Serial.print("x2: ");
 Serial.print(xRead2);
 Serial.print(" | y2: ");
 Serial.print(yRead2);
 Serial.print(" | z2: ");
 Serial.println(zRead2);

 if (fr > 850){delay (10);
 Fall = false;
 msg_sent = false;}
 else{
 Movement = ((xRead + zRead + yRead) - (xRead2 + yRead2 + zRead2));

  if(((315 < xRead2) && (xRead2 < 375)))
 {delay (10);
 Fall = false;
 msg_sent = false;}
 else
{ if((Movement > -10) && (Movement < 10)){ tone(9, 1000, 375);}}
    if(((315 < yRead2) && (yRead2 < 375)))
 {delay (5000);
 Fall = false;
 msg_sent = false;} 
else{ if ((Movement > -10) && (Movement < 10)) {tone(9, 1000, 375);}}
if (fr > 950) 
{delay (5000);
 Fall = false;
 msg_sent = false;}
delay (5000);
 fr = analogRead(fs);
Serial.print("Force reading: ");
Serial.print(fr);
int xRead = analogRead(x);
int yRead = analogRead(y);
int zRead = analogRead(z);
Serial.println("x: ");
 Serial.print(xRead);
 Serial.print(" | y: ");
 Serial.print(yRead);
 Serial.print(" | z: ");
 Serial.println(zRead);
 delay(500);
 int xRead2 = analogRead(x);
int yRead2 = analogRead(y);
int zRead2 = analogRead(z);
Serial.println("x2: ");
 Serial.print(xRead2);
 Serial.print(" | y2: ");
 Serial.print(yRead2);
 Serial.print(" | z2: ");
 Serial.println(zRead2);

//Movement = ((xRead + zRead + yRead) - (xRead2 + yRead2 + zRead2));
Serial.println("Movemnt: ");
 Serial.print(Movement);
if(((315 < xRead2) && (xRead2 < 375) && (fr > 850)))
{delay(10);}
   else{ if((315 < xRead2) && (xRead2 < 375)){delay (10);
   Fall = false;
 msg_sent = false;} 
 else { if ((Movement > -10) && (Movement < 10)) { Fall = true; 
 if(Fall && !msg_sent){ //send the sms
   Serial.println("Sending SMS");

  sendSMS(sendNumber, URLEncode("Someone has fallen and requested you be contacted by the NARC fall detection systems."));
   digitalWrite(9, HIGH);
   msg_sent = true;
 }}}
    if(((315 < yRead2) && (yRead2 < 375) && (fr > 850))){delay (10);
 Fall = false;
 msg_sent = false;}
   else { if((315 < yRead2) && (yRead2 < 375)){delay (10);
 Fall = false;
 msg_sent = false;} 
 else { if((Movement > -10) && (Movement < 10)) { { Fall = true; 
 if(Fall && !msg_sent){ //send the sms
   Serial.println("Sending SMS");

   //this function will send the sms
   //the first argument is the number to send to, formatted like this +12345678901
   //the second argument is the body of the text message, which must be within URLEncode()

 // Make a TCP connection to remote host
 sendSMS(sendNumber, URLEncode("Someone has fallen and requested you be contacted by the NARC fall detection systems."));
   digitalWrite(9, HIGH);
   msg_sent = true;
   delay(5000);
   digitalWrite(9,LOW);

 }}}
 }
 }
 }}}
