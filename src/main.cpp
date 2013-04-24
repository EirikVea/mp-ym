#include "main.h"

DWORD g_remaining = 0;
char *g_rembuf;

YMMUSIC	*g_hYMMusic = 0;

//void DLL_EXPORT mp_PluginInfo(struct mp_plugin_info *info)
//{
//    strcpy(info->name,"YM plugin");
//    info->version = 0x00000101;
//}

//bool DLL_EXPORT mp_Detect(char *filename, struct mp_song_info *songinfo)
//{
//    if(filename == NULL)
//        return false;
//
//    g_hYMMusic = ymMusicCreate();
//
//    if(!ymMusicLoad(g_hYMMusic,filename))
//    {
//        ymMusicDestroy(g_hYMMusic);
//        g_hYMMusic = NULL;
//        return false;
//    }
//    ymMusicInfo_t ymInfo;
//    ymMusicGetInfo(g_hYMMusic,&ymInfo);
//
//    songinfo->format = (char*)malloc(strlen(ymInfo.pSongType) * sizeof(char));
//    wsprintf(songinfo->format,ymInfo.pSongType);
//
//    songinfo->name = (char*)malloc(strlen(ymInfo.pSongName) * sizeof(char));
//    wsprintf(songinfo->name,ymInfo.pSongName);
//
//    songinfo->artist = (char*)malloc(strlen(ymInfo.pSongAuthor) * sizeof(char));
//    wsprintf(songinfo->artist,ymInfo.pSongAuthor);
//
//    songinfo->info = (char*)malloc(strlen(ymInfo.pSongComment) * sizeof(char));
//    wsprintf(songinfo->info,ymInfo.pSongComment);
//
//    songinfo->duration = ymInfo.musicTimeInMs;
//    songinfo->subsongs = 0;
//    songinfo->voices = 3;
//    songinfo->steps = -1;
//
//    ymMusicDestroy(g_hYMMusic);
//    g_hYMMusic = NULL;
//
//    return true;
//}

bool DLL_EXPORT mp_InitPlugin(char* filename, int frequency, int bps, int channels)
{
    if(filename == NULL)
        return false;

    g_hYMMusic = ymMusicCreate();

    if(!ymMusicLoad(g_hYMMusic,filename))
    {
        ymMusicDestroy(g_hYMMusic);
        return false;
    }
    ymMusicInfo_t *ymInfo;
    ymMusicGetInfo(g_hYMMusic,ymInfo);

    return true;
}

//DWORD DLL_EXPORT mp_SetPluginParams()
//{
//
//}
//
//DWORD DLL_EXPORT mp_SetPluginParams()
//{
//
//}
//
//void DLL_EXPORT mp_GetSongInfo(struct mp_song_info *songinf)
//{
//
//}

DWORD DLL_EXPORT mp_GetPosition()
{
    return 0;
}

void DLL_EXPORT mp_SetPosition(DWORD pos)
{

}

DWORD DLL_EXPORT mp_NextSubsong()
{
    return 0;
}

DWORD DLL_EXPORT mp_PreviousSubsong()
{
    return 0;
}

void mp_RemoveStereoSep16(short *buffer, DWORD length, float factor)
{
    short t1;
    for(int i = 0; i < (length >> 1) - 1; i += 1)
    {
        buffer[i] = ((float)buffer[i] * factor);
        buffer[i + 1] = (float)buffer[i + 1] * factor;
        t1 = buffer[i];
        buffer[i] += buffer[i + 1];
        buffer[i + 1] += t1;
    }
}

DWORD DLL_EXPORT mp_FillBuffer(void *buffer, DWORD length)
{
    short *src = (short*)malloc(length/2);
    short *dst = (short*)malloc(length);
    ymMusicCompute(g_hYMMusic,(ymsample*)src,length/4);

    for(int i = 0, j = 0; i < length/4; i++, j+=2)
    {
        dst[j] = src[i];
        dst[j + 1] = src[i];
    }

    memcpy(buffer,dst,length);

    free(src);
    free(dst);

    return length;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
