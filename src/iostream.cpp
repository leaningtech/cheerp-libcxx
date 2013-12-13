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

static mbstate_t state_types[6] = {};

static __stdinbuf<char> __cin [[noinit]];
static __stdoutbuf<char> __cout [[noinit]];
static __stdoutbuf<char> __cerr [[noinit]];
static __stdinbuf<wchar_t>  __wcin [[noinit]];
static __stdoutbuf<wchar_t> __wcout [[noinit]];
static __stdoutbuf<wchar_t> __wcerr [[noinit]];

istream cin [[noinit]];
ostream cout [[noinit]];
ostream cerr [[noinit]];
ostream clog [[noinit]];
wistream wcin  [[noinit]];
wostream wcout [[noinit]];
wostream wcerr [[noinit]];
wostream wclog [[noinit]];

ios_base::Init __start_std_streams;

ios_base::Init::Init()
{
    istream* cin_ptr  = ::new(&cin)  istream(::new(&__cin)  __stdinbuf <char>(stdin, state_types+0) );
    ostream* cout_ptr = ::new(&cout) ostream(::new(&__cout) __stdoutbuf<char>(stdout, state_types+1));
    ostream* cerr_ptr = ::new(&cerr) ostream(::new(&__cerr) __stdoutbuf<char>(stderr, state_types+2));
                        ::new(&clog) ostream(cerr_ptr->rdbuf());
    cin_ptr->tie(cout_ptr);
    _VSTD::unitbuf(*cerr_ptr);
    cerr_ptr->tie(cout_ptr);

    wistream* wcin_ptr  = ::new(&wcin)  wistream(::new(&__wcin)  __stdinbuf <wchar_t>(stdin, state_types+3) );
    wostream* wcout_ptr = ::new(&wcout) wostream(::new(&__wcout) __stdoutbuf<wchar_t>(stdout, state_types+4));
    wostream* wcerr_ptr = ::new(&wcerr) wostream(::new(&__wcerr) __stdoutbuf<wchar_t>(stderr, state_types+5));
                          ::new(&wclog) wostream(wcerr_ptr->rdbuf());
    wcin_ptr->tie(wcout_ptr);
    _VSTD::unitbuf(*wcerr_ptr);
    wcerr_ptr->tie(wcout_ptr);
}

ios_base::Init::~Init()
{
    ostream* cout_ptr = reinterpret_cast<ostream*>(&cout);
    ostream* clog_ptr = reinterpret_cast<ostream*>(&clog);
    cout_ptr->flush();
    clog_ptr->flush();

    wostream* wcout_ptr = reinterpret_cast<wostream*>(&wcout);
    wostream* wclog_ptr = reinterpret_cast<wostream*>(&wclog);
    wcout_ptr->flush();
    wclog_ptr->flush();
}

_LIBCPP_END_NAMESPACE_STD
