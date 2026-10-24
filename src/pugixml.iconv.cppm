module;
#include <iconv.h>
export module pugixml.iconv;

export using ::iconv_t;
export using ::iconv_open;
export using ::iconv;
export using ::iconv_close;