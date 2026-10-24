#include "test.hpp"

#include <limits>
#include <string>

#include <math.h>
#include <string.h>
#include <limits.h>

using namespace pugi;

TEST_XML(dom_attr_assign, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  node.appendAttribute(STR("attr1")) = STR("v1");
  XmlAttribute() = STR("v1");

  node.appendAttribute(STR("attr2")) = -2147483647;
  node.appendAttribute(STR("attr3")) = -2147483647 - 1;
  XmlAttribute() = -2147483647 - 1;

  node.appendAttribute(STR("attr4")) = 4294967295u;
  node.appendAttribute(STR("attr5")) = 4294967294u;
  XmlAttribute() = 4294967295u;

  node.appendAttribute(STR("attr6")) = 0.5;
  XmlAttribute() = 0.5;

  node.appendAttribute(STR("attr7")) = 0.25f;
  XmlAttribute() = 0.25f;

  node.appendAttribute(STR("attr8")) = true;
  XmlAttribute() = true;


  node.appendAttribute(string_view_t(STR("attr9"))) = string_view_t(STR("v2"));

  CHECK_NODE(
      node,
      STR("<node attr1=\"v1\" attr2=\"-2147483647\" attr3=\"-2147483648\" "
          "attr4=\"4294967295\" attr5=\"4294967294\" attr6=\"0.5\" "
          "attr7=\"0.25\" attr8=\"true\" attr9=\"v2\"/>"));
}

TEST_XML(dom_attr_set_name, "<node attr='value' />")
{
  XmlAttribute attr = doc.child(STR("node")).attribute(STR("attr"));

  CHECK(attr.setName(STR("n")));
  CHECK(!XmlAttribute().setName(STR("n")));

  CHECK_NODE(doc, STR("<node n=\"value\"/>"));
}

TEST_XML(dom_attr_set_name_with_size, "<node attr='value' />")
{
  XmlAttribute attr = doc.child(STR("node")).attribute(STR("attr"));

  CHECK(attr.setName(STR("n1234"), 1));
  CHECK(!XmlAttribute().setName(STR("nfail"), 1));

  CHECK_NODE(doc, STR("<node n=\"value\"/>"));
}


TEST_XML(dom_attr_set_name_with_string_view, "<node attr='value' />")
{
  XmlAttribute attr = doc.child(STR("node")).attribute(STR("attr"));

  CHECK(attr.setName(string_view_t()));
  CHECK(!XmlAttribute().setName(string_view_t()));

  CHECK_NODE(doc, STR("<node :anonymous=\"value\"/>"));

  CHECK(attr.setName(string_view_t(STR("n1234"), 1)));
  CHECK(!XmlAttribute().setName(string_view_t(STR("nfail"), 1)));

  CHECK_NODE(doc, STR("<node n=\"value\"/>"));
}

TEST_XML(dom_attr_set_value, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.appendAttribute(STR("attr1")).setValue(STR("v1")));
  CHECK(!XmlAttribute().setValue(STR("v1")));

  CHECK(node.appendAttribute(STR("attr2")).setValue(-2147483647));
  CHECK(node.appendAttribute(STR("attr3")).setValue(-2147483647 - 1));
  CHECK(!XmlAttribute().setValue(-2147483647));

  CHECK(node.appendAttribute(STR("attr4")).setValue(4294967295u));
  CHECK(node.appendAttribute(STR("attr5")).setValue(4294967294u));
  CHECK(!XmlAttribute().setValue(4294967295u));

  CHECK(node.appendAttribute(STR("attr6")).setValue(0.5));
  CHECK(!XmlAttribute().setValue(0.5));

  CHECK(node.appendAttribute(STR("attr7")).setValue(0.25f));
  CHECK(!XmlAttribute().setValue(0.25f));

  CHECK(node.appendAttribute(STR("attr8")).setValue(true));
  CHECK(!XmlAttribute().setValue(true));

  CHECK(node.appendAttribute(STR("attr9")).setValue(STR("v2"), 2));
  CHECK(!XmlAttribute().setValue(STR("v2")));

  CHECK(node.appendAttribute(STR("attr10")).setValue(STR("v3foobar"), 2));
  CHECK(!XmlAttribute().setValue(STR("v3")));


  CHECK(node.appendAttribute(string_view_t(STR("attr11")))
            .setValue(string_view_t(STR("v4"))));
  CHECK(!XmlAttribute().setValue(string_view_t(STR("v4"))));

  CHECK_NODE(node,
             STR("<node attr1=\"v1\" attr2=\"-2147483647\" "
                 "attr3=\"-2147483648\" attr4=\"4294967295\" "
                 "attr5=\"4294967294\" attr6=\"0.5\" attr7=\"0.25\" "
                 "attr8=\"true\" attr9=\"v2\" attr10=\"v3\" attr11=\"v4\"/>"));
}

#if LONG_MAX > 2147483647
TEST_XML(dom_attr_assign_long, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  node.appendAttribute(STR("attr1")) = -9223372036854775807l;
  node.appendAttribute(STR("attr2")) = -9223372036854775807l - 1;
  XmlAttribute() = -9223372036854775807l - 1;

  node.appendAttribute(STR("attr3")) = 18446744073709551615ul;
  node.appendAttribute(STR("attr4")) = 18446744073709551614ul;
  XmlAttribute() = 18446744073709551615ul;

  CHECK_NODE(
      node,
      STR("<node attr1=\"-9223372036854775807\" attr2=\"-9223372036854775808\" "
          "attr3=\"18446744073709551615\" attr4=\"18446744073709551614\"/>"));
}

TEST_XML(dom_attr_set_value_long, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.appendAttribute(STR("attr1")).setValue(-9223372036854775807l));
  CHECK(
      node.appendAttribute(STR("attr2")).setValue(-9223372036854775807l - 1));
  CHECK(!XmlAttribute().setValue(-9223372036854775807l - 1));

  CHECK(node.appendAttribute(STR("attr3")).setValue(18446744073709551615ul));
  CHECK(node.appendAttribute(STR("attr4")).setValue(18446744073709551614ul));
  CHECK(!XmlAttribute().setValue(18446744073709551615ul));

  CHECK_NODE(
      node,
      STR("<node attr1=\"-9223372036854775807\" attr2=\"-9223372036854775808\" "
          "attr3=\"18446744073709551615\" attr4=\"18446744073709551614\"/>"));
}
#else
TEST_XML(dom_attr_assign_long, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  node.appendAttribute(STR("attr1")) = -2147483647l;
  node.appendAttribute(STR("attr2")) = -2147483647l - 1;
  XmlAttribute() = -2147483647l - 1;

  node.appendAttribute(STR("attr3")) = 4294967295ul;
  node.appendAttribute(STR("attr4")) = 4294967294ul;
  XmlAttribute() = 4294967295ul;

  CHECK_NODE(node, STR("<node attr1=\"-2147483647\" attr2=\"-2147483648\" "
                       "attr3=\"4294967295\" attr4=\"4294967294\"/>"));
}

TEST_XML(dom_attr_set_value_long, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.appendAttribute(STR("attr1")).setValue(-2147483647l));
  CHECK(node.appendAttribute(STR("attr2")).setValue(-2147483647l - 1));
  CHECK(!XmlAttribute().setValue(-2147483647l - 1));

  CHECK(node.appendAttribute(STR("attr3")).setValue(4294967295ul));
  CHECK(node.appendAttribute(STR("attr4")).setValue(4294967294ul));
  CHECK(!XmlAttribute().setValue(4294967295ul));

  CHECK_NODE(node, STR("<node attr1=\"-2147483647\" attr2=\"-2147483648\" "
                       "attr3=\"4294967295\" attr4=\"4294967294\"/>"));
}
#endif


TEST_XML(dom_attr_assign_llong, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  node.appendAttribute(STR("attr1")) = -9223372036854775807ll;
  node.appendAttribute(STR("attr2")) = -9223372036854775807ll - 1;
  XmlAttribute() = -9223372036854775807ll - 1;

  node.appendAttribute(STR("attr3")) = 18446744073709551615ull;
  node.appendAttribute(STR("attr4")) = 18446744073709551614ull;
  XmlAttribute() = 18446744073709551615ull;

  CHECK_NODE(
      node,
      STR("<node attr1=\"-9223372036854775807\" attr2=\"-9223372036854775808\" "
          "attr3=\"18446744073709551615\" attr4=\"18446744073709551614\"/>"));
}

TEST_XML(dom_attr_set_value_llong, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.appendAttribute(STR("attr1")).setValue(-9223372036854775807ll));
  CHECK(
      node.appendAttribute(STR("attr2")).setValue(-9223372036854775807ll - 1));
  CHECK(!XmlAttribute().setValue(-9223372036854775807ll - 1));

  CHECK(node.appendAttribute(STR("attr3")).setValue(18446744073709551615ull));
  CHECK(node.appendAttribute(STR("attr4")).setValue(18446744073709551614ull));
  CHECK(!XmlAttribute().setValue(18446744073709551615ull));

  CHECK_NODE(
      node,
      STR("<node attr1=\"-9223372036854775807\" attr2=\"-9223372036854775808\" "
          "attr3=\"18446744073709551615\" attr4=\"18446744073709551614\"/>"));
}

TEST_XML(dom_attr_assign_large_number_float, "<node attr='' />")
{
  XmlNode node = doc.child(STR("node"));

  node.attribute(STR("attr")) = std::numeric_limits< float >::max();

  CHECK(test_node(node, STR("<node attr=\"3.40282347e+038\"/>"), STR(""),
                  format_raw) ||
        test_node(node, STR("<node attr=\"3.40282347e+38\"/>"), STR(""),
                  format_raw));
}

TEST_XML(dom_attr_assign_large_number_double, "<node attr='' />")
{
  XmlNode node = doc.child(STR("node"));

  node.attribute(STR("attr")) = std::numeric_limits< double >::max();

  // Borland C does not print double values with enough precision
#ifdef __BORLANDC__
  CHECK_NODE(node, STR("<node attr=\"1.7976931348623156e+308\"/>"));
#else
  CHECK_NODE(node, STR("<node attr=\"1.7976931348623157e+308\"/>"));
#endif
}

TEST_XML(dom_node_set_name, "<node>text</node>")
{
  CHECK(doc.child(STR("node")).setName(STR("n")));
  CHECK(!doc.child(STR("node")).firstChild().setName(STR("n")));
  CHECK(!XmlNode().setName(STR("n")));

  CHECK_NODE(doc, STR("<n>text</n>"));
}

TEST_XML(dom_node_set_name_with_size, "<node>text</node>")
{
  CHECK(doc.child(STR("node")).setName(STR("nlongname"), 1));
  CHECK(!doc.child(STR("node")).firstChild().setName(STR("n42"), 1));
  CHECK(!XmlNode().setName(STR("nanothername"), 1));

  CHECK_NODE(doc, STR("<n>text</n>"));
}


