#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "GPT12_PWM_Capture.h"

IfxCpu_syncEvent g_cpuSyncEvent = 0;
uint32 finalCndVal_main = 0;
uint32 g_cntOverflow_main = 0;
uint32 g_previousCntVal_main = 0;
uint32 currentCntVal_main = 0;

float32 T3_frequency_main = 0;
int core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /* Initialize the GPT module */
    init_GPT12_module();

    while(1)
    {
        finalCndVal_main;
        g_cntOverflow_main;
        g_previousCntVal_main;
        currentCntVal_main;
        currentCntVal_main;
        generate_PWM_signal();
    }
    return (1);
}
