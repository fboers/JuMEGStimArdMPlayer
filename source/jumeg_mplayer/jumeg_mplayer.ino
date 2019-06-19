/*---------------------------------------------
 * Arduino MEGA 
 * playing wave files from SD card 
 * sending event eventcode TTL signals 
 * NBS Presentation Version using  in NBS out_port.send_string()
 * 
 * 
 * --------------------------------------------
 * @author   F.Boers (f.boers@fz-juelich.de)
 * FZJ-INM4-MEG
 * --- v1.00  14.06.2019
 *---------------------------------------------
 * TTL Output 
 * eventcode 0-255 
 * ---> Port A1-8  Pin 33,34,35,36,37,38,39,40
*/
  
#define BAUDRATE 115200

//--- playing tones => using  Adafruit Music Maker Shield; VS1053 chip
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <string.h>
#include <DirectIO.h>

//--- timer event update state machine
#include <TimerFive.h>
#define TIMER5_INTERVALL_US 500

//--- Music Player/ maker shield
#include <JuMEG_VS1053.h> // sub class of <Adafruit_VS1053.h>
#include <JuMEG_MPlayer.h>
JuMEG_MPlayer MP;

//--- MusicPlayer volume
#define PRESENT_ON_BOTH   0
#define PRESENT_ON_LEFT   1
#define PRESENT_ON_RIGHT  2
#define DEFAULT_VOLUME   20
#define DEFAULT_EVENTCODE_DURATION_MS 100
#define DEFAULT_TONE_DELAY_MS 1500
#define SERIAL_TIMEOUT_MS 50
//---
#define START_CODE       128
#define TRIAL_START_CODE 254
#define TRIAL_STOP_CODE  255

// setup
#define SETUP_PLAY_FILES    1
#define SETUP_STATE_MACHINE 0  // use timer IRQ not jet
#define RUN_TEST            1

// serial error return code
#define ERROR_CODE  255
#define SERIAL_CODE_STOP  99

// MusicPlayer shield settings  
#define SHIELD_RESET  -1     // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3     // VS1053 Data request, ideally an Interrupt pin

//Adafruit_VS1053_FilePlayer ArdMP = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
JuMEG_VS1053_FilePlayer ArdMP = JuMEG_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

/* -------------------------------
 * present tone binaural, send eventcode 
 * 111,0,1,0  => t001,code 1
 * 111,1,2,0  => t002,code 2
 * -------------------------------
 * set value 
 * 131,21,vol,0  => set volume both
 * 131,22,vol,0  => set volume left
 * 131,22,vol,0  => set volume right
 * --------------------------------
 * 131,31,1,0    => set serial send code
 * 131,32,1,0    => set serial verbose
 * 
 */

//---------------------------------
// trial definition
//---------------------------------
struct SER_CMD{
 unsigned long a;
 unsigned long b;
 unsigned long c;
 unsigned long d;
};

bool _send_serial_code = false;
bool _sequence         = false;
bool _is_seq_running   = false;

unsigned long PGRStartTime;
unsigned long time_ms;
unsigned long dtime_ms;


struct CMD_KEYS{
// --- init/tests
static const unsigned long init_pgr           = 100;
static const unsigned long test_play_files    = 101;
static const unsigned long test_eventcodes    = 102;
//--- present wavefile
static const unsigned long present_file_both  = 111;
static const unsigned long present_file_left  = 112;
static const unsigned long present_file_right = 113;
//---
static const unsigned long present            = 121; 
//--- set/get
static const unsigned long set_val            = 131;
static const unsigned long get_val            = 141;

//--- get/set 4 unsigned long
static const unsigned long eventcode_duration = 11;

//--- music player
static const unsigned long volume_both        = 21;
static const unsigned long volume_left        = 22;
static const unsigned long volume_right       = 23; 

//--- nbs presentation send code 
static const unsigned long serial_send_code   = 31; 
static const unsigned long serial_verbose     = 32; 
};

static const CMD_KEYS CmdKeys;

//---------------------------------------------------------------
// pgr_time
//---------------------------------------------------------------
unsigned long pgr_time()
{return millis() - PGRStartTime;} 
//---------------------------------------------------------------
// pgr_time
//---------------------------------------------------------------
unsigned long pgr_time(unsigned long dt)
{return millis() - PGRStartTime - dt;} 

