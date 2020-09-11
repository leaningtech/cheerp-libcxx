//===------------------------ iostream.cpp --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "__std_stream"
#include "string"
#include "new"

#define _str(s) #s
#define str(s) _str(s)
#define _LIBCPP_ABI_NAMESPACE_STR str(_LIBCPP_ABI_NAMESPACE)

_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _LIBCPP_HAS_NO_STDIN
_ALIGNAS_TYPE (istream) _LIBCPP_FUNC_VIS istream cin [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?cin@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_istream@DU?$char_traits@D@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (__stdinbuf<char> ) static __stdinbuf<char> __cin [[cheerp::noinit]];
static mbstate_t mb_cin;
_ALIGNAS_TYPE (wistream) _LIBCPP_FUNC_VIS wistream wcin [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?wcin@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_istream@_WU?$char_traits@_W@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (__stdinbuf<wchar_t> ) static __stdinbuf<wchar_t> __wcin [[cheerp::noinit]];
static mbstate_t mb_wcin;
#endif

#ifndef _LIBCPP_HAS_NO_STDOUT
_ALIGNAS_TYPE (ostream) _LIBCPP_FUNC_VIS ostream cout [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?cout@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_ostream@DU?$char_traits@D@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (__stdoutbuf<char>) static __stdoutbuf<char> __cout [[cheerp::noinit]];
static mbstate_t mb_cout;
_ALIGNAS_TYPE (wostream) _LIBCPP_FUNC_VIS wostream wcout [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?wcout@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_ostream@_WU?$char_traits@_W@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (__stdoutbuf<wchar_t>) static __stdoutbuf<wchar_t> __wcout [[cheerp::noinit]];
static mbstate_t mb_wcout;
#endif

_ALIGNAS_TYPE (ostream) _LIBCPP_FUNC_VIS ostream cerr [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?cerr@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_ostream@DU?$char_traits@D@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (__stdoutbuf<char>) static __stdoutbuf<char> __cerr [[cheerp::noinit]];
static mbstate_t mb_cerr;
_ALIGNAS_TYPE (wostream) _LIBCPP_FUNC_VIS wostream wcerr [[cheerp::noinit]];
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?wcerr@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_ostream@_WU?$char_traits@_W@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (__stdoutbuf<wchar_t>) static __stdoutbuf<wchar_t> __wcerr [[cheerp::noinit]];
static mbstate_t mb_wcerr;

_ALIGNAS_TYPE (ostream) _LIBCPP_FUNC_VIS ostream clog [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?clog@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_ostream@DU?$char_traits@D@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;
_ALIGNAS_TYPE (wostream) _LIBCPP_FUNC_VIS wostream wclog [[cheerp::noinit]]
#if defined(_LIBCPP_ABI_MICROSOFT) && defined(__clang__)
__asm__("?wclog@" _LIBCPP_ABI_NAMESPACE_STR "@std@@3V?$basic_ostream@_WU?$char_traits@_W@" _LIBCPP_ABI_NAMESPACE_STR "@std@@@12@A")
#endif
;

_LIBCPP_HIDDEN ios_base::Init __start_std_streams;

ios_base::Init::Init()
{
#ifndef _LIBCPP_HAS_NO_STDIN
    istream* cin_ptr  = ::new(&cin)  istream(::new(&__cin)  __stdinbuf <char>(stdin, &mb_cin));
    wistream* wcin_ptr  = ::new(&wcin)  wistream(::new(&__wcin)  __stdinbuf <wchar_t>(stdin, &mb_wcin));
#endif
#ifndef _LIBCPP_HAS_NO_STDOUT
    ostream* cout_ptr = ::new(&cout) ostream(::new(&__cout) __stdoutbuf<char>(stdout, &mb_cout));
    wostream* wcout_ptr = ::new(&wcout) wostream(::new(&__wcout) __stdoutbuf<wchar_t>(stdout, &mb_wcout));
#endif
    ostream* cerr_ptr = ::new(&cerr) ostream(::new(&__cerr) __stdoutbuf<char>(stderr, &mb_cerr));
                        ::new(&clog) ostream(cerr_ptr->rdbuf());
    wostream* wcerr_ptr = ::new(&wcerr) wostream(::new(&__wcerr) __stdoutbuf<wchar_t>(stderr, &mb_wcerr));
                          ::new(&wclog) wostream(wcerr_ptr->rdbuf());

#if !defined(_LIBCPP_HAS_NO_STDIN) && !defined(_LIBCPP_HAS_NO_STDOUT)
    cin_ptr->tie(cout_ptr);
    wcin_ptr->tie(wcout_ptr);
#endif
    _VSTD::unitbuf(*cerr_ptr);
    _VSTD::unitbuf(*wcerr_ptr);
#ifndef _LIBCPP_HAS_NO_STDOUT
    cerr_ptr->tie(cout_ptr);
    wcerr_ptr->tie(wcout_ptr);
#endif
}

ios_base::Init::~Init()
{
#ifndef _LIBCPP_HAS_NO_STDOUT
    ostream* cout_ptr = reinterpret_cast<ostream*>(&cout);
    wostream* wcout_ptr = reinterpret_cast<wostream*>(&wcout);
    cout_ptr->flush();
    wcout_ptr->flush();
#endif

    ostream* clog_ptr = reinterpret_cast<ostream*>(&clog);
    wostream* wclog_ptr = reinterpret_cast<wostream*>(&wclog);
    clog_ptr->flush();
    wclog_ptr->flush();
}

_LIBCPP_END_NAMESPACE_STD
