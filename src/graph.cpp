#include "RLogSU/graph.hpp"
#include "RLogSU/error_handler.hpp"
#include "RLogSU/logger.hpp"

#include <cstddef>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include <algorithm>


namespace RLSU::Graphics {

size_t Graph::DrawnGraphsNum = 0;


Graph::Graph(Colors::Color backgrownd_color, Colors::Color default_edges_color)
    : nodes_ptrs()
    , dot_file()
    , BACKGROWND_COLOR   (backgrownd_color)
    , DEFAULT_EDGES_COLOR(default_edges_color)
{
    dot_file.open(TmpDotFileName);

    std::filesystem::create_directory(Log::Logger::GetLogFolder() + "/" + GraphsFolder);

    dot_file << "digraph G{                                           \n"
             << "   bgcolor = \""     << BACKGROWND_COLOR    <<    "\"\n"
             << "   edge [color = \"" << DEFAULT_EDGES_COLOR << "\"]; \n";
}


Graph::~Graph()
{
    // std::remove(TmpDotFileName.c_str());
}

void Graph::AddNode(Node& new_node)
{
    RLSU_INFO("OwnerPtr2 = {}", (void*) new_node.OwnerPtr);

    nodes_ptrs.push_back(new_node.OwnerPtr);
    new_node.pushed_in_graph = true;

    dot_file << NodeNamePrefix << new_node.OwnerPtr
             << "["
             << "  shape     = \"" << new_node.GetShape()       << "\""
             << ", style     = filled"
             << ", fillcolor = \"" << new_node.GetColor()       << "\""
             << ", color     = \"" << new_node.GetBorderColor() << "\""
             << ", label     = \"" << new_node.GetLabel()       << "\""
             << "]" << std::endl;
}

void Graph::AddEdge(const void* node_origin_ptr, const void* node_dest_ptr)
{
    RLSU_ASSERT(ContainsNode(node_origin_ptr), "mb forgot AddNode()?");
    RLSU_ASSERT(ContainsNode(node_dest_ptr)  , "mb forgot AddNode()?");

    dot_file << NodeNamePrefix << node_origin_ptr << "->"
             << NodeNamePrefix << node_dest_ptr << "\n";
}


void Graph::LogGraph()
{
    dot_file << "}\n";
    dot_file.close();

    std::string command = "dot -Tpng " + TmpDotFileName + " -o "
                        + Log::Logger::GetLogFolder() + "/" + GraphsFolder + "/"
                        + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png";

    system(command.c_str());

    RLSU_LOG("<img src={} width=\"{}%\" style=\"margin-left: 3%\">\n"
            , GraphsFolder + "/" + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png"
            , std::min(((double)nodes_ptrs.size() * 8.0), 95.0));
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