#include <Cafe/Misc/Environment.h>
#include <Cafe/Misc/Scope.h>

#ifdef _WIN32
#	include <Windows.h>
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
