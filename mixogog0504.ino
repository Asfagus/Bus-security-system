/* Code Written by DPV TECHNOLOGY. WE have explained all the code in the grey comments.
  we recommend you to go over the code, examine it, play with it, improve it and modify it according to your needs.
  SUBSCRIBE DPV TECHNOLOGY:
  https://www.youtube.com/dpvtechnology?sub_confirmation=1
  54006B184D6A
*/
#include "TinyGPS++.h"
#include "SoftwareSerial.h"

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


char number[50] = {+919920819076};
SoftwareSerial serial_connection(3, 4); //RX=pin 3, TX=pin 4 of gps
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
//LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7); used for digital pins of ard
 LiquidCrystal lcd(14, 15, 16, 17, 18, 19);//PUT R/W GNDf
 //  lcd.setCursor(0, 1); cmd to set row 2

//RFID cards
int count = 0,Astate=0,Bstate=0 ;                                         // count = 0
char a[] = "54006B184D6A";
char b[] = "54006B18381F";
char c[] = "54006B186047";
char NAME[] = "student1";
char input[12];                                         // character array of size 12

// character array of size 12 for rfid scan
//boolean flag = 0;

SoftwareSerial mySerial(9, 10); //Gsm tx to pin 9,rx to pin10
SoftwareSerial upserver(11,12);//11 to 5(D1),12 to 4(D2)
struct Students {
  char Name[50];
  char  num[50];
};

struct Students Student1;
struct Students Student2; 
struct Students Student3;



void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  serial_connection.begin(9600);//This opens up communications to the GPS
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  upserver.begin(9600);
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started
  Serial.println("Bus started from garage");
  lcd.print("Bus Security");
  lcd.setCursor(0, 1); //cmd to set row 1
  lcd.print("System");
  delay(2000);
  lcd.clear();
  lcd.print("Bus Route 1");
  delay(5000);
  Serial.println("Andheri stop arrived");
  lcd.clear();
  lcd.print("Andheri stop arrived");
  delay(180);
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(180);
  }
  delay(1000);
  lcd.clear();
  lcd.print("Andheri stop");
  Serial.println("Start scanning");
  lcd.setCursor(0, 1); //cmd to set row 1
  lcd.print("Start scanning");
  delay(1000);
  pinMode(7, OUTPUT);//MOTOR DRIVER
  digitalWrite(7, LOW);
  int t1,t2,a,b,c;
  t1=millis();
  t2=millis();
  lcd.clear();
  lcd.print("Andheri stop");
  while (t2-t1<10000)
  {
    t2=millis();
    rfidA();
    lcd.setCursor(0, 1);
    lcd.print((10000-(t2-t1))/1000);
    gpsys();
  }
  Serial.println("Stop scanning");
  delay(1000); 
  lcd.clear();
  lcd.setCursor(0, 0); //cmd to set row 0
  lcd.print("Bus Route 1");
  lcd.setCursor(0, 1); //cmd to set row 1
  lcd.print("");
  if (Astate==0)
  {
     
     smsabs(Student3.Name, Student3.num);
     //upserver.println(Student3.Name);
     upserver.print("1\n");
     Serial.println("Sent sms");

  }
  delay(7000);
  Serial.println("Vileparle stop arrived");
  lcd.setCursor(0, 0); //cmd to set row 0
  lcd.print("Vileparle stop arrived");
  delay(180);
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(180);
  }
  delay(1000);
  Serial.println("Start scanning");
  lcd.clear();
  lcd.print("Vileparle stop ");
  lcd.setCursor(0, 1); //cmd to set row 1
  lcd.print("Start scanning");
  delay(1000);
  t1=millis();
  t2=millis();
  lcd.clear();
  lcd.print("Vileparle stop");
  while (t2-t1<10000)
  {
    t2=millis();
    rfidB();
    lcd.setCursor(0, 1);
    lcd.print((10000-(t2-t1))/1000);
  }
    lcd.clear();
  Serial.println("Stop scanning");
  if (Bstate==0)
  {
    smsabs(Student1.Name, Student1.num);
    upserver.print("3\n");

  }
  lcd.clear();
  lcd.setCursor(0, 0); //cmd to set row 0
  lcd.print("Bus Route 1");
  lcd.setCursor(0, 1); //cmd to set row 1
  lcd.print("");
  delay(1000);
  Serial.println("Destination arrived");
  lcd.clear();
  lcd.print("Destination arrived");
  delay(180);
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(180);
  }
}

void loop()
 {
   serial_connection.listen();
   gpsys();
 }
  
  //SMS
int sms(char student_name[], char student_number[])
{
    mySerial.listen();

  mySerial.println("AT");    //Sets the GSM Module in Text Mode
  delay(100);  // Delay of 1 second
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  mySerial.print("AT+CMGS=\"+");
  mySerial.print(student_number);
  mySerial.print("\"\r\n");
  //Serial.println("AT+CMGS=\"+919920819076\"\r"); // Replace x with mobile number
  delay(100);
  Serial.print(student_name);
  //Serial.println(student_number);
  Serial.println(" ENTERED THE BUS");
  mySerial.print(student_name);// The SMS text you want to send
  mySerial.println(" ENTERED THE BUS");// The SMS text you want to send
  mySerial.print("Latitude: ");
  mySerial.println(gps.location.lat(), 6);
  mySerial.print("Longitude: ");
  mySerial.println(gps.location.lng(), 6);
  mySerial.print("Speed MPH: ");
  mySerial.println(gps.speed.mph());
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module
  delay(100);
  return 0;
}

int smsabs(char student_name[], char student_number[])
{
  mySerial.listen();
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);  // Delay of 1 second
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1 second
  Serial.print(student_name);
  Serial.println(student_number);
  Serial.println(" did not board the bus");
  mySerial.print("AT+CMGS=\"+");
  mySerial.print(student_number);
  mySerial.print("\"\r\n");
