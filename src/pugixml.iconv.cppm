module;
#include <cerrno>
#include <cstring>
#include <iconv.h>
export module pugixml.iconv;

export using ::iconv_t;
export using ::iconv_open;
export using ::iconv;
export using ::iconv_close;

export inline char *iconv_last_error_text()
{
  return strerror(errno);
}