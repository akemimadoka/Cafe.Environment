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
