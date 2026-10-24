#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_DEPRECATE

#include "test.hpp"

#include <string.h>
#include <stdio.h>
#include <wchar.h>

#include <utility>
#include <vector>
#include <iterator>
#include <string>

#include "helpers.hpp"

using namespace pugi;

#ifdef PUGIXML_NO_STL
template <typename I> static I move_iter(I base, int n)
{
	if (n > 0) while (n--) ++base;
	else while (n++) --base;
	return base;
}
#else
template <typename I> static I move_iter(I base, int n)
{
	std::advance(base, n);
	return base;
}
#endif

TEST_XML(dom_attr_bool_ops, "<node attr='1'/>")
{
	generic_bool_ops_test(doc.child(STR("node")).attribute(STR("attr")));
}

TEST_XML(dom_attr_eq_ops, "<node attr1='1' attr2='2'/>")
{
	generic_eq_ops_test(doc.child(STR("node")).attribute(STR("attr1")), doc.child(STR("node")).attribute(STR("attr2")));
}

TEST_XML(dom_attr_rel_ops, "<node attr1='1' attr2='2'/>")
{
	generic_rel_ops_test(doc.child(STR("node")).attribute(STR("attr1")), doc.child(STR("node")).attribute(STR("attr2")));
}

TEST_XML(dom_attr_empty, "<node attr='1'/>")
{
	generic_empty_test(doc.child(STR("node")).attribute(STR("attr")));
}

TEST_XML(dom_attr_next_previous_attribute, "<node attr1='1' attr2='2' />")
{
	XmlAttribute attr1 = doc.child(STR("node")).attribute(STR("attr1"));
	XmlAttribute attr2 = doc.child(STR("node")).attribute(STR("attr2"));

	CHECK(attr1.nextAttribute() == attr2);
	CHECK(attr2.nextAttribute() == XmlAttribute());

	CHECK(attr1.previousAttribute() == XmlAttribute());
	CHECK(attr2.previousAttribute() == attr1);

	CHECK(XmlAttribute().nextAttribute() == XmlAttribute());
	CHECK(XmlAttribute().previousAttribute() == XmlAttribute());
}

TEST_XML(dom_attr_name_value, "<node attr='1'/>")
{
	XmlAttribute attr = doc.child(STR("node")).attribute(STR("attr"));

	CHECK_NAME_VALUE(attr, STR("attr"), STR("1"));
	CHECK_NAME_VALUE(XmlAttribute(), STR(""), STR(""));
}

TEST_XML(dom_attr_as_string, "<node attr='1'/>")
{
	XmlAttribute attr = doc.child(STR("node")).attribute(STR("attr"));

	CHECK_STRING(attr.value(), STR("1"));
	CHECK_STRING(XmlAttribute().value(), STR(""));
}

TEST_XML(dom_attr_toInt, "<node attr1='1' attr2='-1' attr3='-2147483648' attr4='2147483647' attr5='0'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlAttribute().toInt() == 0);
	CHECK(node.attribute(STR("attr1")).toInt() == 1);
	CHECK(node.attribute(STR("attr2")).toInt() == -1);
	CHECK(node.attribute(STR("attr3")).toInt() == -2147483647 - 1);
	CHECK(node.attribute(STR("attr4")).toInt() == 2147483647);
	CHECK(node.attribute(STR("attr5")).toInt() == 0);
}

TEST_XML(dom_attr_toInt_hex, "<node attr1='0777' attr2='0x5ab' attr3='0XFf' attr4='-0x20' attr5='-0x80000000' attr6='0x'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == 777); // no octal support! intentional
	CHECK(node.attribute(STR("attr2")).toInt() == 1451);
	CHECK(node.attribute(STR("attr3")).toInt() == 255);
	CHECK(node.attribute(STR("attr4")).toInt() == -32);
	CHECK(node.attribute(STR("attr5")).toInt() == -2147483647 - 1);
	CHECK(node.attribute(STR("attr6")).toInt() == 0);
}

TEST_XML(dom_attr_toUInt, "<node attr1='0' attr2='1' attr3='2147483647' attr4='4294967295' attr5='0'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlAttribute().toUInt() == 0);
	CHECK(node.attribute(STR("attr1")).toUInt() == 0);
	CHECK(node.attribute(STR("attr2")).toUInt() == 1);
	CHECK(node.attribute(STR("attr3")).toUInt() == 2147483647);
	CHECK(node.attribute(STR("attr4")).toUInt() == 4294967295u);
	CHECK(node.attribute(STR("attr5")).toUInt() == 0);
}

TEST_XML(dom_attr_toUInt_hex, "<node attr1='0777' attr2='0x5ab' attr3='0XFf' attr4='0x20' attr5='0xFFFFFFFF' attr6='0x'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toUInt() == 777); // no octal support! intentional
	CHECK(node.attribute(STR("attr2")).toUInt() == 1451);
	CHECK(node.attribute(STR("attr3")).toUInt() == 255);
	CHECK(node.attribute(STR("attr4")).toUInt() == 32);
	CHECK(node.attribute(STR("attr5")).toUInt() == 4294967295u);
	CHECK(node.attribute(STR("attr6")).toUInt() == 0);
}

TEST_XML(dom_attr_toInteger_space, "<node attr1=' \t1234' attr2='\t 0x123' attr3='- 16' attr4='- 0x10'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == 1234);
	CHECK(node.attribute(STR("attr2")).toInt() == 291);
	CHECK(node.attribute(STR("attr3")).toInt() == 0);
	CHECK(node.attribute(STR("attr4")).toInt() == 0);


	CHECK(node.attribute(STR("attr1")).toLongLong() == 1234);
}

TEST_XML(dom_attr_toFloat, "<node attr1='0' attr2='1' attr3='0.12' attr4='-5.1' attr5='3e-4' attr6='3.14159265358979323846'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlAttribute().toFloat() == 0);
	CHECK_DOUBLE(double(node.attribute(STR("attr1")).toFloat()), 0);
	CHECK_DOUBLE(double(node.attribute(STR("attr2")).toFloat()), 1);
	CHECK_DOUBLE(double(node.attribute(STR("attr3")).toFloat()), 0.12);
	CHECK_DOUBLE(double(node.attribute(STR("attr4")).toFloat()), -5.1);
	CHECK_DOUBLE(double(node.attribute(STR("attr5")).toFloat()), 3e-4);
	CHECK_DOUBLE(double(node.attribute(STR("attr6")).toFloat()), 3.14159265358979323846);
}

TEST_XML(dom_attr_toDouble, "<node attr1='0' attr2='1' attr3='0.12' attr4='-5.1' attr5='3e-4' attr6='3.14159265358979323846'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlAttribute().toDouble() == 0);
	CHECK_DOUBLE(node.attribute(STR("attr1")).toDouble(), 0);
	CHECK_DOUBLE(node.attribute(STR("attr2")).toDouble(), 1);
	CHECK_DOUBLE(node.attribute(STR("attr3")).toDouble(), 0.12);
	CHECK_DOUBLE(node.attribute(STR("attr4")).toDouble(), -5.1);
	CHECK_DOUBLE(node.attribute(STR("attr5")).toDouble(), 3e-4);
	CHECK_DOUBLE(node.attribute(STR("attr6")).toDouble(), 3.14159265358979323846);
}

TEST_XML(dom_attr_toBool, "<node attr1='0' attr2='1' attr3='true' attr4='True' attr5='Yes' attr6='yes' attr7='false'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(!XmlAttribute().toBool());
	CHECK(!node.attribute(STR("attr1")).toBool());
	CHECK(node.attribute(STR("attr2")).toBool());
	CHECK(node.attribute(STR("attr3")).toBool());
	CHECK(node.attribute(STR("attr4")).toBool());
	CHECK(node.attribute(STR("attr5")).toBool());
	CHECK(node.attribute(STR("attr6")).toBool());
	CHECK(!node.attribute(STR("attr7")).toBool());

	XmlAttribute attr = node.appendAttribute(STR("attr8"));
	CHECK(attr.toBool(true) == true);
}