//---------------------------------------------
// serial_send_code  
//---------------------------------------------
void serial_send_code(uint8_t code){
  if ( getSendSerialCode() ){Serial.write(code);}; 
}
//---------------------------------
// file_info
//---------------------------------
void file_info( uint8_t idx,uint8_t code){
    Serial.print(" --> present file: idx: ");
    Serial.print( idx);
    Serial.print(" --> name: ");
    Serial.print( MP.getFileName(idx));
    Serial.print(" code: ");
    Serial.println(code);
}

//---------------------------------
// apply_present_file
//---------------------------------
void apply_present_file( uint8_t idx,uint8_t code,uint8_t type){
   
  serial_send_code(code); 
  // if ( getVerbose() ){ file_info(idx,code); }
   
  if (type == PRESENT_ON_LEFT)      { MP.present_left(idx,code); }
  else if (type == PRESENT_ON_RIGHT){ MP.present_right(idx,code);}
  else { MP.present(idx,code); }

 // unsigned long dt  = pgr_time();
 // serial_send_code(SERIAL_CODE_STOP);
  serial_send_code(code); 
 
 } // end of apply_present_file


//---------------------------------------------------------------
// setup_state_machine
//---------------------------------------------------------------
void setup_state_machine(){
 //--- Timer5  
    Timer5.initialize(TIMER5_INTERVALL_US);         // initialize timer1, and set a 1/2 second period
    Timer5.disablePwm(44);
    Timer5.disablePwm(45);
    Timer5.disablePwm(46);
    Timer5.attachInterrupt(state_machine_update);  // attaches callback() as a timer overflow interrupt

} // end of setup_state_machine1

//---------------------------------------------
// state_machine_update
//---------------------------------------------
void state_machine_update(){
   
} // end of state_machine_update

//---------------------------------------------------------------
// test_evencodes
//---------------------------------------------------------------
void test_eventcodes(){
     for ( uint8_t i=0; i< 4;i++)
       {      
        MP.sendEventCode(2 << i);
        delay(DEFAULT_EVENTCODE_DURATION_MS);
       }
     MP.sendEventCode(0);           
 }// test_eventcode

//---------------------------------------------------------------
// test_play_files
//---------------------------------------------------------------
void test_play_files() {
     
     for (uint8_t i=0; i< MP.getNumberOfTones() ; i++)
       {
         apply_present_file(i,i+1,PRESENT_ON_BOTH);
         delay(DEFAULT_TONE_DELAY_MS);
       } 
        
     if ( getVerbose() ) { Serial.println(F("\n DONE")); }    
      
} // end of test_play_files

//---------------------------------------------------------------
// setup_play_files
//---------------------------------------------------------------
void setup_play_files(){

  if ( getVerbose() ){ Serial.println(F("\n-> Setup Adafruit Music Maker; VS1053 chip + SD card + GPIO"));}

//--- initialise the cue tone cls & music maker
  MP.begin(&ArdMP);
  MP.setPgrStartTime(PGRStartTime);
  MP.setVolume(DEFAULT_VOLUME);  
  
  if ( getVerbose )
   { 
     Serial.println(F("Done setup play tone"));
   } // if verbose
   
} // end of setup_play_tone

//---------------------------------------------------------------
// setVerbose
//---------------------------------------------------------------
void setVerbose(uint8_t v){
  MP.setVerbose( bool(v) ); 
}// end of setVerbose

//---------------------------------------------------------------
// getVerbose
//---------------------------------------------------------------
bool getVerbose(void){
  return MP.getVerbose(); 
}// end of getVerbose

//---------------------------------------------------------------
// setSendSerialCode
//---------------------------------------------------------------
void setSendSerialCode(bool v){
 _send_serial_code = v;
}// end of setSendSerialCode
//---------------------------------------------------------------
// getSendSerialCode
//---------------------------------------------------------------
bool getSendSerialCode(void){
 return _send_serial_code;
}// end of getSendSerialCode


