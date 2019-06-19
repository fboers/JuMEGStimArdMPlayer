/**************************************************************************/
/*! 
   JuMEG MPlayer
    reading and presenting audio wav files from SD card 
    send digital marker (TTL)
   
    Hardware:
     Arduino Mega
     MusicMaker Shield 
     SPI communication
     SD card
     Pin 5 Output      
     PortC Cue Tone Event Code

     @file     JuMEG_PlayTone.h
     @author   F.Boers (f.  
     update 07.06.2019
     v1.0  - First release
	
*/
/**************************************************************************/


#include <JuMEG_MPlayer.h>

/**************************************************************************/
/*! 
    @brief  Instantiates a new JuMEG_MusicPlayer class
*/
/**************************************************************************/

JuMEG_MPlayer::JuMEG_MPlayer(){
 }

//---------------------------------------------------------------
// begin
//---------------------------------------------------------------
void JuMEG_MPlayer::begin(JuMEG_VS1053_FilePlayer* ArdMP){
//void JuMEG_MPlayer::begin(Adafruit_VS1053_FilePlayer* ArdMP){
  
 _ArdMP = ArdMP; 
 setup();

} // end of begin

//---------------------------------------------------------------
// begin
//---------------------------------------------------------------
void JuMEG_MPlayer::begin(JuMEG_VS1053_FilePlayer* ArdMP,bool verbose){
//void JuMEG_MPlayer::begin(Adafruit_VS1053_FilePlayer* ArdMP,bool verbose){ 
 setVerbose( verbose );
_ArdMP     = ArdMP; 
 setup();

} // end of begin


//---------------------------------------------------------------
// setup_music_player
//---------------------------------------------------------------
bool JuMEG_MPlayer::setup(){

 if ( getVerbose() )
  { 
    Serial.println(F("-> Adafruit VS1053 Library Test + GPIO"));
  }
//--- initialise the music player
  _status = _ArdMP->begin(); // initialise the music player

  if ( getVerbose() )
    {  
     if ( getStatus() ){ Serial.println(F("Music player ok => VS1053 found"));}
      else{ Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));}
     } // if verbose

  ToneTrigger = LOW;
 
  if ( !SDCardIsSelected() ) 
   {
     _status = SD.begin(CARDCS);

     if (getVerbose())
      { 
        if( getStatus() ){Serial.println(F("SD card OK!"));}
         else{ Serial.println(F("SD failed, or not present"));}
       } // if verbose
     }
      else { _ArdMP_SD_isSelected = true; }
     
   delay(100);  

   if( getStatus() ){ setVolume(_ArdMPVolumeLeft,_ArdMPVolumeRight); }
 
 /* if ( SDCardIsSelected() ) 
   {
     present_both_left_left();
     delay(1000);
     present_both_right_right(); 
     delay(1000);
   }
 */
   return _status;
} // end of setup_music_player

//---------------------------------
// SDCardIsSelected
//---------------------------------
bool JuMEG_MPlayer::SDCardIsSelected(){
    return _ArdMP_SD_isSelected;
} // end of present_both_left

//---------------------------------
// getVolumeLeft
//---------------------------------
uint8_t JuMEG_MPlayer::getVolumeLeft(){
    return _ArdMPVolumeLeft;
} // end of getVolumeLeft

//---------------------------------
// getVolumeRight
//---------------------------------
 uint8_t JuMEG_MPlayer::getVolumeRight(){
    return _ArdMPVolumeRight;
} // end of getVolumeRight

//---------------------------------
// VolumeInfo
//--------------------------------- 
void JuMEG_MPlayer::VolumeInfo(char* t){
    
  if (getVerbose())
      {
        Serial.print("  -> MP set volume < ");
        Serial.print(t);
        Serial.print(" > left: ");
        Serial.print( getVolumeLeft() );
        Serial.print(" > right: ");
        Serial.println( getVolumeRight() );
      }
}      
//---------------------------------
// setVolume
//---------------------------------
 void JuMEG_MPlayer::setVolume(uint8_t v){
      _ArdMPVolumeLeft = v;
      _ArdMPVolumeRight= v;
      _ArdMP->setVolume(_ArdMPVolumeLeft,_ArdMPVolumeRight);
      VolumeInfo("Both");
 } // end of present_both_left

//---------------------------------
// setVolume
//---------------------------------
 void JuMEG_MPlayer::setVolume(uint8_t vl,uint8_t vr){
      _ArdMPVolumeLeft = vl;
      _ArdMPVolumeRight= vr;
      _ArdMP->setVolume(_ArdMPVolumeLeft,_ArdMPVolumeRight); 
      VolumeInfo("Both");
 } // end of present_both_left