TEST_XML(dom_attr_toLongLong, "<node attr1='1' attr2='-1' attr3='-9223372036854775808' attr4='9223372036854775807' attr5='0'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlAttribute().toLongLong() == 0);
	CHECK(node.attribute(STR("attr1")).toLongLong() == 1);
	CHECK(node.attribute(STR("attr2")).toLongLong() == -1);
	CHECK(node.attribute(STR("attr3")).toLongLong() == -9223372036854775807ll - 1);
	CHECK(node.attribute(STR("attr4")).toLongLong() == 9223372036854775807ll);
	CHECK(node.attribute(STR("attr5")).toLongLong() == 0);
}

TEST_XML(dom_attr_toLongLong_hex, "<node attr1='0777' attr2='0x5ab' attr3='0XFf' attr4='-0x20' attr5='-0x8000000000000000' attr6='0x'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toLongLong() == 777); // no octal support! intentional
	CHECK(node.attribute(STR("attr2")).toLongLong() == 1451);
	CHECK(node.attribute(STR("attr3")).toLongLong() == 255);
	CHECK(node.attribute(STR("attr4")).toLongLong() == -32);
	CHECK(node.attribute(STR("attr5")).toLongLong() == -9223372036854775807ll - 1);
	CHECK(node.attribute(STR("attr6")).toLongLong() == 0);
}

TEST_XML(dom_attr_toULongLong, "<node attr1='0' attr2='1' attr3='9223372036854775807' attr4='18446744073709551615' attr5='0'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlAttribute().toULongLong() == 0);
	CHECK(node.attribute(STR("attr1")).toULongLong() == 0);
	CHECK(node.attribute(STR("attr2")).toULongLong() == 1);
	CHECK(node.attribute(STR("attr3")).toULongLong() == 9223372036854775807ull);
	CHECK(node.attribute(STR("attr4")).toULongLong() == 18446744073709551615ull);
	CHECK(node.attribute(STR("attr5")).toULongLong() == 0);
}

TEST_XML(dom_attr_toULongLong_hex, "<node attr1='0777' attr2='0x5ab' attr3='0XFf' attr4='0x20' attr5='0xFFFFFFFFFFFFFFFF' attr6='0x'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toULongLong() == 777); // no octal support! intentional
	CHECK(node.attribute(STR("attr2")).toULongLong() == 1451);
	CHECK(node.attribute(STR("attr3")).toULongLong() == 255);
	CHECK(node.attribute(STR("attr4")).toULongLong() == 32);
	CHECK(node.attribute(STR("attr5")).toULongLong() == 18446744073709551615ull);
	CHECK(node.attribute(STR("attr6")).toULongLong() == 0);
}

TEST(dom_attr_defaults)
{
    XmlAttribute attr;

    CHECK_STRING(attr.value(STR("foo")), STR("foo"));
    CHECK(attr.toInt(42) == 42);
    CHECK(attr.toUInt(42) == 42);
    CHECK(attr.toDouble(42) == 42);
    CHECK(attr.toFloat(42) == 42);
    CHECK(attr.toBool(true) == true);


    CHECK(attr.toLongLong(42) == 42);
    CHECK(attr.toULongLong(42) == 42);

}

TEST_XML(dom_attr_iterator, "<node><node1 attr1='0'/><node2 attr1='0' attr2='1'/><node3/></node>")
{
	XmlNode node1 = doc.child(STR("node")).child(STR("node1"));
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));
	XmlNode node3 = doc.child(STR("node")).child(STR("node3"));

	CHECK(XmlNode().attributes_begin() == XmlAttributeIterator());
	CHECK(XmlNode().attributes_end() == XmlAttributeIterator());

	CHECK(node1.attributes_begin() == XmlAttributeIterator(node1.attribute(STR("attr1")), node1));
	CHECK(move_iter(node1.attributes_begin(), 1) == node1.attributes_end());
	CHECK(move_iter(node1.attributes_end(), -1) == node1.attributes_begin());
	CHECK(*node1.attributes_begin() == node1.attribute(STR("attr1")));
	CHECK_STRING(node1.attributes_begin()->name(), STR("attr1"));

	CHECK(move_iter(node2.attributes_begin(), 2) == node2.attributes_end());
	CHECK(move_iter(node2.attributes_end(), -2) == node2.attributes_begin());

	CHECK(node3.attributes_begin() != XmlAttributeIterator());
	CHECK(node3.attributes_begin() == node3.attributes_end());

	XmlAttributeIterator it = XmlAttributeIterator(node2.attribute(STR("attr2")), node2);
	XmlAttributeIterator itt = it;

	CHECK(itt++ == it);
	CHECK(itt == node2.attributes_end());

	CHECK(itt-- == node2.attributes_end());
	CHECK(itt == it);

	CHECK(++itt == node2.attributes_end());
	CHECK(itt == node2.attributes_end());

	CHECK(--itt == it);
	CHECK(itt == it);

	CHECK(++itt != it);
}

TEST_XML(dom_attr_iterator_end, "<node><node1 attr1='0'/><node2 attr1='0' attr2='1'/><node3/></node>")
{
	XmlNode node1 = doc.child(STR("node")).child(STR("node1"));
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));
	XmlNode node3 = doc.child(STR("node")).child(STR("node3"));

	CHECK(node1.attributes_end() != node2.attributes_end() && node1.attributes_end() != node3.attributes_end() && node2.attributes_end() != node3.attributes_end());
	CHECK(node1.attributes_end() != XmlAttributeIterator() && node2.attributes_end() != XmlAttributeIterator() && node3.attributes_end() != XmlAttributeIterator());
}

TEST_XML(dom_attr_iterator_invalidate, "<node><node1 attr1='0'/><node2 attr1='0' attr2='1'/><node3/></node>")
{
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));

	XmlAttributeIterator it1 = node2.attributes_begin();
	XmlAttributeIterator it2 = move_iter(it1, 1);
	XmlAttributeIterator it3 = move_iter(it2, 1);

	CHECK(it3 == node2.attributes_end());

	// removing attr2, it2 is invalid now, it3 is still past-the-end
	node2.removeAttribute(*it2);

	CHECK(node2.attributes_end() == it3);
	CHECK(move_iter(it1, 1) == it3);
	CHECK(move_iter(it3, -1) == it1);
	CHECK_STRING(it1->name(), STR("attr1"));

	// adding attr2 back, it3 is still past-the-end!
	XmlAttributeIterator it2new = XmlAttributeIterator(node2.appendAttribute(STR("attr2-new")), node2);

	CHECK(node2.attributes_end() == it3);
	CHECK(move_iter(it1, 1) == it2new);
	CHECK(move_iter(it2new, 1) == it3);
	CHECK(move_iter(it3, -1) == it2new);
	CHECK_STRING(it2new->name(), STR("attr2-new"));

	// removing both attributes, it3 is now equal to the begin
	node2.removeAttribute(*it1);
	node2.removeAttribute(*it2new);
	CHECK(!node2.firstAttribute());

	CHECK(node2.attributes_begin() == it3);
	CHECK(node2.attributes_end() == it3);
}

