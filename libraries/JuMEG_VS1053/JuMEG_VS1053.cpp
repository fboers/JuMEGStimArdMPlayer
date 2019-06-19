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

//#include <Adafruit_VS1053.h>
#include <JuMEG_VS1053.h>

/*
JuMEG_VS1053_FilePlayer::JuMEG_VS1053_FilePlayer(){
}

JuMEG_VS1053_FilePlayer::~JuMEG_VS1053_FilePlayer(){
}

*/


boolean JuMEG_VS1053_FilePlayer::playFullFile(const char *trackname,uint8_t code) {
  if (! startPlayingFile(trackname,code) ) return false;
  //portEVENTCODE = code;
  
  while (playingMusic) {
    // twiddle thumbs
    feedBuffer(code);
    delay(5);           // give IRQs a chance
  }
  // music file finished!
 
  portEVENTCODE = 0;
  
  return true;
}


boolean JuMEG_VS1053_FilePlayer::startPlayingFile(const char *trackname,uint8_t code) {
  // reset playback
  sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW);
  // resync
  sciWrite(VS1053_REG_WRAMADDR, 0x1e29);
  sciWrite(VS1053_REG_WRAM, 0);

  currentTrack = SD.open(trackname);
  if (!currentTrack) {
    return false;
  }
    
  _sendingEventCode=false;
  
  // We know we have a valid file. Check if .mp3
  // If so, check for ID3 tag and jump it if present.
  if (isMP3File(trackname)) {
    currentTrack.seek(mp3_ID3Jumper(currentTrack));
  }
 
   
  // don't let the IRQ get triggered by accident here
  noInterrupts();

  // As explained in datasheet, set twice 0 in REG_DECODETIME to set time back to 0
  sciWrite(VS1053_REG_DECODETIME, 0x00);
  sciWrite(VS1053_REG_DECODETIME, 0x00);

  playingMusic = true;

  // wait till its ready for data
  while (! readyForData() ) {
#if defined(ESP8266)
	yield();
#endif
  }


  while (playingMusic && readyForData()) {
    feedBuffer(code);
  }
  
  // ok going forward, we can use the IRQ
  interrupts();
 
 // portEVENTCODE = 0;

  return true;
}

void JuMEG_VS1053_FilePlayer::feedBuffer(uint8_t code) {
  noInterrupts();
  // dont run twice in case interrupts collided
  // This isn't a perfect lock as it may lose one feedBuffer request if
  // an interrupt occurs before feedBufferLock is reset to false. This
  // may cause a glitch in the audio but at least it will not corrupt
  // state.
  if (feedBufferLock) {
    interrupts();
    return;
  }
  feedBufferLock = true;
  interrupts();

  feedBuffer_noLock(code);
 
  feedBufferLock = false;
}


void JuMEG_VS1053_FilePlayer::feedBuffer_noLock(uint8_t code){
  if ((! playingMusic) // paused or stopped
      || (! currentTrack) 
      || (! readyForData())) {
    return; // paused or stopped
  }
  // Feed the hungry buffer! :)
  while (readyForData()) {
    // Read some audio data from the SD card file
    int bytesread = currentTrack.read(mp3buffer, VS1053_DATABUFFERLEN);
    
    if (bytesread == 0) {
      // must be at the end of the file, wrap it up!
      playingMusic = false;
      currentTrack.close();
      break;
    }
    
    if (!_sendingEventCode)
     {
       portEVENTCODE     = code; // delay ~ 3,3 ms
       _sendingEventCode = true;
     }
     
    playData(mp3buffer, bytesread);
  
  }

}

boolean JuMEG_VS1053_FilePlayer::isSendingEventCode(void){
     return _sendingEventCode;
}