//---------------------------------------------------------------
// init_program
//---------------------------------------------------------------
void init_program(){
  
  PGRStartTime = millis();
    
  if (!SD.begin(CARDCS)) {
     if ( getVerbose() ){Serial.println(F("SD failed, or not present"));}
     serial_send_code(ERROR_CODE);
     while (1);  // don't do anything more
    }   
  
  //--- SD card list files
  if (getVerbose()){ printDirectory(SD.open("/"), 0); }
  
  if ( SETUP_PLAY_FILES    > 0 ) {setup_play_files();}
  if ( SETUP_STATE_MACHINE > 0 ) {setup_state_machine();}
  if ( RUN_TEST > 0)    
     {
      test_play_files();
      test_eventcodes();
     }
     
}// end of init_program



//---------------------------------------------------------------
// setup
//---------------------------------------------------------------
void setup() {
   
 Serial.begin(BAUDRATE);
 Serial.setTimeout(SERIAL_TIMEOUT_MS);
 Serial.flush();
  
//--- init
 init_program();

} // end of setup

//---------------------------------------------
// MAIN LOOP  
//---------------------------------------------
void loop() {
     
     check_serialEvent();
    
}  // end of loop


//---------------------------------------------
// process serial events  
//---------------------------------------------
void check_serialEvent(){  
unsigned  long t0;
SER_CMD cmd; 
 
  while ( Serial.available() )
   {
    // t0=micros();
    cmd.a = Serial.parseInt(); 
  //--- tone 
    if( cmd.a == CmdKeys.present_file_both )
      {
       cmd.b = Serial.parseInt();
       cmd.c = Serial.parseInt();
       //Serial.println( "both in ");
       //Serial.println( micros()-t0 ); 
       //apply_present_file(uint8_t(cmd.b), uint8_t(cmd.c) & 0XFF,PRESENT_ON_BOTH);
       apply_present_file(uint8_t(cmd.b), uint8_t(cmd.c),PRESENT_ON_BOTH);
  
       //Serial.println( "both out");
       //Serial.println( micros()-t0 ); 
      }
    else if( cmd.a == CmdKeys.present_file_left )
      {
       cmd.b = Serial.parseInt();
       cmd.c = Serial.parseInt();
      // apply_present_file(uint8_t(cmd.b), uint8_t(cmd.c) & 0XFF,PRESENT_ON_LEFT);
       apply_present_file(uint8_t(cmd.b), uint8_t(cmd.c),PRESENT_ON_LEFT);
  
       //Serial.println( "left");
       //Serial.println( micros()-t0 );
      
      }
    else if( cmd.a == CmdKeys.present_file_right)
      {
       cmd.b = Serial.parseInt();
       cmd.c = Serial.parseInt();
       //apply_present_file(uint8_t(cmd.b), uint8_t(cmd.c) & 0XFF,PRESENT_ON_RIGHT);
       apply_present_file(uint8_t(cmd.b), uint8_t(cmd.c),PRESENT_ON_RIGHT);
       }
    else if( cmd.a == CmdKeys.test_play_files   ) { test_play_files(); }
    
    //else if( SerCmd.a == CmdKeys.present_eventcode  ) { }
   
    else if( cmd.a == CmdKeys.set_val)
     {
       cmd.b = Serial.parseInt();
       cmd.c = Serial.parseInt();
     //--- tone volume
       if ( cmd.b == CmdKeys.volume_left)      { MP.setVolumeLeft( cmd.c );}
       else if( cmd.b == CmdKeys.volume_right) { MP.setVolumeRight(cmd.c );}
       else if( cmd.b == CmdKeys.volume_both)  { MP.setVolume(     cmd.c );}
     //--- serial feedback
       else if( cmd.b == CmdKeys.serial_verbose )   { setVerbose( cmd.c );}
       else if( cmd.b == CmdKeys.serial_send_code ) { setSendSerialCode( uint8_t( cmd.c ) );}
 
     //--- eventcode
     // duration
     // code
   
     } // else setVal
    else if( cmd.a == CmdKeys.init_pgr       ){ init_program();    } 
    else if( cmd.a == CmdKeys.test_play_files){ test_play_files(); }  
    else if( cmd.a == CmdKeys.test_eventcodes){ test_eventcodes(); }  
    
    //Serial.read();
    Serial.flush();
    //Serial.println( micros()-t0 );
    //Serial.println("----" );
    //Serial.print(cmd.a); Serial.print(",");Serial.println(cmd.b);
   } //end while
   
 
} // check serial event


//---------------------------------------------
// printDirectory  
//---------------------------------------------
void printDirectory(File dir, int numTabs) {
   Serial.println("\n --> SD card files:");
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

