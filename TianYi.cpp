// TianYi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "cuspch.h"
#include "DownloadFile.h"
#include "parase.h"

void frameCallback(VIDEO_FRAME v_frame, void* userData)
{
	std::ofstream fd1("add1.h264", std::ios::out | std::ios::binary | std::ios::app);
	unsigned int frametype = 1;
	if (fd1.is_open())
	{
		fd1.write((char*)v_frame.buff_frame, v_frame.frame_len);

		fd1.close();
	}
}

//下载文件数据接收函数
size_t dl_req_reply(void* buffer, size_t size, size_t nmemb, void* user_p)
{
	Parase* ps = (Parase*)user_p;
	int reverse = 0;
	ps->ps_parse((unsigned char *)buffer , nmemb, frameCallback);

	return nmemb;
}

int main()
{
	std::string dl_url = "http://172.22.172.1:9580/876221_13660360807_3ZWCA333447FELB.ps";
	std::string filename = "1.ps";

	Parase ps;
	DownloadFile::dl_curl_post_req(dl_url,"", dl_req_reply,&ps,filename);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