TEST_XML(dom_node_set_name_with_string_view, "<node>text</node>")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.setName(string_view_t()));
  CHECK(!node.firstChild().setName(string_view_t(STR("n42"), 1)));
  CHECK(!XmlNode().setName(string_view_t(STR("nanothername"), 1)));

  CHECK_NODE(doc, STR("<:anonymous>text</:anonymous>"));

  CHECK(node.setName(string_view_t(STR("nlongname"), 1)));
  CHECK(!doc.child(STR("node"))
             .firstChild()
             .setName(string_view_t(STR("n42"), 1)));
  CHECK(!XmlNode().setName(string_view_t(STR("nanothername"), 1)));

  CHECK_NODE(doc, STR("<n>text</n>"));
}

TEST_XML(dom_node_set_value, "<node>text</node>")
{
  CHECK(doc.child(STR("node")).firstChild().setValue(STR("no text")));
  CHECK(!doc.child(STR("node")).setValue(STR("no text")));
  CHECK(!XmlNode().setValue(STR("no text")));

  CHECK_NODE(doc, STR("<node>no text</node>"));
}

TEST_XML(dom_node_set_value_partially_with_size, "<node>text</node>")
{
  CHECK(doc.child(STR("node")).firstChild().setValue(STR("no text"), 2));
  CHECK(!doc.child(STR("node")).setValue(STR("no text"), 2));
  CHECK(!XmlNode().setValue(STR("no text"), 2));

  CHECK_NODE(doc, STR("<node>no</node>"));
}

TEST_XML(dom_node_set_value_with_size, "<node>text</node>")
{
  CHECK(doc.child(STR("node")).firstChild().setValue(STR("no text"), 7));
  CHECK(!doc.child(STR("node")).setValue(STR("no text"), 7));
  CHECK(!XmlNode().setValue(STR("no text"), 7));

  CHECK_NODE(doc, STR("<node>no text</node>"));
}


TEST_XML(dom_node_set_value_partially_with_string_view, "<node>text</node>")
{
  CHECK(doc.child(STR("node"))
            .firstChild()
            .setValue(string_view_t(STR("no text"), 2)));
  CHECK(!doc.child(STR("node")).setValue(string_view_t(STR("no text"), 2)));
  CHECK(!XmlNode().setValue(string_view_t(STR("no text"), 2)));

  CHECK_NODE(doc, STR("<node>no</node>"));
}

TEST_XML(dom_node_set_value_with_string_view, "<node>text</node>")
{
  CHECK(doc.child(STR("node"))
            .firstChild()
            .setValue(string_view_t(STR("no text"), 7)));
  CHECK(!doc.child(STR("node")).setValue(string_view_t(STR("no text"), 7)));
  CHECK(!XmlNode().setValue(string_view_t(STR("no text"), 7)));

  CHECK_NODE(doc, STR("<node>no text</node>"));
}

TEST_XML(dom_node_set_value_allocated, "<node>text</node>")
{
  CHECK(doc.child(STR("node")).firstChild().setValue(STR("no text")));
  CHECK(!doc.child(STR("node")).setValue(STR("no text")));
  CHECK(!XmlNode().setValue(STR("no text")));
  CHECK(doc.child(STR("node")).firstChild().setValue(STR("no text at all")));

  CHECK_NODE(doc, STR("<node>no text at all</node>"));
}

TEST_XML(dom_node_prependAttribute, "<node><child/></node>")
{
  CHECK(XmlNode().prependAttribute(STR("a")) == XmlAttribute());
  CHECK(doc.prependAttribute(STR("a")) == XmlAttribute());

  XmlAttribute a1 = doc.child(STR("node")).prependAttribute(STR("a1"));
  CHECK(a1);
  a1 = STR("v1");

  XmlAttribute a2 = doc.child(STR("node")).prependAttribute(STR("a2"));
  CHECK(a2 && a1 != a2);
  a2 = STR("v2");

  XmlAttribute a3 =
      doc.child(STR("node")).child(STR("child")).prependAttribute(STR("a3"));
  CHECK(a3 && a1 != a3 && a2 != a3);
  a3 = STR("v3");


  XmlAttribute a4 = doc.child(STR("node"))
                        .child(STR("child"))
                        .prependAttribute(string_view_t(STR("a4")));
  CHECK(a4 && a1 != a4 && a2 != a4 && a3 != a4);
  a4 = STR("v4");

  CHECK_NODE(
      doc,
      STR("<node a2=\"v2\" a1=\"v1\"><child a4=\"v4\" a3=\"v3\"/></node>"));
}

TEST_XML(dom_node_appendAttribute, "<node><child/></node>")
{
  CHECK(XmlNode().appendAttribute(STR("a")) == XmlAttribute());
  CHECK(doc.appendAttribute(STR("a")) == XmlAttribute());

  XmlAttribute a1 = doc.child(STR("node")).appendAttribute(STR("a1"));
  CHECK(a1);
  a1 = STR("v1");

  XmlAttribute a2 = doc.child(STR("node")).appendAttribute(STR("a2"));
  CHECK(a2 && a1 != a2);
  a2 = STR("v2");

  XmlAttribute a3 =
      doc.child(STR("node")).child(STR("child")).appendAttribute(STR("a3"));
  CHECK(a3 && a1 != a3 && a2 != a3);
  a3 = STR("v3");

  CHECK_NODE(doc, STR("<node a1=\"v1\" a2=\"v2\"><child a3=\"v3\"/></node>"));
}

TEST_XML(dom_node_ensureAttribute, "<node a1='v1'><child/></node>")
{
  CHECK(XmlNode().ensureAttribute(STR("a")) == XmlAttribute());
  CHECK(doc.ensureAttribute(STR("a")) == XmlAttribute());

  XmlNode node = doc.child(STR("node"));

  // existing attribute is returned as is
  XmlAttribute a1 = node.ensureAttribute(STR("a1"));
  CHECK(a1 && a1 == node.attribute(STR("a1")));
  CHECK_STRING(a1.value(), STR("v1"));

  // missing attribute is appended
  XmlAttribute a2 = node.ensureAttribute(STR("a2"));
  CHECK(a2 && a2 != a1);
  a2 = STR("v2");

  // ensuring an existing attribute does not add a duplicate
  CHECK(node.ensureAttribute(STR("a2")) == a2);


  XmlAttribute a3 =
      node.child(STR("child")).ensureAttribute(string_view_t(STR("a3")));
  CHECK(a3 && a3 != a1 && a3 != a2);
  a3 = STR("v3");

  CHECK_NODE(doc, STR("<node a1=\"v1\" a2=\"v2\"><child a3=\"v3\"/></node>"));
}

TEST_XML(dom_node_insertAttributeAfter,
         "<node a1='v1'><child a2='v2'/></node>")
{
  CHECK(XmlNode().insertAttributeAfter(STR("a"), XmlAttribute()) ==
        XmlAttribute());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  XmlAttribute a1 = node.attribute(STR("a1"));
  XmlAttribute a2 = child.attribute(STR("a2"));

  CHECK(node.insertAttributeAfter(STR("a"), XmlAttribute()) ==
        XmlAttribute());
  CHECK(node.insertAttributeAfter(STR("a"), a2) == XmlAttribute());

  XmlAttribute a3 = node.insertAttributeAfter(STR("a3"), a1);
  CHECK(a3 && a3 != a2 && a3 != a1);
  a3 = STR("v3");

  XmlAttribute a4 = node.insertAttributeAfter(STR("a4"), a1);
  CHECK(a4 && a4 != a3 && a4 != a2 && a4 != a1);
  a4 = STR("v4");

  XmlAttribute a5 = node.insertAttributeAfter(STR("a5"), a3);
  CHECK(a5 && a5 != a4 && a5 != a3 && a5 != a2 && a5 != a1);
  a5 = STR("v5");

  CHECK(child.insertAttributeAfter(STR("a"), a4) == XmlAttribute());


  XmlAttribute a6 = node.insertAttributeAfter(string_view_t(STR("a6")), a5);
  CHECK(child.insertAttributeAfter(string_view_t(STR("a")), a5) ==
        XmlAttribute());

  CHECK(a6 && a6 != a5 && a6 != a4 && a6 != a3 && a6 != a2 && a6 != a1);
  a6 = STR("v6");

  CHECK_NODE(doc, STR("<node a1=\"v1\" a4=\"v4\" a3=\"v3\" a5=\"v5\" "
                      "a6=\"v6\"><child a2=\"v2\"/></node>"));
}

TEST_XML(dom_node_insertAttributeBefore,
         "<node a1='v1'><child a2='v2'/></node>")
{
  CHECK(XmlNode().insertAttributeBefore(STR("a"), XmlAttribute()) ==
        XmlAttribute());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  XmlAttribute a1 = node.attribute(STR("a1"));
  XmlAttribute a2 = child.attribute(STR("a2"));

  CHECK(node.insertAttributeBefore(STR("a"), XmlAttribute()) ==
        XmlAttribute());
  CHECK(node.insertAttributeBefore(STR("a"), a2) == XmlAttribute());

  XmlAttribute a3 = node.insertAttributeBefore(STR("a3"), a1);
  CHECK(a3 && a3 != a2 && a3 != a1);
  a3 = STR("v3");

  XmlAttribute a4 = node.insertAttributeBefore(STR("a4"), a1);
  CHECK(a4 && a4 != a3 && a4 != a2 && a4 != a1);
  a4 = STR("v4");

  XmlAttribute a5 = node.insertAttributeBefore(STR("a5"), a3);
  CHECK(a5 && a5 != a4 && a5 != a3 && a5 != a2 && a5 != a1);
  a5 = STR("v5");

  CHECK(child.insertAttributeBefore(STR("a"), a4) == XmlAttribute());


  XmlAttribute a6 = node.insertAttributeBefore(string_view_t(STR("a6")), a1);
  CHECK(child.insertAttributeBefore(string_view_t(STR("a")), a4) ==
        XmlAttribute());

  CHECK(a6 && a6 != a5 && a6 != a4 && a6 != a3 && a6 != a2 && a6 != a1);
  a6 = STR("v6");

  CHECK_NODE(doc, STR("<node a5=\"v5\" a3=\"v3\" a4=\"v4\" a6=\"v6\" "
                      "a1=\"v1\"><child a2=\"v2\"/></node>"));
}

TEST_XML(dom_node_prependCopy_attribute,
         "<node a1='v1'><child a2='v2'/><child/></node>")
{
  CHECK(XmlNode().prependCopy(XmlAttribute()) == XmlAttribute());
  CHECK(XmlNode().prependCopy(doc.child(STR("node")).attribute(STR("a1"))) ==
        XmlAttribute());
  CHECK(doc.prependCopy(doc.child(STR("node")).attribute(STR("a1"))) ==
        XmlAttribute());
  CHECK(doc.child(STR("node")).prependCopy(XmlAttribute()) == XmlAttribute());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  XmlAttribute a1 = node.attribute(STR("a1"));
  XmlAttribute a2 = child.attribute(STR("a2"));

  XmlAttribute a3 = node.prependCopy(a1);
  CHECK(a3 && a3 != a2 && a3 != a1);

  XmlAttribute a4 = node.prependCopy(a2);
  CHECK(a4 && a4 != a3 && a4 != a2 && a4 != a1);

  XmlAttribute a5 = node.lastChild().prependCopy(a1);
  CHECK(a5 && a5 != a4 && a5 != a3 && a5 != a2 && a5 != a1);

  CHECK_NODE(doc, STR("<node a2=\"v2\" a1=\"v1\" a1=\"v1\"><child "
                      "a2=\"v2\"/><child a1=\"v1\"/></node>"));

  a3.setName(STR("a3"));
  a3 = STR("v3");

  a4.setName(STR("a4"));
  a4 = STR("v4");

  a5.setName(STR("a5"));
  a5 = STR("v5");

  CHECK_NODE(doc, STR("<node a4=\"v4\" a3=\"v3\" a1=\"v1\"><child "
                      "a2=\"v2\"/><child a5=\"v5\"/></node>"));
}

