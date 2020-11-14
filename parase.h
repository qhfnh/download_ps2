#pragma once

#define EQUITPREFIX(temp_,index) (temp_[index] == 0 && temp_[index + 1] == 0 && temp_[index + 2] == 1)
#define EQUIT00(temp_,index) (temp_[index] == 0 && temp_[index + 1] == 0 && temp_[index + 2] == 0)
#define EQUITBIT(temp_,index,t) temp_[index] == 0 && temp_[index + 1] == 0 && temp_[index + 2] == 1 && temp_[index + 3] == t
#define SUMLEN(temp_,index)  temp_[index] << 8 | temp_[index + 1]


typedef enum
{
	ps_packet_head_loading,
	ps_data_loading,
	ps_continue,
	ps_error,
}Header;
typedef struct
{
	unsigned int frame_len;
	unsigned char buff_frame[1024 * 400];
	unsigned int stamp;
	unsigned long pts;
	bool is_key; //�Ƿ��ǹؼ�֡
}VIDEO_FRAME; //֡��Ϣ


using getFrameCallback = void (*)(VIDEO_FRAME v_frame, void* userData);



class Parase
{
public:
	Parase();
	~Parase();
	inline void setusedata(void* userData)
	{
		this->userData_ = userData;
	}
	Header ps_parse(const unsigned char* temp_,  int len,getFrameCallback frameCallback);

	inline int getRemainLen() 
	{
		return remain_len_;
	}

private:
	Header parase_pes(int& index, int len);
private:
	bool is_video_;//�Ƿ�����Ƶ֡

	VIDEO_FRAME video_frame_; //����һ֡��ֵ
	void* userData_; //���ڴ���ص��е��û��Զ�������

	unsigned char* temp_; //���ڱ��洫����������
	unsigned int temp_len_; //temp_�ĳ��� 

	unsigned int remain_len_; //���ڱ�����һ��û�н����꣬������Ҫ�õĳ��� ��Ҫ����û�н���������ݴ���temp_��ͷ�������´�ʹ�� 
	unsigned int reserve_;	//��������˵���������û���õ� ������Ҫ�ĳ��� remain_len_��reserve_�����෴�ģ�������һ��Ϊ��
};

