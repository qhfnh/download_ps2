#include "cuspch.h"
#include "parase.h"

#define TEMP_DATA 1024 * 50

Parase::Parase() :
	is_video_(false),
	userData_(NULL),
	remain_len_(0),
	reserve_(0),
	temp_len_(0)
{
	video_frame_.frame_len = 0;
	video_frame_.stamp = 0;
	video_frame_.is_key = false;
	memset(video_frame_.buff_frame, 0, 1024 * 400);
	temp_ = new unsigned char[TEMP_DATA];
	memset(temp_,0, TEMP_DATA);
}

Parase::~Parase()
{
	if (temp_ != NULL)
	{
		delete[] temp_;
	}
}

Header Parase::parase_pes(int& index, int temp_len_)
{

	int start_index = index;
	int PES_packet_length = SUMLEN(temp_, index);
	int packet_end = index + PES_packet_length + 2;//这个包的结尾
	//.........
	index += 2;
	int fix_biat_10 = (temp_[index] & 0xc0) >> 6;
	int packet_inde = index;
	std::cout << fix_biat_10 << std::endl;
	if (2 == (temp_[index] & 0xc0) >> 6) // '10' fix
	{
		int PES_scrambing_control = (temp_[index] & 0x30) >> 4;
		int PES_priority = (temp_[index] & 0x08) >> 3;
		int data_alignment_indicator = (temp_[index] & 0x04) >> 2;
		int copyright = (temp_[index] & 0x02) >> 1;
		int original_or_copy = temp_[index] & 0x01;

		int PTS_DES_flags = (temp_[index + 1] & 0xc0) >> 6;
		int ESCR_flag = (temp_[index + 1] & 0x20) >> 5;
		int ES_rate_flag = (temp_[index + 1] & 0x10) >> 4;
		int DSM_trick_mode_flag = (temp_[index + 1] & 0x08) >> 3;
		int additional_copy_info_flag = (temp_[index + 1] & 0x04) >> 2;
		int PES_CRC_flag = (temp_[index + 1] & 0x02) >> 1;
		int PES_extrnsion_flag = temp_[index + 1] & 0x01;
		int PES_header_data_length = temp_[index + 2];
		index += 3;

		if (PTS_DES_flags == 3)//PTS_DES_flags == '11'
		{
			std::cout << PTS_DES_flags << std::endl;
			//PTS
			int fix_0011 = (temp_[index] & 0xf0) >> 4;
			unsigned int PTS32_30 = (temp_[index] & 0x0e) >> 1;
			int marker_bit = temp_[index] & 0x01;
			unsigned int PTS29_15 = (temp_[index + 1] << 7) | ((temp_[index + 2] & 0xfe) >> 1);
			int marter_bit1 = temp_[index + 2] & 0x01;
			unsigned int PTS14_0 = (temp_[index + 3] << 7) | ((temp_[index + 4] & 0xfe) >> 1);
			int marter_bit2 = temp_[index + 4] & 0x01;

			unsigned long long pts = (PTS32_30 << 30) + (PTS29_15 << 15) + PTS14_0;
			index += 5;
			//DTS
			int fix_0001 = (temp_[index] & 0xf0) >> 4;
			int DTS32_30 = (temp_[index] & 0x0e) >> 1;
			int marker_bit_d = temp_[index] & 0x01;
			int DTS29_15 = (temp_[index + 1] << 7) | ((temp_[index + 2] & 0xfe) >> 1);
			int marter_bit1_d = temp_[index + 2] & 0x01;
			int DTS14_0 = (temp_[index + 3] << 7) | ((temp_[index + 4] & 0xfe) >> 1);
			int marter_bit2_d = temp_[index + 4] & 0x01;
			video_frame_.pts = video_frame_.pts / 90;
			index += 5;
			std::cout << "index:" << index << ":0xe0:";

		}
		else if (PTS_DES_flags == 2)
		{

			std::cout << PTS_DES_flags << std::endl;
			int fix_0011 = (temp_[index] & 0xf0) >> 4;
			int PTS32_30 = (temp_[index] & 0x0e) >> 1;
			int marker_bit = temp_[index] & 0x01;
			int PTS29_15 = (temp_[index + 1] << 7) | ((temp_[index + 2] & 0xfe) >> 1);
			int marter_bit1 = temp_[index + 2] & 0x01;
			int PTS14_0 = (temp_[index + 3] << 7) | ((temp_[index + 4] & 0xfe) >> 1);
			int marter_bit2 = temp_[index + 4] & 0x01;
			index += 5;
			std::cout << "index:" << index << ":0xc0:";

			video_frame_.pts = (PTS32_30 << 30) + (PTS29_15 << 15) + PTS14_0;
			video_frame_.stamp = video_frame_.pts / 90;
		}
		else
		{
			remain_len_ = temp_len_ - start_index + 4;
			for (int i = 0; i < remain_len_; i++)
			{
				temp_[i] = temp_[temp_len_ - remain_len_ + i];
			}
			return Header::ps_packet_head_loading;
		}

		if (index > temp_len_)//解析头时 缓存数据不够
		{
			std::cout << " temp_len_:" << temp_len_ << ":index > temp_len_" << std::endl;
			remain_len_ = temp_len_ - start_index + 4;
			for (int i = 0; i < remain_len_; i++)
			{
				temp_[i] = temp_[temp_len_ - remain_len_ + i];
			}
			return Header::ps_packet_head_loading;
		}
		else if (packet_end >= temp_len_ || index == temp_len_) //解析数据时不够
		{
			if (is_video_) //只添加视频 包含ps包中的所用pes（0xe0）
			{
				memcpy(video_frame_.buff_frame + video_frame_.frame_len, temp_ + index, temp_len_ - index);
				video_frame_.frame_len += temp_len_ - index;
			}
	
			remain_len_ = 0;
			reserve_ = packet_end - temp_len_;
			std::cout << " temp_len_:" << temp_len_ << " reserve_:"<< reserve_<<" index == temp_len_" << std::endl;
			return Header::ps_data_loading;
		}
		else //正常解析
		{
			if (is_video_) ////只添加视频
			{
				memcpy(video_frame_.buff_frame + video_frame_.frame_len, temp_ + index, PES_packet_length - PES_header_data_length - 3);
				video_frame_.frame_len += PES_packet_length - PES_header_data_length - 3;
			}
			remain_len_ = 0;
			index = packet_end;
			std::cout << (int)temp_[index] << ":" << (int)temp_[index + 1] << ":" << (int)temp_[index + 2] << ":" << (int)temp_[index + 3] << std::endl;
			return Header::ps_continue;
		}

	}
	else
	{
		remain_len_ = temp_len_ - start_index + 4;
		for (int i = 0; i < remain_len_; i++)
		{
			temp_[i] = temp_[temp_len_ - remain_len_ + i];
		}
		return Header::ps_packet_head_loading;
	}
}