TEST_XML(dom_attr_iterator_const, "<node attr1='0' attr2='1'/>")
{
    XmlNode node = doc.child(STR("node"));

    const XmlAttributeIterator i1 = node.attributes_begin();
    const XmlAttributeIterator i2 = ++XmlAttributeIterator(i1);
    const XmlAttributeIterator i3 = ++XmlAttributeIterator(i2);

    CHECK(*i1 == node.attribute(STR("attr1")));
    CHECK(*i2 == node.attribute(STR("attr2")));
    CHECK(i3 == node.attributes_end());

    CHECK_STRING(i1->name(), STR("attr1"));
    CHECK_STRING(i2->name(), STR("attr2"));
}

TEST_XML(dom_node_bool_ops, "<node/>")
{
	generic_bool_ops_test(doc.child(STR("node")));
}

TEST_XML(dom_node_eq_ops, "<node><node1/><node2/></node>")
{
	generic_eq_ops_test(doc.child(STR("node")).child(STR("node1")), doc.child(STR("node")).child(STR("node2")));
}

TEST_XML(dom_node_rel_ops, "<node><node1/><node2/></node>")
{
	generic_rel_ops_test(doc.child(STR("node")).child(STR("node1")), doc.child(STR("node")).child(STR("node2")));
}

TEST_XML(dom_node_empty, "<node/>")
{
	generic_empty_test(doc.child(STR("node")));
}

TEST_XML(dom_node_iterator, "<node><node1><child1/></node1><node2><child1/><child2/></node2><node3/></node>")
{
	XmlNode node1 = doc.child(STR("node")).child(STR("node1"));
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));
	XmlNode node3 = doc.child(STR("node")).child(STR("node3"));

	CHECK(XmlNode().begin() == XmlNodeIterator());
	CHECK(XmlNode().end() == XmlNodeIterator());

	CHECK(node1.begin() == XmlNodeIterator(node1.child(STR("child1"))));
	CHECK(move_iter(node1.begin(), 1) == node1.end());
	CHECK(move_iter(node1.end(), -1) == node1.begin());
	CHECK(*node1.begin() == node1.child(STR("child1")));
	CHECK_STRING(node1.begin()->name(), STR("child1"));

	CHECK(move_iter(node2.begin(), 2) == node2.end());
	CHECK(move_iter(node2.end(), -2) == node2.begin());

	CHECK(node3.begin() != XmlNodeIterator());
	CHECK(node3.begin() == node3.end());

	XmlNodeIterator it = node2.child(STR("child2"));
	XmlNodeIterator itt = it;

	CHECK(itt++ == it);
	CHECK(itt == node2.end());

	CHECK(itt-- == node2.end());
	CHECK(itt == it);

	CHECK(++itt == node2.end());
	CHECK(itt == node2.end());

	CHECK(--itt == it);
	CHECK(itt == it);

	CHECK(++itt != it);
}

TEST_XML(dom_node_iterator_end, "<node><node1><child1/></node1><node2><child1/><child2/></node2><node3/></node>")
{
	XmlNode node1 = doc.child(STR("node")).child(STR("node1"));
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));
	XmlNode node3 = doc.child(STR("node")).child(STR("node3"));

	CHECK(node1.end() != node2.end() && node1.end() != node3.end() && node2.end() != node3.end());
	CHECK(node1.end() != XmlNodeIterator() && node2.end() != XmlNodeIterator() && node3.end() != XmlNodeIterator());
}

TEST_XML(dom_node_iterator_invalidate, "<node><node1><child1/></node1><node2><child1/><child2/></node2><node3/></node>")
{
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));

	XmlNodeIterator it1 = node2.begin();
	XmlNodeIterator it2 = move_iter(it1, 1);
	XmlNodeIterator it3 = move_iter(it2, 1);

	CHECK(it3 == node2.end());

	// removing child2, it2 is invalid now, it3 is still past-the-end
	node2.removeChild(*it2);

	CHECK(node2.end() == it3);
	CHECK(move_iter(it1, 1) == it3);
	CHECK(move_iter(it3, -1) == it1);
	CHECK_STRING(it1->name(), STR("child1"));

	// adding attr2 back, it3 is still past-the-end!
	XmlNodeIterator it2new = node2.appendChild();
	it2new->setName(STR("child2-new"));

	CHECK(node2.end() == it3);
	CHECK(move_iter(it1, 1) == it2new);
	CHECK(move_iter(it2new, 1) == it3);
	CHECK(move_iter(it3, -1) == it2new);
	CHECK_STRING(it2new->name(), STR("child2-new"));

	// removing both nodes, it3 is now equal to the begin
	node2.removeChild(*it1);
	node2.removeChild(*it2new);
	CHECK(!node2.firstChild());

	CHECK(node2.begin() == it3);
	CHECK(node2.end() == it3);
}

TEST_XML(dom_node_iterator_const, "<node><child1/><child2/></node>")
{
    XmlNode node = doc.child(STR("node"));

    const XmlNodeIterator i1 = node.begin();
    const XmlNodeIterator i2 = ++XmlNodeIterator(i1);
    const XmlNodeIterator i3 = ++XmlNodeIterator(i2);

    CHECK(*i1 == node.child(STR("child1")));
    CHECK(*i2 == node.child(STR("child2")));
    CHECK(i3 == node.end());

    CHECK_STRING(i1->name(), STR("child1"));
    CHECK_STRING(i2->name(), STR("child2"));
}

TEST_XML(dom_node_parent, "<node><child/></node>")
{
	CHECK(XmlNode().parent() == XmlNode());
	CHECK(doc.child(STR("node")).child(STR("child")).parent() == doc.child(STR("node")));
	CHECK(doc.child(STR("node")).parent() == doc);
}

TEST_XML(dom_node_root, "<node><child/></node>")
{
	CHECK(XmlNode().root() == XmlNode());
	CHECK(doc.child(STR("node")).child(STR("child")).root() == doc);
	CHECK(doc.child(STR("node")).root() == doc);
}

TEST_XML_FLAGS(dom_node_type, "<?xml?><!DOCTYPE><?pi?><!--comment--><node>pcdata<![CDATA[cdata]]></node>", parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype)
{
	CHECK(XmlNode().type() == node_null);
	CHECK(doc.type() == node_document);

	XmlNodeIterator it = doc.begin();

	CHECK((it++)->type() == node_declaration);
	CHECK((it++)->type() == node_doctype);
	CHECK((it++)->type() == node_pi);
	CHECK((it++)->type() == node_comment);
	CHECK((it++)->type() == node_element);

	XmlNodeIterator cit = doc.child(STR("node")).begin();

	CHECK((cit++)->type() == node_pcdata);
	CHECK((cit++)->type() == node_cdata);
}

TEST_XML_FLAGS(dom_node_name_value, "<?xml?><!DOCTYPE id><?pi?><!--comment--><node>pcdata<![CDATA[cdata]]></node>", parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype)
{
	CHECK_NAME_VALUE(XmlNode(), STR(""), STR(""));
	CHECK_NAME_VALUE(doc, STR(""), STR(""));

	XmlNodeIterator it = doc.begin();

	CHECK_NAME_VALUE(*it++, STR("xml"), STR(""));
	CHECK_NAME_VALUE(*it++, STR(""), STR("id"));
	CHECK_NAME_VALUE(*it++, STR("pi"), STR(""));
	CHECK_NAME_VALUE(*it++, STR(""), STR("comment"));
	CHECK_NAME_VALUE(*it++, STR("node"), STR(""));

	XmlNodeIterator cit = doc.child(STR("node")).begin();

	CHECK_NAME_VALUE(*cit++, STR(""), STR("pcdata"));
	CHECK_NAME_VALUE(*cit++, STR(""), STR("cdata"));
}

