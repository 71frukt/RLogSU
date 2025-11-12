#pragma once

#include "RLogSU/logger.hpp"
#include "RLogSU/graph_appearance.hpp"
#include <cmath>
#include <cstddef>
#include <fmt/core.h>
#include <string>
#include <vector>
#include <functional>


namespace RLSU::Graphics {

class Graph
{
public:
    Graph(std::function<size_t(size_t)> SizeToWidth =
            [](size_t graph_size) -> size_t {
                return size_t(std::sqrt(graph_size));
            },

          Colors::Color backgrownd_color    = Colors::DARKSLATEGRAY,
          Colors::Color default_edges_color = Colors::AQUA);

    ~Graph();

    void operator=(const Graph& other) = delete;

    class Node;
    class Edge;

    void AddNode(Node& new_node);
    void AddNode(void* node_ptr, std::string label = "no-label", Colors::Color color = Colors::AQUAMARINE, Colors::Color border_color = Colors::BLACK, Colors::Color fontcolor = Colors::BLACK, Shapes::NodeShape shape = Shapes::ELLIPSE);

    void AddEdge(const Edge& new_edge);
    void AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, double weight = 1.0);
    void AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, const Colors::Color& color, double weight = 1.0);
    void AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, const std::string&   label, double weight = 1.0);
    void AddEdge(const void* node_origin_ptr, const void* node_dest_ptr, const std::string&   label, const Colors::Color& color, double weight = 1.0);

    void LogGraph();

    
    const Colors::Color BACKGROWND_COLOR;
    const Colors::Color DEFAULT_EDGES_COLOR;

private:
    static size_t DrawnGraphsNum;

    static constexpr std::string NodeNamePrefix  = "node_";
    static constexpr std::string GraphNamePrefix = "graph_";
    static constexpr std::string GraphsFolder    = "graphs";
    static constexpr std::string TmpDotFileName  = "tmp_cfg.dot";
    
    std::vector<const void*> nodes_ptrs;

    std::ofstream dot_file;

    [[nodiscard]] const std::string GetCurGraphPath()
    {
        return   Log::Logger::GetLogFolder() + "/" + GraphsFolder + "/"
               + GraphNamePrefix + std::to_string(DrawnGraphsNum) + ".svg";
    }

    bool ContainsNode(const void* node_ptr) const;

    const std::function<size_t(size_t)> SizeToWidth_; 
};

class Graph::Node
{
public:
    Node(const void* owner_ptr)
    : OwnerPtr(owner_ptr)
    , label_        ("no-label")
    , color_        (Colors::AQUAMARINE)
    , border_color_ (Colors::BLACK)
    , fontcolor_    (Colors::BLACK)
    , shape_        (Shapes::ELLIPSE)
    {}

    Node(const void* owner_ptr, const Colors::Color& color, const Shapes::NodeShape& shape)
    : OwnerPtr      (owner_ptr)
    , label_        ("no-label")
    , color_        (color)
    , border_color_ (Colors::BLACK)
    , fontcolor_    (Colors::BLACK)    
    , shape_        (shape)
    {}

    Node(const void* owner_ptr, const std::string& label, const Colors::Color& color, const Colors::Color& border_color, const Colors::Color& fontcolor, const Shapes::NodeShape& shape)
    : OwnerPtr      (owner_ptr)
    , label_        (label)
    , color_        (color)
    , border_color_ (border_color)
    , fontcolor_    (fontcolor)
    , shape_        (shape)
    {}

    Node& operator=(const Node&) = delete;
    Node(const Node&)            = delete;
    
    const void* const OwnerPtr;

    template<typename... Args>
    void SetLabel(const std::string& format, Args&&... args)
    {
        RLSU_ASSERT(!pushed_in_graph);
        label_ = fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    void SetColor       (const Colors::Color    & color)        { RLSU_ASSERT(!pushed_in_graph); color_        = color;        }
    void SetBorderColor (const Colors::Color    & border_color) { RLSU_ASSERT(!pushed_in_graph); border_color_ = border_color; }
    void SetFontcolor   (const Colors::Color    & fontcolor)    { RLSU_ASSERT(!pushed_in_graph); fontcolor_    = fontcolor;    }
    void SetShape       (const Shapes::NodeShape& shape)        { RLSU_ASSERT(!pushed_in_graph); shape_        = shape;        }

    [[nodiscard]] const std::string& GetLabel      () const { return label_;        }
    [[nodiscard]] const std::string& GetColor      () const { return color_;        }
    [[nodiscard]] const std::string& GetBorderColor() const { return border_color_; }
    [[nodiscard]] const std::string& GetFontcolor  () const { return fontcolor_;    }
    [[nodiscard]] const std::string& GetShape      () const { return shape_;        }

    bool pushed_in_graph = false;

private:
    std::string       label_;
    Colors::Color     color_;
    Colors::Color     border_color_;
    Colors::Color     fontcolor_;
    Shapes::NodeShape shape_;
};


class Graph::Edge
{
public:
    Edge(const void* node_origin_ptr, const void* node_dest_ptr, const std::string& edge_label = "", const Colors::Color& edge_color = Colors::AQUA, const Colors::Color& edge_fontcolor = Colors::AQUA, double edge_weight = 1.0)
        : origin_ptr (node_origin_ptr)
        , dest_ptr   (node_dest_ptr)
        , label      (edge_label)
        , color      (edge_color)
        , fontcolor  (edge_fontcolor)
        , weight     (edge_weight)
    {}

    Edge& operator=(const Edge&) = delete;
    Edge(const Edge&)            = delete;

    const void* origin_ptr;
    const void* dest_ptr;
    std::string   label;
    Colors::Color color;
    Colors::Color fontcolor;
    double        weight;
};

} // namespace RLSU::Graphics