Header Parase::ps_parse(const unsigned char* temp1, int len1, getFrameCallback frameCallback)
{
	//这样写可能会印象性能
	if (len1 + remain_len_ > TEMP_DATA)
	{
		std::cout << "temp_len_ + remain_len_ > TEMP_DATA  temp_len_:" << len1 << " remain_len_" << remain_len_ <<std:: endl;
		return ps_error;
	}
	memcpy_s(temp_ + remain_len_, TEMP_DATA, temp1, len1);
	temp_len_ = remain_len_ + len1;

	int index = 0;//当前文件索引
	if (reserve_ >= temp_len_) //处理数据中断
	{
		if (is_video_)
		{
			memcpy(video_frame_.buff_frame + video_frame_.frame_len, temp_ + index, temp_len_);
			video_frame_.frame_len += temp_len_;
		}
		reserve_ -= temp_len_;
		return ps_data_loading;
	}
	else if (reserve_ > 0 && reserve_ < temp_len_)
	{
		if (is_video_)
		{
			memcpy(video_frame_.buff_frame + video_frame_.frame_len, temp_ + index, reserve_);
			video_frame_.frame_len += reserve_;
		}

		index += reserve_;
		reserve_ = 0;
	}


	while (EQUITPREFIX(temp_, index)) //ps 包头
	{
		switch (temp_[index + 3])
		{
		case 0xba:  // ps
		{

			if (video_frame_.frame_len > 0)
			{
				//parase_h264(video_frame_.temp_len_);
				frameCallback(video_frame_ , userData_);
				video_frame_.frame_len = 0;
				//{
				//	unique_lock<mutex>lk(m);
				//	products.push(1);
				//	notify = true;
				//	cond.notify_one();
				//	while (notify || !products.empty()) {

				//		cond.wait(lk);
				//	}
			}
			int start_index = index;
			int stuff_byte = temp_[index + 13] & 0x07;
			index += stuff_byte + 14;
			if (index > temp_len_)
			{
				remain_len_ = temp_len_ - start_index; //包含 +4 了
				for (int i = 0; i < remain_len_; i++)
				{
					temp_[i] = temp_[temp_len_ - remain_len_ + i];
				}
				std::cout << "0xba:" << "index:" << index << " temp_len_:"<< temp_len_<<": index > temp_len_ " << std::endl;
				return ps_packet_head_loading;
			}
			else if (index == temp_len_)
			{
				remain_len_ = 0;
				std::cout << "0xba:" << "index:" << index << ": index == temp_len_ " << std::endl;
				return ps_continue;
			}
			break;
		}
		case 0xbb: //sys
		{
			video_frame_.is_key = true;

			int start_index = index;
			int sys_header_length = SUMLEN(temp_, index + 4);
			index = index + 6 + sys_header_length;
			if (index > temp_len_)
			{
				std::cout << "0xbb:" << "index:" << index << ": index > temp_len_ " << std::endl;
				remain_len_ = temp_len_ - start_index;
				for (int i = 0; i < remain_len_; i++)
				{
					temp_[i] = temp_[temp_len_ - remain_len_ + i];
				}
				return ps_packet_head_loading;
			}
			else if (index == temp_len_)
			{
				std::cout << "0xbb:" << "index:" << index << ": index == temp_len_ " << std::endl;
				remain_len_ = 0;
				return ps_continue;
			}
			std::cout << "0xbb:" << (int)temp_[index] << ":" << (int)temp_[index + 1] << ":" << (int)temp_[index + 2] << ":" << (int)temp_[index + 3] << std::endl;
			break;
		}
		case 0xbc: //psm
		{
			int start_index1 = index;
			int program_stream_map_length = SUMLEN(temp_, index + 4);
			int start_index = index + 8;
			//直接跳过 解析里面的内容时 在break上边 index =start_index;
			index += program_stream_map_length + 6;

			//解析里面内容
			{

				int elementary_stream_info_length = SUMLEN(temp_, start_index);
				start_index += 2;
				int elementary_stream_map_length = SUMLEN(temp_, start_index);
				start_index = start_index + 2;
				for (int i = 0; i < elementary_stream_map_length / 4; i++)
				{
					int stream_type = temp_[index];
					int elementary_stream_id = temp_[index + 1];
					int elementary_stream_info_length = SUMLEN(temp_, start_index + 2);
					start_index = start_index + elementary_stream_info_length + 4;
				}
				start_index += 4; //CRC
			}
			if (index > temp_len_)
			{
				std::cout << "0xbc:" << "index:" << index << ":index > temp_len_ " << std::endl;
				remain_len_ = temp_len_ - start_index1;
				for (int i = 0; i < remain_len_; i++)
				{
					temp_[i] = temp_[temp_len_ - remain_len_ + i];
				}
				return ps_packet_head_loading;
			}
			else if (index == temp_len_)
			{
				std::cout << "0xbc:" << "index:" << index << ":index == temp_len_ " << std::endl;
				remain_len_ = 0;
				return ps_continue;
			}
			std::cout << "0xbc" << (int)temp_[index] << ":" << (int)temp_[index + 1] << ":" << (int)temp_[index + 2] << ":" << (int)temp_[index + 3] << std::endl;
			break;
		}
		case 0xe0: //pse
		{
			is_video_ = true;
			index += 4;

			Header sta = parase_pes( index, temp_len_);
			if (sta == ps_packet_head_loading || sta == ps_data_loading)
			{
				return sta;
			}

			break;
		}
		case 0xc0: //PSE
		{
			is_video_ = false;
			index += 4;
			Header sta = parase_pes(index, temp_len_);
			if (sta == ps_packet_head_loading || sta == ps_data_loading)
			{
				return sta;
			}
			break;
		}
		default:
		{
			//int count = 0;
			std::cout << "index:" << index << std::endl;
			std::cout << "default:" << (int)temp_[index] << ":" << (int)temp_[index + 1] << ":" << (int)temp_[index + 2] << ":" << (int)temp_[index + 3] << std::endl;
			if (index + 4 > temp_len_ && index < temp_len_)
			{
				remain_len_ = temp_len_ - index;
				for (int i = 0; i < remain_len_; i++)
				{
					temp_[i] = temp_[temp_len_ - remain_len_ + i];
				}
				std::cout << "+4 index:" << index << " temp_len_:" << temp_len_ << std::endl;
				return ps_packet_head_loading;
			}
		}

		}

	}
	// 当解析数据时正好包头没有了
	if (index + 3 > temp_len_ && index < temp_len_)
	{
		remain_len_ = temp_len_ - index;
		for (int i = 0; i < remain_len_; i++)
		{
			temp_[i] = temp_[temp_len_ - remain_len_ + i];
		}
		std::cout << "+ 3 index:" <<index << " temp_len_:"<< temp_len_<< std::endl;
		return ps_packet_head_loading;
	}
	return ps_error;
}
