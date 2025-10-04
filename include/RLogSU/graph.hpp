#pragma once

#include "RLogSU/logger.hpp"
#include "RLogSU/graph_appearance.hpp"
#include <cstddef>
#include <fmt/core.h>
#include <string>
#include <vector>

namespace RLSU::Graphics {

class Graph
{
public:
    Graph();
    ~Graph();
    
    class Node;

    void AddNode(Node& new_node);
    void AddEdge(const Node& node1, const Node& node2);

    void LogGraph();

private:
    static size_t DrawnGraphsNum;

    static constexpr std::string NodeNamePrefix  = "node_";
    static constexpr std::string GraphNamePrefix = "graph_";
    static constexpr std::string GraphsFolder    = "graphs";
    static constexpr std::string TmpDotFileName  = "tmp_cfg.dot";
    

    std::vector<size_t> nodes_ids;

    std::ofstream dot_file;

    [[nodiscard]] const std::string GetCurGraphPath()
    {
        return   Log::Logger::GetLogFolder() + "/" + GraphsFolder + "/"
               + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".png";
    }

    bool ContainsNode(size_t node_id) const;
};


class Graph::Node
{
public:
    Node() 
    : id(NodesCounter++) 
    , label_("no-label")
    , color_(Colors::AQUAMARINE)
    , shape_(Shapes::ELLIPSE)
    {}

    Node(const Colors::Color& color, const Shapes::NodeShape& shape)
    : id(NodesCounter++) 
    , label_("no-label")
    , color_(color)
    , shape_(shape)
    {}

    static size_t NodesCounter;
    const size_t id;

    template<typename... Args>
    void SetLabel(const std::string& format, Args&&... args)
    {
        RLSU_ASSERT(!pushed_in_graph);
        label_ = fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    void SetColor(const Colors::Color    & color) { RLSU_ASSERT(!pushed_in_graph); color_ = color; }
    void SetShape(const Shapes::NodeShape& shape) { RLSU_ASSERT(!pushed_in_graph); shape_ = shape; }

    [[nodiscard]] const std::string& GetLabel() const { return label_; }
    [[nodiscard]] const std::string& GetColor() const { return color_; }
    [[nodiscard]] const std::string& GetShape() const { return shape_; }


    bool pushed_in_graph = false;

private:
    std::string       label_;
    Colors::Color     color_;
    Shapes::NodeShape shape_;

};

} // namespace RLSU::Graphics