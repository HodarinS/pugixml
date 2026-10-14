module;
/**
 * pugixml parser - version 1.16
 * --------------------------------------------------------
 * Report bugs and download new versions at https://pugixml.org/
 *
 * SPDX-FileCopyrightText: Copyright (C) 2006-2026, by Arseny Kapoulkine
 * (arseny.kapoulkine@gmail.com) SPDX-License-Identifier: MIT
 *
 * See LICENSE.md or notice at the end of this file.
 */

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

// Some functions can not be noexcept in compact mode
#ifdef PUGIXML_COMPACT
#define noexcept_IF_NOT_COMPACT
#else
#define noexcept_IF_NOT_COMPACT noexcept
#endif

export module pugixml;
import std;
import std.compat;
// Character type used for all internal storage and operations; depends on
// PUGIXML_WCHAR_MODE
export namespace pugi
{

using char_t = char;

// String type used for operations that work with STL string; depends on
// PUGIXML_WCHAR_MODE
using string_t = std::basic_string< char >;

// String view type used for operations that can work with a length delimited
// string; depends on PUGIXML_WCHAR_MODE
using string_view_t = std::basic_string_view< char >;
} // namespace pugi

export constexpr short PUGIXML_VERSION = 1160;
// The PugiXML namespace
namespace pugi
{

// Tree node types
export enum XmlNodeType {
  node_null,        // Empty (null) node handle
  node_document,    // A document tree's absolute root
  node_element,     // Element tag, i.e. '<node/>'
  node_pcdata,      // Plain character data, i.e. 'text'
  node_cdata,       // Character data, i.e. '<![CDATA[text]]>'
  node_comment,     // Comment tag, i.e. '<!-- text -->'
  node_pi,          // Processing instruction, i.e. '<?name?>'
  node_declaration, // Document declaration, i.e. '<?xml version="1.0"?>'
  node_doctype      // Document type declaration, i.e. '<!DOCTYPE doc>'
};

// Parsing options
export {
  // Minimal parsing mode (equivalent to turning all other flags off).
  // Only elements and PCDATA sections are added to the DOM tree, no text
  // conversions are performed.
  inline const unsigned int parse_minimal = 0x0000;

  // This flag determines if processing instructions (node_pi) are added to the
  // DOM tree. This flag is off by default.
  inline const unsigned int parse_pi = 0x0001;

  // This flag determines if comments (node_comment) are added to the DOM tree.
  // This flag is off by default.
  inline const unsigned int parse_comments = 0x0002;

  // This flag determines if CDATA sections (node_cdata) are added to the DOM
  // tree. This flag is on by default.
  inline const unsigned int parse_cdata = 0x0004;

  // This flag determines if plain character data (node_pcdata) that consist
  // only of whitespace are added to the DOM tree. This flag is off by default;
  // turning it on usually results in slower parsing and more memory
  // consumption.
  inline const unsigned int parse_ws_pcdata = 0x0008;

  // This flag determines if character and entity references are expanded during
  // parsing. This flag is on by default.
  inline const unsigned int parse_escapes = 0x0010;

  // This flag determines if EOL characters are normalized (converted to #xA)
  // during parsing. This flag is on by default.
  inline const unsigned int parse_eol = 0x0020;

  // This flag determines if attribute values are normalized using CDATA
  // normalization rules during parsing. This flag is on by default.
  inline const unsigned int parse_wconv_attribute = 0x0040;

  // This flag determines if attribute values are normalized using NMTOKENS
  // normalization rules during parsing. This flag is off by default.
  inline const unsigned int parse_wnorm_attribute = 0x0080;

  // This flag determines if document declaration (node_declaration) is added to
  // the DOM tree. This flag is off by default.
  inline const unsigned int parse_declaration = 0x0100;

  // This flag determines if document type declaration (node_doctype) is added
  // to the DOM tree. This flag is off by default.
  inline const unsigned int parse_doctype = 0x0200;

  // This flag determines if plain character data (node_pcdata) that is the only
  // child of the parent node and that consists only of whitespace is added to
  // the DOM tree. This flag is off by default; turning it on may result in
  // slower parsing and more memory consumption.
  inline const unsigned int parse_ws_pcdata_single = 0x0400;

  // This flag determines if leading and trailing whitespace is to be removed
  // from plain character data. This flag is off by default.
  inline const unsigned int parse_trim_pcdata = 0x0800;

  // This flag determines if plain character data that does not have a parent
  // node is added to the DOM tree, and if an empty document is a valid
  // document. This flag is off by default.
  inline const unsigned int parse_fragment = 0x1000;

  // This flag determines if plain character data is stored in the parent
  // element's value. This significantly changes the structure of the document;
  // this flag is only recommended for parsing documents with many PCDATA nodes
  // in memory-constrained environments. This flag is off by default.
  inline const unsigned int parse_embed_pcdata = 0x2000;

  // This flag determines whether two adjacent pcdata should be merged or not,
  // if no intermediary data are parsed in the document. This flag is off by
  // default.
  inline const unsigned int parse_merge_pcdata = 0x4000;

  // The default parsing mode.
  // Elements, PCDATA and CDATA sections are added to the DOM tree,
  // character/reference entities are expanded, End-of-Line characters are
  // normalized, attribute values are normalized using CDATA normalization
  // rules.
  inline const unsigned int parse_default =
      parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol;

  // The full parsing mode.
  // Nodes of all types are added to the DOM tree, character/reference entities
  // are expanded, End-of-Line characters are normalized, attribute values are
  // normalized using CDATA normalization rules.
  inline const unsigned int parse_full = parse_default | parse_pi |
                                         parse_comments | parse_declaration |
                                         parse_doctype;
}
// These flags determine the encoding of input data for XML document
export enum XmlEncoding {
  encoding_auto, // Auto-detect input encoding using BOM or < / <? detection;
                 // use UTF8 if BOM is not found
  encoding_utf8, // UTF8 encoding
  encoding_utf16_le, // Little-endian UTF16
  encoding_utf16_be, // Big-endian UTF16
  encoding_utf16,    // UTF16 with native endianness
  encoding_utf32_le, // Little-endian UTF32
  encoding_utf32_be, // Big-endian UTF32
  encoding_utf32,    // UTF32 with native endianness
  encoding_wchar,    // The same encoding wchar_t has (either UTF16 or UTF32)
  encoding_latin1    // Latin1 encoding (ISO-8859-1)
};

// Formatting flags
export {
  // Indent the nodes that are written to output stream with as many indentation
  // strings as deep the node is in DOM tree. This flag is on by default.
  inline const unsigned int format_indent = 0x01;

  // Write encoding-specific BOM to the output stream. This flag is off by
  // default.
  inline const unsigned int format_write_bom = 0x02;

  // Use raw output mode (no indentation and no line breaks are written). This
  // flag is off by default.
  inline const unsigned int format_raw = 0x04;

  // Omit default XML declaration even if there is no declaration in the
  // document. This flag is off by default.
  inline const unsigned int format_no_declaration = 0x08;

  // Don't escape attribute values and PCDATA contents. This flag is off by
  // default.
  inline const unsigned int format_no_escapes = 0x10;

  // Open file using text mode in XmlDocument::save_file. This enables special
  // character (i.e. new-line) conversions on some systems. This flag is off by
  // default.
  inline const unsigned int format_save_file_text = 0x20;

  // Write every attribute on a new line with appropriate indentation. This flag
  // is off by default.
  inline const unsigned int format_indent_attributes = 0x40;

  // Don't output empty element tags, instead writing an explicit start and end
  // tag even if there are no children. This flag is off by default.
  inline const unsigned int format_no_empty_element_tags = 0x80;

  // Skip characters belonging to range [0; 32) instead of "&#xNN;" encoding.
  // This flag is off by default.
  inline const unsigned int format_skip_control_chars = 0x100;

  // Use single quotes ' instead of double quotes " for enclosing attribute
  // values. This flag is off by default.
  inline const unsigned int format_attribute_single_quote = 0x200;

  // The default set of formatting flags.
  // Nodes are indented depending on their depth in DOM tree, a default
  // declaration is output if document has none.
  inline const unsigned int format_default = format_indent;

  inline const int default_double_precision = 17;
  inline const int default_float_precision = 9;
}

// Forward declarations
struct XmlAttributeStruct;
struct XmlNodeStruct;

export class XmlNodeIterator;
export class XmlAttributeIterator;
export class XmlNamedNodeIterator;

export class XmlTreeWalker;

export struct XmlParseResult;

export class XmlNode;

export class XmlText;
export class XPathNode;
export class XPathNodeSet;
export class XPathQuery;
export class XPathVariableSet;

// Range-based for loop support
export template < typename It > class XmlObjectRange
{
public:
  typedef It const_iterator;
  typedef It iterator;

  XmlObjectRange(It b, It e)
      : _begin(b),
        _end(e)
  {
  }

  It begin() const
  {
    return _begin;
  }
  It end() const
  {
    return _end;
  }

  bool empty() const
  {
    return _begin == _end;
  }

private:
  It _begin, _end;
};

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

// A light-weight handle for manipulating attributes in DOM tree
export class PUGIXML_CLASS XmlAttribute
{
  friend class XmlAttributeIterator;
  friend class XmlNode;

private:
  XmlAttributeStruct *_attr;


public:
  // Default constructor. Constructs an empty attribute.
  XmlAttribute();

