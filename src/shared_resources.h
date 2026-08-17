#ifndef SHARED_RESOURCES_H_
#define SHARED_RESOURCES_H_

#include <Arduino.h>

typedef enum {STATE_HOME, STATE_ALBUM, STATE_ERROR} nav_State;
typedef enum { EV_SELECT, EV_BACK, EV_UP, EV_DOWN, EV_ERROR } nav_EVENT;

extern QueueHandle_t xEventQueue;
extern SemaphoreHandle_t xMutex;

extern int currentAlbumID; // Identifies what index in the album list
extern int currentSongID;  // Identifies what index in a tracklist

extern int currentAlbumPlaying;
extern int currentTrackPlaying;

extern bool mp3Playing; // Identifies whether an audio is playing rignt now
extern bool audio_Interrupt; // Identifies if the FSM wants to interrupt the
                             // currently playing audio

extern TaskHandle_t xAudioPlaybackTask;


#endif // SHARED_RESOURCES_H_
