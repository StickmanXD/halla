/**********************************************************************************************************************
 * \file GTM_TOM_Interrupt.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "IfxPort.h"
#include "IfxGtm_Tom_Timer.h"
#include "Bsp.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_TOM    1                       /* Interrupt priority number                                    */
#define TOM_FREQ            2.0f                    /* TOM frequency                                                */
#define LED                 &MODULE_P00, 5          /* LED which will be toggled in Interrupt Service Routine (ISR) */
#define Test_PIN1           &MODULE_P02, 0
#define WAIT_TIME           1

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxGtm_Tom_Timer g_timerDriver;                     /* TOM driver                                                   */

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define the Interrupt Service Routine. */
IFX_INTERRUPT(interruptGtmTom, 0, ISR_PRIORITY_TOM);


void make_1ms_pulse(void){
    IfxPort_setPinState(Test_PIN1,IfxPort_State_high);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER,WAIT_TIME));
    IfxPort_setPinState(Test_PIN1,IfxPort_State_low);

}
/* Interrupt Service Routine of the TOM */
void interruptGtmTom(void)
{
    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_timerDriver);                       /* Clear the timer event            */
    IfxPort_togglePin(LED);                                                     /* Toggle the LED                   */
    make_1ms_pulse();
}

/* This function initializes the TOM */
void initGtmTom(void)
{
    IfxGtm_enable(&MODULE_GTM); /* Enable GTM */

    IfxGtm_Tom_Timer_Config timerConfig;                                        /* Timer configuration              */
    IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);                     /* Initialize timer configuration   */

    timerConfig.base.frequency       = TOM_FREQ;                                /* Set timer frequency              */
    timerConfig.base.isrPriority     = ISR_PRIORITY_TOM;                        /* Set interrupt priority           */
    timerConfig.base.isrProvider     = IfxSrc_Tos_cpu0;                         /* Set interrupt provider           */
    timerConfig.tom                  = IfxGtm_Tom_1;                            /* Define the timer used            */
    timerConfig.timerChannel         = IfxGtm_Tom_Ch_0;                         /* Define the channel used          */
    timerConfig.clock                = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk3;          /* Define the CMU clock used        */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);               /* Enable the CMU clock             */
    IfxGtm_Tom_Timer_init(&g_timerDriver, &timerConfig);                        /* Initialize the TOM               */

    IfxPort_setPinModeOutput(LED, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);  /* Set pin mode         */
    IfxPort_setPinModeOutput(Test_PIN1, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);  /* Set pin mode         */

    IfxGtm_Tom_Timer_run(&g_timerDriver); /* Start the TOM */
}
