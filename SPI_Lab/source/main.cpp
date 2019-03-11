#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>
#include "L0_LowLevel/interrupt.hpp"
#include <cstdint>
#include <iterator>
#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
#include "pinconn.hpp"
#include "L3_Application/oled_terminal.hpp"
#include "LabSPI.hpp"
  LabGPIO P1_10(1,10); //CE
  LabGPIO P1_04(1,4); // MISO
  LabGPIO P1_00 (1,0); //SCLK
  LabGPIO P1_01(1,1); //MOSI
  OledTerminal oled_terminal;
  pinconn pc;
void SpiInitialize(void)
{
  // Initialize SSP peripheral 2
  LPC_SC -> PCONP |= (1<<20);
  P1_10.SetLow();
  pc.ssp_clk(1,0);
  pc.ssp_miso(1,4);
  pc.ssp_mosi(1,1);
  LPC_SSP2->CPSR |= (1<<2);
  LPC_SSP2->CR0 = (0b1000000111);
//  LPC_SSP2->CR0 &= ~(0xFF<<0);
  //LPC_SSP2->CR0 |= (0b0111<<0);
  //LPC_SSP2->CR0 |= (1<<8);
  LPC_SSP2->CR1 &= ~(0b111 << 0);
  LPC_SSP2->CR1 |= (0b1<<1);

}

uint8_t SpiTransfer(uint8_t data_out)
{
  // Send data_out and retrieve returned byte
    if(LPC_SSP2->SR & (1<<1))
    {
        LPC_SSP2->DR = data_out;
    }
    else return 0xFF;
    while(LPC_SSP2->SR & (1<<4)){}
    if(LPC_SSP2->SR &(1<<2))
    {
        
        return static_cast<uint16_t>(LPC_SSP2->DR);
    }
    
}

// WARNING: This is just a sample; you will have to fill in plenty of your own code as per requirements
void ReadSignature()
{
  uint16_t data[4];
  
  // The simplest test is to try to read the signature of the Adesto flash and print it out
  //AdestoSelect();
  P1_10.SetLow();
  P1_10.SetAsOutput();
    // TODO: Find the correct commands you need to send to the Adesto flash device 
    //       to retrieve its device signature.
    SpiTransfer(0x9F);
    for(int i =0; i<4; i++){
    data[i] = static_cast<uint8_t>(SpiTransfer(0x9F));
    
    }
  LOG_INFO("STOP");
  P1_10.SetHigh();
  
  oled_terminal.printf("Returned data: %x %x %x %x\n", data[0], data[1], data[2], data[3]);
}


int main() 
{ 
  LOG_INFO("Start\n");
  
  LabSpi Demo(LabSpi::kSS2);
  Demo.Initialize(8,LabSpi::kSPI, 1, LabSpi::kMaster);
  Demo.read();
  
  //oled_terminal.Initialize();
  //SpiInitialize();
  //ReadSignature();
  
}
