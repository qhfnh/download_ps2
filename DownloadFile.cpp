#include "cuspch.h"
#include "DownloadFile.h"

DownloadFile::DownloadFile()

{

}

DownloadFile::~DownloadFile()
{
}

CURLcode DownloadFile::dl_curl_post_req(const std::string& url, const std::string& postParams, const downloadCallBack callback, void* userData ,const std::string& filename )
{
	//char* pc = new char[1024];//足够长
	//strcpy(pc, filename.c_str());

	//FILE* fp = fopen(pc, "wb");

	// curl初始化  
	CURL* curl = curl_easy_init();
	// curl返回值 
	CURLcode res;
	if (curl)
	{
		// set params
		//设置curl的请求头
		struct curl_slist* header_list = NULL;
		header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko Core/1.63.6788.400 QQBrowser/10.3.2843.400");
		header_list = curl_slist_append(header_list, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		//不接收响应头数据0代表不接收 1代表接收
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);

		//设置请求为post请求
		curl_easy_setopt(curl, CURLOPT_POST, 1);


		//设置请求的URL地址
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		//设置post请求的参数
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str());

		//设置ssl验证
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

		//CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

		//设置数据接收和写入函数
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, userData);

		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

		//设置超时时间
		//curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6);
		//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

		// 开启post请求
		res = curl_easy_perform(curl);
	}
	//释放curl 
	curl_easy_cleanup(curl);

	return res;
}


