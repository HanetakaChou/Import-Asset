//===-- ConvertUTFWrapper.cpp - Wrap ConvertUTF.h with clang data types -----===
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../include/ConvertUTF.h"
#include <string>
#include <vector>
#include <assert.h>

namespace llvm
{
  static_assert(sizeof(wchar_t) == 2, "Expected wchar_t to be 2 bytes");

  bool convertUTF16ToUTF8String(std::wstring const &SrcUTF16, std::string &DstUTF8)
  {
    assert(DstUTF8.empty());

    // Avoid OOB by returning early on empty input.
    if (SrcUTF16.empty())
      return true;

    const UTF16 *Src = reinterpret_cast<const UTF16 *>(SrcUTF16.data());
    const UTF16 *SrcEnd = reinterpret_cast<const UTF16 *>(SrcUTF16.data() + SrcUTF16.size());

    assert((uintptr_t)Src % sizeof(UTF16) == 0);

    // TODO: Byteswap if necessary.
    assert(Src[0] != UNI_UTF16_BYTE_ORDER_MARK_SWAPPED);

    // Skip the BOM for conversion.
    if (Src[0] == UNI_UTF16_BYTE_ORDER_MARK_NATIVE)
    {
      Src++;
    }

    // Just allocate enough space up front.  We'll shrink it later.  Allocate
    // enough that we can fit a null terminator without reallocating.
    DstUTF8.resize((sizeof(UTF16) * SrcUTF16.size()) * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
    UTF8 *Dst = reinterpret_cast<UTF8 *>(&DstUTF8[0]);
    UTF8 *DstEnd = Dst + DstUTF8.size();

    ConversionResult CR = ConvertUTF16toUTF8(&Src, SrcEnd, &Dst, DstEnd, strictConversion);
    assert(CR != targetExhausted);

    if (CR != conversionOK)
    {
      DstUTF8.clear();
      return false;
    }

    DstUTF8.resize(reinterpret_cast<char *>(Dst) - &DstUTF8[0]);
    DstUTF8.push_back(0);
    DstUTF8.pop_back();
    return true;
  }

  bool convertUTF8ToUTF16String(std::string const &SrcUTF8, std::wstring &DstUTF16)
  {
    assert(DstUTF16.empty());

    // Avoid OOB by returning early on empty input.
    if (SrcUTF8.empty())
    {
      DstUTF16.push_back(0);
      DstUTF16.pop_back();
      return true;
    }

    const UTF8 *Src = reinterpret_cast<const UTF8 *>(SrcUTF8.data());
    const UTF8 *SrcEnd = reinterpret_cast<const UTF8 *>(SrcUTF8.data() + SrcUTF8.size());

    // Allocate the same number of UTF-16 code units as UTF-8 code units. Encoding
    // as UTF-16 should always require the same amount or less code units than the
    // UTF-8 encoding.  Allocate one extra byte for the null terminator though,
    // so that someone calling DstUTF16.data() gets a null terminated string.
    // We resize down later so we don't have to worry that this over allocates.
    DstUTF16.resize(SrcUTF8.size() + 1);
    UTF16 *Dst = reinterpret_cast<UTF16 *>(&DstUTF16[0]);
    UTF16 *DstEnd = Dst + DstUTF16.size();

    ConversionResult CR = ConvertUTF8toUTF16(&Src, SrcEnd, &Dst, DstEnd, strictConversion);
    assert(CR != targetExhausted);

    if (CR != conversionOK)
    {
      DstUTF16.clear();
      return false;
    }

    DstUTF16.resize(Dst - reinterpret_cast<UTF16 *>(&DstUTF16[0]));
    DstUTF16.push_back(0);
    DstUTF16.pop_back();
    return true;
  }

} // end namespace llvm