//---------------------------------
// setVolumeLeft
//---------------------------------
 void JuMEG_MPlayer::setVolumeLeft(uint8_t v){
      _ArdMPVolumeLeft = v;
      _ArdMP->setVolume(_ArdMPVolumeLeft,_ArdMPVolumeRight); 
      VolumeInfo("Left");
 } // end of present_both_left

//---------------------------------
// setVolumeRight
//---------------------------------
 void JuMEG_MPlayer::setVolumeRight(uint8_t v){
      _ArdMPVolumeRight = v;
      _ArdMP->setVolume(_ArdMPVolumeLeft,_ArdMPVolumeRight); 
      VolumeInfo("Right");
 } // end of present_both_left

//---------------------------------
// present_left
//---------------------------------
 void JuMEG_MPlayer::present_left(uint8_t idx,uint8_t code){
  _ArdMPVolumeRight_save =_ArdMPVolumeRight;
  if( code <1 ) {code= idx;}
  present(idx,code,_ArdMPVolumeLeft,MP_VOLUME_MIN );
  setVolumeRight(_ArdMPVolumeRight_save);
} // end of present_both_left

//---------------------------------
// present_left
//---------------------------------
 void JuMEG_MPlayer::present_left(uint8_t idx,uint8_t code,uint8_t v){
  _ArdMPVolumeRight_save =_ArdMPVolumeRight;
  if( code <1 ) {code= idx;}
  present(idx,code,v,MP_VOLUME_MIN );
  setVolumeRight(_ArdMPVolumeRight_save);
} // end of present_both_left

//---------------------------------
// present_right
//---------------------------------
 void JuMEG_MPlayer::present_right(uint8_t idx,uint8_t code){
  _ArdMPVolumeLeft_save =_ArdMPVolumeLeft;
  if( code <1 ) {code= idx;}
  present(idx,code,MP_VOLUME_MIN,_ArdMPVolumeRight);
  setVolumeLeft(_ArdMPVolumeLeft_save);
 } // end of present_both_left

//---------------------------------
// present_right
//---------------------------------
 void JuMEG_MPlayer::present_right(uint8_t idx,uint8_t code, uint8_t v){
  _ArdMPVolumeLeft_save =_ArdMPVolumeLeft;
  if( code <1 ) {code=idx;}
  present(idx,code,MP_VOLUME_MIN,v);
  setVolumeLeft(_ArdMPVolumeLeft_save);
 } // end of present_both_left


//---------------------------------------------------------------
// present
//---------------------------------------------------------------
void JuMEG_MPlayer::present(uint8_t idx,uint8_t code){
    
     // unsigned long t0 = millis();

     //ToneTrigger   = HIGH; 
     
     
     //portEVENTCODE = code;
     _onset = millis(); 
     _ArdMP->playFullFile( _file_list[idx],code );
     _offset = millis(); 
     portEVENTCODE = 0;
     //ToneTrigger   = LOW;   
} // end present

//---------------------------------------------------------------
// present update volume
//---------------------------------------------------------------
void JuMEG_MPlayer::present(uint8_t idx,uint8_t code,uint8_t vl, uint8_t vr){
     setVolume(vl,vr);   
     //ToneTrigger   = HIGH; 
     //portEVENTCODE = code;  
     _onset = millis();  
     _ArdMP->playFullFile( _file_list[idx],code );
     _offset = millis(); 
     portEVENTCODE = 0;
     //ToneTrigger   = LOW;   
} // end present

//---------------------------------------------
// getOnset
//---------------------------------------------
unsigned long JuMEG_MPlayer::getOnset(){
     return _onset - _pgr_start_time;
}

//---------------------------------------------
// getOffset
//---------------------------------------------
unsigned long JuMEG_MPlayer::getOffset(){
     return _offset - _pgr_start_time;
}

//---------------------------------------------------------------
// getStatus
//---------------------------------------------------------------
bool JuMEG_MPlayer::getStatus(){ return _status;}

//---------------------------------------------
// getNumberOfTones
//---------------------------------------------
uint8_t JuMEG_MPlayer::getNumberOfTones(){
     return NUMBER_OF_FILES;
}
//---------------------------------------------
// getFileName
//---------------------------------------------
char* JuMEG_MPlayer::getFileName(uint8_t i){
     return _file_list[i];
}
