#include <stdint.h>
#include "../inc/tm4c123gxl.h"
#include "lib/include.h"
 
// fin  = fxtal / [(Q+1) * (N+1)]
//      = 25MHz / [(0+1) * (4+1)] = 5MHz
// fvoc = fin * MDIV
//      = 5MHz * 96.00 = 480MHz
// fsyclk = fvco / (PSYSDIV + 1)
//        = 480 / (3 + 1 ) = 120MHz
#define MINT    96
#define MFRAC   0
#define N       4
#define Q       0
#define PSYSDIV 3
 
#define FBCHT   0x6
#define EBCHT   0x6
#define FBCE    0
#define EBCE    0
#define FWS     0x5
#define EWS     0x5
#define MB1     0x00100010
 extern void SystemClock_120MHz(void)
{
    // 1. Configure high-freq oscillator, power to main oscillator, and use external osiallator
    //    by setting SYSCTL_MOSCCTL register
    SYSCTL->MOSCCTL |= 1 << 4;          // high speed crystal connected to MOSC (>= 10 MHz)
    SYSCTL->MOSCCTL &= ~(1 << 2);       // enable external oscillator
    SYSCTL->MOSCCTL &= ~(1 << 3);       // power up the main oscillator
     
    // 2. Waiting for Oscillator ready by polling SYSCTL_RIS
    while(!(SYSCTL->RIS & (1 << 8))){}; // wait for oscillator ready
 
    // 3. Configure PLL clock source by SYSCTL_RSCLKCFG
    SYSCTL->RSCLKCFG &= ~(0xFF << 20);  // clear oscillator and PLL source field
    SYSCTL->RSCLKCFG |= (0x3 << 20);    // set MOSC as oscillator source
    SYSCTL->RSCLKCFG |= (0x3 << 24);    // set MOSC as PLL input source
 
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
}
