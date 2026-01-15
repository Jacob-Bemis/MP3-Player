#ifndef SCAN_SD_H
#define SCAN_SD_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define MAX_ALBUM 50
#define MAX_SONGS 50

struct Song{
    String name;
    int songID; 
    String songFilePath;
};

struct Album{
    String albumName;
    String artistName;
    int albumID;
    Song trackList[MAX_SONGS];
    int trackCount; 
};



void parseSD(Album *albumList);
extern int albumCount;

#endif