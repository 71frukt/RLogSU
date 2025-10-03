#pragma once

#include <string>

namespace RLSU::Graphics {

using Color = std::string;

const Color BLACK         = "black"        ;
const Color WHITE         = "white"        ;
const Color RED           = "red"          ;
const Color GREEN         = "green"        ;
const Color BLUE          = "blue"         ;
const Color YELLOW        = "yellow"       ;
const Color SKYBLUE       = "skyblue"      ;
const Color AQUA          = "aqua"         ;    // default for edges
const Color AQUAMARINE    = "aquamarine"   ;    // default for nodes
const Color GREY          = "grey"         ;
const Color DARKSLATEGRAY = "darkslategray";    // default for backgrownd 


using NodeShape = std::string;

const NodeShape ELLIPSE  = "ellipse" ;          // default for nodes
const NodeShape CIRCLE   = "circle"  ;
const NodeShape BOX      = "box"     ;
const NodeShape BOX3D    = "box3d"   ;
const NodeShape TRIANGLE = "triangle";


} // namespace RLSU::Graphics