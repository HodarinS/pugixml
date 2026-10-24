#include "test.hpp"

#include "helpers.hpp"

#include <limits.h>

using namespace pugi;

TEST_XML_FLAGS(dom_text_empty, "<node><a>foo</a><b><![CDATA[bar]]></b><c><?pi value?></c><d/></node>", parse_default | parse_pi)
{
    XmlNode node = doc.child(STR("node"));

    CHECK(node.child(STR("a")).text());
    CHECK(node.child(STR("b")).text());
    CHECK(!node.child(STR("c")).text());
    CHECK(!node.child(STR("d")).text());
    CHECK(!XmlNode().text());
    CHECK(!XmlText());

    generic_empty_test(node.child(STR("a")).text());
}

TEST_XML(dom_text_bool_ops, "<node>foo</node>")
{
    generic_bool_ops_test(doc.child(STR("node")).text());
}

TEST_XML_FLAGS(dom_text_get, "<node><a>foo</a><b><node/><![CDATA[bar]]></b><c><?pi value?></c><d/></node>", parse_default | parse_pi)
{
    XmlNode node = doc.child(STR("node"));

    CHECK_STRING(node.child(STR("a")).text().value(), STR("foo"));
    CHECK_STRING(node.child(STR("a")).firstChild().text().value(), STR("foo"));

    CHECK_STRING(node.child(STR("b")).text().value(), STR("bar"));
    CHECK_STRING(node.child(STR("b")).lastChild().text().value(), STR("bar"));

    CHECK_STRING(node.child(STR("c")).text().value(), STR(""));
    CHECK_STRING(node.child(STR("c")).firstChild().text().value(), STR(""));

    CHECK_STRING(node.child(STR("d")).text().value(), STR(""));

    CHECK_STRING(XmlNode().text().value(), STR(""));
}

TEST_XML_FLAGS(dom_text_as_string, "<node><a>foo</a><b><node/><![CDATA[bar]]></b><c><?pi value?></c><d/></node>", parse_default | parse_pi)
{
    XmlNode node = doc.child(STR("node"));

    CHECK_STRING(node.child(STR("a")).text().value(), STR("foo"));
    CHECK_STRING(node.child(STR("a")).firstChild().text().value(), STR("foo"));

    CHECK_STRING(node.child(STR("b")).text().value(), STR("bar"));
    CHECK_STRING(node.child(STR("b")).lastChild().text().value(), STR("bar"));

    CHECK_STRING(node.child(STR("c")).text().value(), STR(""));
    CHECK_STRING(node.child(STR("c")).firstChild().text().value(), STR(""));

    CHECK_STRING(node.child(STR("d")).text().value(), STR(""));

    CHECK_STRING(XmlNode().text().value(), STR(""));
}

TEST_XML(dom_text_toInt, "<node><text1>1</text1><text2>-1</text2><text3>-2147483648</text3><text4>2147483647</text4><text5>0</text5></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlText().toInt() == 0);
	CHECK(node.child(STR("text1")).text().toInt() == 1);
	CHECK(node.child(STR("text2")).text().toInt() == -1);
	CHECK(node.child(STR("text3")).text().toInt() == -2147483647 - 1);
	CHECK(node.child(STR("text4")).text().toInt() == 2147483647);
    CHECK(node.child(STR("text5")).text().toInt() == 0);
}

TEST_XML(dom_text_toInt_hex, "<node><text1>0777</text1><text2>0x5ab</text2><text3>0XFf</text3><text4>-0x20</text4><text5>-0x80000000</text5><text6>0x</text6></node>")
{
    XmlNode node = doc.child(STR("node"));

    CHECK(node.child(STR("text1")).text().toInt() == 777); // no octal support! intentional
    CHECK(node.child(STR("text2")).text().toInt() == 1451);
    CHECK(node.child(STR("text3")).text().toInt() == 255);
    CHECK(node.child(STR("text4")).text().toInt() == -32);
    CHECK(node.child(STR("text5")).text().toInt() == -2147483647 - 1);
    CHECK(node.child(STR("text6")).text().toInt() == 0);
}

