#pragma once

#include <string>

namespace RLSU::Graphics {

namespace Colors {

class Color : public std::string
{
public:
    explicit Color(const char* str)        : std::string(str)         {}
    explicit Color(const std::string& str) : std::string(str)            {}
    explicit Color(std::string&& str)      : std::string(std::move(str)) {}

    virtual ~Color() = default;
};

const Color BLACK         = Color("black"        );
const Color WHITE         = Color("white"        );
const Color RED           = Color("red"          );
const Color GREEN         = Color("green"        );
const Color BLUE          = Color("blue"         );
const Color YELLOW        = Color("yellow"       );
const Color CYAN          = Color("cyan"         );
const Color DARKRED       = Color("darkred"      );
const Color DARKGREEN     = Color("darkgreen"    );
const Color DARKBLUE      = Color("darkblue"     );
const Color DARKYELLOW    = Color("darkyellow"   );
const Color DARKCYAN      = Color("darkcyan"     );
const Color DARKDARKRED   = Color("#470101"    );
const Color SKYBLUE       = Color("skyblue"      );
const Color PINK          = Color("pink"         );
const Color AQUA          = Color("aqua"         );    // default for edges and node borders
const Color AQUAMARINE    = Color("aquamarine"   );    // default for nodes
const Color GREY          = Color("grey"         );
const Color DARKSLATEGRAY = Color("darkslategray");    // default for backgrownd 

} // namespace Colors


namespace Shapes {

class NodeShape : public std::string
{
public:
    explicit NodeShape(const char* str)        : std::string(str)         {}
    explicit NodeShape(const std::string& str) : std::string(str)            {}
    explicit NodeShape(std::string&& str)      : std::string(std::move(str)) {}

    virtual ~NodeShape() = default;
};

const NodeShape NODE_ELLIPSE  = NodeShape("ellipse" );          // default for nodes
const NodeShape NODE_CIRCLE   = NodeShape("circle"  );
const NodeShape NODE_BOX      = NodeShape("box"     );
const NodeShape NODE_BOX3D    = NodeShape("box3d"   );
const NodeShape NODE_TRIANGLE = NodeShape("triangle");
const NodeShape NODE_DIAMOND  = NodeShape("diamond" );
const NodeShape NODE_RECORD   = NodeShape("record"  );


class EdgeEndShape : public std::string
{
public:
    explicit EdgeEndShape(const char* str)        : std::string(str)         {}
    explicit EdgeEndShape(const std::string& str) : std::string(str)            {}
    explicit EdgeEndShape(std::string&& str)      : std::string(std::move(str)) {}
};

const EdgeEndShape EDGE_END_NORMAL = EdgeEndShape("normal");       // default for arrowtails
const EdgeEndShape EDGE_END_NONE   = EdgeEndShape("none"  );       // default for arrowheads
const EdgeEndShape EDGE_END_DOT    = EdgeEndShape("dot"   );


} // namespace Shapes
} // namespace RLSU::Graphics