TEST_XML(dom_node_child, "<node><child1/><child2/></node>")
{
	CHECK(XmlNode().child(STR("n")) == XmlNode());

	CHECK(doc.child(STR("n")) == XmlNode());
	CHECK_NAME_VALUE(doc.child(STR("node")), STR("node"), STR(""));
	CHECK(doc.child(STR("node")).child(STR("child2")) == doc.child(STR("node")).lastChild());
}


TEST_XML(dom_node_child_stringview, "<node><child1/><child2/></node>")
{
	CHECK(XmlNode().child(string_view_t(STR("n"))) == XmlNode());
	CHECK(doc.child(string_view_t()) == XmlNode());
	CHECK(doc.child(string_view_t(STR("n"))) == XmlNode());

	XmlNode node = doc.child(string_view_t(STR("node")));
	CHECK_NAME_VALUE(node, STR("node"), STR(""));
	CHECK(node.child(string_view_t(STR("child2"))) == node.lastChild());

	// verify only the characters in the view of the string view are included in the comparison
	CHECK_NAME_VALUE(doc.child(string_view_t(STR("node_andextratext"), 4)), STR("node"), STR(""));
	CHECK(doc.child(string_view_t(STR("node"), 2)) == XmlNode());
}

TEST_XML(dom_node_child_interior_null, "<node><child1/><child2/></node>")
{
	const char_t name[] = STR("node\0extra");
	size_t len = (sizeof(name) / sizeof(char_t)) - 1;
	CHECK(len == 10);

	XmlNode node = doc.child(string_view_t(name, 4)); // "node" view excluding null
	CHECK_NAME_VALUE(node, STR("node"), STR(""));
	CHECK(doc.child(string_view_t(name, 5)) == XmlNode()); // "node\0" view including null
	CHECK(doc.child(string_view_t(name, len)) == XmlNode()); // "node\0extra" view

	node.setName(string_view_t(name, len));
	CHECK_NODE(doc, STR("<node><child1/><child2/></node>"));
	CHECK_NAME_VALUE(node, STR("node"), STR(""));
	CHECK_NAME_VALUE(doc.child(string_view_t(name, 4)), STR("node"), STR(""));  // "node" view excluding null
	CHECK(doc.child(string_view_t(name, 5)) == XmlNode()); // "node\0" view including null
	CHECK(doc.child(string_view_t(name, len)) == XmlNode()); // "node\0extra" view
}


TEST_XML(dom_node_attribute, "<node attr1='0' attr2='1'/>")
{
	CHECK(XmlNode().attribute(STR("a")) == XmlAttribute());

	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("n")) == XmlAttribute());
	CHECK_NAME_VALUE(node.attribute(STR("attr1")), STR("attr1"), STR("0"));
	CHECK(node.attribute(STR("attr2")) == node.lastAttribute());
}

TEST_XML(dom_node_attribute_stringview, "<node attr1='0' attr2='1'/>")
{
	CHECK(XmlNode().attribute(string_view_t(STR("a"))) == XmlAttribute());

	XmlNode node = doc.child(string_view_t(STR("node")));

	CHECK(node.attribute(string_view_t()) == XmlAttribute());
	CHECK(node.attribute(string_view_t(STR("n"))) == XmlAttribute());
	CHECK_NAME_VALUE(node.attribute(string_view_t(STR("attr1"))), STR("attr1"), STR("0"));
	CHECK(node.attribute(string_view_t(STR("attr2"))) == node.lastAttribute());

	// verify only the characters in the view of the string view are included in the comparison
	CHECK_NAME_VALUE(node.attribute(string_view_t(STR("attr1_andextratext"), 5)), STR("attr1"), STR("0"));
	CHECK(node.attribute(string_view_t(STR("attr1"), 2)) == XmlAttribute());
}

TEST_XML(dom_node_attribute_interior_null, "<node attr1='0' attr2='1'/>")
{
	XmlNode node = doc.child(STR("node"));
	CHECK_NAME_VALUE(node, STR("node"), STR(""));

	const char_t name[] = STR("attr2\0extra");
	size_t len = (sizeof(name) / sizeof(char_t)) - 1;
	CHECK(len == 11);
	CHECK_NAME_VALUE(node.attribute(string_view_t(name, 5)), STR("attr2"), STR("1")); // "attr2" view excluding null
	CHECK(node.attribute(string_view_t(name, 6)) == XmlAttribute()); // "attr2\0" view including null
	CHECK(node.attribute(string_view_t(name, len)) == XmlAttribute()); // "attr2\0extra" view

	XmlAttribute attr = node.attribute(STR("attr2"));
	CHECK_NAME_VALUE(attr, STR("attr2"), STR("1"));
	attr.setName(string_view_t(name, len));

	CHECK_NODE(doc, STR("<node attr1=\"0\" attr2=\"1\"/>"));
	CHECK_NAME_VALUE(node.attribute(string_view_t(name, 5)), STR("attr2"), STR("1")); // "attr2" view excluding null
	CHECK(node.attribute(string_view_t(name, 6)) == XmlAttribute()); // "attr2\0" view including null
	CHECK(node.attribute(string_view_t(name, len)) == XmlAttribute()); // "attr2\0extra" view
}

TEST_XML(dom_node_next_previousSibling, "<node><child1/><child2/><child3/></node>")
{
	CHECK(XmlNode().nextSibling() == XmlNode());
	CHECK(XmlNode().nextSibling(STR("n")) == XmlNode());

	CHECK(XmlNode().previousSibling() == XmlNode());
	CHECK(XmlNode().previousSibling(STR("n")) == XmlNode());

	XmlNode child1 = doc.child(STR("node")).child(STR("child1"));
	XmlNode child2 = doc.child(STR("node")).child(STR("child2"));
	XmlNode child3 = doc.child(STR("node")).child(STR("child3"));

	CHECK(child1.nextSibling() == child2);
	CHECK(child3.nextSibling() == XmlNode());

	CHECK(child1.previousSibling() == XmlNode());
	CHECK(child3.previousSibling() == child2);

	CHECK(child1.nextSibling(STR("child3")) == child3);
	CHECK(child1.nextSibling(STR("child")) == XmlNode());

	CHECK(child1.nextSibling(string_view_t(STR("child3"))) == child3);
	CHECK(child1.nextSibling(string_view_t(STR("child"))) == XmlNode());

	CHECK(child3.previousSibling(STR("child1")) == child1);
	CHECK(child3.previousSibling(STR("child")) == XmlNode());

	CHECK(child3.previousSibling(string_view_t(STR("child1"))) == child1);
	CHECK(child3.previousSibling(string_view_t(STR("child"))) == XmlNode());
}

TEST_XML(dom_node_childValue, "<node><novalue/><child1>value1</child1><child2>value2<n/></child2><child3><![CDATA[value3]]></child3>value4</node>")
{
	CHECK_STRING(XmlNode().childValue(), STR(""));
	CHECK_STRING(XmlNode().childValue(STR("n")), STR(""));

	XmlNode node = doc.child(STR("node"));

	CHECK_STRING(node.childValue(), STR("value4"));
	CHECK_STRING(node.child(STR("child1")).childValue(), STR("value1"));
	CHECK_STRING(node.child(STR("child2")).childValue(), STR("value2"));
	CHECK_STRING(node.child(STR("child3")).childValue(), STR("value3"));
	CHECK_STRING(node.childValue(STR("child3")), STR("value3"));
	CHECK_STRING(node.childValue(STR("novalue")), STR(""));
}

TEST_XML(dom_node_first_last_attribute, "<node attr1='0' attr2='1'/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.firstAttribute() == node.attribute(STR("attr1")));
	CHECK(node.lastAttribute() == node.attribute(STR("attr2")));

	CHECK(XmlNode().firstAttribute() == XmlAttribute());
	CHECK(XmlNode().lastAttribute() == XmlAttribute());

	CHECK(doc.firstAttribute() == XmlAttribute());
	CHECK(doc.lastAttribute() == XmlAttribute());
}

