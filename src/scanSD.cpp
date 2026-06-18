#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <scanSD.h>

#define SD_CS   10
#define SD_SCK  12
#define SD_MISO 13
#define SD_MOSI 11



int albumCount = 0;
SPIClass spi(FSPI); 

int scanTracks(File &file, Album &album){
  int trackCount = 0;
  while(true){
    File song =  file.openNextFile();
    if (trackCount >= MAX_SONGS || !song){
      break;
    }
    if (!song.isDirectory()){
      String name = song.name();
      name.toLowerCase();
      if (name.endsWith(".mp3")){
        Song nuSong;
        nuSong.songID = trackCount;
        int endIndex = name.indexOf(".mp3");
        String songName = name.substring(0, endIndex);
        nuSong.name = songName;
        String filePath = "/Music/" +  (String)file.name() + "/" + name;
        nuSong.songFilePath = filePath;
        album.trackList[trackCount] = nuSong;
        trackCount++;
      }
    }
    song.close();
  }
  return trackCount;
}

void parseSD(Album *albumList){
  Serial.println("parseSD: start");

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); 
  spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spi, 1000000)){
    Serial.println("SD Card Failed");
    return;
  }
  Serial.println("SD Card is good");

  File root = SD.open("/Music");
  if ((!root ) || (!root.isDirectory())){
    Serial.println("Music Directory not found");
    return;
  }

  while(true){
    File album = root.openNextFile();
    if (albumCount >= MAX_ALBUM || !album){
        break;
    }
    if (album.isDirectory()){
      Album nuAlbum;
      String name = album.name();
      int dashIndex = name.indexOf("-");
      String artistName = name.substring(0, dashIndex);
      String albumName = name.substring(dashIndex+1);
      albumName.trim();
      artistName.trim();

      nuAlbum.artistName = artistName;
      nuAlbum.albumName = albumName;
      nuAlbum.albumID = albumCount;
      int numSongs = scanTracks(album, nuAlbum);
      nuAlbum.trackCount = numSongs;

      albumList[albumCount] = nuAlbum;
      albumCount++;
    }
    album.close();
  }
  root.close();
}
