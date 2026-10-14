#include "test.hpp"

#include "writer_string.hpp"

using namespace pugi;

TEST(parse_pi_skip)
{
	XmlDocument doc;

	unsigned int flag_sets[] = {parse_fragment, parse_fragment | parse_declaration};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = flag_sets[i];

		CHECK(doc.loadString(STR("<?pi?><?pi value?>"), flags));
		CHECK(!doc.firstChild());

		CHECK(doc.loadString(STR("<?pi <tag/> value?>"), flags));
		CHECK(!doc.firstChild());
	}
}

TEST(parse_pi_parse)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<?pi1?><?pi2 value?>"), parse_fragment | parse_pi));

	XmlNode pi1 = doc.firstChild();
	XmlNode pi2 = doc.lastChild();

	CHECK(pi1 != pi2);
	CHECK(pi1.type() == node_pi);
	CHECK_STRING(pi1.name(), STR("pi1"));
	CHECK_STRING(pi1.value(), STR(""));
	CHECK(pi2.type() == node_pi);
	CHECK_STRING(pi2.name(), STR("pi2"));
	CHECK_STRING(pi2.value(), STR("value"));
}

TEST(parse_pi_parse_spaces)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<?target  \r\n\t  value ?>"), parse_fragment | parse_pi));

	XmlNode pi = doc.firstChild();

	CHECK(pi.type() == node_pi);
	CHECK_STRING(pi.name(), STR("target"));
	CHECK_STRING(pi.value(), STR("value "));
}

TEST(parse_pi_error)
{
	XmlDocument doc;

	unsigned int flag_sets[] = {parse_fragment, parse_fragment | parse_pi};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = flag_sets[i];

		CHECK(doc.loadString(STR("<?"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<??"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?>"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?#?>"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name>"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name ?"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name?"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name? "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name?  "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name  "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name   "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value  "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value  ?"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value  ? "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value  ? >"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name value  ? > "), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name&"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?name&?"), flags).status == status_bad_pi);
	}

	CHECK(doc.loadString(STR("<?xx#?>"), parse_fragment | parse_pi).status == status_bad_pi);
	CHECK(doc.loadString(STR("<?name&?>"), parse_fragment | parse_pi).status == status_bad_pi);
	CHECK(doc.loadString(STR("<?name& x?>"), parse_fragment | parse_pi).status == status_bad_pi);
}

TEST(parse_pi_error_buffer_boundary)
{
	char buf1[] = "<?name?>";
	char buf2[] = "<?name?x";

	XmlDocument doc;
	CHECK(doc.loadBufferInplace(buf1, 8, parse_fragment | parse_pi));
	CHECK(doc.loadBufferInplace(buf2, 8, parse_fragment | parse_pi).status == status_bad_pi);
}

TEST(parse_comments_skip)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<!----><!--value-->"), parse_fragment));
	CHECK(!doc.firstChild());
}

TEST(parse_comments_parse)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<!----><!--value-->"), parse_fragment | parse_comments));

	XmlNode c1 = doc.firstChild();
	XmlNode c2 = doc.lastChild();

	CHECK(c1 != c2);
	CHECK(c1.type() == node_comment);
	CHECK_STRING(c1.name(), STR(""));
	CHECK_STRING(c1.value(), STR(""));
	CHECK(c2.type() == node_comment);
	CHECK_STRING(c2.name(), STR(""));
	CHECK_STRING(c2.value(), STR("value"));
}

TEST(parse_comments_parse_no_eol)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<!--\r\rval1\rval2\r\nval3\nval4\r\r-->"), parse_fragment | parse_comments));

	XmlNode c = doc.firstChild();
	CHECK(c.type() == node_comment);
	CHECK_STRING(c.value(), STR("\r\rval1\rval2\r\nval3\nval4\r\r"));
}

TEST(parse_comments_parse_eol)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<!--\r\rval1\rval2\r\nval3\nval4\r\r-->"), parse_fragment | parse_comments | parse_eol));

	XmlNode c = doc.firstChild();
	CHECK(c.type() == node_comment);
	CHECK_STRING(c.value(), STR("\n\nval1\nval2\nval3\nval4\n\n"));
}

TEST(parse_comments_error)
{
	XmlDocument doc;

	unsigned int flag_sets[] = {parse_fragment, parse_fragment | parse_comments, parse_fragment | parse_comments | parse_eol};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = flag_sets[i];

		CHECK(doc.loadString(STR("<!-"), flags).status == status_bad_comment);
		CHECK(doc.loadString(STR("<!--"), flags).status == status_bad_comment);
		CHECK(doc.loadString(STR("<!--v"), flags).status == status_bad_comment);
		CHECK(doc.loadString(STR("<!-->"), flags).status == status_bad_comment);
		CHECK(doc.loadString(STR("<!--->"), flags).status == status_bad_comment);
		CHECK(doc.loadString(STR("<!-- <!-- --><!- -->"), flags).status == status_bad_comment);
	}
}

TEST(parse_cdata_skip)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<![CDATA[]]><![CDATA[value]]>"), parse_fragment));
	CHECK(!doc.firstChild());
}

TEST(parse_cdata_skip_contents)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node><![CDATA[]]>hello<![CDATA[value]]>, world!</node>"), parse_fragment));
	CHECK_NODE(doc, STR("<node>hello, world!</node>"));
}

TEST(parse_cdata_parse)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<![CDATA[]]><![CDATA[value]]>"), parse_fragment | parse_cdata));

	XmlNode c1 = doc.firstChild();
	XmlNode c2 = doc.lastChild();

	CHECK(c1 != c2);
	CHECK(c1.type() == node_cdata);
	CHECK_STRING(c1.name(), STR(""));
	CHECK_STRING(c1.value(), STR(""));
	CHECK(c2.type() == node_cdata);
	CHECK_STRING(c2.name(), STR(""));
	CHECK_STRING(c2.value(), STR("value"));
}

TEST(parse_cdata_parse_no_eol)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<![CDATA[\r\rval1\rval2\r\nval3\nval4\r\r]]>"), parse_fragment | parse_cdata));

	XmlNode c = doc.firstChild();
	CHECK(c.type() == node_cdata);
	CHECK_STRING(c.value(), STR("\r\rval1\rval2\r\nval3\nval4\r\r"));
}

TEST(parse_cdata_parse_eol)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<![CDATA[\r\rval1\rval2\r\nval3\nval4\r\r]]>"), parse_fragment | parse_cdata | parse_eol));

	XmlNode c = doc.firstChild();
	CHECK(c.type() == node_cdata);
	CHECK_STRING(c.value(), STR("\n\nval1\nval2\nval3\nval4\n\n"));
}

TEST(parse_cdata_error)
{
	XmlDocument doc;

	unsigned int flag_sets[] = {parse_fragment, parse_fragment | parse_cdata, parse_fragment | parse_cdata | parse_eol};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = flag_sets[i];

		CHECK(doc.loadString(STR("<!["), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![C"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CD"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDA"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDAT"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA["), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA[]"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA[data"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA[data]"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA[data]]"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA[>"), flags).status == status_bad_cdata);
		CHECK(doc.loadString(STR("<![CDATA[ <![CDATA[]]><![CDATA ]]>"), flags).status == status_bad_cdata);
	}
}

