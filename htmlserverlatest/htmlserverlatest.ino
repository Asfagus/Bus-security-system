#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"

// Replace with your network credentials
const char* ssid = "NETGEAR91";
const char* password = "silenttrain866";
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
SoftwareSerial downserver(4,10);
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
SoftwareSerial serial_connection(5, 1); //RX=pin 3, TX=pin 4 of gps

String page = "";
String inData,apos="APositive",aneg="ANegative";
double latt=19.138812,lon=72.826820,sped=52,sat=4; 
String Astat,Bstat; 



void setup(){
  
  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  serial_connection.begin(9600);//must keep gps and SERIAL both on 9600 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](){
    page = "<html><h1><u><center>GSM</u>  <u> Security</u>  <u> System</u></h1></center><body style=""background-color:gainsboro;""><br>Bus status towards school<br>Bus Location:"+String(latt)+","+String(lon)+"<br><iframe width=""600"" height=""450"" frameborder=""1"" style=""border:1"" src=""https://www.google.co.in/maps/embed/v1/view?key=AIzaSyAJSS8EhvJOjEOWFP4WTqTDQHtSnVB8h3U&center="+String(latt)+","+String(lon)+"&zoom=18"" allowfullscreen> </iframe> </br> <br>Bus Speed:"+String(sped)+"<br> Number of Satellites:"+String(sat)+"<br><h2>Andheri Students</h2><br><TABLE BORDER=""5""    WIDTH=""50%""   CELLPADDING=""4"" CELLSPACING=""3""><TR> <TD>Name</TD><TD>Mobile No.</TD> <TD>Status</TD> </TR><TR> <TD>Akshit Rohra</TD><TD>+919920819076</TD><TD>"+String(Astat)+"</TD></TR><TR><TD>Data 5</TD><TD>Data 6</TD> <TD>Data7</TD></TR></TABLE><h2>Vile Parle Students</h2><TABLE BORDER=""5""  WIDTH=""50""% CELLPADDING=""4"" CELLSPACING=""3""><TR><TD>Name</TD> <TD>Mobile No.</TD><TD>Status</TD></TR><TR><TD>Akshit Asrani</TD> <TD>+919594815887</TD><TD>"+String(Bstat)+"</TD> </TR><TR><TD>Data 5</TD><TD>Data 6</TD><TD>Data7</TD>  </TR></TABLE></body>";
    server.send(200, "text/html", page);
  });
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(){
  serial_connection.listen();
  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  

  server.handleClient();
  //downserver.listen();
  //code to recieve status
  while (downserver.available() > 0)
    {
        char recieved = downserver.read();
        inData += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print("MCU Received: ");
            Serial.print(inData);
       
           
              if (inData[0] == '0')
              {
              Astat="Entered  ";
              Serial.println("A boarded");
              }
             else if (inData[0] == '1')
              {
              Astat="Not Entered";
              Serial.println("A did not board");

               }
              else if (inData[0]=='2'){
              Bstat="Entered";
              Serial.println("B boardED");

              }
              else if (inData[0]=='3'){
              Bstat="Not Entered";
              Serial.println("B did not board");

              }
            inData = ""; // Clear recieved buffer
        }
    }
   
  if (gps.location.lat()!=0)
  {
   latt=gps.location.lat();
   lon=gps.location.lng();
   sped=gps.speed.mph();
   sat=gps.satellites.value();
  }
}