TEST_XML(dom_node_first_lastChild, "<node><child1/><child2/></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.firstChild() == node.child(STR("child1")));
	CHECK(node.lastChild() == node.child(STR("child2")));

	CHECK(XmlNode().firstChild() == XmlNode());
	CHECK(XmlNode().lastChild() == XmlNode());

	CHECK(doc.firstChild() == node);
	CHECK(doc.lastChild() == node);
}

TEST_XML(dom_node_findChildByAttribute, "<node><stub attr='value3' /><child1 attr='value1'/><child2 attr='value2'/><child2 attr='value3'/></node>")
{
	CHECK(XmlNode().findChildByAttribute(STR("name"), STR("attr"), STR("value")) == XmlNode());
	CHECK(XmlNode().findChildByAttribute(STR("attr"), STR("value")) == XmlNode());

	XmlNode node = doc.child(STR("node"));

	CHECK(node.findChildByAttribute(STR("child2"), STR("attr"), STR("value3")) == node.lastChild());
	CHECK(node.findChildByAttribute(STR("child2"), STR("attr3"), STR("value3")) == XmlNode());
	CHECK(node.findChildByAttribute(STR("attr"), STR("value2")) == node.child(STR("child2")));
	CHECK(node.findChildByAttribute(STR("attr3"), STR("value")) == XmlNode());
}

TEST(dom_node_findChildByAttribute_null)
{
	XmlDocument doc;
	XmlNode node0 = doc.appendChild();
	XmlNode node1 = doc.appendChild(STR("a"));
	XmlNode node2 = doc.appendChild(STR("a"));
	XmlNode node3 = doc.appendChild(STR("a"));

	(void)node0;

	// this adds an attribute with null name and/or value in the internal representation
	node1.appendAttribute(STR(""));
	node2.appendAttribute(STR("id"));
	node3.appendAttribute(STR("id")) = STR("1");

	// make sure findChildByAttribute works if name/value is null
	CHECK(doc.findChildByAttribute(STR("unknown"), STR("wrong")) == XmlNode());
	CHECK(doc.findChildByAttribute(STR("id"), STR("wrong")) == XmlNode());
	CHECK(doc.findChildByAttribute(STR("id"), STR("")) == node2);
	CHECK(doc.findChildByAttribute(STR("id"), STR("1")) == node3);

	CHECK(doc.findChildByAttribute(STR("a"), STR("unknown"), STR("wrong")) == XmlNode());
	CHECK(doc.findChildByAttribute(STR("a"), STR("id"), STR("wrong")) == XmlNode());
	CHECK(doc.findChildByAttribute(STR("a"), STR("id"), STR("")) == node2);
	CHECK(doc.findChildByAttribute(STR("a"), STR("id"), STR("1")) == node3);
}

struct find_predicate_const
{
	bool result;

	find_predicate_const(bool result_): result(result_)
	{
	}

	template <typename T> bool operator()(const T&) const
	{
		return result;
	}
};

struct find_predicate_prefix
{
	const char_t* prefix;

	find_predicate_prefix(const char_t* prefix_): prefix(prefix_)
	{
	}

	template <typename T> bool operator()(const T& obj) const
	{
	#ifdef PUGIXML_WCHAR_MODE
		// can't use wcsncmp here because of a bug in DMC
		return std::basic_string<char_t>(obj.name()).compare(0, wcslen(prefix), prefix) == 0;
	#else
		return strncmp(obj.name(), prefix, strlen(prefix)) == 0;
	#endif
	}
};

TEST_XML(dom_node_findAttribute, "<node attr1='0' attr2='1'/>")
{
	CHECK(XmlNode().findAttribute(find_predicate_const(true)) == XmlAttribute());

	XmlNode node = doc.child(STR("node"));

	CHECK(doc.findAttribute(find_predicate_const(true)) == XmlAttribute());
	CHECK(node.findAttribute(find_predicate_const(true)) == node.firstAttribute());
	CHECK(node.findAttribute(find_predicate_const(false)) == XmlAttribute());
	CHECK(node.findAttribute(find_predicate_prefix(STR("attr2"))) == node.lastAttribute());
	CHECK(node.findAttribute(find_predicate_prefix(STR("attr"))) == node.firstAttribute());
}

TEST_XML(dom_node_findChild, "<node><child1/><child2/></node>")
{
	CHECK(XmlNode().findChild(find_predicate_const(true)) == XmlNode());

	XmlNode node = doc.child(STR("node"));

	CHECK(node.child(STR("node")).child(STR("child1")).findChild(find_predicate_const(true)) == XmlNode());
	CHECK(node.findChild(find_predicate_const(true)) == node.firstChild());
	CHECK(node.findChild(find_predicate_const(false)) == XmlNode());
	CHECK(node.findChild(find_predicate_prefix(STR("child2"))) == node.lastChild());
	CHECK(node.findChild(find_predicate_prefix(STR("child"))) == node.firstChild());
}

TEST_XML(dom_node_findNode, "<node><child1/><child2/></node>")
{
	CHECK(XmlNode().findNode(find_predicate_const(true)) == XmlNode());

	XmlNode node = doc.child(STR("node"));

	CHECK(node.child(STR("node")).child(STR("child1")).findNode(find_predicate_const(true)) == XmlNode());
	CHECK(node.findNode(find_predicate_const(true)) == node.firstChild());
	CHECK(node.findNode(find_predicate_const(false)) == XmlNode());
	CHECK(node.findNode(find_predicate_prefix(STR("child2"))) == node.lastChild());
	CHECK(node.findNode(find_predicate_prefix(STR("child"))) == node.firstChild());
	CHECK(doc.findNode(find_predicate_prefix(STR("child"))) == node.firstChild());
	CHECK(doc.findNode(find_predicate_prefix(STR("child2"))) == node.lastChild());
	CHECK(doc.findNode(find_predicate_prefix(STR("child3"))) == XmlNode());
}

#ifndef PUGIXML_NO_STL
TEST_XML(dom_node_path, "<node><child1>text<child2/></child1></node>")
{
	CHECK(XmlNode().path() == STR(""));

	CHECK(doc.path() == STR(""));
	CHECK(doc.child(STR("node")).path() == STR("/node"));
	CHECK(doc.child(STR("node")).child(STR("child1")).path() == STR("/node/child1"));
	CHECK(doc.child(STR("node")).child(STR("child1")).child(STR("child2")).path() == STR("/node/child1/child2"));
	CHECK(doc.child(STR("node")).child(STR("child1")).firstChild().path() == STR("/node/child1/"));

	CHECK(doc.child(STR("node")).child(STR("child1")).path('\\') == STR("\\node\\child1"));

	doc.appendChild(node_element);
	CHECK(doc.lastChild().path() == STR("/"));
}
#endif

