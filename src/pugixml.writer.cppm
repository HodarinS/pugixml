module;
// If no API is defined, assume default
#ifndef PUGIXML_API
#define PUGIXML_API
#endif

// If no API for classes is defined, assume default
#ifndef PUGIXML_CLASS
#define PUGIXML_CLASS PUGIXML_API
#endif

// If no API for functions is defined, assume default
#ifndef PUGIXML_FUNCTION
#define PUGIXML_FUNCTION PUGIXML_API
#endif
#include <cassert>
export module pugixml.wrtier;
import std.compat;

namespace pugi
{
// Writer interface for node printing (see XmlNode::print)
export class PUGIXML_CLASS XmlWriter
{
public:
  virtual ~XmlWriter();

  // Write memory chunk into stream/file/whatever
  virtual void write(const void *data, size_t size) = 0;
};

// XmlWriter implementation for FILE*
export class PUGIXML_CLASS XmlWriterFile : public XmlWriter
{
public:
  // Construct writer from a FILE* object; void* is used to avoid header
  // dependencies on stdio
  XmlWriterFile(void *file);

  virtual void write(const void *data, size_t size) override;

private:
  void *file;
};

// XmlWriter implementation for streams
export class PUGIXML_CLASS XmlWriterStream : public XmlWriter
{
public:
  // Construct writer from an output stream object
  XmlWriterStream(std::basic_ostream< char > &stream);
  XmlWriterStream(std::basic_ostream< wchar_t > &stream);

  virtual void write(const void *data, size_t size) override;

private:
  std::basic_ostream< char > *narrow_stream;
  std::basic_ostream< wchar_t > *wide_stream;
};
} // namespace pugi


pugi::XmlWriter::~XmlWriter()
{
}

pugi::XmlWriterFile::XmlWriterFile(void *file_)
    : file(file_)
{
}

void pugi::XmlWriterFile::write(const void *data, size_t size)
{
  size_t result = fwrite(data, 1, size, static_cast< FILE * >(file));
  (void)!result; // unfortunately we can't do proper error handling here
}


pugi::XmlWriterStream::XmlWriterStream(std::basic_ostream< char > &stream)
    : narrow_stream(&stream),
      wide_stream(nullptr)
{
}

pugi::XmlWriterStream::XmlWriterStream(std::basic_ostream< wchar_t > &stream)
    : narrow_stream(nullptr),
      wide_stream(&stream)
{
}

void pugi::XmlWriterStream::write(const void *data, size_t size)
{
  if (narrow_stream)
  {
    assert(!wide_stream);
    narrow_stream->write(reinterpret_cast< const char * >(data),
                         static_cast< std::streamsize >(size));
  }
  else
  {
    assert(wide_stream);
    assert(size % sizeof(wchar_t) == 0);

    wide_stream->write(reinterpret_cast< const wchar_t * >(data),
                       static_cast< std::streamsize >(size / sizeof(wchar_t)));
  }
}
