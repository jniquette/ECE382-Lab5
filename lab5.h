//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.h
// Date:	Fall 2014
// Purp:	Include file for the MSP430
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Page 76 : MSP430 Optimizing C/C++ Compiler v 4.3 User's Guide
//-----------------------------------------------------------------
typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

#define		TRUE				1
#define		FALSE				0

//-----------------------------------------------------------------
// Function prototypes found in lab5.c
//-----------------------------------------------------------------
void initMSP430();
__interrupt void pinChange (void);
__interrupt void timerOverflow (void);


//-----------------------------------------------------------------
// Each PxIES bit selects the interrupt edge for the corresponding I/O pin.
//	Bit = 0: The PxIFGx flag is set with a low-to-high transition
//	Bit = 1: The PxIFGx flag is set with a high-to-low transition
//-----------------------------------------------------------------

#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6


#define		averageLogic0Pulse	540		//0x0200
#define		averageLogic1Pulse	540*3	//0x0645
#define		averageStartPulse	4433	//hi only	//low and hi = 13336	//0x1100
#define		minLogic0Pulse		375		//from confidence interval //averageLogic0Pulse - 100
#define		maxLogic0Pulse		705		//from confidence interval	//averageLogic0Pulse + 100
#define		minLogic1Pulse		375*3	//averageLogic1Pulse - 100
#define		maxLogic1Pulse		705*3	//averageLogic1Pulse + 100
#define		minStartPulse		averageStartPulse - 500
#define		maxStartPulse		averageStartPulse + 500

#define		POWER		0x2CD3750A
#define		ZERO		0x2CD3057A
#define		ONE			0x2CD32D52
#define		TWO			0x2CD33D42
#define		THREE		0x2CD33946

#define		BIG_UP		0x2CD36C13
#define		BIG_DOWN	0xACD3601F
#define		BIG_LEFT	0xACD3304F
#define		BIG_RIGHT	0xACD3205F