  // Constructs attribute from internal pointer
  explicit XmlAttribute(XmlAttributeStruct *attr);

  explicit operator bool() const;

  // Comparison operators (compares wrapped attribute pointers)
  bool operator==(const XmlAttribute &r) const;
  bool operator!=(const XmlAttribute &r) const;
  bool operator<(const XmlAttribute &r) const;
  bool operator>(const XmlAttribute &r) const;
  bool operator<=(const XmlAttribute &r) const;
  bool operator>=(const XmlAttribute &r) const;

  // Check if attribute is empty (null)
  bool empty() const;

  // Get attribute name/value, or "" if attribute is empty
  const char_t *name() const;
  const char_t *value(const char_t *def = "") const;

  // Get attribute value as a number, or the default value if conversion did
  // not succeed or attribute is empty
  int as_int(int def = 0) const;
  unsigned int as_uint(unsigned int def = 0) const;
  double as_double(double def = 0) const;
  float as_float(float def = 0) const;
  long long as_llong(long long def = 0) const;
  unsigned long long as_ullong(unsigned long long def = 0) const;

  // Get attribute value as bool (returns true if first character is in
  // '1tTyY' set), or the default value if attribute is empty
  bool as_bool(bool def = false) const;

  // Set attribute name/value (returns false if attribute is empty or there is
  // not enough memory)
  bool setName(const char_t *rhs);
  bool setName(const char_t *rhs, size_t size);
  bool setName(string_view_t rhs);
  bool setValue(const char_t *rhs);
  bool setValue(const char_t *rhs, size_t size);
  bool setValue(string_view_t rhs);

  // Set attribute value with type conversion (numbers are converted to
  // strings, boolean is converted to "true"/"false")
  bool setValue(int rhs);
  bool setValue(unsigned int rhs);
  bool setValue(long rhs);
  bool setValue(unsigned long rhs);
  bool setValue(double rhs);
  bool setValue(double rhs, int precision);
  bool setValue(float rhs);
  bool setValue(float rhs, int precision);
  bool setValue(bool rhs);
  bool setValue(long long rhs);
  bool setValue(unsigned long long rhs);

