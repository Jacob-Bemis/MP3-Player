#include "shared_resources.h"


QueueHandle_t xEventQueue = NULL;
SemaphoreHandle_t xMutex = NULL;

int currentAlbumID = 0;
int currentSongID = 0;
int currentAlbumPlaying = 0;
int currentTrackPlaying = 0;


bool mp3Playing = false;
bool audio_Interrupt = false;
TaskHandle_t xAudioPlaybackTask = NULL;
