/**
 * main.c
 */
#include "lib/include.h"

void SetSystemClock_40MHz(void)
{
	// 1. Configure high-freq oscillator, power to main oscillator, and use external oscillator
    //    by setting SYSCTL_MOSCCTL register
    SYSCTL->MOSCCTL |= 1 << 4;          // high speed crystal connected to MOSC (>= 10 MHz)
    SYSCTL->MOSCCTL &= ~(1 << 2);       // enable external oscillator
    SYSCTL->MOSCCTL &= ~(1 << 3);       // power up the main oscillator

    // 2. Waiting for Oscillator ready by polling SYSCTL_RIS
    while(!(SYSCTL->RIS & (1 << 8))){}; // wait for oscillator ready

    // 3. Configure PLL clock source by SYSCTL_RSCLKCFG
    SYSCTL->RSCLKCFG &= ~(0xFF << 20);  // clear oscillator and PLL source field
    SYSCTL->RSCLKCFG |= (0x3 << 20);    // set MOSC as oscillator source
    SYSCTL->RSCLKCFG |= (0x3 << 24);    // set MOSC as PLL input source/////////////////////////////////////

    // 4. Setup PLL Freq and turn on the PLL power by SYSCTL_PLLFREQ0 and FREQ1
    SYSCTL->PLLFREQ0 = (MFRAC << 10) | MINT;    // set PLL M integer and fractional values
    SYSCTL->PLLFREQ1 = (Q << 8) | N;            // set PLL N and Q
    SYSCTL->PLLFREQ0 |= 1 << 23;                // power up the PLL

    // 5. Configure the memory timing parameter by SYSCTL->MEMTIM0
    // SYSCTL->MEMTIM0 = (EBCHT << 22) | (EBCE << 21) | (EWS << 16) | (FBCHT << 6) | (FBCE << 5) | FWS;      // set memory timing parameters
    SYSCTL->MEMTIM0 = (EBCHT << 22) | (EBCE << 21) | (EWS << 16) | (FBCHT << 6) | (FBCE << 5) | FWS; // set memory timing parameters
    SYSCTL->MEMTIM0 |= (1 << 4) | (1 << 20);

    // 6. Wait for PLL to lock by polling SYSCTL_PLLSTAT
    while( (SYSCTL->PLLSTAT & 0x01) != 0x01) {};

    // 7. Enable MEMTIM update, New PLLFREQ accept, and USEPLL
    SYSCTL->RSCLKCFG |= (1 << 31) | (1 << 30) | (1 << 28) | PSYSDIV;
    // set PLL system clock divisor, lock the system clock to the PLL, and force a memory timing register update
}

void Configurar_UART3(void)
{
	SYSCTL->RCGCUART = (1<<3); // Habilitar UART 3
    SYSCTL->RCGCGPIO |= (1<<0); // Habilitar puerto A          Pag 1163
    GPIOA_AHB->DIR = (1<<5) | (0<<4); // Entrada(Rx) y salida(Tx)
    GPIOA_AHB->AFSEL = (1<<5 | 1<<4); //A5 Tx, A4 Rx
    GPIOA_AHB->PCTL = (0x01<<20) | (0x01<<16); // indica que tipo de periferico
    GPIOA_AHB->DEN = (1<<5) | (1<<4); //Digitales

    UART3->CTL = (0<<9) | (0<<8) | (0<<0);
    //IBRD = 40,000,000 / (16 * 115200) = 21.7013   pag 1172
    //FBRD = integer(0.7013 * 64 + 0.5) = 45.38
    UART3->IBRD=21;
    UART3->FBRD=45;
    UART3->LCRH = (0x3<<5)|(0<<4); //1186, bit 5 longitud de palabra (min 5 max 8) || Bit 4 desabilita la pila
    UART3->CC = (0<<0); // seleccionar reloj del sistema
    UART3->CTL = (1<<0)|(1<<8)|(1<<9); // bit 8 y 9 habilitan transmisi�n y recepci�n, bit 0 habilita el UART
}

char readChar(void)
{
	int v;
    char c;
    while ((UART3->FR & (1<<4)) !=0);
    v = UART3->DR & 0xFF;
    c=v;
    return c;
}

void writeDato(char c)
{
	while((UART3->FR & (1<<5)) != 0);
    UART3->DR = c;
}

int main(void)
{
	SetSystemClock_40MHz();
	Configurar_UART3();

	uint32_t a;
	while(1)
	{
		//Lectura de dato
	    Dato=readChar();
	    //Escritura de dato
	    writeDato('H');
	    for(a=0;a<200000;a++){}
	    writeDato('O');
	    for(a=0;a<200000;a++){}
	    writeDato('L');
	    for(a=0;a<200000;a++){}
	    writeDato('A');
	    for(a=0;a<200000;a++){}
	}

}
