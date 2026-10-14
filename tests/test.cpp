#define _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_DEPRECATE

#include "test.hpp"

#include "writer_string.hpp"

#ifndef PUGIXML_NO_XPATH
static void build_document_order(std::vector< pugi::XPathNode > &result,
                                 pugi::XmlNode root)
{
  result.push_back(pugi::XPathNode());

  pugi::XmlNode cur = root;

  for (;;)
  {
    result.push_back(cur);

    for (pugi::XmlAttribute a = cur.firstAttribute(); a; a = a.nextAttribute())
      result.push_back(pugi::XPathNode(a, cur));

    if (cur.first_child())
      cur = cur.first_child();
    else if (cur.next_sibling())
      cur = cur.next_sibling();
    else
    {
      while (cur && !cur.next_sibling())
        cur = cur.parent();
      cur = cur.next_sibling();

      if (!cur)
        break;
    }
  }
}
#endif

bool test_string_equal(const pugi::char_t *lhs, const pugi::char_t *rhs)
{
  return (!lhs || !rhs) ? lhs == rhs :
#ifdef PUGIXML_WCHAR_MODE
                        wcscmp(lhs, rhs) == 0;
#else
                        strcmp(lhs, rhs) == 0;
#endif
}

bool test_node(const pugi::XmlNode &node, const pugi::char_t *contents,
               const pugi::char_t *indent, unsigned int flags)
{
  xml_writer_string writer;

  node.print(writer, indent, flags, get_native_encoding());

  return writer.as_string() == contents;
}

bool test_double_nan(double value)
{
#if defined(_MSC_VER) || defined(__BORLANDC__)
  return _isnan(value) != 0;
#else
  return value != value;
#endif
}

#ifndef PUGIXML_NO_XPATH
static size_t strlength(const pugi::char_t *s)
{
#ifdef PUGIXML_WCHAR_MODE
  return wcslen(s);
#else
  return strlen(s);
#endif
}

bool test_xpath_string(const pugi::XPathNode &node, const pugi::char_t *query,
                       pugi::XPathVariableSet *variables,
                       const pugi::char_t *expected)
{
  pugi::XPathQuery q(query, variables);
  if (!q)
    return false;

  const size_t capacity = 64;
  pugi::char_t result[capacity];

  size_t size = q.evaluate_string(result, capacity, node);

  if (size != strlength(expected) + 1)
    return false;

  if (size <= capacity)
    return test_string_equal(result, expected);

  std::basic_string< pugi::char_t > buffer(size, ' ');

  return q.evaluate_string(&buffer[0], size, node) == size &&
         test_string_equal(buffer.c_str(), expected);
}

bool test_xpath_boolean(const pugi::XPathNode &node, const pugi::char_t *query,
                        pugi::XPathVariableSet *variables, bool expected)
{
  pugi::XPathQuery q(query, variables);
  if (!q)
    return false;

  return q.evaluate_boolean(node) == expected;
}

bool test_xpath_number(const pugi::XPathNode &node, const pugi::char_t *query,
                       pugi::XPathVariableSet *variables, double expected)
{
  pugi::XPathQuery q(query, variables);
  if (!q)
    return false;

  double value = q.evaluate_number(node);
  double absolute_error = fabs(value - expected);

  const double tolerance = 1e-15;
  return absolute_error < tolerance ||
         absolute_error < fabs(expected) * tolerance;
}

bool test_xpath_number_nan(const pugi::XPathNode &node,
                           const pugi::char_t *query,
                           pugi::XPathVariableSet *variables)
{
  pugi::XPathQuery q(query, variables);
  if (!q)
    return false;

  return test_double_nan(q.evaluate_number(node));
}

bool test_xpath_fail_compile(const pugi::char_t *query,
                             pugi::XPathVariableSet *variables)
{
  return !pugi::XPathQuery(query, variables);
}

void XPathNodeSet_tester::check(bool condition)
{
  if (!condition)
  {
    test_runner::_failure_message = message;
    longjmp(test_runner::_failure_buffer, 1);
  }
}

XPathNodeSet_tester::XPathNodeSet_tester(const pugi::XPathNodeSet &set,
                                         const char *message_)
    : last(0),
      message(message_)
{
  result = set;

  // only sort unsorted sets so that we're able to verify reverse order for some
  // axes
  if (result.type() == pugi::XPathNodeSet::type_unsorted)
    result.sort();

  if (result.empty())
  {
    document_order = 0;
    document_size = 0;
  }
  else
  {
    std::vector< pugi::XPathNode > order;
    build_document_order(
        order,
        (result[0].attribute() ? result[0].parent() : result[0].node()).root());

    document_order = new pugi::XPathNode[order.size()];
    std::copy(order.begin(), order.end(), document_order);

    document_size = order.size();
  }
}

XPathNodeSet_tester::~XPathNodeSet_tester()
{
  // check that we processed everything
  check(last == result.size());

  delete[] document_order;
}

XPathNodeSet_tester &XPathNodeSet_tester::operator%(unsigned int expected)
{
  // check element count
  check(last < result.size());

  // check document order
  check(expected < document_size);
  check(result.begin()[last] == document_order[expected]);

  // continue to the next element
  last++;

  return *this;
}

#endif

bool is_little_endian()
{
  unsigned int ui = 1;
  return *reinterpret_cast< char * >(&ui) == 1;
}

pugi::XmlEncoding get_native_encoding()
{
#ifdef PUGIXML_WCHAR_MODE
  return pugi::encoding_wchar;
#else
  return pugi::encoding_utf8;
#endif
}
