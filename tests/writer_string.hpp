#ifndef HEADER_TEST_WRITER_STRING_HPP
#define HEADER_TEST_WRITER_STRING_HPP

import pugixml;

#include <string>

struct xml_writer_string: public pugi::XmlWriter
{
	std::string contents;

	virtual void write(const void* data, size_t size) override;

	std::string as_narrow() const;
	std::basic_string<wchar_t> as_wide() const;
	std::basic_string<pugi::char_t> as_string() const;
};

std::string save_narrow(const pugi::XmlDocument& doc, unsigned int flags, pugi::XmlEncoding encoding);
bool test_save_narrow(const pugi::XmlDocument& doc, unsigned int flags, pugi::XmlEncoding encoding, const char* expected, size_t length);

std::string write_narrow(pugi::XmlNode node, unsigned int flags, pugi::XmlEncoding encoding);
bool test_write_narrow(pugi::XmlNode node, unsigned int flags, pugi::XmlEncoding encoding, const char* expected, size_t length);

std::basic_string<wchar_t> write_wide(pugi::XmlNode node, unsigned int flags, pugi::XmlEncoding encoding);

#endif
