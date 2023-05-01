#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <ESP32WebServer.h>
#include <WiFi.h>

#define OLED_RESET -1
#define ONE_WIRE_BUS 13
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_SSD1306 display(OLED_RESET);
int ledPin = 2;
int analogPin = 34;
const int sensorPin = 32;
int val = 0;
int moisture;
char ssid[] = "OPPO Reno4";
char pass[] = "plubzagg";

char ipaddress[20];

WiFiServer server(80);
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

#define UPDATE_INTERVAL_HOUR (1)
#define UPDATE_INTERVAL_MIN (0)
#define UPDATE_INTERVAL_SEC (0)

#define UPDATE_INTERVAL_MS (((UPDATE_INTERVAL_HOUR * 60 * 60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC) * 1000)

int Temperature = 0;
int Humidity = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("Dallas Temperature IC Control Library");
  sensors.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setTextColor(WHITE);

  Serial.print("Connecting");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected");
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

int xx = 140;
int yy = 0;
int tt = 5;

unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;

void loop()
{

  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.print("Temperature is: ");
  int temp = sensors.getTempCByIndex(0);
  Serial.print(temp);
  Serial.println(" *C");
  val = analogRead(analogPin);

  int sensorValue = analogRead(sensorPin);
  int moisture = map(sensorValue, 0, 4095, 0, 100);
  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  Serial.println("Light Dependent Resistor : ");
  Serial.println(val);

  WiFiClient client = server.available();

  time_ms = millis();
  time_dif = time_ms - time_1000_ms_buf;

  if (time_dif >= 1000)
  {
    time_1000_ms_buf = time_ms;
    Temperature = 25;
    Humidity = 34;
    digitalWrite(13, !digitalRead(13));
  }
  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    {
      currentTime = millis();
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();

            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\">");
            client.println("<head>");
            client.println("<title>Emoplanter</title>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">");
            client.println("<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Poppins\">");
            client.println("<link rel=\"icon\" type=\"image/x-icon\" href=\"https://scontent.fbkk12-4.fna.fbcdn.net/v/t1.15752-9/343153931_638044944418843_1744380192600157582_n.png?_nc_cat=103&ccb=1-7&_nc_sid=ae9488&_nc_eui2=AeGaJOCrT9hnSDqu0ekJ3pUGVHqA_7cTEWlUeoD_txMRaabWaIzdZ-VSAC3Yxw6JjLJ_di12lLMw2TZMHZxtNXMJ&_nc_ohc=QrfX-cT9sr0AX_cygLz&_nc_ht=scontent.fbkk12-4.fna&oh=03_AdQm1Jvc117NR91ORJsZg0ORfD7sqof-DIgZX6DFOjxUcQ&oe=64735FB4\">");
            client.println("<style>");
            client.println("body,h1,h2,h3,h4,h5 {font-family: \"Poppins\", sans-serif}");
            client.println("body {font-size:16px; overflow: hidden; }");
            client.println(".w3-half img{margin-bottom:-6px;margin-top:16px;opacity:0.8;cursor:pointer}");
            client.println(".w3-half img:hover{opacity:1}");
            client.println("::-webkit-scrollbar { width: 0px; height: 5px; }");
            client.println("::-webkit-scrollbar-thumb { background-color: rgb(114, 113, 113); border-radius: 10px; height: 200px; }");
            client.println("::-webkit-scrollbar-track { background-color: transparent; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");

            client.println("<nav class=' w3-sidebar w3-collapse w3-top w3-large' style=\"z-index:3;width:300px;font-weight:bold;background-color: #0D2329;\" id=\"mySidebar\" ><br>");
            client.println("<a href=\"javascript:void(0)\" onclick=\"w3_close()\" class=\"w3-button w3-hide-large w3-display-topleft\" style=\"width:100%;font-size:22px; background-color: #FF914D;\">Back</a>");
            client.println("<img src ='https://i.ibb.co/qxSWpkr/Emoplanter.gif?fbclid=IwAR2ArWEsMDtK0S20wgdf93GVVdcaR1fePebwKdePVanqr_MCtC40CSTo7rs' width=\"350px\" style=\"margin-left: -35px; padding-top: 30px\">");
            client.println("<div class=\"w3-container\"></div>");
            client.println("<div class=\"w3-bar-block\" style=\"color: white; margin-left: 8px;\">");
            client.println("<a href=\"#\" onclick=\"w3_close()\" class=\"w3-bar-item w3-button w3-hover-white\" >Home</a>");
            client.println("<a href=\"#sample\" onclick=\"w3_close()\" class=\"w3-bar-item w3-button w3-hover-white\">Work Samples</a>");
            client.println("<a href=\"#emotion\" onclick=\"w3_close()\" class=\"w3-bar-item w3-button w3-hover-white\">Realtime Emotion</a>");
            client.println("<a href=\"#dev\" onclick=\"w3_close()\" class=\"w3-bar-item w3-button w3-hover-white\">Developers</a>");
            client.println("<a href=\"#contact\" onclick=\"w3_close()\" class=\"w3-bar-item w3-button w3-hover-white\">Contact</a>");
            client.println("<br>");
            client.println("<br>");
            client.println("<p style=\"color: #D1FF5D; margin-left: 15px;\">01076108 Circuits and Electronics in Practice of<br>Computer Enginneering</p>");
            client.println("<p style=\"color: #FF914D; margin-left: 15px;\">KMITL, 1 Chalong Krung, 1 Alley, Lat Krabang, Bangkok 10520</p>");
            client.println("<p style=\"color: white; margin-left: 15px;\">@Copyright.Choll.Khris");
            client.println("</div>");
            client.println("</nav>");

            client.println("<header class=\"w3-container w3-top w3-hide-large w3-xlarge w3-padding\" style=\"background-color: #0D2329;\">");
            client.println("<a href=\"javascript:void(0)\" class=\"w3-button w3-margin-right\" style=\"color: #FF914D;\" onclick=\"w3_open()\">☰</a>");
            client.println("<span style=\"color: white;\">Menu</span>");
            client.println("</header>");

            client.println("<div class=\"w3-overlay w3-hide-large\" onclick=\"w3_close()\" style=\"cursor:pointer\" title=\"close side menu\" id=\"myOverlay\"></div>");

            client.println("<div class=\"w3-main\" style=\"margin-left:340px;margin-right:40px;\">");
            client.println("<div class=\"w3-container\" style=\"margin-top:20px;color: #0D2329;\" id=\"#\">");
            client.println("<br>");
            client.println("<h1 class=\"w3-jumbo\" style=\"color: #0D2329\"><b>Emoplanter <span style=\"font-size: 30px; color: #05664F;\">The IOT Emotional Plant Pot</span></b></h1>");
            client.println("<hr style=\"width:50px;border:5px solid #FF914D\" class=\"w3-round\">");
            client.println("<p><b style=\"color: #0D2329;\">Work Inspiration.</b></p>");
            client.println("<p>Cactus has become very popular nowadays for decoration because it is a commonly found ornamental plant on desks, computer screens, or in front of houses, and it is a plant that is easy to care for and can tolerate different environmental conditions. It also takes a long time to grow, making it suitable for decoration in a pot as it doesn't need to be repotted as often as other plants. In addition, the temperature in our homes tends to be warm all the time, making it easy to take care of. However, how do we know what our cactus needs, what it lacks, or not, including cultivating other plants? We know that plants have life, but how do we know how they feel? We see the benefits and importance of it, so we are interested in developing EmoPlanter.</p>");
            client.println("<p> <b style=\"color: #0D2329;\">EmoPlanter:</b> A pot that can reveal the emotions and needs of cactus.</p>");
            client.println("<p><b style=\"color: #0D2329;\">Emo:</b> Emotion</p>");
            client.println("<p><b style=\"color: #0D2329;\">Planter:</b> Planting equipment, seedling equipment, and care equipment.</p></p><br>");
            client.println("<h1 class=\"w3-xxxlarge\" style=\"color: #0D2329\" id=\"sample\"><b>Work Samples</b></h1>");
            client.println("<hr style=\"width:50px;border:5px solid #FF914D\" class=\"w3-round\">");
            client.println("</div>");

            client.println("<div class=\"w3-row-padding\">");
            client.println("<div class=\"w3-half\">");
            client.println("<img src=\"https://scontent.fbkk6-2.fna.fbcdn.net/v/t1.15752-9/344300559_195079396677974_2470272347172444815_n.jpg?_nc_cat=101&ccb=1-7&_nc_sid=ae9488&_nc_eui2=AeG2xieyk3tg8UlkwlUsL-YNi5bSdRKFRqeLltJ1EoVGp1MHFCuzFU0LWgFOixIfOTX_gAAGAxvUeJTHWLvCVote&_nc_ohc=QaTt3t8h70wAX-CyNQE&_nc_ht=scontent.fbkk6-2.fna&oh=03_AdR1CZu2V3oBITVEnILcU4C2qaIwlYtMjBJaft40hwgYSA&oe=6476AC4E\" style=\"width:100%\" onclick=\"onClick(this)\" alt=\"Circuit Project\">");
            client.println("<img src=\"https://scontent.fbkk6-2.fna.fbcdn.net/v/t1.15752-9/344263415_1215127782496728_2217792661814427857_n.jpg?_nc_cat=101&ccb=1-7&_nc_sid=ae9488&_nc_eui2=AeHGp_xhUSOBL4fJPRYX8NYjblOSihueDDBuU5KKG54MMKCCg7nNpB3-8lLyKNGa_uLbtM1gwsnVx565ml4PcuO1&_nc_ohc=Fl39EG_mbucAX_KUuQK&_nc_ht=scontent.fbkk6-2.fna&oh=03_AdTQlU9K2vhathusr8gNEqpnrO00Orae2hnp_6WKfiuBQQ&oe=6476A604\" style=\"width:100%\" onclick=\"onClick(this)\" alt=\"Feeling fresh\">");
            client.println("</div>");
            client.println("<div class=\"w3-half\">");
            client.println("<img src=\"https://scontent.fbkk3-6.fna.fbcdn.net/v/t1.15752-9/340764979_580619037499192_197213629036979324_n.png?_nc_cat=106&ccb=1-7&_nc_sid=ae9488&_nc_eui2=AeHaS1z5Z8sMjE0Ylc2K5GHH4ARQrZbccPXgBFCtltxw9QwIm4r-EAbtJiV_PtdFPm-U_ChFrbmWGIXJ84f-bz-Q&_nc_ohc=HlhuYYoYqn0AX8Vi59k&_nc_ht=scontent.fbkk3-6.fna&oh=03_AdQU8yTybSvyQcDT2-eRKjgFSVBlE7TzTQWTb32UYutgdQ&oe=6476D1B6\" style=\"width:100%\" onclick=\"onClick(this)\" alt=\"Circuit Project\">");
            client.println("<img src=\"https://scontent.fbkk4-2.fna.fbcdn.net/v/t1.15752-9/344029756_275967631452571_8943238359895345502_n.png?_nc_cat=110&ccb=1-7&_nc_sid=ae9488&_nc_eui2=AeESV7Tou2NMVUCqD9T0_DBLMXbOfREWVUAxds59ERZVQJffxmE_sdF9hlzVliMkQYkUIFZtFU3kdl0jE8bT_4v6&_nc_ohc=6FkLB-oAZtIAX_fHq0R&_nc_ht=scontent.fbkk4-2.fna&oh=03_AdSPTdpo8b633EPjxN4TceJryUeSDmvTJv2sKWErl6DRDQ&oe=6476D117\" style=\"width:100%\" onclick=\"onClick(this)\" alt=\"Circuit Project\">");
            client.println("</div>");
            client.println("</div>");

            client.println("<div id=\"modal01\" class=\"w3-modal w3-black\" style=\"padding-top:0\" onclick=\"this.style.display='none'\">");
            client.println("<span class=\"w3-button w3-black w3-xxlarge w3-display-topright\">×</span>");
            client.println("<div class=\"w3-modal-content w3-animate-zoom w3-center w3-transparent w3-padding-64\">");
            client.println("<img id=\"img01\" class=\"w3-image\">");
            client.println("<p id=\"caption\"></p>");
            client.println("</div>");
            client.println("</div>");

            client.println("<div class=\"w3-container\" id=\"emotion\" style=\"margin-top:75px\">");
            client.println("<h1 class=\"w3-xxxlarge\" style=\"color: #0D2329;\"><b>Realtime Emotion</b></h1>");
            client.println("<hr style=\"width:50px;border:5px solid #FF914D\" class=\"w3-round\">");
            client.println("<p>These values are being read from sensors those inside our planter.</p>");
            client.println("</div>");

            client.println("<div class=\"w3-row-padding\">");
            client.println("<div class=\"w3-half w3-margin-bottom\">");
            client.println("<ul class=\"w3-ul w3-light-grey w3-center\">");
            client.println("<li class=\"w3-xlarge w3-padding-32\" style=\"background-color: #0D2329;\"><span style=\"color: white;\">Quality Values</span></li>");
            if (moisture < 3 || moisture > 6)
            {
              client.println("<li class=\"w3-padding-32 \"><h2>Soil Moisture</h2></li>");
            }
            else
            {
              client.println("<li class=\"w3-padding-16 \"><h2>Soil Moisture</h2></li>");
            }
            client.println("<li class=\"w3-padding-16\"><h2>Temperature</h2></li>");
            client.println("<li class=\"w3-padding-48\"><h2>Light Dependent Resistor</h2></li>");
            if (moisture < 3 || moisture > 6)
            {
              client.println("<li class=\"w3-padding-32\"><h2>Planter's emotion<h2>");
            }
            else
            {
              client.println("<li class=\"w3-padding-16\"><h2>Planter's emotion<h2>");
            }
            client.println("<img src=\"https://media1.giphy.com/media/YpTSRFbHGuRfy1goOb/giphy.gif?cid=6c09b952f4e0a27e55a96cb115edb685131db6663cee8621&ep=v1_internal_gifs_gifId&rid=giphy.gif&ct=s\" width=\"50px\" alt=\"\" style=\"margin-bottom: 10px;\"><span style=\"color: green; font-size: large;\">Happy</span><br>");
            client.println("<img src=\"https://i.pinimg.com/originals/15/d3/a2/15d3a2ebd771371393fcdc3adc83876b.gif\" width=\"50px\" alt=\"\" style=\"margin-bottom: 10px;\"><span style=\"color: #87a96b; font-size: large;\">Netural</span><br>");
            client.println("<img src=\"https://i.pinimg.com/originals/7d/82/03/7d820389353cdd8a21f533416544a617.gif\" width=\"50px\" alt=\"\" style=\"margin-bottom: 10px;\"><span style=\"color: #FF914D; font-size: large;\">Sick</span><br>");
            client.println("<img src=\"https://i.pinimg.com/originals/be/d0/db/bed0dbbfebceab064d0a0b4c6df5ac22.gif\" width=\"50px\" alt=\"\" style=\"margin-bottom: 10px;\"><span style=\"color: red; font-size: large;\">Sad</span>");
            client.println("</li>");
            client.println("</ul>");
            client.println("</div>");

            client.println("<div class=\"w3-half\">");
            client.println("<ul class=\"w3-ul w3-light-grey w3-center\">");
            client.println("<li class=\"w3-xlarge w3-padding-32\" style=\"background-color: #05664F;\"><span style=\"color: white;\">Realtime Data From Planter</span></li>");
            client.println("<li class=\"w3-padding-16\"><h2>");
            client.println(moisture);
            if (moisture < 3)
            {
              client.println(" %</h2><h4 id=\"count-down\"></h4></ li>");
            }
            else
            {
              client.println(" %</h2></li>");
            }
            client.println("<li class=\"w3-padding-16\"><h2>");
            client.println(temp);
            client.println(" °C</h2></li>");
            client.println("<li class=\"w3-padding-16\"><h2>");
            client.println(val);
            if ((temp >= 27 && temp <= 32) && (val >= 0 && val <= 280))
            {
              client.println("</h2><h4 id=\"countup\"></h4></li>");
            }
            else
            {
              client.println("</h2><h4>Please Give me a Sunlight</h4></li>");
            }
            if ((moisture >= 3 && moisture <= 6) && (temp >= 27 && temp <= 32) && (val >= 0 && val <= 280))
            {
              client.println("<img src=\"https://media1.giphy.com/media/YpTSRFbHGuRfy1goOb/giphy.gif?cid=6c09b952f4e0a27e55a96cb115edb685131db6663cee8621&ep=v1_internal_gifs_gifId&rid=giphy.gif&ct=s\" width=\"280px\" alt=\"\"><h2 style=\"color: green;\">Happy</h2><br>");
            }
            else if ((moisture < 3 || moisture > 6) && (temp >= 27 && temp <= 32) && (val >= 0 && val <= 280) ||
                     ((moisture >= 3 && moisture <= 6) && (temp < 27 || temp > 32) && (val >= 0 && val <= 280)) ||
                     ((moisture >= 3 && moisture <= 6) && (temp >= 27 && temp <= 32) && (val < 0 || val > 280)))
            {
              client.println("<img src=\"https://i.pinimg.com/originals/15/d3/a2/15d3a2ebd771371393fcdc3adc83876b.gif\" width=\"280px\" alt=\"\" style=\"margin-bottom: 10px;\"><h2 style=\"color: #87a96b;\">Netural</h2><br>");
            }
            else if ((moisture < 3 || moisture > 6) && (temp < 27 || temp > 32) && (val >= 0 && val <= 280) ||
                     ((moisture >= 3 && moisture <= 6) && (temp < 27 || temp > 32) && (val < 0 || val > 280)) ||
                     ((moisture < 3 || moisture > 6) && (temp >= 27 && temp <= 32) && (val < 0 || val > 280)))
            {
              client.println("<img src=\"https://i.pinimg.com/originals/7d/82/03/7d820389353cdd8a21f533416544a617.gif\" width=\"280px\" alt=\"\" style=\"margin-bottom: 10px;\"><h2 style=\"color: #FF9D;\">Sick</h2><br>");
            }
            else if ((moisture < 3 || moisture > 6) && (temp < 27 || temp > 32) && (val < 0 || val > 280))
            {
              client.println("<img src=\"https://i.pinimg.com/originals/be/d0/db/bed0dbbfebceab064d0a0b4c6df5ac22.gif\" width=\"280px\" alt=\"\" style=\"margin-bottom: 10px;\"><h2 style=\"color: red;\">Sad</h2><br>");
            }
            client.println("<li class=\"w3-padding-16\"></li>");
            client.println("</ul>");
            client.println("</div>");
            client.println("</div>");
            client.println("<div class=\"w3-container\" id=\"dev\" style=\"margin-top:75px\">");
            client.println("<h1 class=\"w3-xxxlarge\" style=\"color: #0D2329;\"><b>Developers</b></h1>");
            client.println("<hr style=\"width:50px;border:5px solid #FF914D\" class=\"w3-round\">");
            client.println("<p>The best team in the world.</p>");
            client.println("<p><b>Circuit soilders </b>:</p>");
            client.println("</div>");

            client.println("<div class=\"w3-row-padding w3-grayscale\">");
            client.println("<div class=\"w3-col m4 w3-margin-bottom\">");
            client.println("<div class=\"w3-light-grey\">");
            client.println("<img src=\"https://scontent.fbkk12-2.fna.fbcdn.net/v/t39.30808-6/218435537_1197339287396112_5375186524094989009_n.jpg?_nc_cat=104&ccb=1-7&_nc_sid=174925&_nc_eui2=AeFogsYfaYp1LSCIMfEw91Lw01g7scw_3kDTWDuxzD_eQHiCrfuagKW0RsJlJSpKa3HHWQb2Xcn0ziv2fSNlZwAP&_nc_ohc=pCTASS-4RTgAX8-eICS&_nc_ht=scontent.fbkk12-2.fna&oh=00_AfBi5c8jkbGe2AtZ0ajKULpt5SMcpfTSkAaAmjQgTYCyfw&oe=645015EB\" alt=\"Choll\" style=\"width:100%\">");
            client.println("<div class=\"w3-container\">");
            client.println("<h3>65010195 | Chollasak Anuwareepong</h3>");
            client.println("<p class=\"w3-opacity\">CE61 Student</p>");
            client.println("<p>Circuits and Electronics | MCU Programming</p>");
            client.println("</div>");
            client.println("</div>");
            client.println("</div>");
            client.println("<div class=\"w3-col m4 w3-margin-bottom\">");
            client.println("<div class=\"w3-light-grey\">");
            client.println("<img src=\"https://avatars.githubusercontent.com/u/84142253?v=4\" alt=\"Khris\" style=\"width:100%\">");
            client.println("<div class=\"w3-container\">");
            client.println("<h3>65010107 | Khris Bharmmano</h3>");
            client.println("<p class=\"w3-opacity\">CE61 Student </p>");
            client.println("<p>Circuits and Electronics | MCU Programming</p>");
            client.println("</div>");
            client.println("</div>");
            client.println("</div>");
            client.println("<div class=\"w3-container\" id=\"contact\" style=\"margin-top:75px\">");
            client.println("<h1 class=\"w3-xxxlarge\" style=\"color: #0D2329;\"><b>Contact</b></h1>");
            client.println("<hr style=\"width:50px;border:5px solid #FF914D\" class=\"w3-round\">");
            client.println("<p>Do you want us to style your home? Fill out the form and fill me in with the details :) We love meeting new people!</p>");
            client.println("<h3>65010195 | Chollasak Anuwareepong: <b>65010195@kmitl.ac.th</b></h3>");
            client.println("<h3>65010107 | Khris Bharmmano: <b>65010107@kmitl.ac.th</b></h3>");
            client.println("</div>");
            client.println("</div>");

            client.println("<div class=\"w3-light-grey w3-container w3-padding-32\" style=\"margin-top:75px;padding-right:58px\"><p class=\"w3-right\">Powered by <a href=\"https://www.ce.kmitl.ac.th/\" title=\"W3.CSS\" target=\"_blank\" class=\"w3-hover-opacity\">ce.kmitl</a></p></div>");

            client.println("<script>");
            client.println("function w3_open() {");
            client.println("document.getElementById(\"mySidebar\").style.display = \"block\";");
            client.println("document.getElementById(\"myOverlay\").style.display = \"block\";");
            client.println("}");
            client.println("");
            client.println("function w3_close() {");
            client.println("document.getElementById(\"mySidebar\").style.display = \"none\";");
            client.println("document.getElementById(\"myOverlay\").style.display = \"none\";");
            client.println("}");
            client.println("");
            client.println("function onClick(element) {");
            client.println("document.getElementById(\"img01\").src = element.src;");
            client.println("document.getElementById(\"modal01\").style.display = \"block\";");
            client.println("var captionText = document.getElementById(\"caption\");");
            client.println("captionText.innerHTML = element.alt;");
            client.println("}");
            client.println("var now = new Date().getTime();");
            client.println("var countDownDate = new Date(now + (7 * 24 * 60 * 60 * 1000)).getTime();");
            if (moisture < 3 || moisture > 6)
            {
              client.println("var x = setInterval(function() {");
              client.println("var now = new Date().getTime();");
              client.println("var distance = countDownDate - now;");
              client.println("var days = Math.floor(distance / (1000 * 60 * 60 * 24));");
              client.println("var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));");
              client.println("var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));");
              client.println("var seconds = Math.floor((distance % (1000 * 60)) / 1000);");
              client.println("document.getElementById(\"count-down\").innerHTML = \"Please water before : \" + days + \"d \" + hours + \"h \" + minutes + \"m \" + seconds + \"s \";");
              client.println("if (distance < 0) {");
              client.println("clearInterval(x);");
              client.println("document.getElementById(\"count-down\").innerHTML = \"Please Recovery\";");
              client.println("}");
              client.println("}, 1000);");
            }
            if ((temp >= 27 && temp <= 32) && (val >= 0 && val <= 280))
            {
              client.println("let countUpTime = 0;");
              client.println("");
              client.println("const x = setInterval(function() {");
              client.println("");
              client.println("  countUpTime += 1000;");
              client.println("");
              client.println("  // Time calculations for hours, minutes and seconds");
              client.println("  const hours = Math.floor(countUpTime / (1000 * 60 * 60));");
              client.println("  const minutes = Math.floor((countUpTime % (1000 * 60 * 60)) / (1000 * 60));");
              client.println("  const seconds = Math.floor((countUpTime % (1000 * 60)) / 1000);");
              client.println("");
              client.println("  // If the elapsed time is more than 4 hours (in milliseconds), show \"Time's up!\"");
              client.println("  if (countUpTime >= 10 * 1000) {");
              client.println("    clearInterval(x);");
              client.println("    document.getElementById(\"countup\").innerHTML = \"I'm Full !! Please move me into the house\";");
              client.println("  }");
              client.println("  else {");
              client.println("    // Display the result in the element with id=\"countup\"");
              client.println("    document.getElementById(\"countup\").innerHTML = \"Eating : \" + hours + \"h \" + minutes + \"m \" + seconds + \"s \";");
              client.println("  }");
              client.println("");
              client.println("}, 1000);");
            }
            client.println("</script>");
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}