TEST(parse_ws_pcdata_skip)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("  "), parse_fragment));
	CHECK(!doc.firstChild());

	CHECK(doc.loadString(STR("<root>  <node>  </node>  </root>"), parse_minimal));

	XmlNode root = doc.child(STR("root"));

	CHECK(root.firstChild() == root.lastChild());
	CHECK(!root.firstChild().firstChild());
}

TEST(parse_ws_pcdata_parse)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<root>  <node>  </node>  </root>"), parse_minimal | parse_ws_pcdata));

	XmlNode root = doc.child(STR("root"));

	XmlNode c1 = root.firstChild();
	XmlNode c2 = c1.nextSibling();
	XmlNode c3 = c2.nextSibling();

	CHECK(c3 == root.lastChild());

	CHECK(c1.type() == node_pcdata);
	CHECK_STRING(c1.value(), STR("  "));
	CHECK(c3.type() == node_pcdata);
	CHECK_STRING(c3.value(), STR("  "));

	CHECK(c2.firstChild() == c2.lastChild());
	CHECK(c2.firstChild().type() == node_pcdata);
	CHECK_STRING(c2.firstChild().value(), STR("  "));
}

static int get_tree_node_count(XmlNode n)
{
    int result = 1;

    for (XmlNode c = n.firstChild(); c; c = c.nextSibling())
        result += get_tree_node_count(c);

    return result;
}

TEST(parse_ws_pcdata_permutations)
{
    struct test_data_t
    {
        unsigned int mask; // 1 = default flags, 2 = parse_ws_pcdata, 4 = parse_ws_pcdata_single
        const char_t* source;
        const char_t* result;
        int nodes; // negative if parsing should fail
    };

    test_data_t test_data[] =
    {
        // external pcdata should be discarded (whitespace or not)
        {7, STR("ext1<node/>"), STR("<node/>"), 2},
        {7, STR("ext1<node/>ext2"), STR("<node/>"), 2},
        {7, STR(" <node/>"), STR("<node/>"), 2},
        {7, STR("<node/> "), STR("<node/>"), 2},
        {7, STR(" <node/> "), STR("<node/>"), 2},
        // inner pcdata should be preserved
        {7, STR("<node>inner</node>"), STR("<node>inner</node>"), 3},
        {7, STR("<node>inner1<child/>inner2</node>"), STR("<node>inner1<child/>inner2</node>"), 5},
        {7, STR("<node>inner1<child>deep</child>inner2</node>"), STR("<node>inner1<child>deep</child>inner2</node>"), 6},
        // empty pcdata nodes should never be created
        {7, STR("<node>inner1<child></child>inner2</node>"), STR("<node>inner1<child/>inner2</node>"), 5},
        {7, STR("<node><child></child>inner2</node>"), STR("<node><child/>inner2</node>"), 4},
        {7, STR("<node>inner1<child></child></node>"), STR("<node>inner1<child/></node>"), 4},
        {7, STR("<node><child></child></node>"), STR("<node><child/></node>"), 3},
        // comments, pi or other nodes should not cause pcdata creation either
        {7, STR("<node><!----><child><?pi?></child><![CDATA[x]]></node>"), STR("<node><child/><![CDATA[x]]></node>"), 4},
        // leading/trailing pcdata whitespace should be preserved (note: this will change if parse_ws_pcdata_trim is introduced)
        {7, STR("<node>\t \tinner1<child> deep   </child>\t\ninner2\n\t</node>"), STR("<node>\t \tinner1<child> deep   </child>\t\ninner2\n\t</node>"), 6},
        // whitespace-only pcdata preservation depends on the parsing mode
        {1, STR("<node>\n\t<child>   </child>\n\t<child> <deep>  </deep> </child>\n\t<!---->\n\t</node>"), STR("<node><child/><child><deep/></child></node>"), 5},
        {2, STR("<node>\n\t<child>   </child>\n\t<child> <deep>  </deep> </child>\n\t<!---->\n\t</node>"), STR("<node>\n\t<child>   </child>\n\t<child> <deep>  </deep> </child>\n\t\n\t</node>"), 13},
        {4, STR("<node>\n\t<child>   </child>\n\t<child> <deep>  </deep> </child>\n\t<!---->\n\t</node>"), STR("<node><child>   </child><child><deep>  </deep></child></node>"), 7},
        // current implementation of parse_ws_pcdata_single has an unfortunate bug; reproduce it here
        {4, STR("<node>\t\t<!---->\n\n</node>"), STR("<node>\n\n</node>"), 3},
        // error case: terminate PCDATA in the middle
        {7, STR("<node>abcdef"), STR("<node>abcdef</node>"), -3},
        {5, STR("<node>      "), STR("<node/>"), -2},
        {2, STR("<node>      "), STR("<node>      </node>"), -3},
        // error case: terminate PCDATA as early as possible
        {7, STR("<node>"), STR("<node/>"), -2},
        {7, STR("<node>a"), STR("<node>a</node>"), -3},
        {5, STR("<node> "), STR("<node/>"), -2},
        {2, STR("<node> "), STR("<node> </node>"), -3},
    };

    for (size_t i = 0; i < sizeof(test_data) / sizeof(test_data[0]); ++i)
    {
        const test_data_t& td = test_data[i];

        for (int flag = 0; flag < 3; ++flag)
        {
            if (td.mask & (1 << flag))
            {
                unsigned int flags[] = {parse_default, parse_default | parse_ws_pcdata, parse_default | parse_ws_pcdata_single};

                XmlDocument doc;
                CHECK((td.nodes > 0) == doc.loadString(td.source, flags[flag]));
                CHECK_NODE(doc, td.result);

                int nodes = get_tree_node_count(doc);
                CHECK((td.nodes < 0 ? -td.nodes : td.nodes) == nodes);
            }
        }
    }
}

TEST(parse_ws_pcdata_fragment_permutations)
{
    struct test_data_t
    {
        unsigned int mask; // 1 = default flags, 2 = parse_ws_pcdata, 4 = parse_ws_pcdata_single
        const char_t* source;
        const char_t* result;
        int nodes; // negative if parsing should fail
    };

    test_data_t test_data[] =
    {
        // external pcdata should be preserved
        {7, STR("ext1"), STR("ext1"), 2},
        {5, STR("    "), STR(""), 1},
        {2, STR("    "), STR("    "), 2},
        {7, STR("ext1<node/>"), STR("ext1<node/>"), 3},
        {7, STR("<node/>ext2"), STR("<node/>ext2"), 3},
        {7, STR("ext1<node/>ext2"), STR("ext1<node/>ext2"), 4},
        {7, STR("ext1<node1/>ext2<node2/>ext3"), STR("ext1<node1/>ext2<node2/>ext3"), 6},
        {5, STR(" <node/>"), STR("<node/>"), 2},
        {2, STR(" <node/>"), STR(" <node/>"), 3},
        {5, STR("<node/> "), STR("<node/>"), 2},
        {2, STR("<node/> "), STR("<node/> "), 3},
        {5, STR(" <node/> "), STR("<node/>"), 2},
        {2, STR(" <node/> "), STR(" <node/> "), 4},
        {5, STR(" <node1/> <node2/> "), STR("<node1/><node2/>"), 3},
        {2, STR(" <node1/> <node2/> "), STR(" <node1/> <node2/> "), 6},
    };

    for (size_t i = 0; i < sizeof(test_data) / sizeof(test_data[0]); ++i)
    {
        const test_data_t& td = test_data[i];

        for (int flag = 0; flag < 3; ++flag)
        {
            if (td.mask & (1 << flag))
            {
                unsigned int flags[] = {parse_default, parse_default | parse_ws_pcdata, parse_default | parse_ws_pcdata_single};

                XmlDocument doc;
                CHECK((td.nodes > 0) == doc.loadString(td.source, flags[flag] | parse_fragment));
                CHECK_NODE(doc, td.result);

                int nodes = get_tree_node_count(doc);
                CHECK((td.nodes < 0 ? -td.nodes : td.nodes) == nodes);
            }
        }
    }
}

