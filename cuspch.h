#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <time.h>
#include <queue>

//windows
#ifdef WIN32

#include <windows.h>

#include <tchar.h>
#include <atlbase.h>
#include <atlconv.h>

#include <wininet.h>
#pragma comment( lib, "wininet.lib" )

#include<curl/curl.h>
#pragma comment(lib,"libcurl.lib")

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libavutil/pixfmt.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/parseutils.h>
	#include <libavutil/opt.h>
	#include <libswscale/swscale.h>
	#include <libavutil/time.h>

};
#pragma comment (lib, "avcodec.lib")
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "swresample.lib")
#pragma comment (lib, "swscale.lib")
#endif // WIN32



