#include <Cafe/ErrorHandling/CommonExceptions.h>
#include <Cafe/Misc/Environment.h>
#include <Cafe/Misc/Scope.h>
#include <Cafe/TextUtils/Format.h>
#include <Cafe/TextUtils/Misc.h>

#ifdef _WIN32
#	include <Winnls.h>
#	include <windef.h>
#	include <winbase.h>
#else
#	include <cstdlib>
#endif

using namespace Cafe;
using namespace Environment;

#ifdef _WIN32
Encoding::CodePage::CodePageType Environment::GetNarrowEncoding() noexcept
{
	return static_cast<Encoding::CodePage::CodePageType>(GetACP());
}
#endif

EnvironmentVariableManager& EnvironmentVariableManager::GetInstance()
{
	static EnvironmentVariableManager s_Instance;
	return s_Instance;
}

std::optional<Encoding::StringView<Encoding::CodePage::Utf8>> EnvironmentVariableManager::GetValue(
    Encoding::StringView<Encoding::CodePage::Utf8> const& key) const
{
	if (const auto iter = m_Cache.find(key); iter != m_Cache.end())
	{
		return iter->second;
	}

#ifdef _WIN32
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getenvironmentvariable
	constexpr std::size_t BufferSize = 32767;
	wchar_t buffer[BufferSize];
	const auto nativeKey = TextUtils::EncodeToWide(key);
	const auto writtenSize = GetEnvironmentVariableW(nativeKey.data(), buffer, BufferSize);
	if (!writtenSize)
	{
		const auto lastError = GetLastError();
		if (lastError == ERROR_ENVVAR_NOT_FOUND)
		{
			m_Cache.emplace(key, std::nullopt);
			return {};
		}

		CAFE_THROW(
		    ErrorHandling::SystemException, lastError,
		    TextUtils::FormatString(CAFE_UTF8_SV("Cannot fetch environment variable \"${0}\"."), key));
	}
	const auto [iter, succeed] = m_Cache.emplace(
	    key, TextUtils::EncodeFromWide<Encoding::CodePage::Utf8>({ buffer, writtenSize }));
	assert(succeed);
	return iter->second;
#else
	Encoding::String<Encoding::CodePage::Utf8> mayBeKeyStorage;
	const auto keyValue = reinterpret_cast<const char*>(
	    key.IsNullTerminated() ? key.GetData() : (mayBeKeyStorage = key).GetData());
	const auto value = std::getenv(keyValue);
	if (!value)
	{
		m_Cache.emplace(key, std::nullopt);
		return {};
	}

	const auto [iter, succeed] = m_Cache.emplace(
	    key, TextUtils::AsNullTerminatedStringView<Encoding::CodePage::Utf8>(
	             reinterpret_cast<
	                 const Encoding::CodePage::CodePageTrait<Encoding::CodePage::Utf8>::CharType*>(
	                 value)));
	assert(succeed);
	return iter->second;
#endif
}

void EnvironmentVariableManager::SetValue(
    Encoding::StringView<Encoding::CodePage::Utf8> const& key,
    Encoding::StringView<Encoding::CodePage::Utf8> const& value)
{
	const auto rollbackIter = [&] {
		const auto iter = m_Cache.find(key);
		if (iter != m_Cache.end())
		{
			iter->second = value;
			return iter;
		}

		const auto ib = m_Cache.emplace(key, value);
		assert(ib.second);
		return ib.first;
	}();

	CAFE_SCOPE_FAIL
	{
		m_Cache.erase(rollbackIter);
	};

#ifdef _WIN32
	const auto nativeKey = TextUtils::EncodeToWide(key);
	const auto nativeValue = TextUtils::EncodeToWide(value);

	if (!SetEnvironmentVariableW(nativeKey.data(), nativeValue.data()))
	{
		CAFE_THROW(
		    ErrorHandling::SystemException,
		    TextUtils::FormatString(CAFE_UTF8_SV("Cannot set environment variable \"${0}\"."), key));
	}
#else
	Encoding::String<Encoding::CodePage::Utf8> mayBeKeyStorage;
	Encoding::String<Encoding::CodePage::Utf8> mayBeValueStorage;
	const auto keyValue = reinterpret_cast<const char*>(
	    key.IsNullTerminated() ? key.GetData() : (mayBeKeyStorage = key).GetData());
	const auto valueValue = reinterpret_cast<const char*>(
	    value.IsNullTerminated() ? value.GetData() : (mayBeValueStorage = value).GetData());

	if (setenv(keyValue, valueValue, 1) < 0)
	{
		CAFE_THROW(
		    ErrorHandling::SystemException,
		    TextUtils::FormatString(CAFE_UTF8_SV("Cannot set environment variable \"${0}\"."), key));
	}
#endif
}

void EnvironmentVariableManager::Remove(Encoding::StringView<Encoding::CodePage::Utf8> const& key)
{
	m_Cache.erase(key);

#ifdef _WIN32
	const auto nativeKey = TextUtils::EncodeToWide(key);

	if (!SetEnvironmentVariableW(nativeKey.data(), nullptr))
	{
		CAFE_THROW(
		    ErrorHandling::SystemException,
		    TextUtils::FormatString(CAFE_UTF8_SV("Cannot remove environment variable \"${0}\"."), key));
	}
#else
	Encoding::String<Encoding::CodePage::Utf8> mayBeKeyStorage;
	const auto keyValue = reinterpret_cast<const char*>(
	    key.IsNullTerminated() ? key.GetData() : (mayBeKeyStorage = key).GetData());

	if (unsetenv(keyValue) < 0)
	{
		CAFE_THROW(
		    ErrorHandling::SystemException,
		    TextUtils::FormatString(CAFE_UTF8_SV("Cannot remove environment variable \"${0}\"."), key));
	}
#endif
}

void EnvironmentVariableManager::FlushCache(
    Encoding::StringView<Encoding::CodePage::Utf8> const& key) const
{
	if (key.IsEmpty())
	{
		m_Cache.clear();
	}
	else
	{
		m_Cache.erase(key);
	}
}
