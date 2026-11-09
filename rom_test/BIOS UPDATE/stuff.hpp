#ifndef log 
	#define log printf("\n *** "); printf
#endif

#ifndef UART_SPEED
	#define UART_SPEED CBR_57600
#endif 


class Stream
{
public:
	enum KIND { STREAM_IS_BUFFER,  STREAM_IS_FILE,  STREAM_IS_BYTE };
	
	static SW32 getFileSize(SW8* filePath_)
	{
		FILE *f = fopen(filePath_, "rb");
		if (!f) return -1;
		fseek(f, 0, SEEK_END);
		SW32 size = ftell(f);
		fclose(f);
		return size;
	}

	KIND type;
	union
	{
		W8* buffer;
		SW8* path;
		W8 byte;
	};
	W1 valid;
	SW32 size;
	SW32 offset;
	FILE* f;
	static Stream asByte(SW32 byte_=0, SW32 size_=0)
	{
		Stream st;
		st.type = STREAM_IS_BYTE;
		st.byte = byte_;
		st.size = size_;
		st.valid =1;
		return st;
	}
	static Stream asFile(SW8* filename_, SW32 offset_=0, SW32 size_=0)
	{
		Stream st;
		st.type = STREAM_IS_FILE;
		st.path = filename_;
		st.offset = offset_;
		st.f=0;
		st.valid = 0;
		st.size = ((size_==0)? getFileSize(filename_): size_);
		return st;
	}
	static Stream asBuffer(W8* buffer_, SW32 size_)
	{
		Stream st;
		st.type = STREAM_IS_BUFFER;
		st.buffer = buffer_;
		st.offset = 0;
		st.size = size_;
		st.f = 0;
		st.valid = 1;
		return st;
	}

	SW32 currOffset;
	W0 fill(W8 byte_, W32 size_)
	{
		switch(type)
		{
			case STREAM_IS_BYTE: byte_;
			case STREAM_IS_BUFFER: 
				memset(buffer+currOffset, byte_, VL_MIN(size_, size-(currOffset+size_)));
				currOffset+=size_;
			break;
			case STREAM_IS_FILE:
			{
				for (W32 i=0; i< size_; i++)
					fwrite(&byte_, 1, 1, f);
				size=VL_MAX(size,currOffset+size_);
				currOffset+=size_;
				size = VL_MAX(currOffset, size);
			}
		}
	}
	W0 seek(W32 offset_)
	{
		currOffset = offset_;
		switch(type)
		{
			case STREAM_IS_FILE:   fseek(f, offset_, SEEK_SET);
		}
	}
	W8 read()
	{
		currOffset++;
		switch(type)
		{
			case STREAM_IS_BYTE: return byte;
			case STREAM_IS_BUFFER: return buffer[currOffset-1];
			case STREAM_IS_FILE:
			{
				W8 byte_;
				fread(&byte_, 1, 1, f);
				return byte_;
			}
		}
		return 0;
	}
	W0 write(const SW8* buf_, SW32 sz_)
	{
		switch(type)
		{
			case STREAM_IS_BYTE: byte = buf_[0]; break;
			case STREAM_IS_BUFFER:
				memcpy(buffer+currOffset, buf_, sz_ );	
			break;
			case STREAM_IS_FILE:
			{
				fwrite(buf_, sz_, 1, f);
			}
			break;
		}
	}
	template<typename TYPE_>
	W0 writeAs(TYPE_ word_)
	{
		W32 curr = currOffset;
		currOffset+=sizeof(TYPE_);
		switch(type)
		{
			case STREAM_IS_BYTE: byte = word_; break;
			case STREAM_IS_BUFFER: *(TYPE_*)(buffer+curr)=word_; break;
			case STREAM_IS_FILE:
			{
				fwrite(&word_, sizeof(TYPE_), 1, f);
			}
			break;
		}
	}
	template<typename TYPE_>
	TYPE_ readAs()
	{
		W32 curr = currOffset;
		currOffset+=sizeof(TYPE_);
		switch(type)
		{
			case STREAM_IS_BYTE: return byte;
			case STREAM_IS_BUFFER: return *(TYPE_*)(buffer+curr);
			case STREAM_IS_FILE:
			{
				TYPE_ word_;
				fread(&word_, 1,  sizeof(TYPE_), f);
				return word_;
			}
		}
		return 0;
	}
	W1 open(const SW8* str_)
	{
		currOffset = offset;
		if (type==STREAM_IS_FILE)
		{
			f = fopen(path, str_);
			if (!f) return 1;
			fseek(f, offset, SEEK_SET);
		}
		return 0;
	}
	W0 close()
	{
		if (type==STREAM_IS_FILE && f)
			fclose(f);
	}
	W0 copyTo(Stream s2_, W32 inputSize_=0)
	{
		SW8 temp[512];
		currOffset+=inputSize_;
		s2_.currOffset+=inputSize_;
		if (inputSize_==0) inputSize_=size;
		while(inputSize_)
		{
			const W32 toCopy = VL_MIN(inputSize_,512);
			fread(temp,toCopy,1,f);
			fwrite(temp,toCopy,1,s2_.f);
			inputSize_-=toCopy;
		}
	}
};
SW32 verifyStreams( Stream s1_, Stream s2_, SW32 size_)
{
	SW32 diffCounts = 0;
	s1_.open("r");
	s2_.open("r");
	for (SW32 i=0; i< size_; i++)
	{
		const W8 a = s1_.read(), b=s2_.read();
		diffCounts += a!=b;
	}
	s1_.close();
	s2_.close();
	if (diffCounts)
	{	log("<!> verificacao: falharam %u bytes de %u!", diffCounts,size_); }
	return diffCounts;
}
W8 conPortBuff[30];
W0 conProgressCallback(W64 done_, W64 total_, SW8* appendix_=0)
{
	if (done_==0)
		for (W32 i=0; i< 30; i++) conPortBuff[i] = 176;

	W32 x,y; VL_SCR_GETCURSOR(x,y);
	
	VL_FP toIp = (VL_FP)done_/(VL_FP)total_;
	W32 toI = toIp*29.0f;
	for (W32 i=0; i< toI; i++) 
		conPortBuff[i] = 178;

	VL_SCR_CLEAR_CHAR_LINES(y+1,2,32)
	VL_SCR_SETCURSOR(0,y+1);
	printf("[%s] %3u%%", conPortBuff, (W32)(toIp*100.0f));
	if (appendix_) 
		printf(" %s", appendix_);

	if (done_==total_)
	{
		VL_SCR_SETCURSOR(0,y+1);
		printf("%s", std::string(80,32).c_str());
	}
	VL_SCR_SETCURSOR(x,y);
}

