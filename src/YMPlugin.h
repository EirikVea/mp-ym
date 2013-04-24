#ifndef _YMPlugin_h_
#define _YMPlugin_h_

using namespace std;

#include <windows.h>
#include "Plugin.h"
#include "MusicInfo.h"
#include "YmMusic.h"

class YMPlugin : public IPlugin, IAudioPlugin, IMusicInfo
{
    private:
        string m_name;
        DWORD m_version;
        UUID m_uuid;
        string m_uuid_str;

        DWORD m_frequency;
        DWORD m_channels;
        DWORD m_bps;

        bool m_valid;
        bool m_looped;
        bool m_playing;
        bool m_paused;

        YMMUSIC *m_ymMusic;
        ymMusicInfo_t m_ymInfo;

        string m_Artist;       // Artist
        string m_Title;        // Title
        string m_Album;        // Album
        string m_Year;         // Year/date as a string
        string m_Comments;     // Comments, info, message
        string m_Duration;     // Format hh:mm:ss
        string m_Format;       // Format name
        string m_FormatShort;  // Short format, maximum 4 characters
        string m_FileName;     // Only filename, not complete path
    public:
        YMPlugin();
        ~YMPlugin();

    /* IPlugin interface begin */
        DWORD GetVersion();     // Get the version of this plugin
        DWORD GetType();        // Get the type of this plugin
        const char* GetName();  // Get the name of this plugin
        // Get the UUID of the plugin. Will also return the UUID as a sting Format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
        const char* GetID(UUID& uuid);
    /* IPlugin interface end */

    /* IAudioPlugin interface begin */
        void Initialize(int frequency, int bps, int channels);
        bool Load(char* filename);
        void Unload();

        bool HasValidData();
        DWORD GetSubsongCnt();

        DWORD GetLength(DWORD subsong);
        DWORD GetCurPos(DWORD subsong);
        DWORD SetCurPos(DWORD subsong, DWORD pos);

        void SetLoopMode(bool loopmode);
        bool IsLooped();

        void Play();
        void Pause();
        void Stop();
        DWORD NextSubsong();
        DWORD PrevSubsong();

        DWORD FillBuffer(void* pbuffer, DWORD length); // The plugin host calls this when it wants a new audio frame
    /* IAudioPlugin interface end */

    /* IMusicInfo interface begin */
        const char* Artist();       // Artist
        const char* Title();        // Title
        const char* Album();        // Album
        const char* Year();         // Year/date as a string
        const char* Comments();     // Comments, info, message
        const char* Duration();     // Format hh:mm:ss
        const char* Format();       // Format name
        const char* FormatShort();  // Short format, maximum 4 characters
        const char* FileName();     // Only filename, not complete path
    /* IMusicInfo interface end */
};

#endif // _YMPlugin_h_
