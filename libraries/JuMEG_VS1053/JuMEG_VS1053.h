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

#ifndef JuMEG_VS1053_h
#define JuMEG_VS1053_h 

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_VS1053.h>

#include <DirectIO.h>

class JuMEG_VS1053_FilePlayer: public Adafruit_VS1053_FilePlayer{ 
public:

  //JuMEG_VS1053_FilePlayer(int8_t mosi, int8_t miso, int8_t clk,int8_t rst, int8_t cs, int8_t dcs, int8_t dreq,int8_t cardCS):
  //                        Adafruit_VS1053_FilePlayer{(mosi,miso,clk,rst,cs,dcs,dreq,cardCS){};
                            
  //JuMEG_VS1053_FilePlayer(int8_t cs, int8_t dcs, int8_t dreq,int8_t cardCS):Adafruit_VS1053_FilePlayer(cs,dcs,dreq,cardCS){};
  JuMEG_VS1053_FilePlayer(int8_t rst, int8_t cs, int8_t dcs, int8_t dreq,int8_t cardCS):Adafruit_VS1053_FilePlayer(rst,cs,dcs,dreq,cardCS){};
                                                                                                                 

  void feedBuffer(uint8_t code);
  boolean startPlayingFile(const char *trackname,uint8_t code);
  boolean playFullFile(const char *trackname,uint8_t code);
  boolean isSendingEventCode(void);
  
  
 protected:
//--- init DirectIO LPST ports C; 
//  OutputPort<PORT_A> portADDR; 
  OutputPort<PORT_C> portEVENTCODE; 



 private:
 
 void feedBuffer_noLock(uint8_t code);
     //void feedBuffer_noLock(void);
 volatile boolean _sendingEventCode;
  //uint8_t _cardCS;
};




/*
  JuMEG_VS1053_FilePlayer (int8_t mosi, int8_t miso, int8_t clk, 
			      int8_t rst, int8_t cs, int8_t dcs, int8_t dreq,
			      int8_t cardCS);
  JuMEG_VS1053_FilePlayer (int8_t rst, int8_t cs, int8_t dcs, int8_t dreq,
			      int8_t cardCS);
  JuMEG_VS1053_FilePlayer (int8_t cs, int8_t dcs, int8_t dreq,
			      int8_t cardCS);




  boolean begin(void);
  boolean useInterrupt(uint8_t type);
  File currentTrack;
  volatile boolean playingMusic;
  void feedBuffer(void);
  static boolean isMP3File(const char* fileName);
  unsigned long mp3_ID3Jumper(File mp3);
  boolean startPlayingFile(const char *trackname,uint8_t code);
  boolean playFullFile(const char *trackname,uint8_t code);
  void stopPlaying(void);
  boolean paused(void);
  boolean stopped(void);
  void pausePlaying(boolean pause);

 protected:
//--- init DirectIO LPST ports C; 
//  OutputPort<PORT_A> portADDR; 
  OutputPort<PORT_C> portEVENTCODE; 



 private:
//  void feedBuffer_noLock(void);

  uint8_t _cardCS;
*/ 
//}; // end of cls JuMEG_VS1053


#endif