  // Set attribute value (equivalent to setValue without error checking)
  XmlAttribute &operator=(const char_t *rhs);
  XmlAttribute &operator=(int rhs);
  XmlAttribute &operator=(unsigned int rhs);
  XmlAttribute &operator=(long rhs);
  XmlAttribute &operator=(unsigned long rhs);
  XmlAttribute &operator=(double rhs);
  XmlAttribute &operator=(float rhs);
  XmlAttribute &operator=(bool rhs);
  XmlAttribute &operator=(string_view_t rhs);
  XmlAttribute &operator=(long long rhs);
  XmlAttribute &operator=(unsigned long long rhs);

  // Get next/previous attribute in the attribute list of the parent node
  XmlAttribute nextAttribute() const;
  XmlAttribute previousAttribute() const;

  // Get hash value (unique for handles to the same object)
  size_t hash_value() const;

  // Get internal pointer
  XmlAttributeStruct *internal_object() const;
};

// A light-weight handle for manipulating nodes in DOM tree
class PUGIXML_CLASS XmlNode
{
  friend class XmlAttributeIterator;
  friend class XmlNodeIterator;
  friend class XmlNamedNodeIterator;

protected:
  XmlNodeStruct *_root;


public:
  // Default constructor. Constructs an empty node.
  XmlNode();

  // Constructs node from internal pointer
  explicit XmlNode(XmlNodeStruct *p);

  // Safe bool conversion operator
  explicit operator bool() const;

  // Comparison operators (compares wrapped node pointers)
  bool operator==(const XmlNode &r) const;
  bool operator!=(const XmlNode &r) const;
  bool operator<(const XmlNode &r) const;
  bool operator>(const XmlNode &r) const;
  bool operator<=(const XmlNode &r) const;
  bool operator>=(const XmlNode &r) const;

  // Check if node is empty (null)
  bool empty() const;

  // Get node type
  XmlNodeType type() const;

  // Get node name, or "" if node is empty or it has no name
  const char_t *name() const;

  // Get node value, or "" if node is empty or it has no value
  // Note: For <node>text</node> node.value() does not return "text"! Use
  // child_value() or text() methods to access text inside nodes.
  const char_t *value() const;

  // Get attribute list
  XmlAttribute firstAttribute() const;
  XmlAttribute lastAttribute() const;

  // Get children list
  XmlNode first_child() const;
  XmlNode last_child() const;

  // Get next/previous sibling in the children list of the parent node
  XmlNode next_sibling() const;
  XmlNode previous_sibling() const;

  // Get parent node
  XmlNode parent() const;

  // Get root of DOM tree this node belongs to
  XmlNode root() const;

  // Get text object for the current node
  XmlText text() const;

  // Get child, attribute or next/previous sibling with the specified name
  XmlNode child(const char_t *name) const;
  XmlAttribute attribute(const char_t *name) const;
  XmlNode next_sibling(const char_t *name) const;
  XmlNode previous_sibling(const char_t *name) const;
  XmlNode child(string_view_t name) const;
  XmlAttribute attribute(string_view_t name) const;
  XmlNode next_sibling(string_view_t name) const;
  XmlNode previous_sibling(string_view_t name) const;

  // Get attribute, starting the search from a hint (and updating hint so that
  // searching for a sequence of attributes is fast)
  XmlAttribute attribute(const char_t *name, XmlAttribute &hint) const;
  XmlAttribute attribute(string_view_t name, XmlAttribute &hint) const;

  // Get child value of current node; that is, value of the first child node
  // of type PCDATA/CDATA
  const char_t *child_value() const;

  // Get child value of child with specified name. Equivalent to
  // child(name).child_value().
  const char_t *child_value(const char_t *name) const;

  // Set node name/value (returns false if node is empty, there is not enough
  // memory, or node can not have name/value)
  bool setName(const char_t *rhs);
  bool setName(const char_t *rhs, size_t size);
  bool setName(string_view_t rhs);
  bool setValue(const char_t *rhs);
  bool setValue(const char_t *rhs, size_t size);
  bool setValue(string_view_t rhs);

  // Add attribute with specified name. Returns added attribute, or empty
  // attribute on errors.
  XmlAttribute append_attribute(const char_t *name);
  XmlAttribute prepend_attribute(const char_t *name);
  XmlAttribute insert_attribute_after(const char_t *name,
                                      const XmlAttribute &attr);
  XmlAttribute insert_attribute_before(const char_t *name,
                                       const XmlAttribute &attr);
  XmlAttribute append_attribute(string_view_t name);
  XmlAttribute prepend_attribute(string_view_t name);
  XmlAttribute insert_attribute_after(string_view_t name,
                                      const XmlAttribute &attr);
  XmlAttribute insert_attribute_before(string_view_t name,
                                       const XmlAttribute &attr);

  // Get attribute with specified name, adding one if it does not exist.
  // Returns the existing or added attribute, or empty attribute on errors.
  XmlAttribute ensure_attribute(const char_t *name);
  XmlAttribute ensure_attribute(string_view_t name);

  // Add a copy of the specified attribute. Returns added attribute, or empty
  // attribute on errors.
  XmlAttribute append_copy(const XmlAttribute &proto);
  XmlAttribute prepend_copy(const XmlAttribute &proto);
  XmlAttribute insert_copy_after(const XmlAttribute &proto,
                                 const XmlAttribute &attr);
  XmlAttribute insert_copy_before(const XmlAttribute &proto,
                                  const XmlAttribute &attr);

  // Add child node with specified type. Returns added node, or empty node on
  // errors.
  XmlNode append_child(XmlNodeType type = node_element);
  XmlNode prepend_child(XmlNodeType type = node_element);
  XmlNode insert_child_after(XmlNodeType type, const XmlNode &node);
  XmlNode insert_child_before(XmlNodeType type, const XmlNode &node);

