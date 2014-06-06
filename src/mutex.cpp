//===------------------------- mutex.cpp ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#define _LIBCPP_BUILDING_MUTEX
#include "mutex"
#include "limits"
#include "system_error"
#include "cassert"

_LIBCPP_BEGIN_NAMESPACE_STD
#ifndef _LIBCPP_HAS_NO_THREADS

const defer_lock_t  defer_lock = {};
const try_to_lock_t try_to_lock = {};
const adopt_lock_t  adopt_lock = {};

mutex::~mutex()
{
#ifndef __CHEERP__
    pthread_mutex_destroy(&__m_);
#endif
}

void
mutex::lock()
{
#ifdef __CHEERP__
    if(__m_)
      client::console.log("Cheerp: mutex::lock can't block");
    else
      __m_++;
#else
    int ec = pthread_mutex_lock(&__m_);
    if (ec)
        __throw_system_error(ec, "mutex lock failed");
#endif
}

bool
mutex::try_lock() _NOEXCEPT
{
#ifdef __CHEERP__
    if(__m_)
      return false;
    else
    {
      __m_++;
      return true;
    }
#else
    return pthread_mutex_trylock(&__m_) == 0;
#endif
}

void
mutex::unlock() _NOEXCEPT
{
#ifdef __CHEERP__
    __m_--;
#else
    int ec = pthread_mutex_unlock(&__m_);
    (void)ec;
    assert(ec == 0);
#endif
}

// recursive_mutex

recursive_mutex::recursive_mutex()
{
#ifdef __CHEERP__
    __m_ = 0;
#else
    pthread_mutexattr_t attr;
    int ec = pthread_mutexattr_init(&attr);
    if (ec)
        goto fail;
    ec = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (ec)
    {
        pthread_mutexattr_destroy(&attr);
        goto fail;
    }
    ec = pthread_mutex_init(&__m_, &attr);
    if (ec)
    {
        pthread_mutexattr_destroy(&attr);
        goto fail;
    }
    ec = pthread_mutexattr_destroy(&attr);
    if (ec)
    {
        pthread_mutex_destroy(&__m_);
        goto fail;
    }
    return;
fail:
    __throw_system_error(ec, "recursive_mutex constructor failed");
#endif
}

recursive_mutex::~recursive_mutex()
{
#ifndef __CHEERP__
    int e = pthread_mutex_destroy(&__m_);
    (void)e;
    assert(e == 0);
#endif
}

void
recursive_mutex::lock()
{
#ifdef __CHEERP__
    __m_++;
#else
    int ec = pthread_mutex_lock(&__m_);
    if (ec)
        __throw_system_error(ec, "recursive_mutex lock failed");
#endif
}

void
recursive_mutex::unlock() _NOEXCEPT
{
#ifdef __CHEERP__
    __m_--;
#else
    int e = pthread_mutex_unlock(&__m_);
    (void)e;
    assert(e == 0);
#endif
}

bool
recursive_mutex::try_lock() _NOEXCEPT
{
#ifdef __CHEERP__
    __m_++;
    return true;
#else
    return pthread_mutex_trylock(&__m_) == 0;
#endif
}

// timed_mutex

timed_mutex::timed_mutex()
    : __locked_(false)
{
}

timed_mutex::~timed_mutex()
{
    lock_guard<mutex> _(__m_);
}

void
timed_mutex::lock()
{
    unique_lock<mutex> lk(__m_);
#ifdef __CHEERP__
    if (__locked_)
        client::console.log("Cheerp: timed_mutex::lock can't block");
#else
    while (__locked_)
        __cv_.wait(lk);
#endif
    __locked_ = true;
}

bool
timed_mutex::try_lock() _NOEXCEPT
{
    unique_lock<mutex> lk(__m_, try_to_lock);
    if (lk.owns_lock() && !__locked_)
    {
        __locked_ = true;
        return true;
    }
    return false;
}

void
timed_mutex::unlock() _NOEXCEPT
{
    lock_guard<mutex> _(__m_);
    __locked_ = false;
#ifndef __CHEERP__
    __cv_.notify_one();
#endif
}

// recursive_timed_mutex

recursive_timed_mutex::recursive_timed_mutex()
    : __count_(0)
#ifndef __CHEERP__
      ,__id_(0)
#endif
{
}

recursive_timed_mutex::~recursive_timed_mutex()
{
    lock_guard<mutex> _(__m_);
}

void
recursive_timed_mutex::lock()
{
#ifndef __CHEERP__
    pthread_t id = pthread_self();
#endif
    unique_lock<mutex> lk(__m_);
#ifndef __CHEERP__
    if (pthread_equal(id, __id_))
#endif
    {
        if (__count_ == numeric_limits<size_t>::max())
            __throw_system_error(EAGAIN, "recursive_timed_mutex lock limit reached");
        ++__count_;
        return;
    }
#ifndef __CHEERP__
    while (__count_ != 0)
        __cv_.wait(lk);
    __count_ = 1;
    __id_ = id;
#endif
}

bool
recursive_timed_mutex::try_lock() _NOEXCEPT
{
#ifndef __CHEERP__
    pthread_t id = pthread_self();
#endif
    unique_lock<mutex> lk(__m_, try_to_lock);
#ifdef __CHEERP__
    return true;
#else
    if (lk.owns_lock() && (__count_ == 0 || pthread_equal(id, __id_)))
    {
        if (__count_ == numeric_limits<size_t>::max())
            return false;
        ++__count_;
        __id_ = id;
        return true;
    }
    return false;
#endif
}

void
recursive_timed_mutex::unlock() _NOEXCEPT
{
    unique_lock<mutex> lk(__m_);
    if (--__count_ == 0)
    {
#ifndef __CHEERP__
        __id_ = 0;
#endif
        lk.unlock();
#ifndef __CHEERP__
        __cv_.notify_one();
#endif
    }
}

#endif // !_LIBCPP_HAS_NO_THREADS

// If dispatch_once_f ever handles C++ exceptions, and if one can get to it
// without illegal macros (unexpected macros not beginning with _UpperCase or
// __lowercase), and if it stops spinning waiting threads, then call_once should
// call into dispatch_once_f instead of here. Relevant radar this code needs to
// keep in sync with:  7741191.

#ifndef _LIBCPP_HAS_NO_THREADS
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cv  = PTHREAD_COND_INITIALIZER;
#endif

void
__call_once(volatile unsigned long& flag, void* arg, void(*func)(void*))
{
#if defined(_LIBCPP_HAS_NO_THREADS)
    if (flag == 0)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            flag = 1;
            func(arg);
            flag = ~0ul;
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            flag = 0ul;
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
#else // !_LIBCPP_HAS_NO_THREADS
    pthread_mutex_lock(&mut);
    while (flag == 1)
        pthread_cond_wait(&cv, &mut);
    if (flag == 0)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif  // _LIBCPP_NO_EXCEPTIONS
            flag = 1;
            pthread_mutex_unlock(&mut);
            func(arg);
            pthread_mutex_lock(&mut);
            flag = ~0ul;
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cv);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            pthread_mutex_lock(&mut);
            flag = 0ul;
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&cv);
            throw;
        }
#endif  // _LIBCPP_NO_EXCEPTIONS
    }
    else
        pthread_mutex_unlock(&mut);
#endif // !_LIBCPP_HAS_NO_THREADS

}
_LIBCPP_END_NAMESPACE_STD
