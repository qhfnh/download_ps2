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
	//char* pc = new char[1024];//�㹻��
	//strcpy(pc, filename.c_str());

	//FILE* fp = fopen(pc, "wb");

	// curl��ʼ��  
	CURL* curl = curl_easy_init();
	// curl����ֵ 
	CURLcode res;
	if (curl)
	{
		// set params
		//����curl������ͷ
		struct curl_slist* header_list = NULL;
		header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko Core/1.63.6788.400 QQBrowser/10.3.2843.400");
		header_list = curl_slist_append(header_list, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		//��������Ӧͷ����0�������� 1�������
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);

		//��������Ϊpost����
		curl_easy_setopt(curl, CURLOPT_POST, 1);


		//���������URL��ַ
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		//����post����Ĳ���
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str());

		//����ssl��֤
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

		//CURLOPT_VERBOSE��ֵΪ1ʱ������ʾ��ϸ�ĵ�����Ϣ
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

		//�������ݽ��պ�д�뺯��
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, userData);

		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

		//���ó�ʱʱ��
		//curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6);
		//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

		// ����post����
		res = curl_easy_perform(curl);
	}
	//�ͷ�curl 
	curl_easy_cleanup(curl);

	return res;
}


