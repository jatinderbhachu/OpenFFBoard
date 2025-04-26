#pragma once
#include <zephyr/kernel.h>

#include "common.hpp"

namespace cpp_freertos {
class Mutex {
public:
    Mutex();
    virtual ~Mutex();

    virtual bool Lock();
    virtual bool Unlock();

protected:
    struct k_mutex handle;
};

class LockGuard {
public:
    LockGuard(Mutex &m);
    ~LockGuard();

private:
    Mutex &mutex;
};

/**
 *  Standard usage Mutex.
 *  By default calls to Lock these objects block forever, but this can be
 *  changed by simply passing in a argument to the Lock() method.
 *  These objects are not recursively acquirable. Calling Lock() twice from
 *  the same Thread (i.e. task) will deadlock.
 *
 *  @note Standard mutexes use less resources than recursive mutexes. You
 *        should typically use this type of Mutex, unless you have a strong
 *        need for a MutexRecursive mutex.
 */
class MutexStandard : public Mutex {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
        /**
         *  Create a standard, non-recursize Mutex.
         *
         *  @throws ThreadMutexException on failure.
         */
        MutexStandard();

        /**
         *  Lock the Mutex.
         *
         *  @param Timeout How long to wait to get the Lock until giving up.
         *  @return true if the Lock was acquired, false if it timed out.
         */
        virtual bool Lock(TickType_t Timeout);

        /**
         *  Unlock the Mutex.
         *
         *  @return true if the Lock was released, false if it failed. (Hint,
         *           if it fails, did you call Lock() first?)
         */
        virtual bool Unlock();
};


}
