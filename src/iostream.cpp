//===------------------------ iostream.cpp --------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "__std_stream"
#include "string"
#include "new"

_LIBCPP_BEGIN_NAMESPACE_STD

#ifndef _LIBCPP_HAS_NO_STDIN
_LIBCPP_FUNC_VIS istream cin [[cheerp::noinit]];
static __stdinbuf<char> __cin [[cheerp::noinit]];
static mbstate_t mb_cin;
_LIBCPP_FUNC_VIS wistream wcin  [[cheerp::noinit]];
static __stdinbuf<wchar_t>  __wcin [[cheerp::noinit]];
static mbstate_t mb_wcin;
#endif

#ifndef _LIBCPP_HAS_NO_STDOUT
_LIBCPP_FUNC_VIS ostream cout [[cheerp::noinit]];
static __stdoutbuf<char> __cout [[cheerp::noinit]];
static mbstate_t mb_cout;
_LIBCPP_FUNC_VIS wostream wcout [[cheerp::noinit]];
static __stdoutbuf<wchar_t> __wcout [[cheerp::noinit]];
static mbstate_t mb_wcout;
#endif

_LIBCPP_FUNC_VIS ostream cerr [[cheerp::noinit]];
static __stdoutbuf<char> __cerr [[cheerp::noinit]];
static mbstate_t mb_cerr;
_LIBCPP_FUNC_VIS wostream wcerr [[cheerp::noinit]];
static __stdoutbuf<wchar_t> __wcerr [[cheerp::noinit]];
static mbstate_t mb_wcerr;

_LIBCPP_FUNC_VIS ostream clog [[cheerp::noinit]];
_LIBCPP_FUNC_VIS wostream wclog [[cheerp::noinit]];

ios_base::Init __start_std_streams;

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