TEST_XML(dom_text_toUInt, "<node><text1>0</text1><text2>1</text2><text3>2147483647</text3><text4>4294967295</text4><text5>0</text5></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlText().toUInt() == 0);
	CHECK(node.child(STR("text1")).text().toUInt() == 0);
	CHECK(node.child(STR("text2")).text().toUInt() == 1);
	CHECK(node.child(STR("text3")).text().toUInt() == 2147483647);
	CHECK(node.child(STR("text4")).text().toUInt() == 4294967295u);
    CHECK(node.child(STR("text5")).text().toUInt() == 0);
}

TEST_XML(dom_text_toUInt_hex, "<node><text1>0777</text1><text2>0x5ab</text2><text3>0XFf</text3><text4>0x20</text4><text5>0xFFFFFFFF</text5><text6>0x</text6></node>")
{
    XmlNode node = doc.child(STR("node"));

    CHECK(node.child(STR("text1")).text().toUInt() == 777); // no octal support! intentional
    CHECK(node.child(STR("text2")).text().toUInt() == 1451);
    CHECK(node.child(STR("text3")).text().toUInt() == 255);
    CHECK(node.child(STR("text4")).text().toUInt() == 32);
    CHECK(node.child(STR("text5")).text().toUInt() == 4294967295u);
    CHECK(node.child(STR("text6")).text().toUInt() == 0);
}

TEST_XML(dom_text_toInteger_space, "<node><text1> \t\n1234</text1><text2>\n\t 0x123</text2><text3>- 16</text3><text4>- 0x10</text4></node>")
{
    XmlNode node = doc.child(STR("node"));

    CHECK(node.child(STR("text1")).text().toInt() == 1234);
    CHECK(node.child(STR("text2")).text().toInt() == 291);
    CHECK(node.child(STR("text3")).text().toInt() == 0);
    CHECK(node.child(STR("text4")).text().toInt() == 0);
}

TEST_XML(dom_text_toFloat, "<node><text1>0</text1><text2>1</text2><text3>0.12</text3><text4>-5.1</text4><text5>3e-4</text5><text6>3.14159265358979323846</text6><text7> \t\n+2.5</text7></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlText().toFloat() == 0);
	CHECK_DOUBLE(double(node.child(STR("text1")).text().toFloat()), 0);
	CHECK_DOUBLE(double(node.child(STR("text2")).text().toFloat()), 1);
	CHECK_DOUBLE(double(node.child(STR("text3")).text().toFloat()), 0.12);
	CHECK_DOUBLE(double(node.child(STR("text4")).text().toFloat()), -5.1);
	CHECK_DOUBLE(double(node.child(STR("text5")).text().toFloat()), 3e-4);
	CHECK_DOUBLE(double(node.child(STR("text6")).text().toFloat()), 3.14159265358979323846);
	CHECK_DOUBLE(double(node.child(STR("text7")).text().toFloat()), 2.5);
}

TEST_XML(dom_text_toDouble, "<node><text1>0</text1><text2>1</text2><text3>0.12</text3><text4>-5.1</text4><text5>3e-4</text5><text6>3.14159265358979323846</text6><text7> \t\n+2.5</text7></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlText().toDouble() == 0);
	CHECK_DOUBLE(node.child(STR("text1")).text().toDouble(), 0);
	CHECK_DOUBLE(node.child(STR("text2")).text().toDouble(), 1);
	CHECK_DOUBLE(node.child(STR("text3")).text().toDouble(), 0.12);
	CHECK_DOUBLE(node.child(STR("text4")).text().toDouble(), -5.1);
	CHECK_DOUBLE(node.child(STR("text5")).text().toDouble(), 3e-4);
	CHECK_DOUBLE(node.child(STR("text6")).text().toDouble(), 3.14159265358979323846);
	CHECK_DOUBLE(node.child(STR("text7")).text().toDouble(), 2.5);
}

