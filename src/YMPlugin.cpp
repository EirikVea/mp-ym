#include "YMPlugin.h"

YMPlugin *g_pYMPlugin = NULL;

DWORD DLL_EXPORT ObtainPluginInterface(IPlugin *pIPlugin)
{
    g_pYMPlugin = new YMPlugin();
    pIPlugin = dynamic_cast<IPlugin*>(g_pYMPlugin);

    return 0;
}

DWORD DLL_EXPORT ReleasePluginInterface()
{
    delete g_pYMPlugin;
    g_pYMPlugin = NULL;

    return 0;
}

YMPlugin::YMPlugin()
{
    m_name = "YM Plugin";
    m_version = 0x00000101;
    m_uuid.Data1 = 0x05FC2180;
    m_uuid.Data2 = 0x025D;
    m_uuid.Data3 = 0x11E2;
    m_uuid.Data4[0] = 0x20;
    m_uuid.Data4[1] = 0x0C;
    m_uuid.Data4[2] = 0x9A;
    m_uuid.Data4[3] = 0x66;
    m_uuid.Data4[4] = 0xA2;
    m_uuid.Data4[5] = 0x1F;
    m_uuid.Data4[6] = 0x08;
    m_uuid.Data4[7] = 0x00;

    m_frequency = 0;
    m_bps = 0;
    m_channels = 0;

    m_ymMusic = NULL;

    m_valid = false;
    m_looped = false;
    m_playing = false;
    m_paused = false;

    m_FormatShort = "YM";
}

YMPlugin::~YMPlugin()
{
}

// Get the version of this plugin
DWORD YMPlugin::GetVersion()
{
    return m_version;
}

// Get the type of this plugin
DWORD YMPlugin::GetType()
{
    return MP_PLUGIN_TYPE_AUDIO;
}

// Get the name of this plugin
const char* YMPlugin::GetName()
{
    return m_name.c_str();
}

// Return the UUID of the plugin as a string. Format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
const char* YMPlugin::GetID(UUID& uuid)
{
    uuid = m_uuid;
    m_uuid_str.clear();
    cout << hex << m_uuid.Data1 << "-"
        << m_uuid.Data2 << "-" << m_uuid.Data3 << "-"
        << m_uuid.Data4[0] << m_uuid.Data4[1] << "-"
        << m_uuid.Data4[2] << m_uuid.Data4[3]
        << m_uuid.Data4[4] << m_uuid.Data4[5]
        << m_uuid.Data4[6] << m_uuid.Data4[7] << endl;

        return NULL;
}

void YMPlugin::Initialize(int frequency, int bps, int channels)
{
    m_frequency = frequency;
    m_bps = bps;
    m_channels = channels;
}

bool YMPlugin::Load(char* filename)
{
    m_ymMusic = ymMusicCreate();
    if(!(m_valid = ymMusicLoad(m_ymMusic,filename)))
    {
        Unload();
        return m_valid;
    }

    ymMusicGetInfo(m_ymMusic,&m_ymInfo);
    m_Artist = m_ymInfo.pSongAuthor;
    m_Title = m_ymInfo.pSongName;
    m_Format = m_ymInfo.pSongType;
    m_Comments = m_ymInfo.pSongComment;
    m_FileName = filename;
    m_FileName.erase(0,m_FileName.rfind('\\') + 1);

    return m_valid;
}

void YMPlugin::Unload()
{
    ymMusicDestroy(m_ymMusic);
    m_ymMusic = NULL;
}

bool YMPlugin::HasValidData()
{
    return m_valid;
}

DWORD YMPlugin::GetSubsongCnt()
{
    return 0;
}

DWORD YMPlugin::GetLength(DWORD subsong)
{
    if(m_valid)
        return m_ymInfo.musicTimeInMs;

    return 0;
}

DWORD YMPlugin::GetCurPos(DWORD subsong)
{
    return ymMusicGetPos(m_ymMusic);
}

DWORD YMPlugin::SetCurPos(DWORD subsong, DWORD pos)
{
    if(m_valid)
    {
        if(ymMusicIsSeekable(m_ymMusic))
        {
            ymMusicSeek(m_ymMusic, pos);
            return pos;
        }
    }

    return 0;
}

void YMPlugin::SetLoopMode(bool loopmode)
{
    ymMusicSetLoopMode(m_ymMusic,loopmode);
    m_looped = loopmode;
}

bool YMPlugin::IsLooped()
{
    return m_looped;
}

void YMPlugin::Play()
{
    ymMusicPlay(m_ymMusic);
    m_playing = true;
    m_paused = false;
}

void YMPlugin::Pause()
{
    ymMusicPause(m_ymMusic);
    m_paused = true;
}

void YMPlugin::Stop()
{
    ymMusicStop(m_ymMusic);
    m_playing = false;
    m_paused = false;
}

DWORD YMPlugin::NextSubsong()
{
    return 0;
}

DWORD YMPlugin::PrevSubsong()
{
    return 0;
}

// The plugin host calls this when it wants a new audio frame
DWORD YMPlugin::FillBuffer(void* pbuffer, DWORD length)
{
    short *src = new short[length<<1];
    short *dst = new short[length];

    if(ymMusicCompute(m_ymMusic,(ymsample*)src,length<<2))
    {
        for(DWORD i = 0, j = 0; i < length<<2; i++, j+=2)
        {
            dst[j] = src[i];
            dst[j + 1] = src[i];
        }

        memcpy(pbuffer,dst,length);
    }

    delete dst;
    delete src;

    return length;
}

const char* YMPlugin::Artist()
{
    return m_Artist.c_str();
}

const char* YMPlugin::Title()
{
    return m_Title.c_str();
}

const char* YMPlugin::Album()
{
    return m_Album.c_str();
}

const char* YMPlugin::Year()
{
    return m_Year.c_str();
}

const char* YMPlugin::Comments()
{
    return m_Comments.c_str();
}

const char* YMPlugin::Duration()
{
    return m_Duration.c_str();
}

const char* YMPlugin::Format()
{
    return m_Format.c_str();
}

const char* YMPlugin::FormatShort()
{
    return m_FormatShort.c_str();
}

const char* YMPlugin::FileName()
{
    return m_FileName.c_str();
}