TEST(parse_pcdata_no_eol)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<root>\r\rval1\rval2\r\nval3\nval4\r\r</root>"), parse_minimal));

	CHECK_STRING(doc.childValue(STR("root")), STR("\r\rval1\rval2\r\nval3\nval4\r\r"));
}

TEST(parse_pcdata_eol)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<root>\r\rval1\rval2\r\nval3\nval4\r\r</root>"), parse_minimal | parse_eol));

	CHECK_STRING(doc.childValue(STR("root")), STR("\n\nval1\nval2\nval3\nval4\n\n"));
}

TEST(parse_pcdata_skip_ext)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("pre<root/>post"), parse_minimal));
	CHECK(doc.firstChild() == doc.lastChild());
	CHECK(doc.firstChild().type() == node_element);
}

TEST(parse_pcdata_error)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<root>pcdata"), parse_minimal).status == status_end_element_mismatch);
}

TEST(parse_pcdata_trim)
{
    struct test_data_t
    {
        const char_t* source;
        const char_t* result;
        unsigned int flags;
    };

    test_data_t test_data[] =
    {
    	{ STR("<node> text</node>"), STR("text"), 0 },
    	{ STR("<node>\t\n text</node>"), STR("text"), 0 },
    	{ STR("<node>text </node>"), STR("text"), 0 },
    	{ STR("<node>text \t\n</node>"), STR("text"), 0 },
    	{ STR("<node>\r\n\t text \t\n\r</node>"), STR("text"), 0 },
    	{ STR(" text"), STR("text"), parse_fragment },
    	{ STR("\t\n text"), STR("text"), parse_fragment },
    	{ STR("text "), STR("text"), parse_fragment },
    	{ STR("text \t\n"), STR("text"), parse_fragment },
    	{ STR("\r\n\t text \t\n\r"), STR("text"), parse_fragment },
    	{ STR("<node>\r\n\t text \t\n\r more \r\n\t</node>"), STR("text \t\n\r more"), 0 },
    	{ STR("<node>\r\n\t text \t\n\r more \r\n\t</node>"), STR("text \t\n\n more"), parse_eol },
    	{ STR("<node>\r\n\t text \r\n\r\n\r\n\r\n\r\n\r\n\r\n more \r\n\t</node>"), STR("text \n\n\n\n\n\n\n more"), parse_eol },
    	{ STR("<node>     test&amp;&amp;&amp;&amp;&amp;&amp;&amp;    </node>"), STR("test&amp;&amp;&amp;&amp;&amp;&amp;&amp;"), 0 },
    	{ STR("<node>     test&amp;&amp;&amp;&amp;&amp;&amp;&amp;    </node>"), STR("test&&&&&&&"), parse_escapes },
        { STR("     test&amp;&amp;&amp;&amp;&amp;&amp;&amp;    "), STR("test&&&&&&&"), parse_fragment | parse_escapes },
        { STR("<node>\r\n\t text \t\n\r m&amp;&amp;e \r\n\t</node>"), STR("text \t\n\n m&&e"), parse_eol | parse_escapes }
    };

    for (size_t i = 0; i < sizeof(test_data) / sizeof(test_data[0]); ++i)
    {
        const test_data_t& td = test_data[i];

        XmlDocument doc;
        CHECK(doc.loadString(td.source, td.flags | parse_trim_pcdata));

        const char_t* value = doc.child(STR("node")) ? doc.childValue(STR("node")) : doc.text().value();
        CHECK_STRING(value, td.result);
    }
}

TEST(parse_pcdata_trim_empty)
{
	unsigned int flags[] = { 0, parse_ws_pcdata, parse_ws_pcdata_single, parse_ws_pcdata | parse_ws_pcdata_single };

	for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i)
	{
		XmlDocument doc;
		CHECK(doc.loadString(STR("<node>   </node>"), flags[i] | parse_trim_pcdata));

		XmlNode node = doc.child(STR("node"));
		CHECK(node);
		CHECK(!node.firstChild());
	}
}

TEST(parse_escapes_skip)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id='&lt;&gt;&amp;&apos;&quot;'>&lt;&gt;&amp;&apos;&quot;</node>"), parse_minimal));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("&lt;&gt;&amp;&apos;&quot;"));
}

TEST(parse_escapes_parse)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id='&lt;&gt;&amp;&apos;&quot;'>&lt;&gt;&amp;&apos;&quot;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("<>&'\""));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("<>&'\""));
}

TEST(parse_escapes_code)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#1;&#32;&#x20;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("\01  "));
}

TEST(parse_escapes_code_exhaustive_dec)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#/;&#01;&#2;&#3;&#4;&#5;&#6;&#7;&#8;&#9;&#:;&#a;&#A;&#XA;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&#/;\x1\x2\x3\x4\x5\x6\x7\x8\x9&#:;&#a;&#A;&#XA;"));
}

TEST(parse_escapes_code_exhaustive_hex)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#x/;&#x01;&#x2;&#x3;&#x4;&#x5;&#x6;&#x7;&#x8;&#x9;&#x:;&#x@;&#xA;&#xB;&#xC;&#xD;&#xE;&#xF;&#xG;&#x`;&#xa;&#xb;&#xc;&#xd;&#xe;&#xf;&#xg;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&#x/;\x1\x2\x3\x4\x5\x6\x7\x8\x9&#x:;&#x@;\xa\xb\xc\xd\xe\xf&#xG;&#x`;\xa\xb\xc\xd\xe\xf&#xg;"));
}

TEST(parse_escapes_code_restore)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#1&#32;&#x1&#32;&#1-&#32;&#x1-&#32;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&#1 &#x1 &#1- &#x1- "));
}

