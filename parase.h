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
	bool is_key; //是否是关键帧
}VIDEO_FRAME; //帧信息


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
	bool is_video_;//是否是视频帧

	VIDEO_FRAME video_frame_; //保留一帧数值
	void* userData_; //用于传入回调中的用户自定义数据

	unsigned char* temp_; //用于保存传进来的数据
	unsigned int temp_len_; //temp_的长度 

	unsigned int remain_len_; //用于保存这一次没有解析完，但还需要用的长度 需要将还没有解析完的数据传入temp_开头，用于下次使用 
	unsigned int reserve_;	//解析完成了但还有数据没有拿到 ，还需要的长度 remain_len_与reserve_基本相反的，至少有一个为零
};

