#include "shared_resources.h"


QueueHandle_t xEventQueue = NULL;
SemeaphoreHandle_t xMutex = NULL;

int currentAlbumID = 0;
int currentSongID = 0;
