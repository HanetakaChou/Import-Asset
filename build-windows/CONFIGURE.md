
# CONFIGURE  

libiconv-1.18  

install MSYS2  

```cmd  

REM export PATH="/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.42.34433/bin/Hostx86/x86:$PATH" 

export PATH="/c/Program Files/LLVM/bin:$PATH"

export PATH="/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.42.34433/bin/Hostx64/x64:$PATH"  

cd <Your libiconv repository root directory>
export CC=clang-cl  
export LD=link  
./configure --enable-extra-encodings=no --enable-year2038=yes --disable-nls --enable-shared=yes  
```  

create visual studio project (and use clang-cl)   
add "lib/iconv.c" "libcharset/lib/localcharset.c" "lib/compat.c" to your project  

>> lib/config.h  
set "ENABLE_EXTRA" to "0"  
>  
>> libcharset/config.h   
set "HAVE_SETLOCALE" to "0"  
>  
>> fix -I  
-I srclib  
iconv.c: -I include  
compat.c: -I include  
localcharset.c: -I libcharset  
>  
>> fix -D  
-D_WIN32_WINNT=_WIN32_WINNT_WIN7  
-D_CRT_SECURE_NO_WARNINGS  
-DHAVE_CONFIG_H  
iconv.c: -DBUILDING_LIBICONV 
compat.c: -DBUILDING_LIBICONV  
localcharset.c: -DBUILDING_LIBCHARSET  
>  
>> lib/canonical.h  
lib/canonical_dos.h  
lib/canonical_local.h  
change "(int)(long)" to "offsetof" (of "struct stringpool_t" / "struct stringpool2_t")  
>  
>> lib/iconv.c  
change "(int)(long)" within "S" to "offsetof" (of "struct stringpool2_t")  
>  

```def  
EXPORTS
	libiconv_open
	libiconv
	libiconv_close
	libiconv_open_into
	libiconvctl
	libiconvlist
	iconv_canonicalize
	libiconv_set_relocation_prefix
```  

```cpp  
#define WIN32_LEAN_AND_MEAN 1
#include <sdkddkver.h>
#include <Windows.h>
#include <assert.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
    }
    break;
    case DLL_PROCESS_DETACH:
    {
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
    {
        // we have disabled the "Thread Library Calls"
        // assert(false);
    }
    }
    return TRUE;
}
```
