#pragma once

class msg_t
{
private:
	BOOL overflowed;
	char* data;
	int maxsize;
	int cursize;
	int readcount;
	int bit;

public:
	msg_t()
	{
		this->overflowed = 0;
		this->data = nullptr;
		this->maxsize = 0;
		this->cursize = 0;
		this->readcount = 0;
		this->bit = 0;
	}

	void Init(char* data, int length, bool read, bool clean = false)
	{
		this->overflowed = FALSE;
		this->data = data;
		this->maxsize = length;
		this->cursize = read ? length : 0;
		this->readcount = 0;
		this->bit = 0;

		if (clean)
			XMemSet(data, 0, length);
	}

	BOOL DoWeHaveBuffer()
	{
		return this->data && this->maxsize;
	}

	char* GetBuffer()
	{
		return this->data;
	}

	int GetBufferSize()
	{
		return this->maxsize;
	}

	void ClearBuffer()
	{
		XMemSet(this->data, 0, this->maxsize);
	}

	void Reset(bool read, bool clean)
	{
		this->overflowed = FALSE;
		this->cursize = read ? this->maxsize : 0;
		this->readcount = 0;
		this->bit = 0;

		if (clean && this->data)
			XMemSet(this->data, 0, this->maxsize);
	}

	void WriteByte(unsigned __int8 c)
	{
		if (this->cursize >= this->maxsize)
			this->overflowed = TRUE;
		else
			this->data[this->cursize++] = c;
	}

	void WriteBit1()
	{
		int bit = this->bit & 7;

		if (!bit)
		{
			if (this->cursize >= this->maxsize)
			{
				this->overflowed = TRUE;
				return;
			}
			this->bit = 8 * this->cursize;
			this->data[this->cursize++] = 0;
		}
		this->data[this->bit++ >> 3] |= 1 << bit;
	}

	void WriteBit0()
	{
		if ((this->bit & 7) == 0)
		{
			if (this->cursize >= this->maxsize)
			{
				this->overflowed = TRUE;
				return;
			}
			this->bit = 8 * this->cursize;
			this->data[this->cursize++] = 0;
		}
		++this->bit;
	}

	void WriteInt64(unsigned __int64 c)
	{
		int newsize = this->cursize + 8;

		if (newsize > this->maxsize)
			this->overflowed = TRUE;
		else
		{
			*(unsigned __int64*)&this->data[this->cursize] = c;
			this->cursize = newsize;
		}
	}

	void WriteLong(unsigned __int32 c)
	{
		int newsize = this->cursize + 4;

		if (newsize > this->maxsize)
			this->overflowed = TRUE;
		else
		{
			*(unsigned __int32*)&this->data[this->cursize] = c;
			this->cursize = newsize;
		}
	}

	void WriteShort(unsigned __int16 c)
	{
		int newsize = this->cursize + 2;

		if (newsize > this->maxsize)
			this->overflowed = TRUE;
		else
		{
			*(unsigned __int16*)&this->data[this->cursize] = c;
			this->cursize = newsize;
		}
	}

	void WriteData(char* data, int length)
	{
		int newsize = length + this->cursize;

		if (newsize > this->maxsize)
			this->overflowed = TRUE;
		else
		{
			XMemCpy(&this->data[this->cursize], (char*)data, length);
			this->cursize = newsize;
		}
	}

	void WriteString(char* s)
	{
		int l = strlen(s);

		if (l < 1024)
			this->WriteData(s, l + 1);
		else
			this->WriteData((char*)"", 1);
	}

	unsigned __int8 GetByte(int where)
	{
		if (where <= this->cursize)
			return (unsigned __int8)this->data[where];
		return 0;
	}

	unsigned __int8 ReadByte()
	{
		unsigned __int8 result;

		if (this->readcount >= this->cursize)
		{
			this->overflowed = TRUE;
			result = -1;
		}
		else
		{
			unsigned __int8 c = this->GetByte(this->readcount);
			++this->readcount;
			result = c;
		}
		return result;
	}

	int ReadBit()
	{
		int bit = this->bit & 7;

		if (!bit)
		{
			if (this->readcount >= this->cursize)
			{
				this->overflowed = TRUE;
				return -1;
			}
			this->bit = 8 * this->readcount++;
		}
		int v2 = this->GetByte(this->bit >> 3);
		++this->bit;
		return (v2 >> bit) & 1;
	}

	unsigned __int64 ReadInt64()
	{
		unsigned __int64 result;
		unsigned __int64 c;
		int newcount = this->readcount + 8;

		if (newcount > this->cursize)
		{
			this->overflowed = TRUE;
			result = 0;
		}
		else
		{
			int where = this->readcount;
			for (int i = 0; i < 8; ++i)
				*((unsigned __int8*)&c + i) = this->GetByte(where++);
			this->readcount = newcount;
			result = c;
		}
		return _byteswap_uint64(result);
	}

	unsigned __int32 ReadLong()
	{
		unsigned __int32 result;
		unsigned __int32 c;
		int newcount = this->readcount + 4;

		if (newcount > this->cursize)
		{
			this->overflowed = TRUE;
			result = -1;
		}
		else
		{
			int where = this->readcount;
			for (int i = 0; i < 4; ++i)
				*((unsigned __int8*)&c + i) = this->GetByte(where++);
			this->readcount = newcount;
			result = c;
		}
		return _byteswap_ulong(result);
	}

	unsigned __int16 ReadShort()
	{
		unsigned __int16 result;
		unsigned __int16 c;
		int newcount = this->readcount + 2;

		if (newcount > this->cursize)
		{
			this->overflowed = TRUE;
			result = -1;
		}
		else
		{
			int where = this->readcount;
			for (int i = 0; i < 2; ++i)
				*((unsigned __int8*)&c + i) = this->GetByte(where++);
			this->readcount = newcount;
			result = c;
		}
		return _byteswap_ushort(result);
	}

	char* ReadString(char* string, int maxChars)
	{
		for (int l = 0; ; ++l)
		{
			unsigned __int8 c = this->ReadByte();
			if (c == -1)
				c = 0;
			if (l < maxChars)
				string[l] = c;
			if (!c)
				break;
		}
		string[maxChars - 1] = 0;
		return string;
	}

	void ReadData(char* data, int len)
	{
		int newcount = len + this->readcount;
		XMemCpy((char*)data, &this->data[this->readcount], len);
		this->readcount = newcount;
	}
};