TEST(parse_escapes_char_restore)
{
	XmlDocument doc;

	CHECK(doc.loadString(STR("<node>&q&#32;&qu&#32;&quo&#32;&quot&#32;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&q &qu &quo &quot "));

	CHECK(doc.loadString(STR("<node>&a&#32;&ap&#32;&apo&#32;&apos&#32;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&a &ap &apo &apos "));

	CHECK(doc.loadString(STR("<node>&a&#32;&am&#32;&amp&#32;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&a &am &amp "));

	CHECK(doc.loadString(STR("<node>&l&#32;&lt&#32;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&l &lt "));

	CHECK(doc.loadString(STR("<node>&g&#32;&gt&#32;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&g &gt "));
}

TEST(parse_escapes_unicode)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#x03B3;&#x03b3;&#x24B62;</node>"), parse_minimal | parse_escapes));

#ifdef PUGIXML_WCHAR_MODE
	const char_t* v = doc.childValue(STR("node"));

	size_t wcharsize = sizeof(wchar_t);

	CHECK(v[0] == 0x3b3 && v[1] == 0x3b3 && (wcharsize == 2 ? v[2] == wchar_cast(0xd852) && v[3] == wchar_cast(0xdf62) : v[2] == wchar_cast(0x24b62)));
#else
	CHECK_STRING(doc.childValue(STR("node")), "\xce\xb3\xce\xb3\xf0\xa4\xad\xa2");
#endif
}

TEST(parse_escapes_error)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#x03g;&#ab;&quot</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&#x03g;&#ab;&quot"));

	CHECK(!doc.loadString(STR("<node id='&#x12")));
	CHECK(!doc.loadString(STR("<node id='&g")));
	CHECK(!doc.loadString(STR("<node id='&gt")));
	CHECK(!doc.loadString(STR("<node id='&l")));
	CHECK(!doc.loadString(STR("<node id='&lt")));
	CHECK(!doc.loadString(STR("<node id='&a")));
	CHECK(!doc.loadString(STR("<node id='&amp")));
	CHECK(!doc.loadString(STR("<node id='&apos")));
}

TEST(parse_escapes_code_invalid)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node>&#;&#x;&;&#x-;&#-;</node>"), parse_minimal | parse_escapes));
	CHECK_STRING(doc.childValue(STR("node")), STR("&#;&#x;&;&#x-;&#-;"));
}

TEST(parse_escapes_attribute)
{
	XmlDocument doc;

	for (int wnorm = 0; wnorm < 2; ++wnorm)
		for (int eol = 0; eol < 2; ++eol)
			for (int wconv = 0; wconv < 2; ++wconv)
			{
				unsigned int flags = parse_escapes;

				flags |= (wnorm ? parse_wnorm_attribute : 0);
				flags |= (eol ? parse_eol : 0);
				flags |= (wconv ? parse_wconv_attribute : 0);

				CHECK(doc.loadString(STR("<node id='&quot;'/>"), flags));
				CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("\""));
			}
}

TEST(parse_attribute_spaces)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id1='v1' id2 ='v2' id3= 'v3' id4 = 'v4' id5 \n\r\t = \r\t\n 'v5' />"), parse_minimal));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id1")).value(), STR("v1"));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id2")).value(), STR("v2"));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id3")).value(), STR("v3"));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id4")).value(), STR("v4"));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id5")).value(), STR("v5"));
}

TEST(parse_attribute_quot)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id1='v1' id2=\"v2\"/>"), parse_minimal));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id1")).value(), STR("v1"));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id2")).value(), STR("v2"));
}

TEST(parse_attribute_no_eol_no_wconv)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id=' \t\r\rval1  \rval2\r\nval3\nval4\r\r'/>"), parse_minimal));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR(" \t\r\rval1  \rval2\r\nval3\nval4\r\r"));
}

TEST(parse_attribute_eol_no_wconv)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id=' \t\r\rval1  \rval2\r\nval3\nval4\r\r'/>"), parse_minimal | parse_eol));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR(" \t\n\nval1  \nval2\nval3\nval4\n\n"));
}

TEST(parse_attribute_no_eol_wconv)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id=' \t\r\rval1  \rval2\r\nval3\nval4\r\r'/>"), parse_minimal | parse_wconv_attribute));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("    val1   val2 val3 val4  "));
}

TEST(parse_attribute_eol_wconv)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id=' \t\r\rval1  \rval2\r\nval3\nval4\r\r'/>"), parse_minimal | parse_eol | parse_wconv_attribute));
	CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("    val1   val2 val3 val4  "));
}

TEST(parse_attribute_wnorm)
{
	XmlDocument doc;

	for (int eol = 0; eol < 2; ++eol)
		for (int wconv = 0; wconv < 2; ++wconv)
		{
			unsigned int flags = parse_minimal | parse_wnorm_attribute | (eol ? parse_eol : 0) | (wconv ? parse_wconv_attribute : 0);
			CHECK(doc.loadString(STR("<node id=' \t\r\rval1  \rval2\r\nval3\nval4\r\r'/>"), flags));
			CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("val1 val2 val3 val4"));
		}
}

TEST(parse_attribute_variations)
{
	XmlDocument doc;

	for (int wnorm = 0; wnorm < 2; ++wnorm)
		for (int eol = 0; eol < 2; ++eol)
			for (int wconv = 0; wconv < 2; ++wconv)
				for (int escapes = 0; escapes < 2; ++escapes)
				{
					unsigned int flags = parse_minimal;

					flags |= (wnorm ? parse_wnorm_attribute : 0);
					flags |= (eol ? parse_eol : 0);
					flags |= (wconv ? parse_wconv_attribute : 0);
					flags |= (escapes ? parse_escapes : 0);

					CHECK(doc.loadString(STR("<node id='1'/>"), flags));
					CHECK_STRING(doc.child(STR("node")).attribute(STR("id")).value(), STR("1"));
				}
}


TEST(parse_attribute_error)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node id"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id "), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id  "), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id   "), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id/"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id?/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id=/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id='/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id=\"/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id=\"'/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id='\"/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id='\"/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node #/>"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node#/>"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node id1='1'id2='2'/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node id&='1'/>"), parse_minimal).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<node &='1'/>"), parse_minimal).status == status_bad_start_element);
}

TEST(parse_attribute_termination_error)
{
	XmlDocument doc;

	for (int wnorm = 0; wnorm < 2; ++wnorm)
		for (int eol = 0; eol < 2; ++eol)
			for (int wconv = 0; wconv < 2; ++wconv)
			{
				unsigned int flags = parse_minimal;

				flags |= (wnorm ? parse_wnorm_attribute : 0);
				flags |= (eol ? parse_eol : 0);
				flags |= (wconv ? parse_wconv_attribute : 0);

				CHECK(doc.loadString(STR("<node id='value"), flags).status == status_bad_attribute);
			}
}

TEST(parse_attribute_quot_inside)
{
	XmlDocument doc;

	for (int wnorm = 0; wnorm < 2; ++wnorm)
		for (int eol = 0; eol < 2; ++eol)
			for (int wconv = 0; wconv < 2; ++wconv)
			{
				unsigned int flags = parse_escapes;

				flags |= (wnorm ? parse_wnorm_attribute : 0);
				flags |= (eol ? parse_eol : 0);
				flags |= (wconv ? parse_wconv_attribute : 0);

				CHECK(doc.loadString(STR("<node id1='\"' id2=\"'\"/>"), flags));
				CHECK_STRING(doc.child(STR("node")).attribute(STR("id1")).value(), STR("\""));
				CHECK_STRING(doc.child(STR("node")).attribute(STR("id2")).value(), STR("'"));
			}
}

TEST(parse_attribute_wnorm_coverage)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<n a1='v' a2=' ' a3='x y' a4='x  y' a5='x   y' />"), parse_wnorm_attribute));
	CHECK_NODE(doc, STR("<n a1=\"v\" a2=\"\" a3=\"x y\" a4=\"x y\" a5=\"x y\"/>"));

	CHECK(doc.loadString(STR("<n a1='v' a2=' ' a3='x y' a4='x  y' a5='x   y' />"), parse_wnorm_attribute | parse_escapes));
	CHECK_NODE(doc, STR("<n a1=\"v\" a2=\"\" a3=\"x y\" a4=\"x y\" a5=\"x y\"/>"));
}

TEST(parse_attribute_wconv_coverage)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<n a1='v' a2='\r' a3='\r\n\n' a4='\n' />"), parse_wconv_attribute));
	CHECK_NODE(doc, STR("<n a1=\"v\" a2=\" \" a3=\"  \" a4=\" \"/>"));

	CHECK(doc.loadString(STR("<n a1='v' a2='\r' a3='\r\n\n' a4='\n' />"), parse_wconv_attribute | parse_escapes));
	CHECK_NODE(doc, STR("<n a1=\"v\" a2=\" \" a3=\"  \" a4=\" \"/>"));
}