TEST_XML(dom_text_toBool, "<node><text1>0</text1><text2>1</text2><text3>true</text3><text4>True</text4><text5>Yes</text5><text6>yes</text6><text7>false</text7></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(!XmlText().toBool());
	CHECK(!node.child(STR("text1")).text().toBool());
	CHECK(node.child(STR("text2")).text().toBool());
	CHECK(node.child(STR("text3")).text().toBool());
	CHECK(node.child(STR("text4")).text().toBool());
	CHECK(node.child(STR("text5")).text().toBool());
	CHECK(node.child(STR("text6")).text().toBool());
	CHECK(!node.child(STR("text7")).text().toBool());

	XmlNode text = node.appendChild(node_pcdata);
	CHECK(text.text().toBool(true) == true);
}


TEST_XML(dom_text_toLongLong, "<node><text1>1</text1><text2>-1</text2><text3>-9223372036854775808</text3><text4>9223372036854775807</text4><text5>0</text5></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlText().toLongLong() == 0);
	CHECK(node.child(STR("text1")).text().toLongLong() == 1);
	CHECK(node.child(STR("text2")).text().toLongLong() == -1);
	CHECK(node.child(STR("text3")).text().toLongLong() == -9223372036854775807ll - 1);
	CHECK(node.child(STR("text4")).text().toLongLong() == 9223372036854775807ll);
	CHECK(node.child(STR("text5")).text().toLongLong() == 0);
}

TEST_XML(dom_text_toLongLong_hex, "<node><text1>0777</text1><text2>0x5ab</text2><text3>0XFf</text3><text4>-0x20</text4><text5>-0x8000000000000000</text5><text6>0x</text6></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.child(STR("text1")).text().toLongLong() == 777); // no octal support! intentional
	CHECK(node.child(STR("text2")).text().toLongLong() == 1451);
	CHECK(node.child(STR("text3")).text().toLongLong() == 255);
	CHECK(node.child(STR("text4")).text().toLongLong() == -32);
	CHECK(node.child(STR("text5")).text().toLongLong() == -9223372036854775807ll - 1);
	CHECK(node.child(STR("text6")).text().toLongLong() == 0);
}

TEST_XML(dom_text_toULongLong, "<node><text1>0</text1><text2>1</text2><text3>9223372036854775807</text3><text4>18446744073709551615</text4><text5>0</text5></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(XmlText().toULongLong() == 0);
	CHECK(node.child(STR("text1")).text().toULongLong() == 0);
	CHECK(node.child(STR("text2")).text().toULongLong() == 1);
	CHECK(node.child(STR("text3")).text().toULongLong() == 9223372036854775807ll);
	CHECK(node.child(STR("text4")).text().toULongLong() == 18446744073709551615ull);
	CHECK(node.child(STR("text5")).text().toULongLong() == 0);
}

TEST_XML(dom_text_toULongLong_hex, "<node><text1>0777</text1><text2>0x5ab</text2><text3>0XFf</text3><text4>0x20</text4><text5>0xFFFFFFFFFFFFFFFF</text5><text6>0x</text6></node>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.child(STR("text1")).text().toULongLong() == 777); // no octal support! intentional
	CHECK(node.child(STR("text2")).text().toULongLong() == 1451);
	CHECK(node.child(STR("text3")).text().toULongLong() == 255);
	CHECK(node.child(STR("text4")).text().toULongLong() == 32);
	CHECK(node.child(STR("text5")).text().toULongLong() == 18446744073709551615ull);
	CHECK(node.child(STR("text6")).text().toULongLong() == 0);
}

TEST_XML(dom_text_get_no_state, "<node/>")
{
    XmlNode node = doc.child(STR("node"));
    XmlText t = node.text();

    CHECK(!t);
    CHECK(t.value() && *t.value() == 0);
    CHECK(!node.firstChild());

    node.appendChild(node_pcdata);

    CHECK(t);
    CHECK_STRING(t.value(), STR(""));

    node.firstChild().setValue(STR("test"));

    CHECK(t);
    CHECK_STRING(t.value(), STR("test"));
}

TEST_XML(dom_text_set, "<node/>")
{
    XmlNode node = doc.child(STR("node"));
    XmlText t = node.text();

    t.set(STR(""));
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK_NODE(node, STR("<node/>"));

    t.set(STR("boo"));
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK(node.firstChild() == node.lastChild());
    CHECK_NODE(node, STR("<node>boo</node>"));

    t.set(STR("foobarfoobar"));
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK(node.firstChild() == node.lastChild());
    CHECK_NODE(node, STR("<node>foobarfoobar</node>"));

    t.set(STR(""));
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK_NODE(node, STR("<node/>"));
}