TEST_XML(dom_node_firstElementByPath, "<node><child1>text<child2/></child1></node>")
{
	CHECK(XmlNode().firstElementByPath(STR("/")) == XmlNode());
	CHECK(XmlNode().firstElementByPath(STR("a")) == XmlNode());

	CHECK(doc.firstElementByPath(STR("")) == doc);
	CHECK(doc.firstElementByPath(STR("/")) == doc);

	CHECK(doc.firstElementByPath(STR("/node/")) == doc.child(STR("node")));
	CHECK(doc.firstElementByPath(STR("node/")) == doc.child(STR("node")));
	CHECK(doc.firstElementByPath(STR("node")) == doc.child(STR("node")));
	CHECK(doc.firstElementByPath(STR("/node")) == doc.child(STR("node")));

#ifndef PUGIXML_NO_STL
	CHECK(doc.firstElementByPath(STR("/node/child1/child2")).path() == STR("/node/child1/child2"));
#endif

	CHECK(doc.firstElementByPath(STR("/node/child2")) == XmlNode());

	CHECK(doc.firstElementByPath(STR("\\node\\child1"), '\\') == doc.child(STR("node")).child(STR("child1")));

	CHECK(doc.child(STR("node")).firstElementByPath(STR("..")) == doc);
	CHECK(doc.child(STR("node")).firstElementByPath(STR(".")) == doc.child(STR("node")));

	CHECK(doc.child(STR("node")).firstElementByPath(STR("../node/./child1/../.")) == doc.child(STR("node")));

	CHECK(doc.child(STR("node")).firstElementByPath(STR("child1")) == doc.child(STR("node")).child(STR("child1")));
	CHECK(doc.child(STR("node")).firstElementByPath(STR("child1/")) == doc.child(STR("node")).child(STR("child1")));
	CHECK(doc.child(STR("node")).firstElementByPath(STR("child")) == XmlNode());
	CHECK(doc.child(STR("node")).firstElementByPath(STR("child11")) == XmlNode());

	CHECK(doc.firstElementByPath(STR("//node")) == doc.child(STR("node")));
}

struct test_walker: XmlTreeWalker
{
	std::basic_string<char_t> log;
	unsigned int call_count;
	unsigned int stop_count;

	test_walker(unsigned int stop_count_ = 0): call_count(0), stop_count(stop_count_)
	{
	}

	std::basic_string<char_t> depthstr() const
	{
		char buf[32];

	#if __cplusplus >= 201103 || defined(__APPLE__) // Xcode 14 warns about use of sprintf in C++98 builds
		snprintf(buf, sizeof(buf), "%d", depth());
	#else
		sprintf(buf, "%d", depth());
	#endif

	#ifdef PUGIXML_WCHAR_MODE
		wchar_t wbuf[32];
		std::copy(buf, buf + strlen(buf) + 1, &wbuf[0]);

		return std::basic_string<char_t>(wbuf);
	#else
		return std::basic_string<char_t>(buf);
	#endif
	}

	bool begin(XmlNode& node) override
	{
		log += STR("|");
		log += depthstr();
		log += STR(" <");
		log += node.name();
		log += STR("=");
		log += node.value();

		return ++call_count != stop_count && XmlTreeWalker::begin(node);
	}

	bool for_each(XmlNode& node) override
	{
		log += STR("|");
		log += depthstr();
		log += STR(" !");
		log += node.name();
		log += STR("=");
		log += node.value();

		return ++call_count != stop_count && XmlTreeWalker::end(node);
	}

	bool end(XmlNode& node) override
	{
		log += STR("|");
		log += depthstr();
		log += STR(" >");
		log += node.name();
		log += STR("=");
		log += node.value();

		return ++call_count != stop_count;
	}
};

TEST_XML(dom_node_traverse, "<node><child>text</child></node>")
{
	test_walker walker;

	CHECK(doc.traverse(walker));

	CHECK(walker.call_count == 5);
	CHECK(walker.log == STR("|-1 <=|0 !node=|1 !child=|2 !=text|-1 >="));
}

TEST_XML(dom_node_traverse_siblings, "<node><child/><child>text</child><child/></node>")
{
	test_walker walker;

	CHECK(doc.traverse(walker));

	CHECK(walker.call_count == 7);
	CHECK(walker.log == STR("|-1 <=|0 !node=|1 !child=|1 !child=|2 !=text|1 !child=|-1 >="));
}

TEST(dom_node_traverse_empty)
{
	test_walker walker;

	CHECK(XmlNode().traverse(walker));

	CHECK(walker.call_count == 2);
	CHECK(walker.log == STR("|-1 <=|-1 >="));
}

TEST_XML(dom_node_traverse_child, "<node><child>text</child></node><another>node</another>")
{
	test_walker walker;

	CHECK(doc.child(STR("node")).traverse(walker));

	CHECK(walker.call_count == 4);
	CHECK(walker.log == STR("|-1 <node=|0 !child=|1 !=text|-1 >node="));
}

TEST_XML(dom_node_traverse_stop_begin, "<node><child>text</child></node>")
{
	test_walker walker(1);

	CHECK(!doc.traverse(walker));

	CHECK(walker.call_count == 1);
	CHECK(walker.log == STR("|-1 <="));
}

TEST_XML(dom_node_traverse_stop_for_each, "<node><child>text</child></node>")
{
	test_walker walker(3);

	CHECK(!doc.traverse(walker));

	CHECK(walker.call_count == 3);
	CHECK(walker.log == STR("|-1 <=|0 !node=|1 !child="));
}

TEST_XML(dom_node_traverse_stop_end, "<node><child>text</child></node>")
{
	test_walker walker(5);

	CHECK(!doc.traverse(walker));

	CHECK(walker.call_count == 5);
	CHECK(walker.log == STR("|-1 <=|0 !node=|1 !child=|2 !=text|-1 >="));
}

TEST_XML_FLAGS(dom_offset_debug, "<?xml?><!DOCTYPE><?pi?><!--comment--><node>pcdata<![CDATA[cdata]]></node>", parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype)
{
	CHECK(XmlNode().offset_debug() == -1);
	CHECK(doc.offset_debug() == 0);

	XmlNodeIterator it = doc.begin();

	CHECK((it++)->offset_debug() == 2);
	CHECK((it++)->offset_debug() == 16);
	CHECK((it++)->offset_debug() == 19);
	CHECK((it++)->offset_debug() == 27);
	CHECK((it++)->offset_debug() == 38);

	XmlNodeIterator cit = doc.child(STR("node")).begin();

	CHECK((cit++)->offset_debug() == 43);
	CHECK((cit++)->offset_debug() == 58);
}

TEST(dom_offset_debug_encoding)
{
	char buf[] = { 0, '<', 0, 'n', 0, '/', 0, '>' };

	XmlDocument doc;
	CHECK(doc.loadBuffer(buf, sizeof(buf)));

	CHECK(doc.offset_debug() == 0);
	CHECK(doc.firstChild().offset_debug() == 1);
}

TEST_XML(dom_offset_debug_append, "<node/>")
{
	XmlNode c1 = doc.firstChild();
	XmlNode c2 = doc.appendChild(STR("node"));
	XmlNode c3 = doc.appendChild(node_pcdata);

	CHECK(doc.offset_debug() == 0);
	CHECK(c1.offset_debug() == 1);
	CHECK(c2.offset_debug() == -1);
	CHECK(c3.offset_debug() == -1);

	c1.setName(STR("nodenode"));
	CHECK(c1.offset_debug() == -1);
}

TEST_XML(dom_offset_debug_appendBuffer, "<node/>")
{
	CHECK(doc.offset_debug() == 0);
	CHECK(doc.firstChild().offset_debug() == 1);

	CHECK(doc.appendBuffer("<node/>", 7));
	CHECK(doc.offset_debug() == -1);
	CHECK(doc.firstChild().offset_debug() == -1);
	CHECK(doc.lastChild().offset_debug() == -1);
}

TEST_XML(dom_internal_object, "<node attr='value'>value</node>")
{
	XmlNode node = doc.child(STR("node"));
	XmlAttribute attr = node.firstAttribute();
	XmlNode value = node.firstChild();

	CHECK(XmlNode().internal_object() == 0);
	CHECK(XmlAttribute().internal_object() == 0);

    CHECK(node.internal_object() != 0);
    CHECK(value.internal_object() != 0);
    CHECK(node.internal_object() != value.internal_object());

    CHECK(attr.internal_object() != 0);

    XmlNode node_copy = node;
    CHECK(node_copy.internal_object() == node.internal_object());

    XmlAttribute attr_copy = attr;
    CHECK(attr_copy.internal_object() == attr.internal_object());
}

