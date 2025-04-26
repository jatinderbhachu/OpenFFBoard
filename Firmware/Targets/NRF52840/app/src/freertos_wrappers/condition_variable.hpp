#pragma once

#include <list>
#include "mutex.hpp"

namespace cpp_freertos {

//
//  Forward declaration. We need to prevent a circular dependency
//  between the Thread class and the ConditionVariable class.
//
class Thread;


/**
 *  Class implementation of condition variable based on
 *  FreeRTOS C++ Wrapper classes.
 *  
 *  A condition variable isn't really a variable. It's a list
 *  of threads.
 *
 *  The design here is that a Thread "waits", and a ConditionVariable
 *  "signals". This affects where the public interfaces reside.
 */
class ConditionVariable {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    
        /**
         *  Constructor to create a condition variable.
         */
        ConditionVariable();

        /**
         *  Signal a thread waiting on this ConditionVariable.
         *  Signaling is implemented as FIFO.
         */
        void Signal();

        /**
         *  Signal all threads waiting on this ConditionVariable.
         */
        void Broadcast();


    /////////////////////////////////////////////////////////////////////////
    //
    //  Private API
    //  The internals of this wrapper class.
    //
    /////////////////////////////////////////////////////////////////////////
    private:

        /**
         *  Protect the internal ConditionVariable state.
         */
        MutexStandard Lock;

        /**
         *  Implementation of a wait list of Threads.
         */
        std::list<Thread *> WaitList;

        /**
         *  Internal helper function to queue a Thread to 
         *  this ConditionVariable's wait list.
         */
        void AddToWaitList(Thread *thread);

    /**
     *  The Thread class and the ConditionVariable class are interdependent.
     *  If we allow the Thread class to access the internals of the
     *  ConditionVariable, we can reduce the public interface which is a
     *  good thing.
     */
    friend class Thread;
};


}
