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
};

const NodeShape ELLIPSE  = NodeShape("ellipse" );          // default for nodes
const NodeShape CIRCLE   = NodeShape("circle"  );
const NodeShape BOX      = NodeShape("box"     );
const NodeShape BOX3D    = NodeShape("box3d"   );
const NodeShape TRIANGLE = NodeShape("triangle");
const NodeShape DIAMOND  = NodeShape("diamond" );

} // namespace Shapes

} // namespace RLSU::Graphics