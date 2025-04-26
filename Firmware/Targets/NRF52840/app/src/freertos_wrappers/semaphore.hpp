#pragma once
/**
 *  C++ exceptions are used by default when constructors fail.
 *  If you do not want this behavior, define the following in your makefile
 *  or project. Note that in most / all cases when a constructor fails,
 *  it's a fatal error. In the cases when you've defined this, the new 
 *  default behavior will be to issue a configASSERT() instead.
 */
#include <exception>
#include <string>
#include <cstdio>
#include <zephyr/kernel.h>
#include "common.hpp"

namespace cpp_freertos {


/**
 *
 *  Base wrapper class around FreeRTOS's implementation of semaphores.
 *
 *  It is not expected that an application will derive from this class.
 *
 *  Note that we distinguish between Semaphore, Binary Semaphores,
 *  Counting Semaphores, and Mutexes. Mutexes, while implemented as a kind
 *  of semaphore in FreeRTOS, are conceptually very different in use and
 *  behavior from semaphores. We acknowledge this difference in the class
 *  heirarchy, implementing mutextes as a completely different class heirarchy.
 */
class Semaphore {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
        /**
         *  Aquire (take) a semaphore.
         *
         *  Example of blocking indefinitely:
         *      aSemaphore.Take();
         *
         *  Example of blocking for 100 ticks:
         *      aSemaphore.Take(100);
         *
         *  @param Timeout How long to wait to get the Lock until giving up.
         *  @return true if the Semaphore was acquired, false if it timed out.
         */
        bool Take(TickType_t Timeout = portMAX_DELAY);

        /**
         *  Release (give) a semaphore.
         *
         *  @return true if the Semaphore was released, false if it failed.
         */
        bool Give();

        /**
         *  Aquire (take) a semaphore from ISR context.
         *
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @return true if the Semaphore was acquired, false if it timed out.
         */
        bool TakeFromISR(BaseType_t *pxHigherPriorityTaskWoken);

        /**
         *  Release (give) a semaphore from ISR context.
         *
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @return true if the Semaphore was released, false if it failed.
         */
        bool GiveFromISR(BaseType_t *pxHigherPriorityTaskWoken);

        /**
         *  Our destructor
         */
        virtual ~Semaphore();

    /////////////////////////////////////////////////////////////////////////
    //
    //  Protected API
    //  Not intended for use by application code.
    //
    /////////////////////////////////////////////////////////////////////////
    protected:
        /**
         *  FreeRTOS semaphore handle.
         */
        struct k_sem handle;

        /**
         *  We do not want a Semaphore ctor. This class should never be
         *  directly created, this is a base class only.
         */
        Semaphore();
};


/**
 *  Wrapper class for Binary Semaphores.
 */
class BinarySemaphore : public Semaphore {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
        /**
         *  Constructor to create a binary semaphore.
         *
         *  @param set Is this semaphore "full" or not?
         *  @throws SemaphoreCreateException on failure.
         *  @return Instance of a BinarySemaphore.
         */
        explicit BinarySemaphore(bool set = false);
};


/**
 *  Wrapper class for Counting Semaphores.
 */
class CountingSemaphore : public Semaphore {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
        /**
         *  Constructor to create a counting semaphore.
         *  This ctor throws a SemaphoreCreateException on failure.
         *
         *  @param maxCount Must be greater than 0.
         *  @param initialCount Must not be greater than maxCount.
         *  @throws SemaphoreCreateException on failure.
         *  @return Instance of a CountingSemaphore.
         */
        CountingSemaphore(UBaseType_t maxCount, UBaseType_t initialCount);
};


}
