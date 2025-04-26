#pragma once
#include "common.hpp"

namespace cpp_freertos
{

    /**
     *  Wrapper class around various critical section type
     *  synchronization mechanisms within FreeRTOS.
     */
    class CriticalSection
    {

        /////////////////////////////////////////////////////////////////////////
        //
        //  Public API
        //  Available from anywhere.
        //
        /////////////////////////////////////////////////////////////////////////
    public:
        /**
         *  Disable context switches as well as maskable interrupts.
         */
        static inline void Enter()
        {
            // printf("%s\n", __FUNCTION__);
        }

        /**
         *  Re-enable context switches.
         */
        static inline void Exit()
        {
            // printf("%s\n", __FUNCTION__);
        }

        /**
         *  Disable context switches as well as maskable interrupts
         *  from an interrupt context.
         *
         *  @return Opaque representation of interrupt mask state.
         *  This must be passed back to the corresponding call to
         *  ExitFromISR().
         *
         *  @note See the following for further details:
         *  http://www.freertos.org/taskENTER_CRITICAL_FROM_ISR_taskEXIT_CRITICAL_FROM_ISR.html
         */
        static inline BaseType_t EnterFromISR()
        {
            // printf("%s\n", __FUNCTION__);
            return 0;
        }

        /**
         *  Re-enable context switches from an interrupt context.
         *
         *  @param savedInterruptStatus This should be the value you
         *  received from calling EnterFromISR().
         *
         *  @note See the following for further details:
         *  http://www.freertos.org/taskENTER_CRITICAL_FROM_ISR_taskEXIT_CRITICAL_FROM_ISR.html
         */
        static inline void ExitFromISR(BaseType_t savedInterruptStatus)
        {
            // printf("%s\n", __FUNCTION__);
        }

        /**
         *  Disable all maskable interrupts.
         */
        static inline void DisableInterrupts()
        {
            // printf("%s\n", __FUNCTION__);
        }

        /**
         *  Enable all maskable interrupts.
         */
        static inline void EnableInterrupts()
        {
            // printf("%s\n", __FUNCTION__);
        }

        /**
         *  Suspend the scheduler without disabling interrupts.
         */
        static inline void SuspendScheduler()
        {
            // printf("%s\n", __FUNCTION__);
        }

        /**
         *  Re-enable the scheduler.
         */
        static inline void ResumeScheduler()
        {
            // printf("%s\n", __FUNCTION__);
        }
    };

}