TEST(parse_attribute_eol_coverage)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<n a1='v' a2='\r' a3='\r\n\n' a4='\n' />"), parse_eol));
	CHECK_NODE(doc, STR("<n a1=\"v\" a2=\"&#10;\" a3=\"&#10;&#10;\" a4=\"&#10;\"/>"));

	CHECK(doc.loadString(STR("<n a1='v' a2='\r' a3='\r\n\n' a4='\n' />"), parse_eol | parse_escapes));
	CHECK_NODE(doc, STR("<n a1=\"v\" a2=\"&#10;\" a3=\"&#10;&#10;\" a4=\"&#10;\"/>"));
}

TEST(parse_tag_single)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node/><node /><node\n/>"), parse_minimal));
	CHECK_NODE(doc, STR("<node/><node/><node/>"));
}

TEST(parse_tag_hierarchy)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<node><n1><n2/></n1><n3><n4><n5></n5></n4></n3 \r\n></node>"), parse_minimal));
	CHECK_NODE(doc, STR("<node><n1><n2/></n1><n3><n4><n5/></n4></n3></node>"));
}

TEST(parse_tag_error)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<"), parse_minimal).status == status_unrecognized_tag);
	CHECK(doc.loadString(STR("<!"), parse_minimal).status == status_unrecognized_tag);
	CHECK(doc.loadString(STR("<!D"), parse_minimal).status == status_unrecognized_tag);
	CHECK(doc.loadString(STR("<#"), parse_minimal).status == status_unrecognized_tag);
	CHECK(doc.loadString(STR("<node#"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node/"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node /"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node / "), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node / >"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node/ >"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("</ node>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("</node"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("</node "), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("<node></ node>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("<node></node"), parse_minimal).status == status_bad_end_element);
	CHECK(doc.loadString(STR("<node></node "), parse_minimal).status == status_bad_end_element);
	CHECK(doc.loadString(STR("<node></nodes>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("<node>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("<node/><"), parse_minimal).status == status_unrecognized_tag);
	CHECK(doc.loadString(STR("<node attr='value'>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("</></node>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("</node>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("</>"), parse_minimal).status == status_end_element_mismatch);
	CHECK(doc.loadString(STR("<node></node v>"), parse_minimal).status == status_bad_end_element);
	CHECK(doc.loadString(STR("<node&/>"), parse_minimal).status == status_bad_start_element);
	CHECK(doc.loadString(STR("<node& v='1'/>"), parse_minimal).status == status_bad_start_element);
}

TEST(parse_declaration_cases)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<?xml?><?xmL?><?xMl?><?xML?><?Xml?><?XmL?><?XMl?><?XML?>"), parse_fragment | parse_pi));
	CHECK(!doc.firstChild());
}

TEST(parse_declaration_attr_cases)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<?xml ?><?xmL ?><?xMl ?><?xML ?><?Xml ?><?XmL ?><?XMl ?><?XML ?>"), parse_fragment | parse_pi));
	CHECK(!doc.firstChild());
}

TEST(parse_declaration_skip)
{
	XmlDocument doc;

	unsigned int flag_sets[] = {parse_fragment, parse_fragment | parse_pi};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = flag_sets[i];

		CHECK(doc.loadString(STR("<?xml?><?xml version='1.0'?>"), flags));
		CHECK(!doc.firstChild());

		CHECK(doc.loadString(STR("<?xml <tag/> ?>"), flags));
		CHECK(!doc.firstChild());
	}
}

TEST(parse_declaration_parse)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("<?xml?><?xml version='1.0'?>"), parse_fragment | parse_declaration));

	XmlNode d1 = doc.firstChild();
	XmlNode d2 = doc.lastChild();

	CHECK(d1 != d2);
	CHECK(d1.type() == node_declaration);
	CHECK_STRING(d1.name(), STR("xml"));
	CHECK(d2.type() == node_declaration);
	CHECK_STRING(d2.name(), STR("xml"));
	CHECK_STRING(d2.attribute(STR("version")).value(), STR("1.0"));
}

TEST(parse_declaration_error)
{
	XmlDocument doc;

	unsigned int flag_sets[] = {parse_fragment, parse_fragment | parse_declaration};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = flag_sets[i];

		CHECK(doc.loadString(STR("<?xml"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?xml?"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?xml>"), flags).status == status_bad_pi);
		CHECK(doc.loadString(STR("<?xml version='1>"), flags).status == status_bad_pi);
	}

	CHECK(doc.loadString(STR("<?xml version='1?>"), parse_fragment | parse_declaration).status == status_bad_attribute);
	CHECK(doc.loadString(STR("<foo><?xml version='1'?></foo>"), parse_fragment | parse_declaration).status == status_bad_pi);
}

TEST(parse_empty)
{
	XmlDocument doc;

	CHECK(doc.loadString(STR("")).status == status_no_document_element && !doc.firstChild());
	CHECK(doc.loadString(STR(""), parse_fragment) && !doc.firstChild());
}

TEST(parse_out_of_memory)
{
	test_runner::_memory_fail_threshold = 256;

	XmlDocument doc;
	CHECK_ALLOC_FAIL(CHECK(doc.loadString(STR("<foo a='1'/>")).status == status_out_of_memory));
	CHECK(!doc.firstChild());
}

TEST(parse_out_of_memory_halfway_node)
{
	const unsigned int count = 10000;
	static char_t text[count * 4];

	for (unsigned int i = 0; i < count; ++i)
	{
		text[4*i + 0] = '<';
		text[4*i + 1] = 'n';
		text[4*i + 2] = '/';
		text[4*i + 3] = '>';
	}

	test_runner::_memory_fail_threshold = 65536;

	XmlDocument doc;
	CHECK_ALLOC_FAIL(CHECK(doc.loadBufferInplace(text, sizeof(text)).status == status_out_of_memory));
	CHECK_NODE(doc.firstChild(), STR("<n/>"));
}

TEST(parse_out_of_memory_halfway_attr)
{
	const unsigned int count = 10000;
	static char_t text[count * 5 + 4];

	text[0] = '<';
	text[1] = 'n';

	for (unsigned int i = 0; i < count; ++i)
	{
		text[5*i + 2] = ' ';
		text[5*i + 3] = 'a';
		text[5*i + 4] = '=';
		text[5*i + 5] = '"';
		text[5*i + 6] = '"';
	}

	text[5 * count + 2] = '/';
	text[5 * count + 3] = '>';

	test_runner::_memory_fail_threshold = 65536;

	XmlDocument doc;
	CHECK_ALLOC_FAIL(CHECK(doc.loadBufferInplace(text, sizeof(text)).status == status_out_of_memory));
	CHECK_STRING(doc.firstChild().name(), STR("n"));
	CHECK_STRING(doc.firstChild().firstAttribute().name(), STR("a"));
	CHECK_STRING(doc.firstChild().lastAttribute().name(), STR("a"));
}

TEST(parse_out_of_memory_conversion)
{
	test_runner::_memory_fail_threshold = 1;

	XmlDocument doc;
	CHECK_ALLOC_FAIL(CHECK(doc.loadBuffer("<foo\x90/>", 7, parse_default, encoding_latin1).status == status_out_of_memory));
	CHECK(!doc.firstChild());
}

TEST(parse_out_of_memory_allocator_state_sync)
{
	const unsigned int count = 10000;
	static char_t text[count * 4];

	for (unsigned int i = 0; i < count; ++i)
	{
		text[4*i + 0] = '<';
		text[4*i + 1] = 'n';
		text[4*i + 2] = '/';
		text[4*i + 3] = '>';
	}

	test_runner::_memory_fail_threshold = 65536;

	XmlDocument doc;
	CHECK_ALLOC_FAIL(CHECK(doc.loadBufferInplace(text, sizeof(text)).status == status_out_of_memory));
	CHECK_NODE(doc.firstChild(), STR("<n/>"));

	test_runner::_memory_fail_threshold = 0;

	for (unsigned int j = 0; j < count; ++j)
		CHECK(doc.append_child(STR("n")));
}

static bool test_offset(const char_t* contents, unsigned int options, XmlParseStatus status, ptrdiff_t offset)
{
	XmlDocument doc;
	XmlParseResult res = doc.loadString(contents, options);

	return res.status == status && res.offset == offset;
}

#define CHECK_OFFSET(contents, options, status, offset) CHECK(test_offset(STR(contents), options, status, offset))

TEST(parse_error_offset)
{
	CHECK_OFFSET("<node/>", parse_default, status_ok, 0);

	test_runner::_memory_fail_threshold = 1;
	CHECK_ALLOC_FAIL(CHECK_OFFSET("<node/>", parse_default, status_out_of_memory, 0));
	test_runner::_memory_fail_threshold = 0;

	CHECK_OFFSET("<3d/>", parse_default, status_unrecognized_tag, 1);
	CHECK_OFFSET(" <3d/>", parse_default, status_unrecognized_tag, 2);
	CHECK_OFFSET(" <", parse_default, status_unrecognized_tag, 1);

	CHECK_OFFSET("<?pi", parse_default, status_bad_pi, 3);
	CHECK_OFFSET("<?pi", parse_default | parse_pi, status_bad_pi, 3);
	CHECK_OFFSET("<?xml", parse_default | parse_declaration, status_bad_pi, 4);

	CHECK_OFFSET("<!----", parse_default, status_bad_comment, 5);
	CHECK_OFFSET("<!----", parse_default | parse_comments, status_bad_comment, 4);

	CHECK_OFFSET("<![CDA", parse_default, status_bad_cdata, 5);
	CHECK_OFFSET("<![CDATA[non-terminated]]", parse_default, status_bad_cdata, 9);

	CHECK_OFFSET("<!DOCTYPE doc", parse_default, status_bad_doctype, 12);
	CHECK_OFFSET("<!DOCTYPE greeting [ <!ATTLIST list type    (bullets|ordered|glossary)  \"orde", parse_default, status_bad_doctype, 76);

	CHECK_OFFSET("<node", parse_default, status_bad_start_element, 4);
	CHECK_OFFSET("<node ", parse_default, status_bad_start_element, 5);
	CHECK_OFFSET("<nod%>", parse_default, status_bad_start_element, 5);

	CHECK_OFFSET("<node a=2>", parse_default, status_bad_attribute, 8);
	CHECK_OFFSET("<node a='2>", parse_default, status_bad_attribute, 9);

	CHECK_OFFSET("<n></n $>", parse_default, status_bad_end_element, 7);
	CHECK_OFFSET("<n></n", parse_default, status_bad_end_element, 5);

	CHECK_OFFSET("<no></na>", parse_default, status_end_element_mismatch, 6);
	CHECK_OFFSET("<no></nod>", parse_default, status_end_element_mismatch, 6);
}

TEST(parse_result_default)
{
	XmlParseResult result;

	CHECK(!result);
	CHECK(result.status == status_internal_error);
	CHECK(result.offset == 0);
	CHECK(result.encoding == encoding_auto);
}

TEST(parse_bom_fragment)
{
	struct test_data_t
	{
		XmlEncoding encoding;
		const char* data;
		size_t size;
		const char_t* text;
	};

	const test_data_t data[] =
	{
		{ encoding_utf8, "\xef\xbb\xbf", 3, STR("") },
		{ encoding_utf8, "\xef\xbb\xbftest", 7, STR("test") },
		{ encoding_utf16_be, "\xfe\xff", 2, STR("") },
		{ encoding_utf16_be, "\xfe\xff\x00t\x00o\x00s\x00t", 10, STR("tost") },
		{ encoding_utf16_le, "\xff\xfe", 2, STR("") },
		{ encoding_utf16_le, "\xff\xfet\x00o\x00s\x00t\x00", 10, STR("tost") },
		{ encoding_utf32_be, "\x00\x00\xfe\xff", 4, STR("") },
		{ encoding_utf32_be, "\x00\x00\xfe\xff\x00\x00\x00t\x00\x00\x00o\x00\x00\x00s\x00\x00\x00t", 20, STR("tost") },
		{ encoding_utf32_le, "\xff\xfe\x00\x00", 4, STR("") },
		{ encoding_utf32_le, "\xff\xfe\x00\x00t\x00\x00\x00o\x00\x00\x00s\x00\x00\x00t\x00\x00\x00", 20, STR("tost") },
	};

	for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i)
	{
		XmlDocument doc;
		CHECK(doc.loadBuffer(data[i].data, data[i].size, parse_fragment, data[i].encoding));
		CHECK_STRING(doc.text().value(), data[i].text);
		CHECK(save_narrow(doc, format_no_declaration | format_raw | format_write_bom, data[i].encoding) == std::string(data[i].data, data[i].size));
	}
}

TEST(parse_bom_fragment_invalid_utf8)
{
	XmlDocument doc;

	CHECK(doc.loadBuffer("\xef\xbb\xbb", 3, parse_fragment, encoding_utf8));

	const char_t* value = doc.text().value();

#ifdef PUGIXML_WCHAR_MODE
	CHECK(value[0] == wchar_cast(0xfefb) && value[1] == 0);
#else
	CHECK_STRING(value, "\xef\xbb\xbb");
#endif
}

TEST(parse_bom_fragment_invalid_utf16)
{
	XmlDocument doc;

	CHECK(doc.loadBuffer("\xff\xfe", 2, parse_fragment, encoding_utf16_be));

	const char_t* value = doc.text().value();

#ifdef PUGIXML_WCHAR_MODE
	CHECK(value[0] == wchar_cast(0xfffe) && value[1] == 0);
#else
	CHECK_STRING(value, "\xef\xbf\xbe");
#endif
}

TEST(parse_bom_fragment_invalid_utf32)
{
	XmlDocument doc;

	CHECK(doc.loadBuffer("\xff\xff\x00\x00", 4, parse_fragment, encoding_utf32_le));

	const char_t* value = doc.text().value();

#ifdef PUGIXML_WCHAR_MODE
	CHECK(value[0] == wchar_cast(0xffff) && value[1] == 0);
#else
	CHECK_STRING(value, "\xef\xbf\xbf");
#endif
}

TEST(parse_pcdata_gap_fragment)
{
	XmlDocument doc;
	CHECK(doc.loadString(STR("a&amp;b"), parse_fragment | parse_escapes));
	CHECK_STRING(doc.text().value(), STR("a&b"));
}

TEST(parse_name_end_eof)
{
	char_t test[] = STR("<node>");

	XmlDocument doc;
	CHECK(doc.loadBufferInplace(test, 6 * sizeof(char_t)).status == status_end_element_mismatch);
	CHECK_STRING(doc.firstChild().name(), STR("node"));
}

TEST(parse_close_tag_eof)
{
	char_t test1[] = STR("<node></node");
	char_t test2[] = STR("<node></nodx");

	XmlDocument doc;
	CHECK(doc.loadBufferInplace(test1, 12 * sizeof(char_t)).status == status_bad_end_element);
	CHECK_STRING(doc.firstChild().name(), STR("node"));

	CHECK(doc.loadBufferInplace(test2, 12 * sizeof(char_t)).status == status_end_element_mismatch);
	CHECK_STRING(doc.firstChild().name(), STR("node"));
}

TEST(parse_fuzz_doctype)
{
	unsigned char data[] =
	{
		0x3b, 0x3c, 0x21, 0x44, 0x4f, 0x43, 0x54, 0x59, 0x50, 0x45, 0xef, 0xbb, 0xbf, 0x3c, 0x3f, 0x78,
		0x6d, 0x6c, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x22, 0x31, 0x2e, 0x30, 0x22,
		0x3f, 0x3e, 0x3c, 0x21, 0x2d, 0x2d, 0x20, 0xe9, 0x80, 0xb1, 0xe5, 0xa0, 0xb1, 0xe3, 0x82, 0xb4,
		0xe3, 0x83, 0xb3, 0x20, 0xef, 0x83, 0x97, 0xe3, 0xa9, 0x2a, 0x20, 0x2d, 0x2d, 0x3e
	};

	XmlDocument doc;
	CHECK(doc.loadBuffer(data, sizeof(data)).status == status_bad_doctype);
}

TEST(parse_embed_pcdata)
{
	// parse twice - once with default and once with embed_pcdata flags
	for (int i = 0; i < 2; ++i)
	{
		unsigned int flags = (i == 0) ? parse_default : parse_default | parse_embed_pcdata;

		XmlDocument doc;
		XmlParseResult res = doc.loadString(STR("<node><key>value</key><child><inner1>value1</inner1><inner2>value2</inner2>outer</child><two>text<data /></two></node>"), flags);
		CHECK(res);

		XmlNode child = doc.child(STR("node")).child(STR("child"));

		// parse_embed_pcdata omits PCDATA nodes so DOM is different
		if (flags & parse_embed_pcdata)
		{
			CHECK_STRING(doc.child(STR("node")).child(STR("key")).value(), STR("value"));
			CHECK(!doc.child(STR("node")).child(STR("key")).firstChild());
		}
		else
		{
			CHECK_STRING(doc.child(STR("node")).child(STR("key")).value(), STR(""));
			CHECK(doc.child(STR("node")).child(STR("key")).firstChild());
			CHECK_STRING(doc.child(STR("node")).child(STR("key")).firstChild().value(), STR("value"));
		}

		// higher-level APIs work the same though
		CHECK_STRING(child.text().value(), STR("outer"));
		CHECK_STRING(child.child(STR("inner1")).text().value(), STR("value1"));

		CHECK_STRING(child.childValue(), STR("outer"));
		CHECK_STRING(child.childValue(STR("inner2")), STR("value2"));

	#ifndef PUGIXML_NO_XPATH
		CHECK_XPATH_NUMBER(doc, STR("count(node/child/*[starts-with(., 'value')])"), 2);
	#endif

		CHECK_NODE(doc, STR("<node><key>value</key><child><inner1>value1</inner1><inner2>value2</inner2>outer</child><two>text<data/></two></node>"));
		CHECK_NODE_EX(doc, STR("<node>\n<key>value</key>\n<child>\n<inner1>value1</inner1>\n<inner2>value2</inner2>outer</child>\n<two>text<data />\n</two>\n</node>\n"), STR("\t"), 0);
		CHECK_NODE_EX(doc, STR("<node>\n\t<key>value</key>\n\t<child>\n\t\t<inner1>value1</inner1>\n\t\t<inner2>value2</inner2>outer</child>\n\t<two>text<data />\n\t</two>\n</node>\n"), STR("\t"), format_indent);
	}
}

TEST_XML_FLAGS(parse_embed_pcdata_fragment, "text", parse_fragment | parse_embed_pcdata)
{
	CHECK_NODE(doc, STR("text"));
	CHECK(doc.firstChild().type() == node_pcdata);
	CHECK_STRING(doc.firstChild().value(), STR("text"));
}

TEST_XML_FLAGS(parse_embed_pcdata_child, "<n><child/>text</n>", parse_embed_pcdata)
{
	XmlNode n = doc.child(STR("n"));

	CHECK_NODE(doc, STR("<n><child/>text</n>"));
	CHECK(n.lastChild().type() == node_pcdata);
	CHECK_STRING(n.lastChild().value(), STR("text"));
}

TEST_XML_FLAGS(parse_embed_pcdata_comment, "<n>text1<!---->text2</n>", parse_embed_pcdata)
{
	XmlNode n = doc.child(STR("n"));

	CHECK_NODE(doc, STR("<n>text1text2</n>"));
	CHECK_STRING(n.value(), STR("text1"));
	CHECK(n.firstChild() == n.lastChild());
	CHECK(n.lastChild().type() == node_pcdata);
	CHECK_STRING(n.lastChild().value(), STR("text2"));
}

TEST(parse_merge_pcdata)
{
	unsigned int flag_sets[] = {parse_cdata, parse_pi, parse_comments, parse_declaration};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = parse_merge_pcdata | flag_sets[i];

		XmlDocument doc;
		XmlParseResult res = doc.loadString(STR("<node>First text<!-- here is a mesh node -->Second text<![CDATA[someothertext]]>some more text<?include somedata?>Last text</node>"), flags);
		CHECK(res);

		XmlNode child = doc.child(STR("node"));

		if (flags & parse_comments)
		{
			CHECK_STRING(child.firstChild().value(), STR("First text"));
			CHECK(child.firstChild().nextSibling().type() == node_comment);
			CHECK_NODE(doc, STR("<node>First text<!-- here is a mesh node -->Second textsome more textLast text</node>"));
		}
		else if (flags & parse_cdata)
		{
			CHECK_STRING(child.firstChild().value(), STR("First textSecond text"));
			CHECK(child.firstChild().nextSibling().type() == node_cdata);
			CHECK_NODE(doc, STR("<node>First textSecond text<![CDATA[someothertext]]>some more textLast text</node>"));
		}
		else if (flags & parse_pi)
		{
			CHECK_STRING(child.firstChild().value(), STR("First textSecond textsome more text"));
			CHECK(child.firstChild().nextSibling().type() == node_pi);
			CHECK_NODE(doc, STR("<node>First textSecond textsome more text<?include somedata?>Last text</node>"));
		}
		else
		{
			CHECK(child.firstChild() == child.lastChild());
			CHECK(child.firstChild().type() == node_pcdata);
			CHECK_NODE(doc, STR("<node>First textSecond textsome more textLast text</node>"));
		}

		CHECK(child.lastChild().type() == node_pcdata);
	}
}

