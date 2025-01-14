#pragma once

#include <map>
#include <set>
#include <sstream>

#define GVDLL
#include <gvc.h>
#include <iomanip>
#include "TreeBase.h"

// Function to generate a hex color based on weight
std::string getDynamicBlueTint(int weight, int minWeight = 1, int maxWeight = 100) {
    // Clamp weight to the valid range
    if (weight < minWeight) weight = minWeight;
    if (weight > maxWeight) weight = maxWeight;

    // Map the weight to the blue channel intensity (0 - 255)
    int blueIntensity = 255 - (weight - minWeight) * 255 / (maxWeight - minWeight);

    // Generate a color string in the format "#RRGGBB" (blue tint)
    std::stringstream ss;
    ss << "#"
        << std::setfill('0') << std::setw(2) << std::hex << 0   // R = 0
        << std::setfill('0') << std::setw(2) << std::hex << 0   // G = 0
        << std::setfill('0') << std::setw(2) << std::hex << blueIntensity; // B = dynamic
    
    return ss.str();
}

// Creates a DOT file, representing the graph, which can be used as input for visualization in Graphviz
template<typename NodeT>
void printImage(std::string fileName, const char* extension, TreeBase<NodeT>& tree, bool ignoreDoubleEdges = false) {

    Agraph_t* g;                            // Graphviz's special graph data structure
    std::map<NodeT*, Agnode_t*> agnodes;      // A mapping between nodes of our graph to nodes in the Graphviz's graph

    // Setting up a Graphviz Context - but only once even for multiple graphs
    static GVC_t* gvc;
    if (!gvc)
        gvc = gvContext();

    g = agopen((char*)"graph", Agdirected, NULL); // Change constant to "Agdirected" for directed graphs
    agsafeset(g, (char*)"dpi", (char*)"300", "");

    int maxWeight = tree.getRoot()->getWeight();

    addNodes(tree.getRoot(), agnodes, g, maxWeight);

    addEdges(tree.getRoot(), agnodes, g);

    int cost = static_cast<Obst<NodeT>&>(tree).getTotalCost();

    addLegend(g, 1, maxWeight, cost);

    gvLayout(gvc, g, "dot");

    fileName = fileName + "." + extension;

    gvRenderFilename(gvc, g, extension, fileName.c_str());

    gvFreeLayout(gvc, g);

    cout << "Success: " << fileName << "\n";

    agclose(g);
}

template<typename NodeT>
void addNodes(NodeT* node, std::map<NodeT*, Agnode_t*>& agnodes, Agraph_t* g, int maxWeight) {
    if (!node) return;
    std::stringstream ss;
    ss << node->getValue();
    char label[20];
    ss >> label;
    
    agnodes[node] = agnode(g, label, true);  // 'nodeLabel' will also internally denote the node identifier in the graph
    
    // Set the fill color based on weight
    int weight = node->getWeight();          // Assuming NodeT has getWeight()
    std::string fillColor = getDynamicBlueTint(weight, 1, maxWeight);

    agsafeset(agnodes[node], (char*)"style", (char*)"filled", "");
    agsafeset(agnodes[node], (char*)"fillcolor", (char*)fillColor.c_str(), ""); // Node background color
    agsafeset(agnodes[node], (char*)"fontcolor", (char*)"#ffffff", ""); // Label color
    agsafeset(agnodes[node], (char*)"color", (char*)"black", ""); // Border color (optional)
    agsafeset(agnodes[node], (char*)"fontname", (char*)"Verdana", "");

    agsafeset(agnodes[node], (char*)"width", (char*)"0.8", "");  // Equal width and height
    agsafeset(agnodes[node], (char*)"height", (char*)"0.8", "");

    addNodes(node->getChild(), agnodes, g, maxWeight);
    addNodes(node->getSibling(), agnodes, g, maxWeight);
}

template<typename NodeT>
void addEdges(NodeT* node, std::map<NodeT*, Agnode_t*>& agnodes, Agraph_t* g) {

    if (!node) return;

    if (node->getChild()) {
        Agedge_t* ghEdge = agedge(g, agnodes[node], agnodes[node->getChild()], nullptr, true);

        if (node->getChild()->getSibling()) {
            NodeT* temp = node->getChild()->getSibling();

            while (temp) {
                Agedge_t* ghEdge = agedge(g, agnodes[node], agnodes[temp], nullptr, true);
                temp = temp->getSibling();
            }
        }
    }

    addEdges(node->getChild(), agnodes, g);
    addEdges(node->getSibling(), agnodes, g);
}

void addLegend(Agraph_t* g, int minWeight, int maxWeight, int cost) {
    // Create a subgraph for the legend
    Agraph_t* legend = agsubg(g, (char*)"cluster_legend", true);
    agsafeset(legend, (char*)"label", (char*)"Legend", "");
    agsafeset(legend, (char*)"style", (char*)"dotted", "");
    agsafeset(legend, (char*)"fontname", (char*)"Verdana", "");

    // Example legend nodes for different weight colors
    for (int i = minWeight; i <= maxWeight; i += (maxWeight - minWeight) / 4) {
        std::stringstream nodeName;
        nodeName << "legend_node_" << i;

        Agnode_t* legendNode = agnode(legend, (char*)nodeName.str().c_str(), true);

        std::stringstream labelStream;
        labelStream << i;

        std::string fillColor = getDynamicBlueTint(i, minWeight, maxWeight);
        agsafeset(legendNode, (char*)"label", (char*)labelStream.str().c_str(), "");
        agsafeset(legendNode, (char*)"style", (char*)"filled", "");
        agsafeset(legendNode, (char*)"fillcolor", (char*)fillColor.c_str(), "");
        agsafeset(legendNode, (char*)"shape", (char*)"circle", "");
        agsafeset(legendNode, (char*)"fontcolor", (char*)"#ffffff", ""); // Label color
        agsafeset(legendNode, (char*)"width", (char*)"0.3", "");      // Set circle width
        agsafeset(legendNode, (char*)"height", (char*)"0.3", "");     // Set circle height
        agsafeset(legendNode, (char*)"fixedsize", (char*)"true", ""); // Ensure fixed size
    }

    Agraph_t* costCluster = agsubg(g, (char*)"cluster_cost", true);
    agsafeset(costCluster, (char*)"style", (char*)"invis", ""); // Make the cluster invisible

    std::stringstream costLabel;
    costLabel << "Cost: " << cost;

    Agnode_t* costNode = agnode(costCluster, (char*)"cost_node", true);
    agsafeset(costNode, (char*)"label", (char*)costLabel.str().c_str(), "");
    agsafeset(costNode, (char*)"shape", (char*)"box", "");
    agsafeset(costNode, (char*)"style", (char*)"filled", "");
    agsafeset(costNode, (char*)"fillcolor", (char*)"#eeeeee", ""); // Light gray background
    agsafeset(costNode, (char*)"fontname", (char*)"Verdana", "");
}