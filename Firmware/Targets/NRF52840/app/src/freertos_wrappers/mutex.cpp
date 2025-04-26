#include "mutex.hpp"

namespace cpp_freertos {

    Mutex::Mutex() {
        k_mutex_init(&handle);
    }
    
    Mutex::~Mutex() {
    }
    
    bool Mutex::Lock() {
        int ret = k_mutex_lock(&handle, K_FOREVER);
        return (ret == 0);
    }

    bool Mutex::Unlock() {
        int ret = k_mutex_unlock(&handle);
        return (ret == 0);
    }
    
    LockGuard::LockGuard(Mutex &m)
        : mutex(m)
    {
        mutex.Lock();
    }
    
    LockGuard::~LockGuard() {
        mutex.Unlock();
    }
    
    MutexStandard::MutexStandard()
    {
    }

    bool MutexStandard::Lock(TickType_t Timeout)
    {
        return Mutex::Lock();
    }

    bool MutexStandard::Unlock()
    {
        return Mutex::Unlock();
    }
} // namespace cpp_freertos