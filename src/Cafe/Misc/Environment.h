#pragma once

#include <Cafe/Encoding/CodePage/UTF-8.h>
#ifdef _WIN32
#	include <Cafe/Encoding/CodePage/UTF-16.h>
#endif
#include <Cafe/Encoding/Strings.h>
#include <Cafe/Misc/Export.h>

#include <optional>
#include <unordered_map>

namespace Cafe::Environment
{
#ifdef _WIN32
	CAFE_PUBLIC Encoding::CodePage::CodePageType GetNarrowEncoding() noexcept;
	constexpr Encoding::CodePage::CodePageType GetWideEncoding() noexcept
	{
		return Encoding::CodePage::Utf16LittleEndian;
	}
#else
	constexpr Encoding::CodePage::CodePageType GetNarrowEncoding() noexcept
	{
		return Encoding::CodePage::Utf8;
	}
	constexpr Encoding::CodePage::CodePageType GetWideEncoding() noexcept
	{
		return Encoding::CodePage::CodePoint;
	}
#endif

	constexpr Encoding::StringView<Encoding::CodePage::Utf8> GetNewLine() noexcept
	{
#ifdef _WIN32
		return CAFE_UTF8_SV("\r\n");
#else
		return CAFE_UTF8_SV("\n");
#endif
	}
} // namespace Cafe::Environment