TEST_XML(dom_node_appendCopy_attribute,
         "<node a1='v1'><child a2='v2'/><child/></node>")
{
  CHECK(XmlNode().appendCopy(XmlAttribute()) == XmlAttribute());
  CHECK(XmlNode().appendCopy(doc.child(STR("node")).attribute(STR("a1"))) ==
        XmlAttribute());
  CHECK(doc.appendCopy(doc.child(STR("node")).attribute(STR("a1"))) ==
        XmlAttribute());
  CHECK(doc.child(STR("node")).appendCopy(XmlAttribute()) == XmlAttribute());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  XmlAttribute a1 = node.attribute(STR("a1"));
  XmlAttribute a2 = child.attribute(STR("a2"));

  XmlAttribute a3 = node.appendCopy(a1);
  CHECK(a3 && a3 != a2 && a3 != a1);

  XmlAttribute a4 = node.appendCopy(a2);
  CHECK(a4 && a4 != a3 && a4 != a2 && a4 != a1);

  XmlAttribute a5 = node.lastChild().appendCopy(a1);
  CHECK(a5 && a5 != a4 && a5 != a3 && a5 != a2 && a5 != a1);

  CHECK_NODE(doc, STR("<node a1=\"v1\" a1=\"v1\" a2=\"v2\"><child "
                      "a2=\"v2\"/><child a1=\"v1\"/></node>"));

  a3.setName(STR("a3"));
  a3 = STR("v3");

  a4.setName(STR("a4"));
  a4 = STR("v4");

  a5.setName(STR("a5"));
  a5 = STR("v5");

  CHECK_NODE(doc, STR("<node a1=\"v1\" a3=\"v3\" a4=\"v4\"><child "
                      "a2=\"v2\"/><child a5=\"v5\"/></node>"));
}

TEST_XML(dom_node_insertCopyAfter_attribute,
         "<node a1='v1'><child a2='v2'/>text</node>")
{
  CHECK(XmlNode().insertCopyAfter(XmlAttribute(), XmlAttribute()) ==
        XmlAttribute());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  XmlAttribute a1 = node.attribute(STR("a1"));
  XmlAttribute a2 = child.attribute(STR("a2"));

  CHECK(node.insertCopyAfter(a1, XmlAttribute()) == XmlAttribute());
  CHECK(node.insertCopyAfter(XmlAttribute(), a1) == XmlAttribute());
  CHECK(node.insertCopyAfter(a2, a2) == XmlAttribute());
  CHECK(node.lastChild().insertCopyAfter(a2, a2) == XmlAttribute());

  XmlAttribute a3 = node.insertCopyAfter(a1, a1);
  CHECK(a3 && a3 != a2 && a3 != a1);

  XmlAttribute a4 = node.insertCopyAfter(a2, a1);
  CHECK(a4 && a4 != a3 && a4 != a2 && a4 != a1);

  XmlAttribute a5 = node.insertCopyAfter(a4, a1);
  CHECK(a5 && a5 != a4 && a5 != a3 && a5 != a2 && a5 != a1);

  CHECK(child.insertCopyAfter(a4, a4) == XmlAttribute());

  CHECK_NODE(doc, STR("<node a1=\"v1\" a2=\"v2\" a2=\"v2\" a1=\"v1\"><child "
                      "a2=\"v2\"/>text</node>"));

  a3.setName(STR("a3"));
  a3 = STR("v3");

  a4.setName(STR("a4"));
  a4 = STR("v4");

  a5.setName(STR("a5"));
  a5 = STR("v5");

  CHECK_NODE(doc, STR("<node a1=\"v1\" a5=\"v5\" a4=\"v4\" a3=\"v3\"><child "
                      "a2=\"v2\"/>text</node>"));
}

TEST_XML(dom_node_insertCopyBefore_attribute,
         "<node a1='v1'><child a2='v2'/>text</node>")
{
  CHECK(XmlNode().insertCopyBefore(XmlAttribute(), XmlAttribute()) ==
        XmlAttribute());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  XmlAttribute a1 = node.attribute(STR("a1"));
  XmlAttribute a2 = child.attribute(STR("a2"));

  CHECK(node.insertCopyBefore(a1, XmlAttribute()) == XmlAttribute());
  CHECK(node.insertCopyBefore(XmlAttribute(), a1) == XmlAttribute());
  CHECK(node.insertCopyBefore(a2, a2) == XmlAttribute());
  CHECK(node.lastChild().insertCopyBefore(a2, a2) == XmlAttribute());

  XmlAttribute a3 = node.insertCopyBefore(a1, a1);
  CHECK(a3 && a3 != a2 && a3 != a1);

  XmlAttribute a4 = node.insertCopyBefore(a2, a1);
  CHECK(a4 && a4 != a3 && a4 != a2 && a4 != a1);

  XmlAttribute a5 = node.insertCopyBefore(a4, a1);
  CHECK(a5 && a5 != a4 && a5 != a3 && a5 != a2 && a5 != a1);

  CHECK(child.insertCopyBefore(a4, a4) == XmlAttribute());

  CHECK_NODE(doc, STR("<node a1=\"v1\" a2=\"v2\" a2=\"v2\" a1=\"v1\"><child "
                      "a2=\"v2\"/>text</node>"));

  a3.setName(STR("a3"));
  a3 = STR("v3");

  a4.setName(STR("a4"));
  a4 = STR("v4");

  a5.setName(STR("a5"));
  a5 = STR("v5");

  CHECK_NODE(doc, STR("<node a3=\"v3\" a4=\"v4\" a5=\"v5\" a1=\"v1\"><child "
                      "a2=\"v2\"/>text</node>"));
}

TEST_XML(dom_node_removeAttribute,
         "<node a1='v1' a2='v2' a3='v3'><child a4='v4'/></node>")
{
  CHECK(!XmlNode().removeAttribute(STR("a")));
  CHECK(!XmlNode().removeAttribute(XmlAttribute()));

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(!node.removeAttribute(STR("a")));
  CHECK(!node.removeAttribute(XmlAttribute()));
  CHECK(!node.removeAttribute(child.attribute(STR("a4"))));

  CHECK_NODE(
      doc,
      STR("<node a1=\"v1\" a2=\"v2\" a3=\"v3\"><child a4=\"v4\"/></node>"));

  CHECK(node.removeAttribute(STR("a1")));
  CHECK(node.removeAttribute(node.attribute(STR("a3"))));
  CHECK(child.removeAttribute(STR("a4")));

  CHECK_NODE(doc, STR("<node a2=\"v2\"><child/></node>"));


  CHECK(!node.removeAttribute(string_view_t()));
  CHECK(!node.removeAttribute(string_view_t(STR("a2"), 1)));
  CHECK(node.removeAttribute(string_view_t(STR("a2extra"), 2)));

  CHECK_NODE(doc, STR("<node><child/></node>"));
}

TEST_XML(dom_node_removeAttributes,
         "<node a1='v1' a2='v2' a3='v3'><child a4='v4'/></node>")
{
  CHECK(!XmlNode().removeAttributes());
  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(child.removeAttributes());
  CHECK_NODE(child, STR("<child/>"));

  CHECK(node.removeAttributes());
  CHECK_NODE(node, STR("<node><child/></node>"));
}

TEST_XML(dom_node_removeAttributes_lots, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  // this test makes sure we generate at least 2 pages (64K) worth of attribute
  // data so that we can trigger page deallocation to make sure code is memory
  // safe
  for (size_t i = 0; i < 10000; ++i)
    node.appendAttribute(STR("a")) = STR("v");

  CHECK_STRING(node.attribute(STR("a")).value(), STR("v"));

  CHECK(node.removeAttributes());

  CHECK_STRING(node.attribute(STR("a")).value(), STR(""));
  CHECK_NODE(node, STR("<node/>"));
}

TEST_XML(dom_node_prependChild, "<node>foo<child/></node>")
{
  CHECK(XmlNode().prependChild() == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().prependChild() == XmlNode());
  CHECK(doc.prependChild(node_document) == XmlNode());
  CHECK(doc.prependChild(node_null) == XmlNode());

  XmlNode n1 = doc.child(STR("node")).prependChild();
  CHECK(n1);
  CHECK(n1.setName(STR("n1")));

  XmlNode n2 = doc.child(STR("node")).prependChild();
  CHECK(n2 && n1 != n2);
  CHECK(n2.setName(STR("n2")));

  XmlNode n3 =
      doc.child(STR("node")).child(STR("child")).prependChild(node_pcdata);
  CHECK(n3 && n1 != n3 && n2 != n3);
  CHECK(n3.setValue(STR("n3")));

  XmlNode n4 = doc.prependChild(node_comment);
  CHECK(n4 && n1 != n4 && n2 != n4 && n3 != n4);
  CHECK(n4.setValue(STR("n4")));

  CHECK_NODE(doc, STR("<!--n4--><node><n2/><n1/>foo<child>n3</child></node>"));
}

TEST_XML(dom_node_appendChild, "<node>foo<child/></node>")
{
  CHECK(XmlNode().appendChild() == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().appendChild() == XmlNode());
  CHECK(doc.appendChild(node_document) == XmlNode());
  CHECK(doc.appendChild(node_null) == XmlNode());

  XmlNode n1 = doc.child(STR("node")).appendChild();
  CHECK(n1);
  CHECK(n1.setName(STR("n1")));

  XmlNode n2 = doc.child(STR("node")).appendChild();
  CHECK(n2 && n1 != n2);
  CHECK(n2.setName(STR("n2")));

  XmlNode n3 =
      doc.child(STR("node")).child(STR("child")).appendChild(node_pcdata);
  CHECK(n3 && n1 != n3 && n2 != n3);
  CHECK(n3.setValue(STR("n3")));

  XmlNode n4 = doc.appendChild(node_comment);
  CHECK(n4 && n1 != n4 && n2 != n4 && n3 != n4);
  CHECK(n4.setValue(STR("n4")));

  CHECK_NODE(doc, STR("<node>foo<child>n3</child><n1/><n2/></node><!--n4-->"));
}

