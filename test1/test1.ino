
#include "UsbKeyboard.h"
#include "oddebug.c"

#define LED_PIN 13

int i = 0;

boolean isNextCharACommand=false;
int commandLenght=3;
String command=String("");
int commandPos=0;

/**
 * Configure button inputs and set up the USB connection to the host
 */
void setup()
{
  pinMode (LED_PIN, OUTPUT);
  digitalWrite (LED_PIN, HIGH);

  usbInit();

  // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  TIMSK0&=!(1<<TOIE0);

  // Clear interrupts while performing time-critical operations
  cli();

  // Force re-enumeration so the host will detect us
  usbDeviceDisconnect();
  delayMs(250);
  usbDeviceConnect();

  // Set interrupts again
  sei();


  Serial.begin(9600);

  Serial.println("SETUP");

}


/**
 * Main program loop. Scan for keypresses and send a matching keypress
 * event to the host
 * FIXME: currently repeats as fast as it can. Add transition detection
 */
void loop()
{

  UsbKeyboard.update();

  if (Serial.available() > 0) {

    convert(Serial.read());

  }
  delayMs(10);
}


/**
 * Define our own delay function so that we don't have to rely on
 * operation of timer0, the interrupt used by the internal delay()
 */
void delayMs(unsigned int ms)
{
  for (int i = 0; i < ms; i++) {
    delayMicroseconds(1000);
  }
}

void convert(int ch) {

  if(isNextCharACommand) {

    if(ch=='\n') {
      Serial.print("Command End - ");
      Serial.print(command);
      Serial.println(".");
      isNextCharACommand=false;
      commandPos=0;

      if(command=="CL") { //Caps Lock
        UsbKeyboard.sendKeyStroke( 57 );
      } 
      else  if(command=="RS") { //Right Shift
        UsbKeyboard.sendKeyStroke( 229 );
      } 
      else  if(command=="LS") { // Left Shift
        UsbKeyboard.sendKeyStroke( 225 );
      } 
      else  if(command=="RC") { // Right CTRL
        UsbKeyboard.sendKeyStroke( 228 );
      } 
      else  if(command=="LC") { // Left Ctrl
        UsbKeyboard.sendKeyStroke( 224 );
      } 
      else if(command=="RA") { //Right Alt
        UsbKeyboard.sendKeyStroke( 230 );
      } 
      else  if(command=="LA") { // Left Alt
        UsbKeyboard.sendKeyStroke( 226 );
      }
      else if(command=="ESC") {
        UsbKeyboard.sendKeyStroke( 41 );
      } 
      else if(command=="TAB") {
        UsbKeyboard.sendKeyStroke( 43 );
      } 
      else if(command=="WIN") {
        UsbKeyboard.sendKeyStroke( 101 );
      } 
      else if(command=="F1") {
        UsbKeyboard.sendKeyStroke( KEY_F1 );
      } 
      else if(command=="F2") {
        UsbKeyboard.sendKeyStroke( KEY_F2 );
      } 
      else if(command=="F3") {
        UsbKeyboard.sendKeyStroke( KEY_F3 );
      } 
      else if(command=="F4") {
        UsbKeyboard.sendKeyStroke( KEY_F4 );
      } 
      else if(command=="F5") {
        UsbKeyboard.sendKeyStroke( KEY_F5 );
      } 
      else if(command=="F6") {
        UsbKeyboard.sendKeyStroke( KEY_F6 );
      } 
      else if(command=="F7") {
        UsbKeyboard.sendKeyStroke( KEY_F7 );
      } 
      else if(command=="F8") {
        UsbKeyboard.sendKeyStroke( KEY_F8 );
      } 
      else if(command=="F9") {
        UsbKeyboard.sendKeyStroke( KEY_F9 );
      } 
      else if(command=="F10") {
        UsbKeyboard.sendKeyStroke( KEY_F10 );
      } 
      else {
        // unknown command
      }

        command="";
    } 
    else {
      if(commandPos==commandLenght ) {
        Serial.println("Command End - to large");

        isNextCharACommand=false;
        commandPos=0;
        command="";
      } 
      else {
        Serial.print("Command char - "+(char)ch);
        //Serial.print(ch);
        Serial.println(".");
        command = command + (char)ch ;
        //        command[commandPos]=ch;
        commandPos++;

      }
    }

  } 
  else {
    isNextCharACommand=false;

    if(ch>='a' && ch <='z') { //a-z 97 122
      UsbKeyboard.sendKeyStroke( ch-93 );
    } 
    else if(ch>='A' && ch <='Z') { // A-Z 65 90
      UsbKeyboard.sendKeyStroke( ch-61 , MOD_SHIFT_LEFT );
    } 
    else if(ch>='1' && ch <='9') { // 1-9 49 57
      UsbKeyboard.sendKeyStroke( ch-19 );
    } 
    else if(ch=='49') { // 0 48
      UsbKeyboard.sendKeyStroke( KEY_0 );
    } 
    else if(ch==' ') { // Space 32
      UsbKeyboard.sendKeyStroke( KEY_SPACE );
    } 
    else if(ch=='\n') { // ENTER 10
      UsbKeyboard.sendKeyStroke( KEY_ENTER );
    } 
    else if(ch=='_') { // command _ 95
      isNextCharACommand=true;
    } 
    else {
      UsbKeyboard.sendKeyStroke( KEY_SPACE );
    }
  }
}