TEST(parse_merge_pcdata_escape)
{
	XmlDocument doc;
	XmlParseResult res = doc.loadString(STR("<node>First &amp;lt; <!-- comment 1 --> Second &amp;gt; <!-- comment 2 --> Third &amp;quot;</node>"), parse_default | parse_merge_pcdata);
	CHECK(res);

	CHECK_STRING(doc.child(STR("node")).childValue(), STR("First &lt;  Second &gt;  Third &quot;"));
}

TEST(parse_merge_pcdata_whitespace)
{
	unsigned int flag_sets[] = {0, parse_ws_pcdata, parse_ws_pcdata_single};

	for (unsigned int i = 0; i < sizeof(flag_sets) / sizeof(flag_sets[0]); ++i)
	{
		unsigned int flags = parse_merge_pcdata | flag_sets[i];

		XmlDocument doc;
		XmlParseResult res = doc.loadString(STR("<node><child1>  <!-- comment 1 -->\t<!-- comment 2 -->\n</child1><child2>text<!-- comment 1-->\t<!-- comment2 --> end</child2></node>"), flags);
		CHECK(res);

		if (flags & parse_ws_pcdata)
		{
			CHECK_STRING(doc.child(STR("node")).child(STR("child1")).childValue(), STR("  \t\n"));
			CHECK_STRING(doc.child(STR("node")).child(STR("child2")).childValue(), STR("text\t end"));
		}
		else if (flags & parse_ws_pcdata_single)
		{
			CHECK_STRING(doc.child(STR("node")).child(STR("child1")).childValue(), STR("\n"));
			CHECK_STRING(doc.child(STR("node")).child(STR("child2")).childValue(), STR("text end"));
		}
		else
		{
			CHECK(!doc.child(STR("node")).child(STR("child1")).firstChild());
			CHECK_STRING(doc.child(STR("node")).child(STR("child2")).childValue(), STR("text end"));
		}
	}
}