TEST_XML(dom_hash_value, "<node attr='value'>value</node>")
{
	XmlNode node = doc.child(STR("node"));
	XmlAttribute attr = node.firstAttribute();
	XmlNode value = node.firstChild();

	CHECK(XmlNode().hash_value() == 0);
	CHECK(XmlAttribute().hash_value() == 0);

    CHECK(node.hash_value() != 0);
    CHECK(value.hash_value() != 0);
    CHECK(node.hash_value() != value.hash_value());

    CHECK(attr.hash_value() != 0);

    XmlNode node_copy = node;
    CHECK(node_copy.hash_value() == node.hash_value());

    XmlAttribute attr_copy = attr;
    CHECK(attr_copy.hash_value() == attr.hash_value());
}

TEST_XML(dom_node_named_iterator, "<node><node1><child/></node1><node2><child/><child/></node2><node3/><node4><child/><x/></node4></node>")
{
	XmlNode node1 = doc.child(STR("node")).child(STR("node1"));
	XmlNode node2 = doc.child(STR("node")).child(STR("node2"));
	XmlNode node3 = doc.child(STR("node")).child(STR("node3"));
	XmlNode node4 = doc.child(STR("node")).child(STR("node4"));

	CHECK(XmlNamedNodeIterator(XmlNode(), STR("child")) == XmlNamedNodeIterator());

	XmlObjectRange<XmlNamedNodeIterator> r1 = node1.children(STR("child"));
	XmlObjectRange<XmlNamedNodeIterator> r2 = node2.children(STR("child"));
	XmlObjectRange<XmlNamedNodeIterator> r3 = node3.children(STR("child"));
	XmlObjectRange<XmlNamedNodeIterator> r4 = node4.children(STR("child"));

	CHECK(r1.begin() != r1.end());
	CHECK(*r1.begin() == node1.firstChild());
	CHECK(r1.begin() == move_iter(r1.end(), -1));
	CHECK(move_iter(r1.begin(), 1) == r1.end());

	CHECK(r2.begin() != r2.end());
	CHECK(*r2.begin() == node2.firstChild());
	CHECK(*move_iter(r2.begin(), 1) == node2.lastChild());
	CHECK(r2.begin() == move_iter(r2.end(), -2));
	CHECK(move_iter(r2.begin(), 1) == move_iter(r2.end(), -1));
	CHECK(move_iter(r2.begin(), 2) == r2.end());

	CHECK(r3.begin() == r3.end());
	CHECK(!(r3.begin() != r3.end()));

	CHECK(r4.begin() != r4.end());
	CHECK(*r4.begin() == node4.firstChild());
	CHECK(r4.begin() == move_iter(r4.end(), -1));
	CHECK(move_iter(r4.begin(), 1) == r4.end());

	XmlNamedNodeIterator it = r1.begin();
	XmlNamedNodeIterator itt = it;

	CHECK(itt == it);

	CHECK(itt++ == it);
	CHECK(itt == r1.end());

	CHECK(itt != it);
	CHECK(itt == ++it);

	CHECK(itt-- == r1.end());
	CHECK(itt == r1.begin());

	CHECK(itt->offset_debug() == 14);
}

TEST_XML(dom_node_children_attributes, "<node1 attr1='value1' attr2='value2' /><node2 />")
{
	XmlObjectRange<XmlNodeIterator> r1 = doc.children();

	CHECK(r1.begin() == doc.begin());
	CHECK(r1.end() == doc.end());

	XmlObjectRange<XmlNodeIterator> r2 = XmlNode().children();

	CHECK(r2.begin() == XmlNodeIterator());
	CHECK(r2.end() == XmlNodeIterator());

	XmlNode node = doc.child(STR("node1"));

	XmlObjectRange<XmlAttributeIterator> r3 = node.attributes();

	CHECK(r3.begin() == node.attributes_begin());
	CHECK(r3.end() == node.attributes_end());

	XmlObjectRange<XmlAttributeIterator> r4 = XmlNode().attributes();

	CHECK(r4.begin() == XmlAttributeIterator());
	CHECK(r4.end() == XmlAttributeIterator());
}



#if __cplusplus >= 201103
TEST_XML(dom_ranged_for, "<node attr1='1' attr2='2'><test>3</test><fake>5</fake><test>4</test></node>")
{
	int index = 1;

	for (XmlNode n: doc.children())
	{
		for (XmlAttribute a: n.attributes())
		{
			CHECK(a.toInt() == index);
			index++;
		}

		for (XmlNode c: n.children(STR("test")))
		{
			CHECK(c.text().toInt() == index);
			index++;
		}
	}

	CHECK(index == 5);
}
#endif

TEST_XML(dom_node_attribute_hinted, "<node attr1='1' attr2='2' attr3='3' />")
{
	XmlNode node = doc.firstChild();
	XmlAttribute attr1 = node.attribute(STR("attr1"));
	XmlAttribute attr2 = node.attribute(STR("attr2"));
	XmlAttribute attr3 = node.attribute(STR("attr3"));

	XmlAttribute hint;
	CHECK(!XmlNode().attribute(STR("test"), hint) && !hint);

	CHECK(node.attribute(STR("attr2"), hint) == attr2 && hint == attr3);
	CHECK(node.attribute(STR("attr3"), hint) == attr3 && !hint);

	CHECK(node.attribute(STR("attr1"), hint) == attr1 && hint == attr2);
	CHECK(node.attribute(STR("attr2"), hint) == attr2 && hint == attr3);
	CHECK(node.attribute(STR("attr1"), hint) == attr1 && hint == attr2);
	CHECK(node.attribute(STR("attr1"), hint) == attr1 && hint == attr2);

	CHECK(!node.attribute(STR("attr"), hint) && hint == attr2);
}


TEST_XML(dom_node_attribute_hinted_stringview, "<node attr1='1' attr2='2' attr3='3' />")
{
	XmlNode node = doc.firstChild();
	string_view_t a1name = string_view_t(STR("attr1"));
	string_view_t a2name = string_view_t(STR("attr2"));
	string_view_t a3name = string_view_t(STR("attr3"));
	XmlAttribute attr1 = node.attribute(a1name);
	XmlAttribute attr2 = node.attribute(a2name);
	XmlAttribute attr3 = node.attribute(a3name);

	XmlAttribute hint;
	CHECK(!XmlNode().attribute(string_view_t(string_view_t(STR("test"))), hint) && !hint);

	CHECK(node.attribute(a2name, hint) == attr2 && hint == attr3);
	CHECK(node.attribute(a3name, hint) == attr3 && !hint);

	CHECK(node.attribute(a1name, hint) == attr1 && hint == attr2);
	CHECK(node.attribute(a2name, hint) == attr2 && hint == attr3);
	CHECK(node.attribute(a1name, hint) == attr1 && hint == attr2);
	CHECK(node.attribute(a1name, hint) == attr1 && hint == attr2);

	CHECK(!node.attribute(string_view_t(), hint) && hint == attr2);
	CHECK(!node.attribute(string_view_t(STR("attr1"), 4), hint) && hint == attr2); // "attr"
	CHECK(node.attribute(string_view_t(STR("attr3_extra"), 5), hint) == attr3 && !hint); // "attr3"
}