  // Add child element with specified name. Returns added node, or empty node
  // on errors.
  XmlNode append_child(const char_t *name);
  XmlNode prepend_child(const char_t *name);
  XmlNode insert_child_after(const char_t *name, const XmlNode &node);
  XmlNode insert_child_before(const char_t *name, const XmlNode &node);
  XmlNode append_child(string_view_t name);
  XmlNode prepend_child(string_view_t name);
  XmlNode insert_child_after(string_view_t name, const XmlNode &node);
  XmlNode insert_child_before(string_view_t name, const XmlNode &node);

  // Get child with specified name, adding one if it does not exist. Returns
  // the existing or added node, or empty node on errors.
  XmlNode ensure_child(const char_t *name);
  XmlNode ensure_child(string_view_t name);

  // Add a copy of the specified node as a child. Returns added node, or empty
  // node on errors.
  XmlNode append_copy(const XmlNode &proto);
  XmlNode prepend_copy(const XmlNode &proto);
  XmlNode insert_copy_after(const XmlNode &proto, const XmlNode &node);
  XmlNode insert_copy_before(const XmlNode &proto, const XmlNode &node);

  // Move the specified node to become a child of this node. Returns moved
  // node, or empty node on errors.
  XmlNode append_move(const XmlNode &moved);
  XmlNode prepend_move(const XmlNode &moved);
  XmlNode insert_move_after(const XmlNode &moved, const XmlNode &node);
  XmlNode insert_move_before(const XmlNode &moved, const XmlNode &node);

  // Remove specified attribute
  bool remove_attribute(const XmlAttribute &a);
  bool remove_attribute(const char_t *name);
  bool remove_attribute(string_view_t name);

  // Remove all attributes
  bool remove_attributes();

  // Remove specified child
  bool remove_child(const XmlNode &n);
  bool remove_child(const char_t *name);
  bool remove_child(string_view_t name);

  // Remove all children
  bool remove_children();

  // Parses buffer as an XML document fragment and appends all nodes as
  // children of the current node. Copies/converts the buffer, so it may be
  // deleted or changed after the function returns. Note: append_buffer
  // allocates memory that has the lifetime of the owning document; removing
  // the appended nodes does not immediately reclaim that memory.
  XmlParseResult append_buffer(const void *contents, size_t size,
                               unsigned int options = parse_default,
                               XmlEncoding encoding = encoding_auto);

  // Find attribute using predicate. Returns first attribute for which
  // predicate returned true.
  template < typename Predicate >
  XmlAttribute find_attribute(Predicate pred) const
  {
    if (!_root)
      return XmlAttribute();

    for (XmlAttribute attrib = firstAttribute(); attrib;
         attrib = attrib.nextAttribute())
      if (pred(attrib))
        return attrib;

    return XmlAttribute();
  }

  // Find child node using predicate. Returns first child for which predicate
  // returned true.
  template < typename Predicate > XmlNode find_child(Predicate pred) const
  {
    if (!_root)
      return XmlNode();

    for (XmlNode node = first_child(); node; node = node.next_sibling())
      if (pred(node))
        return node;

    return XmlNode();
  }

  // Find node from subtree using predicate. Returns first node from subtree
  // (depth-first), for which predicate returned true.
  template < typename Predicate > XmlNode find_node(Predicate pred) const
  {
    if (!_root)
      return XmlNode();

    XmlNode cur = first_child();

    while (cur._root && cur._root != _root)
    {
      if (pred(cur))
        return cur;

      if (cur.first_child())
        cur = cur.first_child();
      else if (cur.next_sibling())
        cur = cur.next_sibling();
      else
      {
        while (!cur.next_sibling() && cur._root != _root)
          cur = cur.parent();

        if (cur._root != _root)
          cur = cur.next_sibling();
      }
    }

    return XmlNode();
  }

  // Find child node by attribute name/value
  XmlNode find_child_by_attribute(const char_t *name, const char_t *attr_name,
                                  const char_t *attr_value) const;
  XmlNode find_child_by_attribute(const char_t *attr_name,
                                  const char_t *attr_value) const;

  // Get the absolute node path from root as a text string.
  string_t path(char_t delimiter = '/') const;

  // Search for a node by path consisting of node names and . or .. elements.
  XmlNode first_element_by_path(const char_t *path,
                                char_t delimiter = '/') const;

  // Recursively traverse subtree with XmlTreeWalker
  bool traverse(XmlTreeWalker &walker);

  // Select single node by evaluating XPath query. Returns first node from the
  // resulting node set.
  XPathNode select_node(const char_t *query,
                        XPathVariableSet *variables = nullptr) const;
  XPathNode select_node(const XPathQuery &query) const;

  // Select node set by evaluating XPath query
  XPathNodeSet select_nodes(const char_t *query,
                            XPathVariableSet *variables = nullptr) const;
  XPathNodeSet select_nodes(const XPathQuery &query) const;

  // Print subtree using a writer object
  void print(XmlWriter &writer, const char_t *indent = "\t",
             unsigned int flags = format_default,
             XmlEncoding encoding = encoding_auto,
             unsigned int depth = 0) const;

  // Print subtree to stream
  void print(std::basic_ostream< char > &os, const char_t *indent = "\t",
             unsigned int flags = format_default,
             XmlEncoding encoding = encoding_auto,
             unsigned int depth = 0) const;
  void print(std::basic_ostream< wchar_t > &os, const char_t *indent = "\t",
             unsigned int flags = format_default, unsigned int depth = 0) const;

  // Child nodes iterators
  typedef XmlNodeIterator iterator;

  iterator begin() const;
  iterator end() const;

  // Attribute iterators
  typedef XmlAttributeIterator attribute_iterator;

  attribute_iterator attributes_begin() const;
  attribute_iterator attributes_end() const;

  // Range-based for support
  XmlObjectRange< XmlNodeIterator > children() const;
  XmlObjectRange< XmlAttributeIterator > attributes() const;

