=======

pugixml is a C++ XML processing library, which consists of a DOM-like interface with rich traversal/modification
capabilities, an extremely fast XML parser which constructs the DOM tree from an XML file/buffer, and an XPath 1.0
implementation for complex data-driven tree queries. Full Unicode support is also available, with Unicode interface
variants and conversions between different Unicode encodings (which happen automatically during parsing/saving).

pugixml is used by a lot of projects, both open-source and proprietary, for performance and easy-to-use interface.

## Changes From Fork

- C++ native module
- C++23 minimum required 
- Removed exeptions (only no exeption mode)
- CamelCase
- AdditionalFunctions
- additional encoding support via iconv (iconv required)

## Documentation

Documentation for the current release of pugixml is available on-line as two separate documents:

* [Quick-start guide](https://pugixml.org/docs/quickstart.html), that aims to provide enough information to start using the library;
* [Complete reference manual](https://pugixml.org/docs/manual.html), that describes all features of the library in detail.

You’re advised to start with the quick-start guide; however, many important library features are either not described in it at all or only mentioned briefly; if you require more information you should read the complete manual.

## Example

Here's an example of how code using pugixml looks; it opens an XML file, goes over all Tool nodes and prints tools that have a Timeout attribute greater than 0:

```c++
import std;
import pugixml;

int main()
{
    pugi::XmlDocument doc;
    pugi::XmlParseResult result = doc.loadFile("xgconsole.xml");
    if (!result)
        return -1;
        
    for (pugi::XmlNode const& tool: doc.child("Profile").child("Tools").children("Tool"))
    {
        int timeout = tool.attribute("Timeout").asInt();
        
        if (timeout > 0)
            std::println("Tool {} has timeout {}", 
                        tool.attribute("Filename").value(), 
                        timeout);
    }
}
```

And the same example using XPath:

```c++
import std;
import pugixml;


int main()
{
    pugi::XmlDocument doc;
    pugi::XmlParseResult result = doc.loadFile("xgconsole.xml");
    if (!result)
        return -1;
        
    pugi::XPathNodeSet tools_with_timeout = doc.selectNodes("/Profile/Tools/Tool[@Timeout > 0]");
    
    for (pugi::XPathNode const& node: tools_with_timeout)
    {
        pugi::XmlNode tool = node.node();
        std::println("Tool {} has timeout {}", 
                        tool.attribute("Filename").value(), 
                        tool.attribute("Timeout").as_int());
    }
}
```


## License

This library is available to anybody free of charge, under the terms of MIT License (see LICENSE.md).

To honor the license agreement, please include attribution into the user-facing product documentation and/or credits, for example using this or similar text:

> Uses pugixml. Copyright (c) 2006-2026, Arseny Kapoulkine

<details>
<summary>This work is based on the pugxml parser, which carried the following Public Domain dedication</summary>

    // Pug XML Parser - Version 1.0002
    // --------------------------------------------------------
    // Copyright (C) 2003, by Kristen Wegner (kristen@tima.net)
    // Released into the Public Domain. Use at your own risk.
    // See pugxml.xml for further information, history, etc.
    // Contributions by Neville Franks (readonly@getsoft.com).
</details>
