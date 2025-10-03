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

size_t Graph::Node::NodesCounter = 0;
size_t Graph::DrawnGraphsNum     = 0;


Graph::Graph()
{
    dot_file.open(TmpDotFileName);

    std::filesystem::create_directory(Log::Logger::GetLogFolder() + "/" + GraphsFolder);

    dot_file << "digraph G{                               \n"
             << "   bgcolor = \"" << DARKSLATEGRAY <<  "\"\n"
             << "   edge [color = \"" << AQUA <<    "\"]; \n";
}

Graph::~Graph()
{
    std::remove(TmpDotFileName.c_str());
}

void Graph::AddNode(Node& new_node)
{
    nodes_ids.push_back(new_node.id);
    new_node.pushed_in_graph = true;

    dot_file << NodeNamePrefix << new_node.id 
             << "["
             << "  shape     = \"" << new_node.GetShape() << "\""
             << ", style     = filled"
             << ", fillcolor = \"" << new_node.GetColor() << "\""
             << ", label     = \"" << new_node.GetLabel() << "\""
             << "]\n";
}

void Graph::AddEdge(const Node& node1, const Node& node2)
{
    RLSU_ASSERT(ContainsNode(node1.id));
    RLSU_ASSERT(ContainsNode(node2.id));

    dot_file << NodeNamePrefix << node1.id << "->"
             << NodeNamePrefix << node2.id << "\n";
}


void Graph::LogGraph()
{
    dot_file << "}\n";
    dot_file.close();

    std::string command = "dot -Tpng " + TmpDotFileName + " -o "
                        + Log::Logger::GetLogFolder() + "/" + GraphsFolder + "/"
                        + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png";

    system(command.c_str());

    RLSU_LOG("<img src = {} width = \"({})%\" style=\"margin-left: 3%\">\n"
            , GraphsFolder + "/" + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png"
            , std::min(((double)nodes_ids.size() / 100.0), 90.0));
    DrawnGraphsNum++;
}


bool Graph::ContainsNode(size_t node_id) const
{
    for (size_t i = 0; i < nodes_ids.size(); i++)
    {
        if (node_id == nodes_ids[i])
            return true;
    }

    return false;
}

} // namespace RLSU::Graphics