  // Range-based for support for all children with the specified name
  // Note: name pointer must have a longer lifetime than the returned object;
  // be careful with passing temporaries!
  XmlObjectRange< XmlNamedNodeIterator > children(const char_t *name) const;

  // Get node offset in parsed file/string (in char_t units) for debugging
  // purposes
  ptrdiff_t offset_debug() const;

  // Get hash value (unique for handles to the same object)
  size_t hash_value() const;

  // Get internal pointer
  XmlNodeStruct *internal_object() const;
};

// A helper for working with text inside PCDATA nodes
export class PUGIXML_CLASS XmlText
{
  friend class XmlNode;

  XmlNodeStruct *_root;

  explicit XmlText(XmlNodeStruct *root);

  XmlNodeStruct *_data_new();
  XmlNodeStruct *_data() const;

public:
  // Default constructor. Constructs an empty object.
  XmlText();

  // Safe bool conversion operator
  explicit operator bool() const;

  // Check if text object is empty (null)
  bool empty() const;

  // Get text, or "" if object is empty
  const char_t *value(const char_t *def = "") const;

  // Get text as a number, or the default value if conversion did not succeed
  // or object is empty
  int as_int(int def = 0) const;
  unsigned int as_uint(unsigned int def = 0) const;
  double as_double(double def = 0) const;
  float as_float(float def = 0) const;
  long long as_llong(long long def = 0) const;
  unsigned long long as_ullong(unsigned long long def = 0) const;

  // Get text as bool (returns true if first character is in '1tTyY' set), or
  // the default value if object is empty
  bool as_bool(bool def = false) const;

  // Set text (returns false if object is empty or there is not enough memory)
  bool set(const char_t *rhs);
  bool set(const char_t *rhs, size_t size);
  bool set(string_view_t rhs);

  // Set text with type conversion (numbers are converted to strings, boolean
  // is converted to "true"/"false")
  bool set(int rhs);
  bool set(unsigned int rhs);
  bool set(long rhs);
  bool set(unsigned long rhs);
  bool set(double rhs);
  bool set(double rhs, int precision);
  bool set(float rhs);
  bool set(float rhs, int precision);
  bool set(bool rhs);
  bool set(long long rhs);
  bool set(unsigned long long rhs);

  // Set text (equivalent to set without error checking)
  XmlText &operator=(const char_t *rhs);
  XmlText &operator=(int rhs);
  XmlText &operator=(unsigned int rhs);
  XmlText &operator=(long rhs);
  XmlText &operator=(unsigned long rhs);
  XmlText &operator=(double rhs);
  XmlText &operator=(float rhs);
  XmlText &operator=(bool rhs);
  XmlText &operator=(string_view_t rhs);
  XmlText &operator=(long long rhs);
  XmlText &operator=(unsigned long long rhs);

  // Get the data node (node_pcdata or node_cdata) for this object
  XmlNode data() const;
};

// Child node iterator (a bidirectional iterator over a collection of
// XmlNode)
class PUGIXML_CLASS XmlNodeIterator
{
  friend class XmlNode;

private:
  mutable XmlNode _wrap;
  XmlNode _parent;

  XmlNodeIterator(XmlNodeStruct *ref, XmlNodeStruct *parent);

public:
  // Iterator traits
  typedef ptrdiff_t difference_type;
  typedef XmlNode value_type;
  typedef XmlNode *pointer;
  typedef XmlNode &reference;
  typedef std::bidirectional_iterator_tag iterator_category;

  // Default constructor
  XmlNodeIterator();

  // Construct an iterator which points to the specified node
  XmlNodeIterator(const XmlNode &node);

  // Iterator operators
  bool operator==(const XmlNodeIterator &rhs) const;
  bool operator!=(const XmlNodeIterator &rhs) const;

  XmlNode &operator*() const;
  XmlNode *operator->() const;

  XmlNodeIterator &operator++();
  XmlNodeIterator operator++(int);

  XmlNodeIterator &operator--();
  XmlNodeIterator operator--(int);
};

// Attribute iterator (a bidirectional iterator over a collection of
// XmlAttribute)
class PUGIXML_CLASS XmlAttributeIterator
{
  friend class XmlNode;

private:
  mutable XmlAttribute _wrap;
  XmlNode _parent;

  XmlAttributeIterator(XmlAttributeStruct *ref, XmlNodeStruct *parent);

public:
  // Iterator traits
  typedef ptrdiff_t difference_type;
  typedef XmlAttribute value_type;
  typedef XmlAttribute *pointer;
  typedef XmlAttribute &reference;
  typedef std::bidirectional_iterator_tag iterator_category;

  // Default constructor
  XmlAttributeIterator();

  // Construct an iterator which points to the specified attribute
  XmlAttributeIterator(const XmlAttribute &attr, const XmlNode &parent);

  // Iterator operators
  bool operator==(const XmlAttributeIterator &rhs) const;
  bool operator!=(const XmlAttributeIterator &rhs) const;

  XmlAttribute &operator*() const;
  XmlAttribute *operator->() const;

  XmlAttributeIterator &operator++();
  XmlAttributeIterator operator++(int);

  XmlAttributeIterator &operator--();
  XmlAttributeIterator operator--(int);
};

// Named node range helper
class PUGIXML_CLASS XmlNamedNodeIterator
{
  friend class XmlNode;

public:
  // Iterator traits
  typedef ptrdiff_t difference_type;
  typedef XmlNode value_type;
  typedef XmlNode *pointer;
  typedef XmlNode &reference;
  typedef std::bidirectional_iterator_tag iterator_category;

  // Default constructor
  XmlNamedNodeIterator();

  // Construct an iterator which points to the specified node
  // Note: name pointer is stored in the iterator and must have a longer
  // lifetime than iterator itself
  XmlNamedNodeIterator(const XmlNode &node, const char_t *name);