TEST_XML(dom_text_set_with_size, "<node/>")
{
    XmlNode node = doc.child(STR("node"));
    XmlText t = node.text();

    t.set(STR(""), 0);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK_NODE(node, STR("<node/>"));

    t.set(STR("boo"), 3);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK(node.firstChild() == node.lastChild());
    CHECK_NODE(node, STR("<node>boo</node>"));

    t.set(STR("foobarfoobar"), 12);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK(node.firstChild() == node.lastChild());
    CHECK_NODE(node, STR("<node>foobarfoobar</node>"));

    t.set(STR(""), 0);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK_NODE(node, STR("<node/>"));
}

TEST_XML(dom_text_set_partially_with_size, "<node/>")
{
    XmlNode node = doc.child(STR("node"));
    XmlText t = node.text();

    t.set(STR("foo"), 0);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK_NODE(node, STR("<node/>"));

    t.set(STR("boofoo"), 3);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK(node.firstChild() == node.lastChild());
    CHECK_NODE(node, STR("<node>boo</node>"));

    t.set(STR("foobarfoobar"), 3);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK(node.firstChild() == node.lastChild());
    CHECK_NODE(node, STR("<node>foo</node>"));

    t.set(STR("foo"), 0);
    CHECK(node.firstChild().type() == node_pcdata);
    CHECK_NODE(node, STR("<node/>"));
}


TEST_XML(dom_text_set_with_string_view, "<node/>")
{
	XmlNode node = doc.child(STR("node"));
	XmlText t = node.text();

	t.set(string_view_t(STR("")));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node/>"));

	t.set(string_view_t(STR("boo")));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK(node.firstChild() == node.lastChild());
	CHECK_NODE(node, STR("<node>boo</node>"));

	t.set(string_view_t(STR("foobarfoobar")));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK(node.firstChild() == node.lastChild());
	CHECK_NODE(node, STR("<node>foobarfoobar</node>"));

	t.set(string_view_t(STR("")));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node/>"));

	t.set(string_view_t(STR("something")));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node>something</node>"));

	// empty string view (null data pointer)
	t.set(string_view_t());
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node/>"));

	t.set(string_view_t(STR("afternulldata")));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node>afternulldata</node>"));
}

TEST_XML(dom_text_set_partially_with_string_view, "<node/>")
{
	XmlNode node = doc.child(STR("node"));
	XmlText t = node.text();

	t.set(string_view_t(STR("foo"), 0));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node/>"));

	t.set(string_view_t(STR("boofoo"), 3));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK(node.firstChild() == node.lastChild());
	CHECK_NODE(node, STR("<node>boo</node>"));

	t.set(string_view_t(STR("foobarfoobar"), 3));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK(node.firstChild() == node.lastChild());
	CHECK_NODE(node, STR("<node>foo</node>"));

	t.set(string_view_t(STR("foo"), 0));
	CHECK(node.firstChild().type() == node_pcdata);
	CHECK_NODE(node, STR("<node/>"));
}

TEST_XML(dom_text_assign, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	node.appendChild(STR("text1")).text() = STR("v1");
	XmlText() = STR("v1");

	node.appendChild(STR("text2")).text() = -2147483647;
	node.appendChild(STR("text3")).text() = -2147483647 - 1;
	XmlText() = -2147483647 - 1;

	node.appendChild(STR("text4")).text() = 4294967295u;
	node.appendChild(STR("text5")).text() = 4294967294u;
	XmlText() = 4294967295u;

	node.appendChild(STR("text6")).text() = 0.5;
	XmlText() = 0.5;

	node.appendChild(STR("text7")).text() = 0.25f;
	XmlText() = 0.25f;

	node.appendChild(STR("text8")).text() = true;
	XmlText() = true;


	node.appendChild(string_view_t(STR("text9"))).text() = string_view_t(STR("v2"));
	XmlText() = string_view_t(STR("text9"));

	CHECK_NODE(node, STR("<node><text1>v1</text1><text2>-2147483647</text2><text3>-2147483648</text3><text4>4294967295</text4><text5>4294967294</text5><text6>0.5</text6><text7>0.25</text7><text8>true</text8><text9>v2</text9></node>"));
}

