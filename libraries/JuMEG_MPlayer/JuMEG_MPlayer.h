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

     @file     JuMEG_CueTone.h
     @author   F.Boers (f.boers@fz-juelich.de)
     update 07.06.2019
     v1.0  - First release
     
     update 07.06.2019

*/	
/**************************************************************************/

#ifndef JuMEG_MPLAYER_h
#define JuMEG_MPLAYER_h 

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
//#include <Adafruit_VS1053.h>
#include <JuMEG_VS1053.h>

#include <JuMEG_XmBase.h>

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3     // VS1053 Data request, ideally an Interrupt pin

//---
#define CODE_OFFSET  128
#define IDX_LEFT  0
#define IDX_RIGHT 1
//---
#define PLAY_ON_LEFT  1
#define PLAY_ON_RIGHT 2
#define PLAY_ON_BOTH  3

//---
#define NUMBER_OF_FILES   3
#define NUMBER_OF_LETTERS 12

//---
#define MP_VOLUME_MIN    255 // ck for 127 ???
#define MP_VOLUME_MAX      0
#define MP_VOLUME_DEFAULT 10
#define MP_TRIGGER_PIN     5


class JuMEG_MPlayer: public JuMEG_XmBase{
 public:
  JuMEG_MPlayer();
//--- Audio Output Trigger
  Output<MP_TRIGGER_PIN> ToneTrigger;

  void begin(JuMEG_VS1053_FilePlayer* ArdMP);  
  void begin(JuMEG_VS1053_FilePlayer* ArdMP,bool verbose);  
 
  //void begin(Adafruit_VS1053_FilePlayer* ArdMP);  
  //void begin(Adafruit_VS1053_FilePlayer* ArdMP,bool verbose);  
  
  bool setup();
  bool SDCardIsSelected();
 
  void setVolume(uint8_t v);
  void setVolume(uint8_t vl,uint8_t vr);

  void setVolumeLeft(uint8_t v);
  uint8_t getVolumeLeft();

  void setVolumeRight(uint8_t v);
  uint8_t getVolumeRight();
  
  void VolumeInfo(char* t);

//--- present wave file bot;left,right
  void present(uint8_t idx,uint8_t code);
  void present(uint8_t idx,uint8_t code,uint8_t vl, uint8_t vr);
  
  void present_left(uint8_t idx,uint8_t code);
  void present_left(uint8_t idx,uint8_t code, uint8_t v);
 
  void present_right(uint8_t idx,uint8_t code);
  void present_right(uint8_t idx,uint8_t code, uint8_t v);
 
  void setStatus(bool v);
  bool getStatus();

  unsigned long getOnset();
  unsigned long getOffset();
  uint8_t getNumberOfTones();
  char* getFileName(uint8_t i);

  
 private: 
 // ToDo do not fix list here: pointer
  const char _file_list[NUMBER_OF_FILES][NUMBER_OF_LETTERS]   = {"t001.wav","t002.wav","t003.wav"};
//  const char _file_list[2][NUMBER_OF_LETTERS]   = {"std.wav","div.wav"};


  //Adafruit_VS1053_FilePlayer* _ArdMP = 0;
  JuMEG_VS1053_FilePlayer*    _ArdMP = 0; 

  uint8_t       _ArdMPVolumeLeft       = MP_VOLUME_DEFAULT;
  uint8_t       _ArdMPVolumeRight      = MP_VOLUME_DEFAULT;
  uint8_t       _ArdMPVolumeLeft_save  = MP_VOLUME_DEFAULT;
  uint8_t       _ArdMPVolumeRight_save = MP_VOLUME_DEFAULT;
  bool          _ArdMP_SD_isSelected   = false; 
  bool          _status;

  volatile unsigned long _onset;
  volatile unsigned long _offset;
  
}; // end of cls JuMEG_PlayTone


#endif