  // Iterator operators
  bool operator==(const XmlNamedNodeIterator &rhs) const;
  bool operator!=(const XmlNamedNodeIterator &rhs) const;

  XmlNode &operator*() const;
  XmlNode *operator->() const;

  XmlNamedNodeIterator &operator++();
  XmlNamedNodeIterator operator++(int);

  XmlNamedNodeIterator &operator--();
  XmlNamedNodeIterator operator--(int);

private:
  mutable XmlNode _wrap;
  XmlNode _parent;
  const char_t *_name;

  XmlNamedNodeIterator(XmlNodeStruct *ref, XmlNodeStruct *parent,
                       const char_t *name);
};

// Abstract tree walker class (see XmlNode::traverse)
class PUGIXML_CLASS XmlTreeWalker
{
  friend class XmlNode;

private:
  int _depth;

protected:
  // Get current traversal depth
  int depth() const;

public:
  XmlTreeWalker();
  virtual ~XmlTreeWalker();

  // Callback that is called when traversal begins
  virtual bool begin(XmlNode &node);

  // Callback that is called for each node traversed
  virtual bool for_each(XmlNode &node) = 0;

  // Callback that is called when traversal ends
  virtual bool end(XmlNode &node);
};

// Parsing status, returned as part of XmlParseResult object
export enum XmlParseStatus
{
  status_ok = 0, // No error

  status_file_not_found, // File was not found during load_file()
  status_io_error,       // Error reading from file/stream
  status_out_of_memory,  // Could not allocate memory
  status_internal_error, // Internal error occurred

  status_unrecognized_tag, // Parser could not determine tag type

  status_bad_pi,      // Parsing error occurred while parsing document
                      // declaration/processing instruction
  status_bad_comment, // Parsing error occurred while parsing comment
  status_bad_cdata,   // Parsing error occurred while parsing CDATA section
  status_bad_doctype, // Parsing error occurred while parsing document type
                      // declaration
  status_bad_pcdata,  // Parsing error occurred while parsing PCDATA section
  status_bad_start_element, // Parsing error occurred while parsing start
                            // element tag
  status_bad_attribute,     // Parsing error occurred while parsing element
                            // attribute
  status_bad_end_element,   // Parsing error occurred while parsing end element
                            // tag
  status_end_element_mismatch, // There was a mismatch of start-end tags
                               // (closing tag had incorrect name, some tag
                               // was not closed or there was an excessive
                               // closing tag)

  status_append_invalid_root, // Unable to append nodes since root type is not
                              // node_element or node_document (exclusive to
                              // XmlNode::append_buffer)

  status_no_document_element // Parsing resulted in a document without element
                             // nodes
};

// Parsing result
export struct PUGIXML_CLASS XmlParseResult
{
  // Parsing status (see XmlParseStatus)
  XmlParseStatus status;

  // Last parsed offset (in char_t units from start of input data)
  ptrdiff_t offset;

  // Source document encoding
  XmlEncoding encoding;

  // Default constructor, initializes object to failed state
  XmlParseResult();

  // Cast to bool operator
  operator bool() const;

  // Get error description
  const char *description() const;
};

// Document class (DOM tree root)
export class PUGIXML_CLASS XmlDocument : public XmlNode
{
private:
  char_t *_buffer;

  char _memory[192];

  // Non-copyable semantics
  XmlDocument(const XmlDocument &);
  XmlDocument &operator=(const XmlDocument &);

  void _create();
  void _destroy();
  void _move(XmlDocument &rhs) noexcept;

public:
  // Default constructor, makes empty document
  XmlDocument();

  // Destructor, invalidates all node/attribute handles to this document
  ~XmlDocument();
  // Move semantics support
  XmlDocument(XmlDocument &&rhs) noexcept;
  XmlDocument &operator=(XmlDocument &&rhs) noexcept;

  // Removes all nodes, leaving the empty document
  void reset();

  // Removes all nodes, then copies the entire contents of the specified
  // document
  void reset(const XmlDocument &proto);
  // Load document from stream.
  XmlParseResult load(std::basic_istream< char > &stream,
                      unsigned int options = parse_default,
                      XmlEncoding encoding = encoding_auto);
  XmlParseResult load(std::basic_istream< wchar_t > &stream,
                      unsigned int options = parse_default);

  // Load document from zero-terminated string. No encoding conversions are
  // applied.
  XmlParseResult loadString(const char_t *contents,
                             unsigned int options = parse_default);

  // Load document from file
  XmlParseResult loadFile(const char *path,
                           unsigned int options = parse_default,
                           XmlEncoding encoding = encoding_auto);
  XmlParseResult loadFile(const wchar_t *path,
                           unsigned int options = parse_default,
                           XmlEncoding encoding = encoding_auto);

  // Load document from buffer. Copies/converts the buffer, so it may be
  // deleted or changed after the function returns.
  XmlParseResult loadBuffer(const void *contents, size_t size,
                             unsigned int options = parse_default,
                             XmlEncoding encoding = encoding_auto);

  // Load document from buffer, using the buffer for in-place parsing (the
  // buffer is modified and used for storage of document data). You should
  // ensure that buffer data will persist throughout the document's lifetime,
  // and free the buffer memory manually once document is destroyed.
  XmlParseResult loadBufferInplace(void *contents, size_t size,
                                     unsigned int options = parse_default,
                                     XmlEncoding encoding = encoding_auto);

  // Load document from buffer, using the buffer for in-place parsing (the
  // buffer is modified and used for storage of document data). You should
  // allocate the buffer with pugixml allocation function; document will free
  // the buffer when it is no longer needed (you can't use it anymore).
  XmlParseResult loadBufferInplaceOwn(void *contents, size_t size,
                                         unsigned int options = parse_default,
                                         XmlEncoding encoding = encoding_auto);

