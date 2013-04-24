#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <string>
#include <windows.h>
#include "YmMusic.h"



#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT mp_PluginInfo(struct mp_plugin_info *info);
bool DLL_EXPORT mp_Detect(char *filename, struct mp_song_info *songinfo);
bool DLL_EXPORT mp_InitPlugin(char *filename, int frequency, int bps, int channels);
DWORD DLL_EXPORT mp_FillBuffer(void *buffer, DWORD length);

DWORD DLL_EXPORT mp_NextSubsong();
DWORD DLL_EXPORT mp_PreviousSubsong();

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