TEST(parse_merge_pcdata_append)
{
	XmlDocument doc;
	doc.append_child(STR("node")).append_child(node_pcdata);
	XmlParseResult res = doc.child(STR("node")).appendBuffer("hello <!--comment-->world", 25, parse_merge_pcdata | parse_fragment);

	CHECK(res.status == status_append_invalid_root);
	CHECK_STRING(doc.child(STR("node")).firstChild().value(), STR(""));

	doc.child(STR("node")).removeChildren();
	res = doc.child(STR("node")).appendBuffer("hello <!--comment-->world", 25, parse_merge_pcdata | parse_fragment);

	CHECK(res.status == status_ok);
	CHECK_STRING(doc.child(STR("node")).firstChild().value(), STR("hello world"));
}

TEST(parse_merge_pcdata_overlap)
{
	XmlDocument doc;
	XmlParseResult res = doc.loadString(STR("<node>short <!-- --> this string is very long so long that copying it will overlap itself</node>"), parse_merge_pcdata);
	CHECK(res);
	CHECK_STRING(doc.childValue(STR("node")), STR("short  this string is very long so long that copying it will overlap itself"));
}

TEST(parse_encoding_detect)
{
	char test[] = "<?xml version='1.0' encoding='utf-8'?><n/>";

	XmlDocument doc;
	CHECK(doc.loadBuffer(test, sizeof(test)));
}