TEST_XML(dom_text_setValue, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.appendChild(STR("text1")).text().set(STR("v1")));
	CHECK(!XmlText().set(STR("v1")));
	CHECK(!XmlText().set(STR("v1"), 2));

	CHECK(!XmlText().set(string_view_t(STR("v1"))));

	CHECK(node.appendChild(STR("text2")).text().set(-2147483647));
	CHECK(node.appendChild(STR("text3")).text().set(-2147483647 - 1));
	CHECK(!XmlText().set(-2147483647 - 1));

	CHECK(node.appendChild(STR("text4")).text().set(4294967295u));
	CHECK(node.appendChild(STR("text5")).text().set(4294967294u));
	CHECK(!XmlText().set(4294967295u));

	CHECK(node.appendChild(STR("text6")).text().set(0.5));
	CHECK(!XmlText().set(0.5));

	CHECK(node.appendChild(STR("text7")).text().set(0.25f));
	CHECK(!XmlText().set(0.25f));

	CHECK(node.appendChild(STR("text8")).text().set(true));
	CHECK(!XmlText().set(true));

	CHECK_NODE(node, STR("<node><text1>v1</text1><text2>-2147483647</text2><text3>-2147483648</text3><text4>4294967295</text4><text5>4294967294</text5><text6>0.5</text6><text7>0.25</text7><text8>true</text8></node>"));
}

#if LONG_MAX > 2147483647
TEST_XML(dom_text_assign_long, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	node.appendChild(STR("text1")).text() = -9223372036854775807l;
	node.appendChild(STR("text2")).text() = -9223372036854775807l - 1;
	XmlText() = -9223372036854775807l - 1;

	node.appendChild(STR("text3")).text() = 18446744073709551615ul;
	node.appendChild(STR("text4")).text() = 18446744073709551614ul;
	XmlText() = 18446744073709551615ul;

	CHECK_NODE(node, STR("<node><text1>-9223372036854775807</text1><text2>-9223372036854775808</text2><text3>18446744073709551615</text3><text4>18446744073709551614</text4></node>"));
}

TEST_XML(dom_text_setValue_long, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.appendChild(STR("text1")).text().set(-9223372036854775807l));
	CHECK(node.appendChild(STR("text2")).text().set(-9223372036854775807l - 1));
	CHECK(!XmlText().set(-9223372036854775807l - 1));

	CHECK(node.appendChild(STR("text3")).text().set(18446744073709551615ul));
	CHECK(node.appendChild(STR("text4")).text().set(18446744073709551614ul));
	CHECK(!XmlText().set(18446744073709551615ul));

	CHECK_NODE(node, STR("<node><text1>-9223372036854775807</text1><text2>-9223372036854775808</text2><text3>18446744073709551615</text3><text4>18446744073709551614</text4></node>"));
}
#else
TEST_XML(dom_text_assign_long, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	node.appendChild(STR("text1")).text() = -2147483647l;
	node.appendChild(STR("text2")).text() = -2147483647l - 1;
	XmlText() = -2147483647l - 1;

	node.appendChild(STR("text3")).text() = 4294967295ul;
	node.appendChild(STR("text4")).text() = 4294967294ul;
	XmlText() = 4294967295ul;

	CHECK_NODE(node, STR("<node><text1>-2147483647</text1><text2>-2147483648</text2><text3>4294967295</text3><text4>4294967294</text4></node>"));
}

TEST_XML(dom_text_setValue_long, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.appendChild(STR("text1")).text().set(-2147483647l));
	CHECK(node.appendChild(STR("text2")).text().set(-2147483647l - 1));
	CHECK(!XmlText().set(-2147483647l - 1));

	CHECK(node.appendChild(STR("text3")).text().set(4294967295ul));
	CHECK(node.appendChild(STR("text4")).text().set(4294967294ul));
	CHECK(!XmlText().set(4294967295ul));

	CHECK_NODE(node, STR("<node><text1>-2147483647</text1><text2>-2147483648</text2><text3>4294967295</text3><text4>4294967294</text4></node>"));
}
#endif


