/**********************************************************************************************************************************
 * The MIT License
 *
 * Copyright (c) 2018 Dranzey. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *Contact information:
 *
 *email: dranzey@outlook.com
 *
 *********************************************************************************************************************************/

#ifndef HEADERFILES_NEC_PROTOCOL_H_
#define HEADERFILES_NEC_PROTOCOL_H_
/*
 * Define labels of remote control
 */
#define NEC_CHM 0xFFA25D
#define NEC_CH 0xFF629D
#define NEC_CHP 0xFFE21D
#define NEC_PREV 0xFF22DD
#define NEC_NEXT 0xFF02FD
#define NEC_PLPA 0xFFC23D
#define NEC_VOLM 0xFFE01F
#define NEC_VOLP 0xFFA857
#define NEC_EQ 0xFF906F
#define NEC_0 0xFF6897
#define NEC_100P 0xFF9867
#define NEC_200P 0xFFB04F
#define NEC_1 0xFF30CF
#define NEC_2 0xFF18E7
#define NEC_3 0xFF7A85
#define NEC_4 0xFF10EF
#define NEC_5 0xFF38C7
#define NEC_6 0xFF5AA5
#define NEC_7 0xFF42BD
#define NEC_8 0xFF4AB5
#define NEC_9 0xFF52AD
/*
 * nec_protocol.c external variables
 */
extern volatile uint8_t NEC_status;

/*
 * nec_protocol.c function prototypes
 */
void NEC_init();
uint32_t NEC_decode();
void TIM2_IRQHandler();
const char* NEC_associate(uint32_t);

#endif /* HEADERFILES_NEC_PROTOCOL_H_ */