//  mySerial.println("AT+CMGS=\"+919920819076\"\r"); // Replace x with mobile number
  delay(100);
  mySerial.print(student_name);// The SMS text you want to send
  mySerial.println(" did not board the bus");// The SMS text you want to send
  mySerial.println("Latitude:");
  mySerial.println(gps.location.lat(), 6);
  mySerial.println("Longitude:");
  mySerial.println(gps.location.lng(), 6);
  mySerial.println("Speed MPH:");
  mySerial.println(gps.speed.mph());
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module
  delay(1000);
  return 0;
}

//RFID
   int rfidA()
{   
   strcpy( Student3.Name, "Akshit Rohra");
   strcpy( Student3.num, "919920819076");
   if (Serial.available())
     {
   count = 0;
   while (Serial.available() && count < 12)         // Read 12 characters and store them in input array
    {
      input[count] = Serial.read();
      count++;
      delay(5);
    }
   //54006B186047
    if ((input[0] == '5' && input[1] == '4' && input[2] == '0' && input[3] == '0' && input[4] == '6' && input[5] == 'B') &&
            (input[6] == '1' && input[7] == '8' && input[8] == '6' && input[9] == '0' && input[10] == '4' && input[11] == '7'))
    {
//    Serial.println("INSIDE 3");
      if (Astate==1)
      {
        goto donee;
      }
      Serial.println(input);                             // Print RFID tag number
      lcd.clear();
      lcd.print(Student3.Name);
      delay(500);
      lcd.clear();
      lcd.print("Door Opening");
      sms(Student3.Name, Student3.num);
      //upserver.println(Student3.Name);
      upserver.println("0\n");
      digitalWrite(7, HIGH);//motordriver on
      digitalWrite(13, HIGH);//motordriver on
      delay(5000);
      digitalWrite(7, LOW);//motordriver off2
      digitalWrite(13, LOW);//motordriver off2      
      Astate=1;
      lcd.clear();
      lcd.print("Andheri stop");
      donee:;
      

    }
    //54006B18381F
    else if ((input[0] == '5' && input[1] == '4' && input[2] == '0' && input[3] == '0' && input[4] == '6' && input[5] == 'B') &&
             (input[6] == '1' && input[7] == '8' && input[8] == '3' && input[9] == '8' && input[10] == '1' && input[11] == 'F'))
    {
      Serial.println("WRONG BUS");
    } 
    //  54006B184D6A
    else if ((input[0] == '5' && input[1] == '4' && input[2] == '0' && input[3] == '0' && input[4] == '6' && input[5] == 'B') &&
        (input[6] == '1' && input[7] == '8' && input[8] == '4' && input[9] == 'D' && input[10] == '6' && input[11] == 'A'))
    {
      Serial.println("Wrong Busstop");
    }
    }
}

int rfidB()
{  strcpy (Student1.Name, "Akshit Asrani");
   strcpy( Student1.num, "919594815887");
      
   if (Serial.available())
     {
   count = 0;
   while (Serial.available() && count < 12)         // Read 12 characters and store them in input array
    {
      input[count] = Serial.read();
      count++;
      delay(5);
    }
   //54006B186047
    if ((input[0] == '5' && input[1] == '4' && input[2] == '0' && input[3] == '0' && input[4] == '6' && input[5] == 'B') &&
            (input[6] == '1' && input[7] == '8' && input[8] == '6' && input[9] == '0' && input[10] == '4' && input[11] == '7'))
    {
      if (Astate==1)
      {
        goto skip;
      }
      else  Serial.println("Wrong Busstop");
          
      skip:;
    }
    //54006B18381F
    else if ((input[0] == '5' && input[1] == '4' && input[2] == '0' && input[3] == '0' && input[4] == '6' && input[5] == 'B') &&
             (input[6] == '1' && input[7] == '8' && input[8] == '3' && input[9] == '8' && input[10] == '1' && input[11] == 'F'))
    {
      Serial.println("WRONG BUS");
    }
   
    //  54006B184D6A
    else if ((input[0] == '5' && input[1] == '4' && input[2] == '0' && input[3] == '0' && input[4] == '6' && input[5] == 'B') &&
        (input[6] == '1' && input[7] == '8' && input[8] == '4' && input[9] == 'D' && input[10] == '6' && input[11] == 'A'))
    {
//      Serial.println("INSIDE 1");
      if (Bstate==1)
      {
        goto done;
      }
      Serial.println(input);                             // Print RFID tag number
      sms(Student1.Name, Student1.num);
      upserver.println("2\n");
      lcd.clear();
      lcd.print(Student1.Name);
      delay(500);
      lcd.clear();
      lcd.print("Door Opening");
      digitalWrite(7, HIGH);//motordriver on
      digitalWrite(13, HIGH);//motordriver on
      delay(5000);
      digitalWrite(7, LOW);//motordriver off2
      digitalWrite(13, LOW);//motordriver off2 
      Bstate=1;
      lcd.clear();
      lcd.print("Vileparle stop");
      done:;
       
    }
    }
}

//GPS: make a function and call it in void loop
int gpsys()
{    
  serial_connection.listen();
  delay(100);
  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
   
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {   /*
      Serial.println("Satellite Count:");
      Serial.println(gps.satellites.value());
      Serial.println("Latitude:");
      Serial.println(gps.location.lat(), 6);
      Serial.println("Longitude:");
      Serial.println(gps.location.lng(), 6);
      Serial.println("Speed MPH:");
      Serial.println(gps.speed.mph());
      Serial.println("Altitude Feet:");
      Serial.println(gps.altitude.feet());
      Serial.println("");
      */
  }
  return 0;
}