  // Save XML document to writer (semantics is slightly different from
  // XmlNode::print, see documentation for details).
  void save(XmlWriter &writer, const char_t *indent = "\t",
            unsigned int flags = format_default,
            XmlEncoding encoding = encoding_auto) const;

  // Save XML document to stream (semantics is slightly different from
  // XmlNode::print, see documentation for details).
  void save(std::basic_ostream< char > &stream, const char_t *indent = "\t",
            unsigned int flags = format_default,
            XmlEncoding encoding = encoding_auto) const;
  void save(std::basic_ostream< wchar_t > &stream, const char_t *indent = "\t",
            unsigned int flags = format_default) const;

  // Save XML to file
  bool saveFile(const char *path, const char_t *indent = "\t",
                 unsigned int flags = format_default,
                 XmlEncoding encoding = encoding_auto) const;
  bool saveFile(const wchar_t *path, const char_t *indent = "\t",
                 unsigned int flags = format_default,
                 XmlEncoding encoding = encoding_auto) const;

  // Get document element
  XmlNode documentElement() const;
};

// XPath query return type
export enum XPathValueType
{
  xpath_type_none,     // Unknown type (query failed to compile)
  xpath_type_node_set, // Node set (XPathNodeSet)
  xpath_type_number,   // Number
  xpath_type_string,   // String
  xpath_type_boolean   // Boolean
};

// XPath parsing result
export struct PUGIXML_CLASS XPathParseResult
{
  // Error message (0 if no error)
  const char *error;

  // Last parsed offset (in char_t units from string start)
  ptrdiff_t offset;

  // Default constructor, initializes object to failed state
  XPathParseResult();

  // Cast to bool operator
  operator bool() const;

  // Get error description
  const char *description() const;
};

// A single XPath variable
export class PUGIXML_CLASS XPathVariable
{
  friend class XPathVariableSet;

protected:
  XPathValueType _type;
  XPathVariable *_next;

  XPathVariable(XPathValueType type);

  // Non-copyable semantics
  XPathVariable(const XPathVariable &);
  XPathVariable &operator=(const XPathVariable &);

public:
  // Get variable name
  const char_t *name() const;

  // Get variable type
  XPathValueType type() const;

  // Get variable value; no type conversion is performed, default value
  // (false, NaN, empty string, empty node set) is returned on type mismatch
  // error
  bool get_boolean() const;
  double get_number() const;
  const char_t *get_string() const;
  const XPathNodeSet &get_node_set() const;

  // Set variable value; no type conversion is performed, false is returned on
  // type mismatch error
  bool set(bool value);
  bool set(double value);
  bool set(const char_t *value);
  bool set(const XPathNodeSet &value);
};

// A set of XPath variables
export class PUGIXML_CLASS XPathVariableSet
{
private:
  XPathVariable *_data[64];

  void _assign(const XPathVariableSet &rhs);
  void _swap(XPathVariableSet &rhs);

  XPathVariable *_find(const char_t *name) const;

  static bool _clone(XPathVariable *var, XPathVariable **out_result);
  static void _destroy(XPathVariable *var);

public:
  // Default constructor/destructor
  XPathVariableSet();
  ~XPathVariableSet();

  // Copy constructor/assignment operator
  XPathVariableSet(const XPathVariableSet &rhs);
  XPathVariableSet &operator=(const XPathVariableSet &rhs);

  // Move semantics support
  XPathVariableSet(XPathVariableSet &&rhs) noexcept;
  XPathVariableSet &operator=(XPathVariableSet &&rhs) noexcept;

  // Add a new variable or get the existing one, if the types match
  XPathVariable *add(const char_t *name, XPathValueType type);

  // Set value of an existing variable; no type conversion is performed, false
  // is returned if there is no such variable or if types mismatch
  bool set(const char_t *name, bool value);
  bool set(const char_t *name, double value);
  bool set(const char_t *name, const char_t *value);
  bool set(const char_t *name, const XPathNodeSet &value);

  // Get existing variable by name
  XPathVariable *get(const char_t *name);
  const XPathVariable *get(const char_t *name) const;
};

// A compiled XPath query object
export class PUGIXML_CLASS XPathQuery
{
private:
  void *_impl;
  XPathParseResult _result;

  // Non-copyable semantics
  XPathQuery(const XPathQuery &);
  XPathQuery &operator=(const XPathQuery &);

public:
  // Construct a compiled object from XPath expression.
  // If PUGIXML_NO_EXCEPTIONS is not defined, throws xpath_exception on
  // compilation errors.
  explicit XPathQuery(const char_t *query,
                      XPathVariableSet *variables = nullptr);

  // Constructor
  XPathQuery();

  // Destructor
  ~XPathQuery();
  // Move semantics support
  XPathQuery(XPathQuery &&rhs) noexcept;
  XPathQuery &operator=(XPathQuery &&rhs) noexcept;

  // Get query expression return type
  XPathValueType return_type() const;

  // Evaluate expression as boolean value in the specified context; performs
  // type conversion if necessary. If PUGIXML_NO_EXCEPTIONS is not defined,
  // throws std::bad_alloc on out of memory errors.
  bool evaluate_boolean(const XPathNode &n) const;

  // Evaluate expression as double value in the specified context; performs
  // type conversion if necessary. If PUGIXML_NO_EXCEPTIONS is not defined,
  // throws std::bad_alloc on out of memory errors.
  double evaluate_number(const XPathNode &n) const;

  // Evaluate expression as string value in the specified context; performs
  // type conversion if necessary. If PUGIXML_NO_EXCEPTIONS is not defined,
  // throws std::bad_alloc on out of memory errors.
  string_t evaluate_string(const XPathNode &n) const;

