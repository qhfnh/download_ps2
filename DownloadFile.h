#pragma once

using downloadCallBack = size_t (*)(void* buffer, size_t size, size_t nmemb, void* user_p);


class DownloadFile
{
public:
	DownloadFile();
	~DownloadFile();


	static CURLcode dl_curl_post_req(const std::string& url, const std::string& postParams, const downloadCallBack callback, void* userData,const std::string &filename = NULL);
private:
};