TEST_XML(dom_node_attribute_hint_interior_null, "<node attr1='1' attr2='2' attr3='3' />")
{
	XmlNode node = doc.firstChild();

	XmlAttribute attr1 = node.attribute(STR("attr1"));
	XmlAttribute attr2 = node.attribute(STR("attr2"));
	XmlAttribute attr3 = node.attribute(STR("attr3"));

	CHECK(node && attr1 && attr2 && attr3);

	const char_t name[] = STR("attr2\0extra");
	size_t len = (sizeof(name) / sizeof(char_t)) - 1;
	CHECK(len == 11);

	XmlAttribute hint;
	CHECK(node.attribute(string_view_t(name, 5), hint) == attr2 && hint == attr3); // "attr2"
	CHECK(node.attribute(string_view_t(name, 5), hint) == attr2 && hint == attr3); // "attr2"

	CHECK(!node.attribute(string_view_t(name, 6), hint) && hint == attr3); // "attr2\0"
	CHECK(!node.attribute(string_view_t(name, len), hint) && hint == attr3); // "attr2\0extra"

	attr2.setName(string_view_t(name, len)); // attr2\0extra

	CHECK(node.attribute(string_view_t(name, 5), hint) == attr2 && hint == attr3); // "attr2"
	CHECK(node.attribute(string_view_t(name, 5), hint) == attr2 && hint == attr3); // "attr2"

	CHECK(!node.attribute(string_view_t(name, 6), hint) && hint == attr3); // "attr2\0"
	CHECK(!node.attribute(string_view_t(name, len), hint) && hint == attr3); // "attr2\0extra"
}


TEST_XML(dom_toInt_overflow, "<node attr1='-2147483649' attr2='2147483648' attr3='-4294967296' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == -2147483647 - 1);
	CHECK(node.attribute(STR("attr2")).toInt() == 2147483647);
	CHECK(node.attribute(STR("attr3")).toInt() == -2147483647 - 1);
}

TEST_XML(dom_toUInt_overflow, "<node attr1='-1' attr2='4294967296' attr3='5294967295' attr4='21474836479' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toUInt() == 0);
	CHECK(node.attribute(STR("attr2")).toUInt() == 4294967295u);
	CHECK(node.attribute(STR("attr3")).toUInt() == 4294967295u);
	CHECK(node.attribute(STR("attr4")).toUInt() == 4294967295u);
}

TEST_XML(dom_toInt_hex_overflow, "<node attr1='-0x80000001' attr2='0x80000000' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == -2147483647 - 1);
	CHECK(node.attribute(STR("attr2")).toInt() == 2147483647);
}

TEST_XML(dom_toUInt_hex_overflow, "<node attr1='-0x1' attr2='0x100000000' attr3='0x123456789' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toUInt() == 0);
	CHECK(node.attribute(STR("attr2")).toUInt() == 4294967295u);
	CHECK(node.attribute(STR("attr3")).toUInt() == 4294967295u);
}

TEST_XML(dom_toInt_many_digits, "<node attr1='0000000000000000000000000000000000000000000000001' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == 1);
	CHECK(node.attribute(STR("attr1")).toUInt() == 1);
}

TEST_XML(dom_toInt_hex_many_digits, "<node attr1='0x0000000000000000000000000000000000000000000000001' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == 1);
	CHECK(node.attribute(STR("attr1")).toUInt() == 1);
}


TEST_XML(dom_toLongLong_overflow, "<node attr1='-9223372036854775809' attr2='9223372036854775808' attr3='-18446744073709551616' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toLongLong() == -9223372036854775807ll - 1);
	CHECK(node.attribute(STR("attr2")).toLongLong() == 9223372036854775807ll);
	CHECK(node.attribute(STR("attr3")).toLongLong() == -9223372036854775807ll - 1);
}

TEST_XML(dom_toULongLong_overflow, "<node attr1='-1' attr2='18446744073709551616' attr3='28446744073709551615' attr4='166020696663385964543' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toULongLong() == 0);
	CHECK(node.attribute(STR("attr2")).toULongLong() == 18446744073709551615ull);
	CHECK(node.attribute(STR("attr3")).toULongLong() == 18446744073709551615ull);
	CHECK(node.attribute(STR("attr4")).toULongLong() == 18446744073709551615ull);
}

TEST_XML(dom_toLongLong_hex_overflow, "<node attr1='-0x8000000000000001' attr2='0x8000000000000000' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toLongLong() == -9223372036854775807ll - 1);
	CHECK(node.attribute(STR("attr2")).toLongLong() == 9223372036854775807ll);
}

TEST_XML(dom_toULongLong_hex_overflow, "<node attr1='-0x1' attr2='0x10000000000000000' attr3='0x12345678923456789' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toULongLong() == 0);
	CHECK(node.attribute(STR("attr2")).toULongLong() == 18446744073709551615ull);
	CHECK(node.attribute(STR("attr3")).toULongLong() == 18446744073709551615ull);
}

TEST_XML(dom_toLongLong_many_digits, "<node attr1='0000000000000000000000000000000000000000000000001' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toLongLong() == 1);
	CHECK(node.attribute(STR("attr1")).toULongLong() == 1);
}

TEST_XML(dom_toLongLong_hex_many_digits, "<node attr1='0x0000000000000000000000000000000000000000000000001' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toLongLong() == 1);
	CHECK(node.attribute(STR("attr1")).toULongLong() == 1);
}


TEST_XML(dom_toInt_plus, "<node attr1='+1' attr2='+0xa' />")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.attribute(STR("attr1")).toInt() == 1);
	CHECK(node.attribute(STR("attr1")).toUInt() == 1);
	CHECK(node.attribute(STR("attr2")).toInt() == 10);
	CHECK(node.attribute(STR("attr2")).toUInt() == 10);


	CHECK(node.attribute(STR("attr1")).toLongLong() == 1);
	CHECK(node.attribute(STR("attr1")).toULongLong() == 1);
	CHECK(node.attribute(STR("attr2")).toLongLong() == 10);
	CHECK(node.attribute(STR("attr2")).toULongLong() == 10);

}

TEST(dom_node_anonymous)
{
	XmlDocument doc;
	doc.appendChild(node_element);
	doc.appendChild(node_element);
	doc.appendChild(node_pcdata);

	CHECK(doc.child(STR("node")) == XmlNode());
	CHECK(doc.firstChild().nextSibling(STR("node")) == XmlNode());
	CHECK(doc.lastChild().previousSibling(STR("node")) == XmlNode());
	CHECK_STRING(doc.childValue(), STR(""));
	CHECK_STRING(doc.lastChild().childValue(), STR(""));
}

TEST_XML(dom_node_anonymous_child, "<node></node>")
{
	XmlNode node = doc.child(STR("node"));
	CHECK_NAME_VALUE(node, STR("node"), STR(""));
	node.setName(STR(""));
	CHECK_NODE(doc, STR("<:anonymous/>"));
	CHECK(doc.firstChild() != XmlNode());
	CHECK_NAME_VALUE(doc.firstChild(), STR(""), STR(""));

	// searching for empty string does not find a node with empty name
	CHECK(doc.child(STR("")) == XmlNode());

	CHECK(doc.child(string_view_t()) == XmlNode());
	CHECK(doc.child(string_view_t(STR("hi"), 0)) == XmlNode());

}

TEST_XML(dom_node_anonymous_attribute, "<node attr='0'/>")
{
	XmlAttribute attr = doc.firstChild().attribute(STR("attr"));
	CHECK(attr != XmlAttribute());
	attr.setName(STR(""));
	CHECK_NODE(doc, STR("<node :anonymous=\"0\"/>"));
	CHECK_NAME_VALUE(doc.firstChild().firstAttribute(), STR(""), STR("0"));

	CHECK(doc.firstChild().attribute(STR("")) == XmlAttribute());

	CHECK(doc.firstChild().attribute(string_view_t()) == XmlAttribute());
	CHECK(doc.firstChild().attribute(string_view_t(STR("hi"), 0)) == XmlAttribute());

}
