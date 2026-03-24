#include "RLogSU/graph.hpp"
#include "RLogSU/logger.hpp"
#include "logger_impl.hpp"

#include <iostream>
#include <cstddef>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include <algorithm>


namespace RLSU::Graphics {

size_t Graph::DrawnGraphsNum = 0;


Graph::Graph(std::function<size_t(size_t)> SizeToWidth,
             Colors::Color backgrownd_color,
             Colors::Color default_edges_color)
    : nodes_ptrs()
    , dot_file()
    , BACKGROWND_COLOR   (backgrownd_color)
    , DEFAULT_EDGES_COLOR(default_edges_color)
    , SizeToWidth_(SizeToWidth)
{
    dot_file.open(TmpDotFileName);

    std::filesystem::create_directory(Log::UnitLogger.GetLogSpace() + "/" + Log::UnitLogger.GetLogsFolder() + "/" + PngGraphsFolder);
    std::filesystem::create_directory(Log::UnitLogger.GetLogSpace() + "/" + Log::UnitLogger.GetLogsFolder() + "/" + SvgGraphsFolder);

    dot_file << "digraph G{                                           \n"
             << "   rankdir=\"HR\"                                    \n"
             << "   bgcolor = \""     << BACKGROWND_COLOR    <<    "\"\n"
             << "   edge [color = \"" << DEFAULT_EDGES_COLOR << "\"]; \n";
}


Graph::~Graph()
{
    std::remove(TmpDotFileName.c_str());
}

void Graph::AddNode(Node& new_node)
{
    nodes_ptrs.push_back(new_node.OwnerPtr);
    new_node.pushed_in_graph = true;

    dot_file << NodeNamePrefix << new_node.OwnerPtr
             << "["
             << "  shape     = \"" << new_node.GetShape()       << "\""
             << ", style     = filled"
             << ", fillcolor = \"" << new_node.GetColor()       << "\""
             << ", color     = \"" << new_node.GetBorderColor() << "\""
             << ", fontcolor = \"" << new_node.GetFontcolor()   << "\""
             << ", label     = \"" << new_node.GetLabel()       << "\""
             << "]" << std::endl;
}

void Graph::AddNode(const void* node_ptr, std::string label, Colors::Color color, Colors::Color border_color, Colors::Color fontcolor, Shapes::NodeShape shape)
{
    Node new_node(node_ptr, label, color, border_color, fontcolor, shape);
    AddNode(new_node);
}


void Graph::AddEdge(const Edge& new_edge)
{
    // assert(ContainsNode(new_edge.origin_ptr) && "mb forgot AddNode()?");
    // assert(ContainsNode(new_edge.dest_ptr)   && "mb forgot AddNode()?");

    dot_file << NodeNamePrefix << new_edge.origin_ptr << "->"
             << NodeNamePrefix << new_edge.dest_ptr
             << " ["
             << "dir=\"both\""
             << "label=\""     << new_edge.label     << "\", "
             << "color=\""     << new_edge.color     << "\", "
             << "fontcolor=\"" << new_edge.fontcolor << "\", "
             << "arrowhead=\"" << new_edge.arrowhead << "\", "
             << "arrowtail=\"" << new_edge.arrowtail << "\", "
             << "weight=\""    << new_edge.weight    << "\"  "
             << "]\n";
}

void Graph::AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, const std::string& label, const Colors::Color& color, double weight)
{
    Graph::Edge new_edge{.origin_ptr = node_origin_ptr    ,
                         .dest_ptr   = node_dest_ptr      ,
                         .label      = label              ,
                         .color      = color              ,
                         .fontcolor  = DEFAULT_EDGES_COLOR, 
                         .weight     = weight};

    AddEdge(new_edge);
}

void Graph::AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, const Colors::Color& color, double weight)
{
    Graph::Edge new_edge{.origin_ptr = node_origin_ptr    ,
                         .dest_ptr   = node_dest_ptr      ,
                         .label      = ""                 ,
                         .color      = color              ,
                         .fontcolor  = DEFAULT_EDGES_COLOR, 
                         .weight     = weight};

    AddEdge(new_edge);
}

void Graph::AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, const std::string& label, double weight)
{
    Graph::Edge new_edge{.origin_ptr = node_origin_ptr    ,
                         .dest_ptr   = node_dest_ptr      ,
                         .label      = label              ,
                         .color      = DEFAULT_EDGES_COLOR,
                         .fontcolor  = DEFAULT_EDGES_COLOR, 
                         .weight     = weight};

    AddEdge(new_edge);
}

void Graph::AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, double weight)
{
    Graph::Edge new_edge{.origin_ptr = node_origin_ptr    ,
                         .dest_ptr   = node_dest_ptr      ,
                         .label      = ""                 ,
                         .color      = DEFAULT_EDGES_COLOR,
                         .fontcolor  = DEFAULT_EDGES_COLOR, 
                         .weight     = weight};

    AddEdge(new_edge);
}


void Graph::LogGraph()
{
    dot_file << "}\n";
    dot_file.close();

    const std::string logfolder = Log::UnitLogger.GetLogSpace() + "/" + Log::UnitLogger.GetLogsFolder();

    std::string svg_graph_path = logfolder + "/" + SvgGraphsFolder + "/" 
                               + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".svg";

    std::string png_graph_path = logfolder + "/" + PngGraphsFolder + "/" 
                               + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png";

    std::string command_create_png = "dot -Tpng " + TmpDotFileName + " -o " + svg_graph_path;
    std::string command_create_svg = "dot -Tpng " + TmpDotFileName + " -o " + png_graph_path;
    

    if (system(command_create_png.c_str()) != 0)
        std::cerr << "Error in command_create_png.c_str()";

    if (system(command_create_svg.c_str()) != 0)
        std::cerr << "Error in command_create_svg.c_str()";

    RLSU_LOG("<img src={} width=\"{}%\" style=\"margin-left: 3%\">\n"
            , PngGraphsFolder + "/" + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png"
            , std::min(((double) SizeToWidth_(nodes_ptrs.size()) * 10.0), 95.0));
    DrawnGraphsNum++;
}


bool Graph::ContainsNode(const void* node_ptr) const
{
    for (size_t i = 0; i < nodes_ptrs.size(); i++)
    {
        if (node_ptr == nodes_ptrs[i])
            return true;
    }

    return false;
}

} // namespace RLSU::Graphics