// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#include <vector>
#include <array>
#include <list>
#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <assert.h>


// permanently don't want to see those:
#pragma warning( disable : 26435 )  // Function 'func' should specify exactly one of 'virtual', 'override', or 'final' (c.128).
#pragma warning( disable : 26409 )  // Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
#pragma warning( disable : 26426 )  // Global initializer calls a non - constexpr function '~Class' (i.22).
#pragma warning( disable : 26429 )  // Symbol '' is never tested for nullness, it can be marked as not_null(f.23).
#pragma warning( disable : 26443 )  // Overriding destructor should not use explicit 'override' or 'virtual' specifiers(c.128).
#pragma warning( disable : 26446 )  // Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
#pragma warning( disable : 26472 )  // Don't use a static_cast for arithmetic conversions. Use brace initialization, gsl::narrow_cast or gsl::narow (type.1).
#pragma warning( disable : 26482 )  // Only index into arrays using constant expressions(bounds.2).
#pragma warning( disable : 26486 )  // Don't pass a pointer that may be invalid to a function. Parameter 2 'p.{vfptr}' in call to 'icall' may be invalid (lifetime.1).
#pragma warning( disable : 26489 )  // Don't dereference a pointer that may be invalid: 'p'. 'p' may have been invalidated at line # (lifetime.1).

// temporarily hide those:
#pragma warning( disable : 26432 )  // If you define or delete any default operation in the type 'class T', define or delete them all (c.21).
#pragma warning( disable : 26436 )  // The type 'mod_const(class T)' with a virtual function needs either public virtual or protected non - virtual destructor(c.35).
#pragma warning( disable : 26440 )  // Function 'f' can be declared 'noexcept' (f.6).
#pragma warning( disable : 26447 )  // The function is declared 'noexcept' but calls function 'f' which may throw exceptions(f.6).

#include "BGResource.h"

#include "Core\BGCoreLocation.h"
#include "Core\BGCore.h"
#include "Core\BGCoreLayout.h"
#include "Core\BGChessPieces.h"
#include "Util\Util.h"

#include "ChildFrm.h"
#include "MainFrm.h"

#include "BG.h"
#include "BGDoc.h"
#include "BGView.h"