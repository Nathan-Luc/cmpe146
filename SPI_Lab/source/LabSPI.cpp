#include "LabSPI.hpp"
     inline static LPC_SSP_TypeDef * SSP[3] = {LPC_SSP0, LPC_SSP1, LPC_SSP2};
bool Initialize(uint8_t data_size_select, LabSpi::FrameModes format, uint8_t divide, LabSpi::Peripheral SSPn)
{
  // Initialize SSP peripheral
  
  
  switch(SSPn)
  
  case 0:

  case 1:

  
  case 2: 
  //SSP 2
  LPC_SC -> PCONP |= (1<<20);
  LabGPIO P1_10(1,10); //CE 
  LabGPIO P1_04(1,4); // MISO
  LabGPIO P1_00 (1,0); //SCLK
  LabGPIO P1_01(1,1); //MOSI
  P1_10.SetLow();
  P1_04.SetAsInput();
  P1_00.SetAsOutput();
  P1_01.SetAsOutput();
  SSP[SSPn]->CR0 &= ~(0xF<<0);
  SSP[SSPn]->CR0 |= (data_size_select-1);
  SSP[SSPn]->CR0 &= ~(0xF<<4);
  SSP[SSPn]->CR1 &= ~(0b111 << 0);
  SSP[SSPn]->CR1 |= (0b1<<1);
}