#pragma once

#include <vector>
#include "rehlds_api_provider.h"

class SizeBufWriter {
	sizebuf_t* output_buf_;
	sizebuf_t temp_buf_;
	std::vector<uint8_t> temp_buf_data_;

protected:
	sizebuf_t* GetTempSizeBuf() {
		return &temp_buf_;
	}	

	sizebuf_t* GetOutputSizeBuf() {
		return output_buf_;
	}

	std::vector<uint8_t>* GetTempStlBuffer() {
		return &temp_buf_data_;
	}

	void SyncTempStlBufferWithTempSizeBuf() {
		temp_buf_.cursize = GetTempStlBuffer()->size();
		if(temp_buf_.cursize > temp_buf_.maxsize) {
			MF_Log("%s: overflow, ignoring sizebuf maxsize", __FUNCTION__);
			temp_buf_.maxsize = temp_buf_.cursize;
		}
	}

public:
	SizeBufWriter(sizebuf_t* output_buf, size_t maxsize)
		: output_buf_(output_buf) {
		temp_buf_data_.resize(maxsize);

		temp_buf_.buffername = "SizeBufWriter::temp_buf_";
		temp_buf_.data = temp_buf_data_.data();
		temp_buf_.maxsize = maxsize;
		temp_buf_.cursize = 0;
		temp_buf_.flags = SIZEBUF_ALLOW_OVERFLOW;
	}
	~SizeBufWriter() { }

	SizeBufWriter* WriteByte(uint8_t data) {
		g_RehldsFuncs->MSG_WriteByte(&temp_buf_, data);
		return this;
	}

	SizeBufWriter* WriteString(std::string data) {
		g_RehldsFuncs->MSG_WriteString(&temp_buf_, data.c_str());
		return this;
	}

	SizeBufWriter* WriteLong(long data) {
		g_RehldsFuncs->MSG_WriteLong(&temp_buf_, data);
		return this;
	}

	SizeBufWriter* WriteBuf(std::vector<uint8_t> data) {
		g_RehldsFuncs->MSG_WriteBuf(&temp_buf_, data.size(), data.data());
		return this;
	}

	void Send() {
		g_RehldsFuncs->MSG_WriteBuf(output_buf_, temp_buf_.cursize, temp_buf_.data);
	}
};