  // Evaluate expression as string value in the specified context; performs
  // type conversion if necessary. At most capacity characters are written to
  // the destination buffer, full result size is returned (includes
  // terminating zero). If PUGIXML_NO_EXCEPTIONS is not defined, throws
  // std::bad_alloc on out of memory errors. If PUGIXML_NO_EXCEPTIONS is
  // defined, returns empty string instead.
  size_t evaluate_string(char_t *buffer, size_t capacity,
                         const XPathNode &n) const;

  // Evaluate expression as node set in the specified context.
  // If PUGIXML_NO_EXCEPTIONS is not defined, throws xpath_exception on type
  // mismatch and std::bad_alloc on out of memory errors. If
  // PUGIXML_NO_EXCEPTIONS is defined, returns empty node set instead.
  XPathNodeSet evaluate_node_set(const XPathNode &n) const;

  // Evaluate expression as node set in the specified context.
  // Return first node in document order, or empty node if node set is empty.
  // If PUGIXML_NO_EXCEPTIONS is not defined, throws xpath_exception on type
  // mismatch and std::bad_alloc on out of memory errors. If
  // PUGIXML_NO_EXCEPTIONS is defined, returns empty node instead.
  XPathNode evaluate_node(const XPathNode &n) const;

  // Get parsing result (used to get compilation errors in
  // PUGIXML_NO_EXCEPTIONS mode)
  const XPathParseResult &result() const;

  // Safe bool conversion operator
  explicit operator bool() const;

};

#ifndef PUGIXML_NO_EXCEPTIONS
#if defined(_MSC_VER)
// C4275 can be ignored in Visual C++ if you are deriving
// from a type in the Standard C++ Library
#pragma warning(push)
#pragma warning(disable : 4275)
#endif
// XPath exception class
export class PUGIXML_CLASS xpath_exception : public std::exception
{
private:
  XPathParseResult _result;

public:
  // Construct exception from parse result
  explicit xpath_exception(const XPathParseResult &result);

  // Get error message
  virtual const char *what() const noexcept override;

  // Get parse result
  const XPathParseResult &result() const;
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif

// XPath node class (either XmlNode or XmlAttribute)
export class PUGIXML_CLASS XPathNode
{
private:
  XmlNode _node;
  XmlAttribute _attribute;

public:
  // Default constructor; constructs empty XPath node
  XPathNode();

  // Construct XPath node from XML node/attribute
  XPathNode(const XmlNode &node);
  XPathNode(const XmlAttribute &attribute, const XmlNode &parent);

  // Get node/attribute, if any
  XmlNode node() const;
  XmlAttribute attribute() const;

  // Get parent of contained node/attribute
  XmlNode parent() const;

  // Safe bool conversion operator
  explicit operator bool() const;

  // Comparison operators
  bool operator==(const XPathNode &n) const;
  bool operator!=(const XPathNode &n) const;
};

// A fixed-size collection of XPath nodes
export class PUGIXML_CLASS XPathNodeSet
{
public:
  // Collection type
  enum type_t
  {
    type_unsorted,      // Not ordered
    type_sorted,        // Sorted by document order (ascending)
    type_sorted_reverse // Sorted by document order (descending)
  };

  // Constant iterator type
  typedef const XPathNode *const_iterator;

  // We define non-constant iterator to be the same as constant iterator so
  // that various generic algorithms (i.e. boost foreach) work
  typedef const XPathNode *iterator;

  // Default constructor. Constructs empty set.
  XPathNodeSet();

  // Constructs a set from iterator range; data is not checked for duplicates
  // and is not sorted according to provided type, so be careful
  XPathNodeSet(const_iterator begin, const_iterator end,
               type_t type = type_unsorted);

  // Destructor
  ~XPathNodeSet();

  // Copy constructor/assignment operator
  XPathNodeSet(const XPathNodeSet &ns);
  XPathNodeSet &operator=(const XPathNodeSet &ns);

  // Move semantics support
  XPathNodeSet(XPathNodeSet &&rhs) noexcept;
  XPathNodeSet &operator=(XPathNodeSet &&rhs) noexcept;

  // Get collection type
  type_t type() const;

  // Get collection size
  size_t size() const;

  // Indexing operator
  const XPathNode &operator[](size_t index) const;

  // Collection iterators
  const_iterator begin() const;
  const_iterator end() const;

  // Sort the collection in ascending/descending order by document order
  void sort(bool reverse = false);

  // Get first node in the collection by document order
  XPathNode first() const;

  // Check if collection is empty
  bool empty() const;

private:
  type_t _type;

  XPathNode _storage[1];

  XPathNode *_begin;
  XPathNode *_end;

  void _assign(const_iterator begin, const_iterator end, type_t type);
  void _move(XPathNodeSet &rhs) noexcept;
};

// Convert wide string to UTF8
export std::basic_string< char > PUGIXML_FUNCTION as_utf8(const wchar_t *str);
export std::basic_string< char >
    PUGIXML_FUNCTION as_utf8(const std::basic_string< wchar_t > &str);

// Convert UTF8 to wide string
export std::basic_string< wchar_t > PUGIXML_FUNCTION as_wide(const char *str);
export std::basic_string< wchar_t >
    PUGIXML_FUNCTION as_wide(const std::basic_string< char > &str);

// Memory allocation function interface; returns pointer to allocated memory
// or NULL on failure
export typedef void *(*allocation_function)(size_t size);

// Memory deallocation function interface
export typedef void (*deallocation_function)(void *ptr);

// Override default memory management functions. All subsequent
// allocations/deallocations will be performed via supplied functions.
export void PUGIXML_FUNCTION set_memory_management_functions(
    allocation_function allocate, deallocation_function deallocate);

// Get current memory management functions
export allocation_function PUGIXML_FUNCTION get_memory_allocation_function();
export deallocation_function PUGIXML_FUNCTION get_memory_deallocation_function();
} // namespace pugi