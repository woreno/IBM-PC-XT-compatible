#define DMA_CHAN0_ADDR	0
#define DMA_CHAN0_CNT	1
#define DMA_STATUS		8
#define DMA_CMD			8	
#define DMA_REQUEST		9	
#define DMA_MASK_CHAN	10	
#define DMA_MODES		11	
#define DMA_REG16_RESET	12	
#define DMA_RESET		13
#define DMA_ERASEMASK	14
#define DMA_MASK		15 
#define M_WaitDMA
#define W8				unsigned char

#define SET_PORT_OUTPUT(W_)    DDR##W_=0xFF;
#define SET_PORT_INPUT(W_)	   DDR##W_=0x0; PORT##W_=0x0; 

//PA0:7 XD7:0 databus
//PC0:3 XA0:3 addr
//PB0:3 0:DREQ0,1:CS,2:XIOW

#define DREQ_TOGGLE PORTB^=1;
#define RST_LO		  PORTB=0;
#define RST_HI		  PORTB=8;
#define CS_XIOW_LO  PORTB&=~6;
#define CS_HI       PORTB|=2;
#define XIOW_HI     PORTB|=1;

#define DATA_PORT PORTA
#define ADDR_PORT PORTC
#define CTRL_PORT PORTB
#define W8_BIT(V,START)         (((V)>>(START))&1)          
#define W8_REVERSE(W)         (W8_BIT(W,7)|(W8_BIT(W,6)<<1)|(W8_BIT(W,5)<<2)|(W8_BIT(W,4)<<3)|(W8_BIT(W,3)<<4)|(W8_BIT(W,2)<<5)|(W8_BIT(W,1)<<6)|(W8_BIT(W,0)<<7))

void out(W8 addr_,W8 data_)
{
  data_ = W8_REVERSE(data_);
	ADDR_PORT = addr_;
	DATA_PORT = data_;
  delay(20); 
	CS_XIOW_LO
	delayMicroseconds(10); 
	XIOW_HI
  delayMicroseconds(10); 
  CS_HI
  delay(20); 
}
void setup() 
{
	SET_PORT_OUTPUT(A)
	SET_PORT_OUTPUT(B)
	SET_PORT_OUTPUT(C)
	RST_HI
	delay(200);
	RST_LO
  DREQ_TOGGLE
}
 
void loop() 
{

  W8 al = 1;

  
  al = 0;
  // configurar DMA
  out(DMA_RESET, al);
  M_WaitDMA
  
  // configurar DMA
  out(DMA_REG16_RESET, al);
  M_WaitDMA
  out(DMA_CHAN0_ADDR, al);
  M_WaitDMA
  out(DMA_CHAN0_ADDR, al);
  M_WaitDMA

  al = 0xFF;
  out(DMA_REG16_RESET, al);
  M_WaitDMA
  out(DMA_CHAN0_CNT, al);
  M_WaitDMA
  out(DMA_CHAN0_CNT, al);
  M_WaitDMA

  al = 0x58;
  out(DMA_MODES, al);
  M_WaitDMA

  al = 0xFE;
  out(DMA_MASK, al);
  M_WaitDMA

  al = 0;
  out(DMA_MASK_CHAN, al);
  M_WaitDMA

  out(DMA_CMD, al);
  M_WaitDMA

  SET_PORT_INPUT(A)
  SET_PORT_INPUT(C)
	// ligar DREQ
	while(1)	
	{
		DREQ_TOGGLE
		delay(20); 
		DREQ_TOGGLE
		delay(20); 
	}
}