enum PortType { PORT_LPT, PORT_PIPE, PORT_COM};
class Port
{
public:
	HANDLE hnd;
	PortType type;
	DCB params;
	std::string portName;
	W0 (*progressCallback)(W64, W64, W0*);
	W0* progressCallbackArg;
	W64 totalRx, totalTx; 
	
	W0 close()	
	{
		if (hnd) CloseHandle(hnd);
		hnd=0;
	}
	W8 setSerialParams(W32 bauds_, W32 dataSize_,  W32 stopBits_, W8 parity_)
	{
		GetCommState(hnd, &params);
		params.BaudRate   = bauds_;  
		params.ByteSize   = dataSize_;    
		params.StopBits	  = stopBits_;
		params.Parity	  = parity_;
		SetCommState(hnd, &params);
		return 0;
	}
	W8 setSerialParamsForArduino(W32 bauds_, W32 dataSize_,  W32 stopBits_, W8 parity_)
	{
		memset(&params, 0, sizeof(DCB));
		// conf. arduino (controlo de fluxo por software)
		params.BaudRate   = bauds_;  
		//params.fBinary = 1;
		//params.fDtrControl = 1;
		//params.fTXContinueOnXoff = 1;
		//params.fRtsControl = 1;
		//params.XonLim = 0x800;
		//params.XoffLim = 0x200;
		//params.ByteSize   = dataSize_;         
		//params.Parity     = parity_;  
		//params.StopBits   = stopBits_;
		//params.XoffChar   = (SW8)0xE0;
		//params.ErrorChar  = (SW8)0xF6;
		//params.EofChar    = (SW8)0x0C;
		params.DCBlength	     = 0x0000001c; 
		params.fBinary	         = 0x00000001;
		params.fParity	         = 0x00000000;
		params.fOutxCtsFlow	     = 0x00000000;
		params.fOutxDsrFlow      = 0x00000000;
		params.fDtrControl	     = 0x00000001;
		params.fDsrSensitivity	 = 0x00000000;
		params.fTXContinueOnXoff = 0x00000001;
		params.fOutX	         = 0x00000000;
		params.fInX              = 0x00000000;
		params.fErrorChar 	     = 0x00000000;
		params.fNull             = 0x00000000;
		params.fRtsControl	     = 0x00000001;
		params.fAbortOnError     = 0x00000000;
		params.fDummy2	         = 0x00000000;
		params.wReserved         = 0x0000;
		params.XonLim	         = 0x866c;
		params.XoffLim	         = 0x219b;
		params.ByteSize	         = 0x08;
		params.Parity	         = 0x00;
		params.StopBits	         = 0x00;
		params.XonChar	         = (SW8)0x11;
		params.XoffChar	         = (SW8)0x13;
		params.ErrorChar         = (SW8)0xf6;
		params.EofChar	         = 0x0c;
		params.EvtChar           = 0x00;
		params.wReserved1        = 0x0000;
		SetCommState(hnd, &params);
		//GetCommState(hnd, &params);
		return 0;
	}
	W8 open(std::string port_, W32 pipeWaitTimeout_=2000)	
	{
		hnd= 0;
		if (port_.size()>3) 
		{
			type = (port_.substr(0,3)=="COM")? PORT_COM:
				   (port_.substr(0,3)=="LPT")? PORT_LPT: PORT_PIPE;
		} else type = PORT_PIPE;

		portName = "\\\\.\\"+port_;
 
		while(1)
		{
			hnd = CreateFile(portName.c_str(),              //nome da porta
									GENERIC_READ | GENERIC_WRITE, // R/W
									0,                            // OVERLAPPED iria ser concurrente
									NULL,                         // Sem segurança
									OPEN_EXISTING,					 // é obrigatório
									  FILE_FLAG_NO_BUFFERING,		 // não é concurrido? FILE_FLAG_OVERLAPPED se for
									0);									 // é obrigatório a 0
			// FILE_FLAG_OVERLAPPED
			if (hnd == INVALID_HANDLE_VALUE) 
				return 1; 
 
			if (type==PORT_PIPE && GetLastError() == ERROR_PIPE_BUSY)
			{
				log("pipe ocupado, esperando que fique livre...");
				if ( !WaitNamedPipe(portName.c_str(), pipeWaitTimeout_) ) 
				{ 
					log("excedidos %u ms de timeout", pipeWaitTimeout_);
					return 1;
				} 
			}else break;
		}
		return 0;
	}
	W0 setTimeouts(W32 timeout_=500, W32 multiplier_=1, W32 constant_=1)
	{
	  COMMTIMEOUTS timeouts = { 0 };
	 // GetCommTimeouts(hnd, &timeouts);
	  timeouts.ReadIntervalTimeout         = timeout_;
	  timeouts.ReadTotalTimeoutConstant    = constant_;
	  timeouts.ReadTotalTimeoutMultiplier  = multiplier_;
	  timeouts.WriteTotalTimeoutConstant   = constant_;
	  timeouts.WriteTotalTimeoutMultiplier = multiplier_;
	  SetCommTimeouts(hnd, &timeouts);
	}
	W8 tx(W0* buf_, W32 bsize_)
	{
		const W1 isBig = bsize_>4096;
		W32 bsize = bsize_;
		DWORD toRXTX, bytes = 0;
		BOOL Status;
		W8* buf = (W8*)buf_;
		if (isBig && progressCallback) progressCallback(0, bsize, progressCallbackArg);
		while(bsize_)
		{
			toRXTX = VL_MIN(bsize_, 2048);
			Status = WriteFile(hnd,	buf,	toRXTX, &bytes, NULL);
			if (bytes!=toRXTX)
				return 1;
			bsize_-=toRXTX;
			buf+=toRXTX;
			if (isBig && progressCallback) progressCallback( buf-(W8*)buf_, bsize, progressCallbackArg);
		}
		if (isBig && progressCallback) progressCallback(bsize, bsize, progressCallbackArg);
		totalTx+=bsize;
		return bsize_;
	}
	W8 rx(W0* buf_, W32 bsize_)
	{
		const W1 isBig = bsize_>4096;
		W32 bsize = bsize_;
		DWORD toRXTX, bytes = 0;
		BOOL Status;
		W8* buf = (W8*)buf_;
		if (isBig && progressCallback) progressCallback(0, bsize, progressCallbackArg);
		while(bsize_)
		{
			toRXTX = VL_MIN(bsize_, 2048);
			Status = ReadFile(hnd,	buf,	toRXTX, &bytes, NULL);
			if (bytes!=toRXTX)
				return 1;
			bsize_-=toRXTX;
			buf+=toRXTX;
			if (isBig && progressCallback) progressCallback( buf-(W8*)buf_, bsize, progressCallbackArg);
		}
		if (isBig && progressCallback) progressCallback(bsize, bsize, progressCallbackArg);
			
		totalRx+=bsize;
		return bsize_;
	}	
	Port()
	{		
		progressCallback = 0;
		hnd=0;
	}
	~Port()
	{
		if (hnd)
			CloseHandle(hnd);
	}
};