TEST_XML(dom_text_assign_llong, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	node.appendChild(STR("text1")).text() = -9223372036854775807ll;
	node.appendChild(STR("text2")).text() = -9223372036854775807ll - 1;
	XmlText() = -9223372036854775807ll - 1;

	node.appendChild(STR("text3")).text() = 18446744073709551615ull;
	node.appendChild(STR("text4")).text() = 18446744073709551614ull;
	XmlText() = 18446744073709551615ull;

	CHECK_NODE(node, STR("<node><text1>-9223372036854775807</text1><text2>-9223372036854775808</text2><text3>18446744073709551615</text3><text4>18446744073709551614</text4></node>"));
}

TEST_XML(dom_text_setValue_llong, "<node/>")
{
	XmlNode node = doc.child(STR("node"));

	CHECK(node.appendChild(STR("text1")).text().set(-9223372036854775807ll));
	CHECK(node.appendChild(STR("text2")).text().set(-9223372036854775807ll - 1));
	CHECK(!XmlText().set(-9223372036854775807ll - 1));

	CHECK(node.appendChild(STR("text3")).text().set(18446744073709551615ull));
	CHECK(node.appendChild(STR("text4")).text().set(18446744073709551614ull));
	CHECK(!XmlText().set(18446744073709551615ull));

	CHECK_NODE(node, STR("<node><text1>-9223372036854775807</text1><text2>-9223372036854775808</text2><text3>18446744073709551615</text3><text4>18446744073709551614</text4></node>"));
}

TEST_XML(dom_text_middle, "<node><c1>notthisone</c1>text<c2/></node>")
{
    XmlNode node = doc.child(STR("node"));
    XmlText t = node.text();

    CHECK_STRING(t.value(), STR("text"));
    t.set(STR("notext"));

    CHECK_NODE(node, STR("<node><c1>notthisone</c1>notext<c2/></node>"));
    CHECK(node.removeChild(t.data()));

    CHECK(!t);
    CHECK_NODE(node, STR("<node><c1>notthisone</c1><c2/></node>"));

    t.set(STR("yestext"));

    CHECK(t);
    CHECK_NODE(node, STR("<node><c1>notthisone</c1><c2/>yestext</node>"));
    CHECK(t.data() == node.lastChild());
}


TEST_XML(dom_text_middle_string_view, "<node><c1>notthisone</c1>text<c2/></node>")
{
	XmlNode node = doc.child(STR("node"));
	XmlText t = node.text();

	CHECK_STRING(t.value(), STR("text"));
	t.set(string_view_t(STR("notext")));

	CHECK_NODE(node, STR("<node><c1>notthisone</c1>notext<c2/></node>"));
	CHECK(node.removeChild(t.data()));

	CHECK(!t);
	CHECK_NODE(node, STR("<node><c1>notthisone</c1><c2/></node>"));

	t.set(string_view_t(STR("yestext")));

	CHECK(t);
	CHECK_NODE(node, STR("<node><c1>notthisone</c1><c2/>yestext</node>"));
	CHECK(t.data() == node.lastChild());
}

TEST_XML_FLAGS(dom_text_data, "<node><a>foo</a><b><![CDATA[bar]]></b><c><?pi value?></c><d/></node>", parse_default | parse_pi)
{
    XmlNode node = doc.child(STR("node"));

    CHECK(node.child(STR("a")).text().data() == node.child(STR("a")).firstChild());
    CHECK(node.child(STR("b")).text().data() == node.child(STR("b")).firstChild());
    CHECK(!node.child(STR("c")).text().data());
    CHECK(!node.child(STR("d")).text().data());
    CHECK(!XmlText().data());
}

TEST(dom_text_defaults)
{
    XmlText text;

    CHECK_STRING(text.value(STR("foo")), STR("foo"));
    CHECK(text.toInt(42) == 42);
    CHECK(text.toUInt(42) == 42);
    CHECK(text.toDouble(42) == 42);
    CHECK(text.toFloat(42) == 42);
    CHECK(text.toBool(true) == true);
    CHECK(text.toLongLong(42) == 42);
    CHECK(text.toULongLong(42) == 42);
}
