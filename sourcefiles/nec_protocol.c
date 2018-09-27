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

#include "stm32f4xx.h"
#include "nec_protocol.h"

volatile uint8_t transDone = 0;
volatile int8_t cnt = -1;
volatile uint32_t captVal[32];
volatile uint8_t NEC_status = 0;

/*
 * Configure TIM2 channel 1 as input capture with falling
 * edge detection
 */
static void TIM2_init()
{
	const uint8_t TIM2PSC = 16;
	const uint32_t TIM2PERIOD = 100000;

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM2->CCER |= TIM_CCER_CC1P;
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->DIER |= TIM_DIER_CC1IE;
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM2->PSC = TIM2PSC - 1;
	TIM2->ARR = TIM2PERIOD - 1;

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 0);

}
/*
 * Initialize NEC protocol transmission
 */
void NEC_init()
{
	TIM2_init();
}
/*
 * Decode signal and return as 32 bit quantity
 */
uint32_t NEC_decode()
{
	const uint16_t freq_500hz = 2000;
	uint32_t dec = 0;

	if (transDone == 1)
	{
		for(int i = 0; i < sizeof(captVal); i++)
		{
			if(captVal[i] >= freq_500hz) // then frequency is less then 500 Hz
			{
				dec |= (1 << (31 - i)); // value is equal to 1

			}
			else
			{
				dec |= (0 << (31 - i)); // value is equal to 0
			}
		}
		NEC_status = 1;
	}
	transDone = 0;
	return dec;
}
/*
 * The interrupt is configured to trigger on every falling edge of signal
 */
void TIM2_IRQHandler()
{
	volatile const uint16_t freq_80hz = 12500, freq_70Hz = 14285;
	volatile const uint16_t freq_1000hz = 1000, freq_300Hz = 3333;
	volatile const uint32_t freq_12_5hz = 80000, freq_7Hz = 150000;
	volatile uint32_t capture = 0;

	if(TIM2->SR & TIM_SR_CC1IF)
	{
		capture = TIM2->CCR1;
		if(((capture > freq_80hz) && (capture < freq_70Hz)) && (cnt == -1)) // then it's start of frame (period > 13.5 ms)
		{
			cnt ++; // increase counter and skip first measure
		}
		if((capture > freq_1000hz && capture < freq_300Hz) &&(cnt >= 0)) // then period is equal to 1.25 ms or 2.25 ms
		{
			captVal[cnt] = capture; // write captured value to buffer
			cnt ++;

			if(cnt == 32) // buffer is full, mark that transmission is done
			{
				transDone = 1;
				cnt = -1;
			}
		}
		if((capture > freq_12_5hz) && (capture < freq_7Hz)) // then key remains down and period is equal to 110 ms
		{
			transDone = 1;
		}
		TIM2->CNT = 0;
	}
}
/*
 * Associate decoded value with controller buttons
 */

const char* NEC_associate(uint32_t decNum)
{
	static char* buttonNames[] = {"CH-","CH","CH+","PREV","NEXT","PLAY/PAUSE","VOL-","VOL+","EQ","0","100+","200+","1","2","3","4"
								 ,"5","6","7","8","9"};
	static char* buttonName[] = {"ButtonName"};

	switch(decNum)
	{
	case NEC_CHM:
		buttonName[0] = buttonNames[0];
		break;

	case NEC_CH:
		buttonName[0] = buttonNames[1];
		break;

	case NEC_CHP:
		buttonName[0] = buttonNames[2];
		break;

	case NEC_PREV:
		buttonName[0] = buttonNames[3];
		break;

	case NEC_NEXT:
		buttonName[0] = buttonNames[4];
		break;

	case NEC_PLPA:
		buttonName[0] = buttonNames[5];
		break;

	case NEC_VOLM:
		buttonName[0] = buttonNames[6];
		break;

	case NEC_VOLP:
		buttonName[0] = buttonNames[7];
		break;

	case NEC_EQ:
		buttonName[0] = buttonNames[8];
		break;

	case NEC_0:
		buttonName[0] = buttonNames[9];
		break;

	case NEC_100P:
		buttonName[0] = buttonNames[10];
		break;

	case NEC_200P:
		buttonName[0] = buttonNames[11];
		break;

	case NEC_1:
		buttonName[0] = buttonNames[12];
		break;

	case NEC_2:
		buttonName[0] = buttonNames[13];
		break;

	case NEC_3:
		buttonName[0] = buttonNames[14];
		break;

	case NEC_4:
		buttonName[0] = buttonNames[15];
		break;

	case NEC_5:
		buttonName[0] = buttonNames[16];
		break;

	case NEC_6:
		buttonName[0] = buttonNames[17];
		break;

	case NEC_7:
		buttonName[0] = buttonNames[18];
		break;

	case NEC_8:
		buttonName[0] = buttonNames[19];
		break;

	case NEC_9:
		buttonName[0] = buttonNames[20];
		break;
	}
	return buttonName[0];
}