TEST_XML(dom_node_insertChildAfter, "<node>foo<child/></node>")
{
  CHECK(XmlNode().insertChildAfter(node_element, XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertChildAfter(node_element, XmlNode()) == XmlNode());
  CHECK(doc.insertChildAfter(node_document, XmlNode()) == XmlNode());
  CHECK(doc.insertChildAfter(node_null, XmlNode()) == XmlNode());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(node.insertChildAfter(node_element, XmlNode()) == XmlNode());
  CHECK(node.insertChildAfter(node_element, node) == XmlNode());
  CHECK(child.insertChildAfter(node_element, node) == XmlNode());

  XmlNode n1 = node.insertChildAfter(node_element, child);
  CHECK(n1 && n1 != node && n1 != child);
  CHECK(n1.setName(STR("n1")));

  XmlNode n2 = node.insertChildAfter(node_element, child);
  CHECK(n2 && n2 != node && n2 != child && n2 != n1);
  CHECK(n2.setName(STR("n2")));

  XmlNode n3 = node.insertChildAfter(node_pcdata, n2);
  CHECK(n3 && n3 != node && n3 != child && n3 != n1 && n3 != n2);
  CHECK(n3.setValue(STR("n3")));

  XmlNode n4 = node.insertChildAfter(node_pi, node.firstChild());
  CHECK(n4 && n4 != node && n4 != child && n4 != n1 && n4 != n2 && n4 != n3);
  CHECK(n4.setName(STR("n4")));

  CHECK(child.insertChildAfter(node_element, n3) == XmlNode());

  CHECK_NODE(doc, STR("<node>foo<?n4?><child/><n2/>n3<n1/></node>"));
}

TEST_XML(dom_node_insertChildBefore, "<node>foo<child/></node>")
{
  CHECK(XmlNode().insertChildBefore(node_element, XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertChildBefore(node_element, XmlNode()) == XmlNode());
  CHECK(doc.insertChildBefore(node_document, XmlNode()) == XmlNode());
  CHECK(doc.insertChildBefore(node_null, XmlNode()) == XmlNode());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(node.insertChildBefore(node_element, XmlNode()) == XmlNode());
  CHECK(node.insertChildBefore(node_element, node) == XmlNode());
  CHECK(child.insertChildBefore(node_element, node) == XmlNode());

  XmlNode n1 = node.insertChildBefore(node_element, child);
  CHECK(n1 && n1 != node && n1 != child);
  CHECK(n1.setName(STR("n1")));

  XmlNode n2 = node.insertChildBefore(node_element, child);
  CHECK(n2 && n2 != node && n2 != child && n2 != n1);
  CHECK(n2.setName(STR("n2")));

  XmlNode n3 = node.insertChildBefore(node_pcdata, n2);
  CHECK(n3 && n3 != node && n3 != child && n3 != n1 && n3 != n2);
  CHECK(n3.setValue(STR("n3")));

  XmlNode n4 = node.insertChildBefore(node_pi, node.firstChild());
  CHECK(n4 && n4 != node && n4 != child && n4 != n1 && n4 != n2 && n4 != n3);
  CHECK(n4.setName(STR("n4")));

  CHECK(child.insertChildBefore(node_element, n3) == XmlNode());

  CHECK_NODE(doc, STR("<node><?n4?>foo<n1/>n3<n2/><child/></node>"));
}

TEST_XML(dom_node_prependChild_name, "<node>foo<child/></node>")
{
  CHECK(XmlNode().prependChild(STR("")) == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().prependChild(STR("")) ==
        XmlNode());

  CHECK(XmlNode().prependChild(string_view_t()) == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().prependChild(string_view_t()) ==
        XmlNode());

  XmlNode n1 = doc.child(STR("node")).prependChild(STR("n1"));
  CHECK(n1);

  XmlNode n2 = doc.child(STR("node")).prependChild(STR("n2"));
  CHECK(n2 && n1 != n2);


  XmlNode n3 = doc.prependChild(string_view_t(STR("n3")));
  CHECK(n3 && n1 != n3 && n2 != n3);

  CHECK_NODE(doc, STR("<n3/><node><n2/><n1/>foo<child/></node>"));
}

TEST_XML(dom_node_appendChild_name, "<node>foo<child/></node>")
{
  CHECK(XmlNode().appendChild(STR("")) == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().appendChild(STR("")) ==
        XmlNode());

  XmlNode n1 = doc.child(STR("node")).appendChild(STR("n1"));
  CHECK(n1);

  XmlNode n2 = doc.child(STR("node")).appendChild(STR("n2"));
  CHECK(n2 && n1 != n2);


  XmlNode n3 = doc.appendChild(string_view_t(STR("n3")));

  CHECK(n3 && n3 != n2 && n3 != n1);

  CHECK_NODE(doc, STR("<node>foo<child/><n1/><n2/></node><n3/>"));
}

TEST_XML(dom_node_ensureChild, "<node>foo<child/></node>")
{
  CHECK(XmlNode().ensureChild(STR("")) == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().ensureChild(STR("n")) ==
        XmlNode());

  XmlNode node = doc.child(STR("node"));

  // existing child is returned as is
  XmlNode child = node.ensureChild(STR("child"));
  CHECK(child && child == node.child(STR("child")));

  // missing child is appended
  XmlNode n1 = node.ensureChild(STR("n1"));
  CHECK(n1 && n1 != child);

  // ensuring an existing child does not add a duplicate
  CHECK(node.ensureChild(STR("child")) == child);


  XmlNode n2 = doc.ensureChild(string_view_t(STR("n2")));

  CHECK(n2 && n2 != child && n2 != n1);

  CHECK_NODE(doc, STR("<node>foo<child/><n1/></node><n2/>"));
}

TEST_XML(dom_node_insertChildAfter_name, "<node>foo<child/></node>")
{
  CHECK(XmlNode().insertChildAfter(STR(""), XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertChildAfter(STR(""), XmlNode()) == XmlNode());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(node.insertChildAfter(STR(""), node) == XmlNode());
  CHECK(child.insertChildAfter(STR(""), node) == XmlNode());

  XmlNode n1 = node.insertChildAfter(STR("n1"), child);
  CHECK(n1 && n1 != node && n1 != child);

  XmlNode n2 = node.insertChildAfter(STR("n2"), child);
  CHECK(n2 && n2 != node && n2 != child && n2 != n1);

  CHECK(child.insertChildAfter(STR(""), n2) == XmlNode());


  XmlNode n3 = node.insertChildAfter(string_view_t(STR("n3")), n1);
  CHECK(n3 && n3 != node && n3 != child && n3 != n2 && n3 != n1);

  CHECK_NODE(doc, STR("<node>foo<child/><n2/><n1/><n3/></node>"));
}

TEST_XML(dom_node_insertChildBefore_name, "<node>foo<child/></node>")
{
  CHECK(XmlNode().insertChildBefore(STR(""), XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertChildBefore(STR(""), XmlNode()) == XmlNode());

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(node.insertChildBefore(STR(""), node) == XmlNode());
  CHECK(child.insertChildBefore(STR(""), node) == XmlNode());

  XmlNode n1 = node.insertChildBefore(STR("n1"), child);
  CHECK(n1 && n1 != node && n1 != child);

  XmlNode n2 = node.insertChildBefore(STR("n2"), child);
  CHECK(n2 && n2 != node && n2 != child && n2 != n1);

  CHECK(child.insertChildBefore(STR(""), n2) == XmlNode());


  XmlNode n3 = node.insertChildBefore(string_view_t(STR("n3")), child);
  CHECK(n3 && n3 != node && n3 != child && n3 != n2 && n3 != n1);

  CHECK_NODE(doc, STR("<node>foo<n1/><n2/><n3/><child/></node>"));
}

TEST_XML(dom_node_removeChild,
         "<node><n1/><n2/><n3/><child><n4/></child></node>")
{
  CHECK(!XmlNode().removeChild(STR("a")));
  CHECK(!XmlNode().removeChild(XmlNode()));

  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(!node.removeChild(STR("a")));
  CHECK(!node.removeChild(XmlNode()));
  CHECK(!node.removeChild(child.child(STR("n4"))));

  CHECK_NODE(doc, STR("<node><n1/><n2/><n3/><child><n4/></child></node>"));

  CHECK(node.removeChild(STR("n1")));
  CHECK(node.removeChild(node.child(STR("n3"))));
  CHECK(child.removeChild(STR("n4")));

  CHECK_NODE(doc, STR("<node><n2/><child/></node>"));


  CHECK(!node.removeChild(string_view_t()));
  CHECK(!node.removeChild(string_view_t(STR("child"), 3)));
  CHECK(!node.removeChild(string_view_t(STR("n2"), 1)));

  CHECK_NODE(doc, STR("<node><n2/><child/></node>"));

  CHECK(node.removeChild(string_view_t(STR("child"))));
  CHECK_NODE(doc, STR("<node><n2/></node>"));

  CHECK(node.removeChild(string_view_t(STR("n2_notinview"), 2)));
  CHECK_NODE(doc, STR("<node/>"));
}

TEST_XML(dom_node_removeChildren,
         "<node><n1/><n2/><n3/><child><n4/></child></node>")
{
  CHECK(!XmlNode().removeChildren());
  XmlNode node = doc.child(STR("node"));
  XmlNode child = node.child(STR("child"));

  CHECK(child.removeChildren());
  CHECK_NODE(child, STR("<child/>"));

  CHECK(node.removeChildren());
  CHECK_NODE(node, STR("<node/>"));
}

TEST_XML(dom_node_removeChildren_lots, "<node/>")
{
  XmlNode node = doc.child(STR("node"));

  // this test makes sure we generate at least 2 pages (64K) worth of node data
  // so that we can trigger page deallocation to make sure code is memory safe
  for (size_t i = 0; i < 10000; ++i)
    node.appendChild().setName(STR("n"));

  CHECK(node.child(STR("n")));

  CHECK(node.removeChildren());

  CHECK(!node.child(STR("n")));
  CHECK_NODE(node, STR("<node/>"));
}

TEST_XML(
    dom_node_removeChild_complex,
    "<node id='1'><n1 id1='1' id2='2'/><n2/><n3/><child><n4/></child></node>")
{
  CHECK(doc.child(STR("node")).removeChild(STR("n1")));

  CHECK_NODE(doc, STR("<node id=\"1\"><n2/><n3/><child><n4/></child></node>"));

  CHECK(doc.removeChild(STR("node")));

  CHECK_NODE(doc, STR(""));
}

TEST_XML(
    dom_node_removeChild_complex_allocated,
    "<node id='1'><n1 id1='1' id2='2'/><n2/><n3/><child><n4/></child></node>")
{
  doc.appendCopy(doc.child(STR("node")));

  CHECK(doc.removeChild(STR("node")));
  CHECK(doc.removeChild(STR("node")));

  CHECK_NODE(doc, STR(""));
}

TEST_XML(dom_node_prependCopy, "<node>foo<child/></node>")
{
  CHECK(XmlNode().prependCopy(XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .prependCopy(doc.child(STR("node"))) == XmlNode());
  CHECK(doc.prependCopy(doc) == XmlNode());
  CHECK(doc.prependCopy(XmlNode()) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node")).prependCopy(doc.child(STR("node")).firstChild());
  CHECK(n1);
  CHECK_STRING(n1.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foofoo<child/></node>"));

  XmlNode n2 = doc.child(STR("node"))
                   .prependCopy(doc.child(STR("node")).child(STR("child")));
  CHECK(n2 && n2 != n1);
  CHECK_STRING(n2.name(), STR("child"));
  CHECK_NODE(doc, STR("<node><child/>foofoo<child/></node>"));

  XmlNode n3 =
      doc.child(STR("node"))
          .child(STR("child"))
          .prependCopy(doc.child(STR("node")).firstChild().nextSibling());
  CHECK(n3 && n3 != n1 && n3 != n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child>foo</child>foofoo<child/></node>"));
}

TEST_XML(dom_node_appendCopy, "<node>foo<child/></node>")
{
  CHECK(XmlNode().appendCopy(XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .appendCopy(doc.child(STR("node"))) == XmlNode());
  CHECK(doc.appendCopy(doc) == XmlNode());
  CHECK(doc.appendCopy(XmlNode()) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node")).appendCopy(doc.child(STR("node")).firstChild());
  CHECK(n1);
  CHECK_STRING(n1.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foo<child/>foo</node>"));

  XmlNode n2 = doc.child(STR("node"))
                   .appendCopy(doc.child(STR("node")).child(STR("child")));
  CHECK(n2 && n2 != n1);
  CHECK_STRING(n2.name(), STR("child"));
  CHECK_NODE(doc, STR("<node>foo<child/>foo<child/></node>"));

  XmlNode n3 = doc.child(STR("node"))
                   .child(STR("child"))
                   .appendCopy(doc.child(STR("node")).firstChild());
  CHECK(n3 && n3 != n1 && n3 != n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foo<child>foo</child>foo<child/></node>"));
}

TEST_XML(dom_node_insertCopyAfter, "<node>foo<child/></node>")
{
  XmlNode child = doc.child(STR("node")).child(STR("child"));

  CHECK(XmlNode().insertCopyAfter(XmlNode(), XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertCopyAfter(doc.child(STR("node")),
                               doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertCopyAfter(doc, doc) == XmlNode());
  CHECK(doc.insertCopyAfter(XmlNode(), doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertCopyAfter(doc.child(STR("node")), XmlNode()) == XmlNode());
  CHECK(doc.insertCopyAfter(doc.child(STR("node")), child) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node"))
          .insertCopyAfter(child, doc.child(STR("node")).firstChild());
  CHECK(n1);
  CHECK_STRING(n1.name(), STR("child"));
  CHECK_NODE(doc, STR("<node>foo<child/><child/></node>"));

  XmlNode n2 = doc.child(STR("node"))
                   .insertCopyAfter(doc.child(STR("node")).firstChild(),
                                      doc.child(STR("node")).lastChild());
  CHECK(n2 && n2 != n1);
  CHECK_STRING(n2.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foo<child/><child/>foo</node>"));

  XmlNode n3 = doc.child(STR("node"))
                   .insertCopyAfter(doc.child(STR("node")).firstChild(),
                                      doc.child(STR("node")).firstChild());
  CHECK(n3 && n3 != n1 && n3 != n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foofoo<child/><child/>foo</node>"));
}

TEST_XML(dom_node_insertCopyBefore, "<node>foo<child/></node>")
{
  XmlNode child = doc.child(STR("node")).child(STR("child"));

  CHECK(XmlNode().insertCopyBefore(XmlNode(), XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertCopyBefore(doc.child(STR("node")),
                                doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertCopyBefore(doc, doc) == XmlNode());
  CHECK(doc.insertCopyBefore(XmlNode(), doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertCopyBefore(doc.child(STR("node")), XmlNode()) == XmlNode());
  CHECK(doc.insertCopyBefore(doc.child(STR("node")), child) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node"))
          .insertCopyBefore(child, doc.child(STR("node")).firstChild());
  CHECK(n1);
  CHECK_STRING(n1.name(), STR("child"));
  CHECK_NODE(doc, STR("<node><child/>foo<child/></node>"));

  XmlNode n2 = doc.child(STR("node"))
                   .insertCopyBefore(doc.child(STR("node")).firstChild(),
                                       doc.child(STR("node")).lastChild());
  CHECK(n2 && n2 != n1);
  CHECK_STRING(n2.name(), STR("child"));
  CHECK_NODE(doc, STR("<node><child/>foo<child/><child/></node>"));

  XmlNode n3 = doc.child(STR("node"))
                   .insertCopyBefore(
                       doc.child(STR("node")).firstChild().nextSibling(),
                       doc.child(STR("node")).firstChild());
  CHECK(n3 && n3 != n1 && n3 != n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foo<child/>foo<child/><child/></node>"));
}

TEST_XML(dom_node_copy_recursive, "<node>foo<child/></node>")
{
  doc.child(STR("node")).appendCopy(doc.child(STR("node")));
  CHECK_NODE(doc, STR("<node>foo<child/><node>foo<child/></node></node>"));
}

TEST_XML(dom_node_copy_crossdoc, "<node/>")
{
  XmlDocument newdoc;
  newdoc.appendCopy(doc.child(STR("node")));
  CHECK_NODE(doc, STR("<node/>"));
  CHECK_NODE(newdoc, STR("<node/>"));
}

TEST_XML(dom_node_copy_crossdoc_attribute, "<node attr='value'/>")
{
  XmlDocument newdoc;
  newdoc.appendChild(STR("copy"))
      .appendCopy(doc.child(STR("node")).attribute(STR("attr")));
  CHECK_NODE(doc, STR("<node attr=\"value\"/>"));
  CHECK_NODE(newdoc, STR("<copy attr=\"value\"/>"));
}

TEST_XML_FLAGS(
    dom_node_copy_types,
    "<?xml version='1.0'?><!DOCTYPE id><root><?pi value?><!--comment--><node "
    "id='1'>pcdata<![CDATA[cdata]]></node></root>",
    parse_full)
{
  doc.appendCopy(doc.child(STR("root")));
  CHECK_NODE(doc, STR("<?xml version=\"1.0\"?><!DOCTYPE id><root><?pi "
                      "value?><!--comment--><node "
                      "id=\"1\">pcdata<![CDATA[cdata]]></node></"
                      "root><root><?pi value?><!--comment--><node "
                      "id=\"1\">pcdata<![CDATA[cdata]]></node></root>"));

  doc.insertCopyBefore(doc.firstChild(), doc.firstChild());
  CHECK_NODE(doc, STR("<?xml version=\"1.0\"?><?xml version=\"1.0\"?><!DOCTYPE "
                      "id><root><?pi value?><!--comment--><node "
                      "id=\"1\">pcdata<![CDATA[cdata]]></node></"
                      "root><root><?pi value?><!--comment--><node "
                      "id=\"1\">pcdata<![CDATA[cdata]]></node></root>"));

  doc.insertCopyAfter(doc.firstChild().nextSibling().nextSibling(),
                        doc.firstChild());
  CHECK_NODE(
      doc,
      STR("<?xml version=\"1.0\"?><!DOCTYPE id><?xml "
          "version=\"1.0\"?><!DOCTYPE id><root><?pi value?><!--comment--><node "
          "id=\"1\">pcdata<![CDATA[cdata]]></node></root><root><?pi "
          "value?><!--comment--><node "
          "id=\"1\">pcdata<![CDATA[cdata]]></node></root>"));
}

TEST(dom_node_declaration_name)
{
  XmlDocument doc;
  doc.appendChild(node_declaration);

  // name 'xml' is auto-assigned
  CHECK(doc.firstChild().type() == node_declaration);
  CHECK_STRING(doc.firstChild().name(), STR("xml"));

  doc.insertChildAfter(node_declaration, doc.firstChild());
  doc.insertChildBefore(node_declaration, doc.firstChild());
  doc.prependChild(node_declaration);

  CHECK_NODE(doc, STR("<?xml?><?xml?><?xml?><?xml?>"));
}

TEST(dom_node_declaration_attributes)
{
  XmlDocument doc;
  XmlNode node = doc.appendChild(node_declaration);
  node.appendAttribute(STR("version")) = STR("1.0");
  node.appendAttribute(STR("encoding")) = STR("utf-8");

  CHECK_NODE(doc, STR("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
}

TEST(dom_node_declaration_top_level)
{
  XmlDocument doc;
  doc.appendChild().setName(STR("node"));

  XmlNode node = doc.firstChild();
  node.appendChild(node_pcdata).setValue(STR("text"));

  CHECK(node.insertChildBefore(node_declaration, node.firstChild()) ==
        XmlNode());
  CHECK(node.insertChildAfter(node_declaration, node.firstChild()) ==
        XmlNode());
  CHECK(node.appendChild(node_declaration) == XmlNode());

  CHECK_NODE(doc, STR("<node>text</node>"));

  CHECK(doc.insertChildBefore(node_declaration, node));
  CHECK(doc.insertChildAfter(node_declaration, node));
  CHECK(doc.appendChild(node_declaration));

  CHECK_NODE(doc, STR("<?xml?><node>text</node><?xml?><?xml?>"));
}

TEST(dom_node_declaration_copy)
{
  XmlDocument doc;
  doc.appendChild(node_declaration);

  doc.appendChild().setName(STR("node"));

  doc.lastChild().appendCopy(doc.firstChild());

  CHECK_NODE(doc, STR("<?xml?><node/>"));
}

TEST(dom_string_out_of_memory)
{
  const unsigned int length = 65536;
  static char_t string[length + 1];

  for (unsigned int i = 0; i < length; ++i)
    string[i] = 'a';
  string[length] = 0;

  XmlDocument doc;
  XmlNode node = doc.appendChild();
  XmlAttribute attr = node.appendAttribute(STR("a"));
  XmlNode text = node.appendChild(node_pcdata);

  // no value => long value
  test_runner::_memory_fail_threshold = 32;

  CHECK_ALLOC_FAIL(CHECK(!node.setName(string)));
  CHECK_ALLOC_FAIL(CHECK(!text.setValue(string)));
  CHECK_ALLOC_FAIL(CHECK(!attr.setName(string)));
  CHECK_ALLOC_FAIL(CHECK(!attr.setValue(string)));

  // set some names/values
  test_runner::_memory_fail_threshold = 0;

  node.setName(STR("n"));
  attr.setValue(STR("v"));
  text.setValue(STR("t"));

  // some value => long value
  test_runner::_memory_fail_threshold = 32;

  CHECK_ALLOC_FAIL(CHECK(!node.setName(string)));
  CHECK_ALLOC_FAIL(CHECK(!text.setValue(string)));
  CHECK_ALLOC_FAIL(CHECK(!attr.setName(string)));
  CHECK_ALLOC_FAIL(CHECK(!attr.setValue(string)));

  // check that original state was preserved
  test_runner::_memory_fail_threshold = 0;

  CHECK_NODE(doc, STR("<n a=\"v\">t</n>"));
}

TEST(dom_node_out_of_memory)
{
  test_runner::_memory_fail_threshold = 65536;

  // exhaust memory limit
  XmlDocument doc;

  XmlNode n = doc.appendChild();
  CHECK(n.setName(STR("n")));

  XmlAttribute a = n.appendAttribute(STR("a"));
  CHECK(a);

  CHECK_ALLOC_FAIL(while (n.appendChild(node_comment)){/* nop */});
  CHECK_ALLOC_FAIL(while (n.appendAttribute(STR("b"))){/* nop */});

  // verify all node modification operations
  CHECK_ALLOC_FAIL(CHECK(!n.appendChild()));
  CHECK_ALLOC_FAIL(CHECK(!n.prependChild()));
  CHECK_ALLOC_FAIL(CHECK(!n.insertChildAfter(node_element, n.firstChild())));
  CHECK_ALLOC_FAIL(
      CHECK(!n.insertChildBefore(node_element, n.firstChild())));
  CHECK_ALLOC_FAIL(CHECK(!n.appendAttribute(STR(""))));
  CHECK_ALLOC_FAIL(CHECK(!n.prependAttribute(STR(""))));
  CHECK_ALLOC_FAIL(CHECK(!n.insertAttributeAfter(STR(""), a)));
  CHECK_ALLOC_FAIL(CHECK(!n.insertAttributeBefore(STR(""), a)));

  // verify node copy operations
  CHECK_ALLOC_FAIL(CHECK(!n.appendCopy(n.firstChild())));
  CHECK_ALLOC_FAIL(CHECK(!n.prependCopy(n.firstChild())));
  CHECK_ALLOC_FAIL(
      CHECK(!n.insertCopyAfter(n.firstChild(), n.firstChild())));
  CHECK_ALLOC_FAIL(
      CHECK(!n.insertCopyBefore(n.firstChild(), n.firstChild())));
  CHECK_ALLOC_FAIL(CHECK(!n.appendCopy(a)));
  CHECK_ALLOC_FAIL(CHECK(!n.prependCopy(a)));
  CHECK_ALLOC_FAIL(CHECK(!n.insertCopyAfter(a, a)));
  CHECK_ALLOC_FAIL(CHECK(!n.insertCopyBefore(a, a)));
}

TEST(dom_node_memory_limit)
{
  const unsigned int length = 65536;
  static char_t string[length + 1];

  for (unsigned int i = 0; i < length; ++i)
    string[i] = 'a';
  string[length] = 0;

  test_runner::_memory_fail_threshold = 32768 * 2 + sizeof(string);

  XmlDocument doc;

  for (int j = 0; j < 32; ++j)
  {
    CHECK(doc.appendChild().setName(string));
    CHECK(doc.removeChild(doc.firstChild()));
  }
}

TEST(dom_node_memory_limit_pi)
{
  const unsigned int length = 65536;
  static char_t string[length + 1];

  for (unsigned int i = 0; i < length; ++i)
    string[i] = 'a';
  string[length] = 0;

  test_runner::_memory_fail_threshold = 32768 * 2 + sizeof(string);

  XmlDocument doc;

  for (int j = 0; j < 32; ++j)
  {
    CHECK(doc.appendChild(node_pi).setValue(string));
    CHECK(doc.removeChild(doc.firstChild()));
  }
}

TEST(dom_node_doctype_top_level)
{
  XmlDocument doc;
  doc.appendChild().setName(STR("node"));

  XmlNode node = doc.firstChild();
  node.appendChild(node_pcdata).setValue(STR("text"));

  CHECK(node.insertChildBefore(node_doctype, node.firstChild()) ==
        XmlNode());
  CHECK(node.insertChildAfter(node_doctype, node.firstChild()) == XmlNode());
  CHECK(node.appendChild(node_doctype) == XmlNode());

  CHECK_NODE(doc, STR("<node>text</node>"));

  CHECK(doc.insertChildBefore(node_doctype, node));
  CHECK(doc.insertChildAfter(node_doctype, node));
  CHECK(doc.appendChild(node_doctype));

  CHECK_NODE(doc, STR("<!DOCTYPE><node>text</node><!DOCTYPE><!DOCTYPE>"));
}

TEST(dom_node_doctype_copy)
{
  XmlDocument doc;
  doc.appendChild(node_doctype);

  doc.appendChild().setName(STR("node"));

  doc.lastChild().appendCopy(doc.firstChild());

  CHECK_NODE(doc, STR("<!DOCTYPE><node/>"));
}

TEST(dom_node_doctype_value)
{
  XmlDocument doc;
  XmlNode node = doc.appendChild(node_doctype);

  CHECK(node.type() == node_doctype);
  CHECK_STRING(node.value(), STR(""));
  CHECK_NODE(node, STR("<!DOCTYPE>"));

  CHECK(node.setValue(STR("id [ foo ]")));
  CHECK_NODE(node, STR("<!DOCTYPE id [ foo ]>"));
}

TEST_XML(dom_node_appendBuffer_native, "<node>test</node>")
{
  XmlNode node = doc.child(STR("node"));

  const char_t data1[] = STR("<child1 id='1' /><child2>text</child2>");
  const char_t data2[] = STR("<child3 />");

  CHECK(node.appendBuffer(data1, sizeof(data1)));
  CHECK(node.appendBuffer(data2, sizeof(data2)));
  CHECK(node.appendBuffer(data1, sizeof(data1)));
  CHECK(node.appendBuffer(data2, sizeof(data2)));
  CHECK(node.appendBuffer(data2, sizeof(data2)));

  CHECK_NODE(
      doc,
      STR("<node>test<child1 id=\"1\"/><child2>text</child2><child3/><child1 "
          "id=\"1\"/><child2>text</child2><child3/><child3/></node>"));
}

TEST_XML(dom_node_appendBuffer_convert, "<node>test</node>")
{
  XmlNode node = doc.child(STR("node"));

  const char data[] = {0, 0, 0, '<', 0, 0, 0, 'n', 0, 0, 0, '/', 0, 0, 0, '>'};

  CHECK(node.appendBuffer(data, sizeof(data)));
  CHECK(
      node.appendBuffer(data, sizeof(data), parse_default, encoding_utf32_be));

  CHECK_NODE(doc, STR("<node>test<n/><n/></node>"));
}

TEST_XML(dom_node_appendBuffer_remove, "<node>test</node>")
{
  XmlNode node = doc.child(STR("node"));

  const char data1[] = "<child1 id='1' /><child2>text</child2>";
  const char data2[] = "<child3 />";

  CHECK(node.appendBuffer(data1, sizeof(data1)));
  CHECK(node.appendBuffer(data2, sizeof(data2)));
  CHECK(node.appendBuffer(data1, sizeof(data1)));
  CHECK(node.appendBuffer(data2, sizeof(data2)));

  CHECK_NODE(
      doc,
      STR("<node>test<child1 id=\"1\"/><child2>text</child2><child3/><child1 "
          "id=\"1\"/><child2>text</child2><child3/></node>"));

  while (node.removeChild(STR("child2")))
  {
  }

  CHECK_NODE(doc, STR("<node>test<child1 id=\"1\"/><child3/><child1 "
                      "id=\"1\"/><child3/></node>"));

  while (node.removeChild(STR("child1")))
  {
  }

  CHECK_NODE(doc, STR("<node>test<child3/><child3/></node>"));

  while (node.removeChild(STR("child3")))
  {
  }

  CHECK_NODE(doc, STR("<node>test</node>"));

  CHECK(doc.removeChild(STR("node")));

  CHECK(!doc.firstChild());
}

TEST(dom_node_appendBuffer_empty_document)
{
  XmlDocument doc;

  const char data[] = "<child1 id='1' /><child2>text</child2>";

  doc.appendBuffer(data, sizeof(data));

  CHECK_NODE(doc, STR("<child1 id=\"1\"/><child2>text</child2>"));
}

TEST_XML(dom_node_appendBuffer_invalid_type, "<node>test</node>")
{
  const char data[] = "<child1 id='1' /><child2>text</child2>";

  CHECK(XmlNode().appendBuffer(data, sizeof(data)).status ==
        status_append_invalid_root);
  CHECK(doc.firstChild()
            .firstChild()
            .appendBuffer(data, sizeof(data))
            .status == status_append_invalid_root);
}

TEST_XML(dom_node_appendBuffer_close_external, "<node />")
{
  XmlNode node = doc.child(STR("node"));

  const char data[] = "<child1 /></node><child2 />";

  CHECK(node.appendBuffer(data, sizeof(data)).status ==
        status_end_element_mismatch);
  CHECK_NODE(doc, STR("<node><child1/></node>"));

  CHECK(node.appendBuffer(data, sizeof(data)).status ==
        status_end_element_mismatch);
  CHECK_NODE(doc, STR("<node><child1/><child1/></node>"));
}

TEST(dom_node_appendBuffer_out_of_memory_extra)
{
  test_runner::_memory_fail_threshold = 1;

  XmlDocument doc;
  CHECK_ALLOC_FAIL(
      CHECK(doc.appendBuffer("<n/>", 4).status == status_out_of_memory));
  CHECK(!doc.firstChild());
}

TEST(dom_node_appendBuffer_out_of_memory_buffer)
{
  test_runner::_memory_fail_threshold = 32768 + 128;

  char data[128] = {0};

  XmlDocument doc;
  CHECK_ALLOC_FAIL(CHECK(doc.appendBuffer(data, sizeof(data)).status ==
                         status_out_of_memory));
  CHECK(!doc.firstChild());
}

TEST(dom_node_appendBuffer_out_of_memory_nodes)
{
  unsigned int count = 4000;
  std::basic_string< char_t > data;

  for (unsigned int i = 0; i < count; ++i)
    data += STR("<a/>");

  test_runner::_memory_fail_threshold =
      32768 + 128 + data.length() * sizeof(char_t) + 32;

#ifdef PUGIXML_COMPACT
  // ... and some space for hash table
  test_runner::_memory_fail_threshold += 2048;
#endif

  XmlDocument doc;
  CHECK_ALLOC_FAIL(
      CHECK(doc.appendBuffer(data.c_str(), data.length() * sizeof(char_t),
                              parse_fragment)
                .status == status_out_of_memory));

  unsigned int valid = 0;

  for (XmlNode n = doc.firstChild(); n; n = n.nextSibling())
  {
    CHECK_STRING(n.name(), STR("a"));
    valid++;
  }

  CHECK(valid > 0 && valid < count);
}

TEST(dom_node_appendBuffer_out_of_memory_name)
{
  test_runner::_memory_fail_threshold = 32768 + 4096;

  char data[4096] = {0};

  XmlDocument doc;
  CHECK(doc.appendChild(STR("root")));
  CHECK_ALLOC_FAIL(
      CHECK(doc.firstChild().appendBuffer(data, sizeof(data)).status ==
            status_out_of_memory));
  CHECK_STRING(doc.firstChild().name(), STR("root"));
}

TEST_XML(dom_node_appendBuffer_fragment, "<node />")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.appendBuffer("1", 1).status == status_no_document_element);
  CHECK_NODE(doc, STR("<node>1</node>"));

  CHECK(node.appendBuffer("2", 1, parse_fragment));
  CHECK_NODE(doc, STR("<node>12</node>"));

  CHECK(node.appendBuffer("3", 1).status == status_no_document_element);
  CHECK_NODE(doc, STR("<node>123</node>"));

  CHECK(node.appendBuffer("4", 1, parse_fragment));
  CHECK_NODE(doc, STR("<node>1234</node>"));
}

TEST_XML(dom_node_appendBuffer_empty, "<node />")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(node.appendBuffer("", 0).status == status_no_document_element);
  CHECK(node.appendBuffer("", 0, parse_fragment).status == status_ok);

  CHECK(node.appendBuffer(0, 0).status == status_no_document_element);
  CHECK(node.appendBuffer(0, 0, parse_fragment).status == status_ok);

  CHECK_NODE(doc, STR("<node/>"));
}

TEST_XML(dom_node_prependMove, "<node>foo<child/></node>")
{
  XmlNode child = doc.child(STR("node")).child(STR("child"));

  CHECK(XmlNode().prependMove(XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().prependMove(child) == XmlNode());
  CHECK(doc.prependMove(doc) == XmlNode());
  CHECK(doc.prependMove(XmlNode()) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node")).prependMove(doc.child(STR("node")).firstChild());
  CHECK(n1 && n1 == doc.child(STR("node")).firstChild());
  CHECK_STRING(n1.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foo<child/></node>"));

  XmlNode n2 = doc.child(STR("node"))
                   .prependMove(doc.child(STR("node")).child(STR("child")));
  CHECK(n2 && n2 != n1 && n2 == child);
  CHECK_STRING(n2.name(), STR("child"));
  CHECK_NODE(doc, STR("<node><child/>foo</node>"));

  XmlNode n3 =
      doc.child(STR("node"))
          .child(STR("child"))
          .prependMove(doc.child(STR("node")).firstChild().nextSibling());
  CHECK(n3 && n3 == n1 && n3 != n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child>foo</child></node>"));
}

TEST_XML(dom_node_appendMove, "<node>foo<child/></node>")
{
  XmlNode child = doc.child(STR("node")).child(STR("child"));

  CHECK(XmlNode().appendMove(XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node")).firstChild().appendMove(child) == XmlNode());
  CHECK(doc.appendMove(doc) == XmlNode());
  CHECK(doc.appendMove(XmlNode()) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node")).appendMove(doc.child(STR("node")).firstChild());
  CHECK(n1 && n1 == doc.child(STR("node")).lastChild());
  CHECK_STRING(n1.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child/>foo</node>"));

  XmlNode n2 =
      doc.child(STR("node")).appendMove(doc.child(STR("node")).lastChild());
  CHECK(n2 && n2 == n1);
  CHECK_STRING(n2.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child/>foo</node>"));

  XmlNode n3 = doc.child(STR("node"))
                   .child(STR("child"))
                   .appendMove(doc.child(STR("node")).lastChild());
  CHECK(n3 && n3 == n1 && n3 == n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child>foo</child></node>"));
}

TEST_XML(dom_node_insertMoveAfter, "<node>foo<child>bar</child></node>")
{
  XmlNode child = doc.child(STR("node")).child(STR("child"));

  CHECK(XmlNode().insertMoveAfter(XmlNode(), XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertMoveAfter(doc.child(STR("node")),
                               doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertMoveAfter(doc, doc) == XmlNode());
  CHECK(doc.insertMoveAfter(XmlNode(), doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertMoveAfter(doc.child(STR("node")), XmlNode()) == XmlNode());
  CHECK(doc.insertMoveAfter(doc.child(STR("node")), child) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node"))
          .insertMoveAfter(child, doc.child(STR("node")).firstChild());
  CHECK(n1 && n1 == child);
  CHECK_STRING(n1.name(), STR("child"));
  CHECK_NODE(doc, STR("<node>foo<child>bar</child></node>"));

  XmlNode n2 =
      doc.child(STR("node"))
          .insertMoveAfter(doc.child(STR("node")).firstChild(), child);
  CHECK(n2 && n2 != n1);
  CHECK_STRING(n2.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child>bar</child>foo</node>"));

  XmlNode n3 = child.insertMoveAfter(doc.child(STR("node")).lastChild(),
                                       child.firstChild());
  CHECK(n3 && n3 != n1 && n3 == n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child>barfoo</child></node>"));
}

TEST_XML(dom_node_insertMoveBefore, "<node>foo<child>bar</child></node>")
{
  XmlNode child = doc.child(STR("node")).child(STR("child"));

  CHECK(XmlNode().insertMoveBefore(XmlNode(), XmlNode()) == XmlNode());
  CHECK(doc.child(STR("node"))
            .firstChild()
            .insertMoveBefore(doc.child(STR("node")),
                                doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertMoveBefore(doc, doc) == XmlNode());
  CHECK(doc.insertMoveBefore(XmlNode(), doc.child(STR("node"))) == XmlNode());
  CHECK(doc.insertMoveBefore(doc.child(STR("node")), XmlNode()) == XmlNode());
  CHECK(doc.insertMoveBefore(doc.child(STR("node")), child) == XmlNode());

  XmlNode n1 =
      doc.child(STR("node"))
          .insertMoveBefore(child, doc.child(STR("node")).firstChild());
  CHECK(n1 && n1 == child);
  CHECK_STRING(n1.name(), STR("child"));
  CHECK_NODE(doc, STR("<node><child>bar</child>foo</node>"));

  XmlNode n2 =
      doc.child(STR("node"))
          .insertMoveBefore(doc.child(STR("node")).lastChild(), child);
  CHECK(n2 && n2 != n1);
  CHECK_STRING(n2.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node>foo<child>bar</child></node>"));

  XmlNode n3 = child.insertMoveBefore(doc.child(STR("node")).firstChild(),
                                        child.firstChild());
  CHECK(n3 && n3 != n1 && n3 == n2);
  CHECK_STRING(n3.value(), STR("foo"));
  CHECK_NODE(doc, STR("<node><child>foobar</child></node>"));
}

TEST_XML(dom_node_move_recursive, "<root><node>foo<child/></node></root>")
{
  XmlNode root = doc.child(STR("root"));
  XmlNode node = root.child(STR("node"));
  XmlNode foo = node.firstChild();
  XmlNode child = node.lastChild();

  CHECK(node.prependMove(node) == XmlNode());
  CHECK(node.prependMove(root) == XmlNode());

  CHECK(node.appendMove(node) == XmlNode());
  CHECK(node.appendMove(root) == XmlNode());

  CHECK(node.insertMoveBefore(node, foo) == XmlNode());
  CHECK(node.insertMoveBefore(root, foo) == XmlNode());

  CHECK(node.insertMoveAfter(node, foo) == XmlNode());
  CHECK(node.insertMoveAfter(root, foo) == XmlNode());

  CHECK(child.appendMove(node) == XmlNode());

  CHECK_NODE(doc, STR("<root><node>foo<child/></node></root>"));
}

TEST_XML(dom_node_move_marker, "<node />")
{
  XmlNode node = doc.child(STR("node"));

  CHECK(doc.insertMoveBefore(node, node) == XmlNode());
  CHECK(doc.insertMoveAfter(node, node) == XmlNode());

  CHECK_NODE(doc, STR("<node/>"));
}

TEST_XML(dom_node_move_crossdoc, "<node/>")
{
  XmlDocument newdoc;
  CHECK(newdoc.appendMove(doc.child(STR("node"))) == XmlNode());
  CHECK_NODE(newdoc, STR(""));
}

TEST_XML(dom_node_move_tree,
         "<root><n1 a1='v1'><c1/>t1</n1><n2 a2='v2'><c2/>t2</n2><n3 "
         "a3='v3'><c3/>t3</n3><n4 a4='v4'><c4/>t4</n4></root>")
{
  XmlNode root = doc.child(STR("root"));
  XmlNode n1 = root.child(STR("n1"));
  XmlNode n2 = root.child(STR("n2"));
  XmlNode n3 = root.child(STR("n3"));
  XmlNode n4 = root.child(STR("n4"));

  // n2 n1 n3 n4
  CHECK(n2 == root.prependMove(n2));

  // n2 n3 n4 n1
  CHECK(n1 == root.appendMove(n1));

  // n2 n4 n3 n1
  CHECK(n4 == root.insertMoveBefore(n4, n3));

  // n2 n4 n1 + n3
  CHECK(n3 == doc.insertMoveAfter(n3, root));

  CHECK_NODE(
      doc, STR("<root><n2 a2=\"v2\"><c2/>t2</n2><n4 a4=\"v4\"><c4/>t4</n4><n1 "
               "a1=\"v1\"><c1/>t1</n1></root><n3 a3=\"v3\"><c3/>t3</n3>"));

  CHECK(n1 == root.child(STR("n1")));
  CHECK(n2 == root.child(STR("n2")));
  CHECK(n3 == doc.child(STR("n3")));
  CHECK(n4 == root.child(STR("n4")));
}

TEST(dom_node_copy_stackless)
{
  unsigned int count = 20000;
  std::basic_string< char_t > data;

  for (unsigned int i = 0; i < count; ++i)
    data += STR("<a>");

  data += STR("text");

  for (unsigned int j = 0; j < count; ++j)
    data += STR("</a>");

  XmlDocument doc;
  CHECK(doc.loadString(data.c_str()));

  XmlDocument copy;
  CHECK(copy.appendCopy(doc.firstChild()));

  CHECK_NODE(doc, data.c_str());
}

TEST(dom_node_removeChild_stackless)
{
  unsigned int count = 20000;
  std::basic_string< char_t > data = STR("<root>");

  for (unsigned int i = 0; i < count; ++i)
    data += STR("<a>");

  data += STR("text");

  for (unsigned int j = 0; j < count; ++j)
    data += STR("</a>");

  data += STR("<tail/></root>");

  XmlDocument doc;
  CHECK(doc.loadString(data.c_str()));

  XmlNode root = doc.child(STR("root"));
  CHECK(root.removeChild(root.child(STR("a"))));

  CHECK_NODE(doc, STR("<root><tail/></root>"));
}

TEST(dom_node_copy_copyless)
{
  std::basic_string< char_t > data;
  data += STR("<node>");
  for (int i = 0; i < 10000; ++i)
    data += STR("pcdata");
  data += STR("<?name value?><child attr1=\"\" attr2=\"value2\"/></node>");

  std::basic_string< char_t > datacopy = data;

  // the document is parsed in-place so there should only be 1 page worth of
  // allocations
  test_runner::_memory_fail_threshold = 32768 + 128;

#ifdef PUGIXML_COMPACT
  // ... and some space for hash table
  test_runner::_memory_fail_threshold += 2048;
#endif

  XmlDocument doc;
  CHECK(doc.loadBufferInplace(&datacopy[0], datacopy.size() * sizeof(char_t),
                              parse_full));

  // this copy should share all string storage; since there are not a lot of
  // nodes we should not have *any* allocations here (everything will fit in the
  // same page in the document)
  XmlNode copy = doc.appendCopy(doc.child(STR("node")));
  XmlNode copy2 = doc.appendCopy(copy);

  CHECK_NODE(copy, data.c_str());
  CHECK_NODE(copy2, data.c_str());
}

TEST(dom_node_copy_copyless_mix)
{
  XmlDocument doc;
  CHECK(doc.loadString(STR("<node>pcdata<?name value?><child attr1=\"\" "
                           "attr2=\"value2\" /></node>"),
                       parse_full));

  XmlNode child = doc.child(STR("node")).child(STR("child"));

  child.setName(STR("copychild"));
  child.attribute(STR("attr2")).setName(STR("copyattr2"));
  child.attribute(STR("attr1")).setValue(STR("copyvalue1"));

  std::basic_string< char_t > data;
  for (int i = 0; i < 10000; ++i)
    data += STR("pcdata");

  doc.child(STR("node")).text().set(data.c_str());

  XmlNode copy = doc.appendCopy(doc.child(STR("node")));
  XmlNode copy2 = doc.appendCopy(copy);

  std::basic_string< char_t > dataxml;
  dataxml += STR("<node>");
  dataxml += data;
  dataxml += STR("<?name value?><copychild attr1=\"copyvalue1\" "
                 "copyattr2=\"value2\"/></node>");

  CHECK_NODE(copy, dataxml.c_str());
  CHECK_NODE(copy2, dataxml.c_str());
}

TEST_XML(dom_node_copy_copyless_taint, "<node attr=\"value\" />")
{
  XmlNode node = doc.child(STR("node"));
  XmlNode copy = doc.appendCopy(node);

  CHECK_NODE(doc, STR("<node attr=\"value\"/><node attr=\"value\"/>"));

  node.setName(STR("nod1"));

  CHECK_NODE(doc, STR("<nod1 attr=\"value\"/><node attr=\"value\"/>"));

  XmlNode copy2 = doc.appendCopy(copy);

  CHECK_NODE(doc, STR("<nod1 attr=\"value\"/><node attr=\"value\"/><node "
                      "attr=\"value\"/>"));

  copy.attribute(STR("attr")).setValue(STR("valu2"));

  CHECK_NODE(doc, STR("<nod1 attr=\"value\"/><node attr=\"valu2\"/><node "
                      "attr=\"value\"/>"));

  copy2.attribute(STR("attr")).setName(STR("att3"));

  CHECK_NODE(doc, STR("<nod1 attr=\"value\"/><node attr=\"valu2\"/><node "
                      "att3=\"value\"/>"));
}

TEST(dom_node_copy_attribute_copyless)
{
  std::basic_string< char_t > data;
  data += STR("<node attr=\"");
  for (int i = 0; i < 10000; ++i)
    data += STR("data");
  data += STR("\"/>");

  std::basic_string< char_t > datacopy = data;

  // the document is parsed in-place so there should only be 1 page worth of
  // allocations
  test_runner::_memory_fail_threshold = 32768 + 128;

#ifdef PUGIXML_COMPACT
  // ... and some space for hash table
  test_runner::_memory_fail_threshold += 2048;
#endif

  XmlDocument doc;
  CHECK(doc.loadBufferInplace(&datacopy[0], datacopy.size() * sizeof(char_t),
                              parse_full));

  // this copy should share all string storage; since there are not a lot of
  // nodes we should not have *any* allocations here (everything will fit in the
  // same page in the document)
  XmlNode copy1 = doc.appendChild(STR("node"));
  copy1.appendCopy(doc.firstChild().firstAttribute());

  XmlNode copy2 = doc.appendChild(STR("node"));
  copy2.appendCopy(copy1.lastAttribute());

  CHECK_NODE(copy1, data.c_str());
  CHECK_NODE(copy2, data.c_str());
}

TEST_XML(dom_node_copy_attribute_copyless_taint, "<node attr=\"value\" />")
{
  XmlNode node = doc.child(STR("node"));
  XmlAttribute attr = node.firstAttribute();

  XmlNode copy1 = doc.appendChild(STR("copy1"));
  XmlNode copy2 = doc.appendChild(STR("copy2"));
  XmlNode copy3 = doc.appendChild(STR("copy3"));

  CHECK_NODE(doc, STR("<node attr=\"value\"/><copy1/><copy2/><copy3/>"));

  copy1.appendCopy(attr);

  CHECK_NODE(
      doc,
      STR("<node attr=\"value\"/><copy1 attr=\"value\"/><copy2/><copy3/>"));

  attr.setName(STR("att1"));
  copy2.appendCopy(attr);

  CHECK_NODE(doc, STR("<node att1=\"value\"/><copy1 attr=\"value\"/><copy2 "
                      "att1=\"value\"/><copy3/>"));

  copy1.firstAttribute().setValue(STR("valu2"));
  copy3.appendCopy(copy1.firstAttribute());

  CHECK_NODE(doc, STR("<node att1=\"value\"/><copy1 attr=\"valu2\"/><copy2 "
                      "att1=\"value\"/><copy3 attr=\"valu2\"/>"));
}

TEST_XML(
    dom_node_copy_out_of_memory_node,
    "<node><child1 /><child2 /><child3>text1<child4 />text2</child3></node>")
{
  test_runner::_memory_fail_threshold = 32768 * 2 + 4096;

  XmlDocument copy;
  CHECK_ALLOC_FAIL(for (int i = 0; i < 1000; ++i)
                       copy.appendCopy(doc.firstChild()));
}

TEST_XML(dom_node_copy_out_of_memory_attr,
         "<node attr1='' attr2='' attr3='' attr4='' attr5='' attr6='' attr7='' "
         "attr8='' attr9='' attr10='' attr11='' attr12='' attr13='' attr14='' "
         "attr15='' />")
{
  test_runner::_memory_fail_threshold = 32768 * 2 + 4096;

  XmlDocument copy;
  CHECK_ALLOC_FAIL(for (int i = 0; i < 1000; ++i)
                       copy.appendCopy(doc.firstChild()));
}

TEST_XML(dom_node_remove_deallocate, "<node attr='value'>text</node>")
{
  XmlNode node = doc.child(STR("node"));

  XmlAttribute attr = node.attribute(STR("attr"));
  attr.setName(STR("longattr"));
  attr.setValue(STR("longvalue"));

  node.setName(STR("longnode"));
  node.text().set(STR("longtext"));

  node.removeAttribute(attr);
  doc.removeChild(node);

  CHECK_NODE(doc, STR(""));
}

TEST_XML(dom_node_set_deallocate, "<node attr='value'>text</node>")
{
  XmlNode node = doc.child(STR("node"));

  XmlAttribute attr = node.attribute(STR("attr"));

  attr.setName(STR("longattr"));
  attr.setValue(STR("longvalue"));
  node.setName(STR("longnode"));

  attr.setName(STR(""));
  attr.setValue(STR(""));
  node.setName(STR(""));
  node.text().set(STR(""));

  CHECK_NODE(doc, STR("<:anonymous :anonymous=\"\"/>"));
}

TEST(dom_node_copy_declaration_empty_name)
{
  XmlDocument doc1;
  XmlNode decl1 = doc1.appendChild(node_declaration);
  decl1.setName(STR(""));

  XmlDocument doc2;
  XmlNode decl2 = doc2.appendCopy(decl1);

  CHECK_STRING(decl2.name(), STR(""));
}

template < typename T > bool fp_equal(T lhs, T rhs)
{
  // Several compilers compare float/double values on x87 stack without proper
  // rounding This causes roundtrip tests to fail, although they correctly
  // preserve the data.
#if (defined(_MSC_VER) && _MSC_VER < 1400) || defined(__MWERKS__)
  return memcmp(&lhs, &rhs, sizeof(T)) == 0;
#else
  return lhs == rhs;
#endif
}

TEST(dom_fp_roundtrip_min_max_float)
{
  XmlDocument doc;
  XmlNode node = doc.appendChild(STR("node"));
  XmlAttribute attr = node.appendAttribute(STR("attr"));

  node.text().set(std::numeric_limits< float >::min());
  CHECK(fp_equal(node.text().toFloat(), std::numeric_limits< float >::min()));

  attr.setValue(std::numeric_limits< float >::max());
  CHECK(fp_equal(attr.toFloat(), std::numeric_limits< float >::max()));
}

TEST(dom_fp_roundtrip_min_max_double)
{
  XmlDocument doc;
  XmlNode node = doc.appendChild(STR("node"));
  XmlAttribute attr = node.appendAttribute(STR("attr"));

  attr.setValue(std::numeric_limits< double >::min());
  CHECK(fp_equal(attr.toDouble(), std::numeric_limits< double >::min()));

  node.text().set(std::numeric_limits< double >::max());
  CHECK(
      fp_equal(node.text().toDouble(), std::numeric_limits< double >::max()));
}

TEST(dom_fp_double_custom_precision)
{
  XmlDocument doc;
  XmlNode node = doc.appendChild(STR("node"));
  XmlAttribute attr = node.appendAttribute(STR("attr"));

  attr.setValue(std::numeric_limits< double >::min(), 20);
  CHECK(fp_equal(attr.toDouble(), std::numeric_limits< double >::min()));

  attr.setValue(1.0f, 5);
  CHECK(fp_equal(attr.toDouble(), static_cast< double >(1.0f)));

  attr.setValue(3.1415926f, 3);
  CHECK(!fp_equal(attr.toDouble(), static_cast< double >(3.1415926f)));

  node.text().set(1.0f, 5);
  CHECK(fp_equal(node.text().toDouble(), static_cast< double >(1.0f)));

  node.text().set(3.1415926f, 3);
  CHECK(!fp_equal(node.text().toDouble(), static_cast< double >(3.1415926f)));

  node.text().set(std::numeric_limits< double >::max(), 20);
  CHECK(
      fp_equal(node.text().toDouble(), std::numeric_limits< double >::max()));
}

const double fp_roundtrip_base[] = {
    0.31830988618379067154, 0.43429448190325182765, 0.57721566490153286061,
    0.69314718055994530942, 0.70710678118654752440, 0.78539816339744830962,
};

TEST(dom_fp_roundtrip_float)
{
  XmlDocument doc;

  for (int e = -125; e <= 128; ++e)
  {
    for (size_t i = 0;
         i < sizeof(fp_roundtrip_base) / sizeof(fp_roundtrip_base[0]); ++i)
    {
      float value = static_cast< float >(ldexp(fp_roundtrip_base[i], e));

      doc.text().set(value);
      CHECK(fp_equal(doc.text().toFloat(), value));
    }
  }
}

// Borland C does not print double values with enough precision
#ifndef __BORLANDC__
TEST(dom_fp_roundtrip_double)
{
  XmlDocument doc;

  for (int e = -1021; e <= 1024; ++e)
  {
    for (size_t i = 0;
         i < sizeof(fp_roundtrip_base) / sizeof(fp_roundtrip_base[0]); ++i)
    {
#if (defined(_MSC_VER) && _MSC_VER < 1400) || defined(__MWERKS__)
      // Not all runtime libraries guarantee roundtripping for denormals
      if (e == -1021 && fp_roundtrip_base[i] < 0.5)
        continue;
#endif

#ifdef __DMC__
      // Digital Mars C does not roundtrip on exactly one combination
      if (e == -12 && i == 1)
        continue;
#endif

      double value = ldexp(fp_roundtrip_base[i], e);

      doc.text().set(value);
      CHECK(fp_equal(doc.text().toDouble(), value));
    }
  }
}
#endif