TEST(parse_encoding_detect_latin1)
{
	char test0[] = "<?xml version='1.0' encoding='utf-8'?><n/>";
	char test1[] = "<?xml version='1.0' encoding='iso-8859-1'?><n/>";
	char test2[] = "<?xml version='1.0' encoding = \"latin1\"?><n/>";
	char test3[] = "<?xml version='1.0' encoding='ISO-8859-1'?><n/>";
	char test4[] = "<?xml version='1.0' encoding = \"LATIN1\"?><n/>";

	XmlDocument doc;
	CHECK(doc.loadBuffer(test0, sizeof(test0)).encoding == encoding_utf8);
	CHECK(doc.loadBuffer(test1, sizeof(test1)).encoding == encoding_latin1);
	CHECK(doc.loadBuffer(test2, sizeof(test2)).encoding == encoding_latin1);
	CHECK(doc.loadBuffer(test3, sizeof(test3)).encoding == encoding_latin1);
	CHECK(doc.loadBuffer(test4, sizeof(test4)).encoding == encoding_latin1);
}

TEST(parse_encoding_detect_auto)
{
	struct data_t
	{
		const char* contents;
		size_t size;
		XmlEncoding encoding;
	};

	const data_t data[] =
	{
		// BOM
		{ "\x00\x00\xfe\xff", 4, encoding_utf32_be },
		{ "\xff\xfe\x00\x00", 4, encoding_utf32_le },
		{ "\xfe\xff  ", 4, encoding_utf16_be },
		{ "\xff\xfe  ", 4, encoding_utf16_le },
		{ "\xef\xbb\xbf ", 4, encoding_utf8 },
		// automatic tag detection for < or <?
		{ "\x00\x00\x00<\x00\x00\x00n\x00\x00\x00/\x00\x00\x00>", 16, encoding_utf32_be },
		{ "<\x00\x00\x00n\x00\x00\x00/\x00\x00\x00>\x00\x00\x00", 16, encoding_utf32_le },
		{ "\x00<\x00?\x00n\x00?\x00>", 10, encoding_utf16_be },
		{ "<\x00?\x00n\x00?\x00>\x00", 10, encoding_utf16_le },
		{ "\x00<\x00n\x00/\x00>", 8, encoding_utf16_be },
		{ "<\x00n\x00/\x00>\x00", 8, encoding_utf16_le },
		// <?xml encoding
		{ "<?xml encoding='latin1'?>", 25, encoding_latin1 },
	};

	for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i)
	{
		XmlDocument doc;
		XmlParseResult result = doc.loadBuffer(data[i].contents, data[i].size, parse_fragment);

		CHECK(result);
		CHECK(result.encoding == data[i].encoding);
	}
}

TEST(parse_encoding_detect_auto_incomplete)
{
	struct data_t
	{
		const char* contents;
		size_t size;
		XmlEncoding encoding;
	};

	const data_t data[] =
	{
		// BOM
		{ "\x00\x00\xfe ", 4, encoding_utf8 },
		{ "\x00\x00  ", 4, encoding_utf8 },
		{ "\xff\xfe\x00 ", 4, encoding_utf16_le },
		{ "\xfe   ", 4, encoding_utf8 },
		{ "\xff   ", 4, encoding_utf8 },
		{ "\xef\xbb  ", 4, encoding_utf8 },
		{ "\xef   ", 4, encoding_utf8 },
		// automatic tag detection for < or <?
		{ "\x00\x00\x00 ", 4, encoding_utf8 },
		{ "<\x00\x00n/\x00>\x00", 8, encoding_utf16_le },
		{ "\x00<n\x00\x00/\x00>", 8, encoding_utf16_be },
		{ "<\x00?n/\x00>\x00", 8, encoding_utf16_le },
		{ "\x00 ", 2, encoding_utf8 },
		// <?xml encoding
		{ "<?xmC encoding='latin1'?>", 25, encoding_utf8 },
		{ "<?xBC encoding='latin1'?>", 25, encoding_utf8 },
		{ "<?ABC encoding='latin1'?>", 25, encoding_utf8 },
		{ "<_ABC encoding='latin1'/>", 25, encoding_utf8 },
	};

	for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i)
	{
		XmlDocument doc;
		XmlParseResult result = doc.loadBuffer(data[i].contents, data[i].size, parse_fragment);

		CHECK(result);
		CHECK(result.encoding == data[i].encoding);
	}
}
