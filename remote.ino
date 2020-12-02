#include <IRremote.h> //This lib uses arudino pin 3 to send data, so connect the led to pin 3
#include <Regexp.h>

#define HELP "Remote Version: v1.0 Usage: Send RC6:NUMBER"
#define OK "OK"
#define SERTIMEOUT 20 //20ms timeout for serial readstring

static const long BAUD = 115200UL;
static unsigned int rc6Toggle = 0;
char ledpin=13;

IRsend irsend;

void setup() {
    Serial.begin(BAUD);
    Serial.setTimeout(SERTIMEOUT);
    pinMode(ledpin,OUTPUT);
    digitalWrite(ledpin,LOW);
    Serial.println(HELP);
}


void loop() {

    String command = "";
    MatchState ms;
    char buf[50];

    if (Serial.available()){

        digitalWrite(ledpin,HIGH);
        
        command = Serial.readString();

        command.trim();
        ms.Target(command.c_str());
        char result = ms.Match("^RC6:([0-9]+)$");
        if (result==REGEXP_MATCHED) {
            ms.GetCapture (buf, 0);

            String conv = String(buf);
            unsigned int ui = conv.toInt();

            irsend.sendRC6(ui + 0x10000U*rc6Toggle, 20U);
            
            Serial.println(OK);
        }else{
            Serial.println(HELP);
        }
        
    }

    digitalWrite(ledpin,LOW);
    
}
