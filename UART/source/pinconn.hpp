#pragma once

#include <cstdint>
#include "L0_LowLevel/LPC40xx.h"
#include <iterator>


class pinconn
{  
public:
    void inactive(uint8_t port, uint8_t pin);
    void pullup(uint8_t port, uint8_t pin);
    void pulldown(uint8_t port, uint8_t pin);
    void repeater(uint8_t port, uint8_t pin);
    void ssp_clk(uint8_t port, uint8_t pin);
    void ssp_miso(uint8_t port, uint8_t pin);
    void ssp_mosi(uint8_t port, uint8_t pin);
    void clearF4(uint8_t port, uint8_t pin);
    void uart2_txd(uint8_t port, uint8_t pin); 
    void uart2_rxd(uint8_t port, uint8_t pin);
    void uart3_txd(uint8_t port, uint8_t pin); 
    void uart3_rxd(uint8_t port, uint8_t pin);

    void uart2_oe(uint8_t port, uint8_t pin);
    void clear7(uint8_t port, uint8_t pin);
};
