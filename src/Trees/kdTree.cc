#ifndef KDTREE_CC
#define KDTREE_CC

#include "kdTree.hh"
#include <algorithm>

template <int dim>
KDTree<dim>::KDTree(Field<Lin::Vector<dim>>* points) : points(points) {
    std::vector<Node> nodes;
    for (size_t i = 0; i < points->size(); ++i) {
        nodes.emplace_back(points->getValue(i), i);
    }
    root = buildTree(nodes, 0);
}

template <int dim>
typename KDTree<dim>::Node* KDTree<dim>::buildTree(std::vector<Node>& nodes, int depth) {
    if (nodes.empty()) {
        return nullptr;
    }

    int axis = depth % dim;
    std::sort(nodes.begin(), nodes.end(), [axis](const Node& a, const Node& b) {
        return a.point[axis] < b.point[axis];
    });

    int medianIndex = nodes.size() / 2;
    Node* node = new Node(nodes[medianIndex]);

    std::vector<Node> leftNodes(nodes.begin(), nodes.begin() + medianIndex);
    std::vector<Node> rightNodes(nodes.begin() + medianIndex + 1, nodes.end());

    node->left = buildTree(leftNodes, depth + 1);
    node->right = buildTree(rightNodes, depth + 1);

    return node;
}

template <int dim>
void KDTree<dim>::searchNeighbors(Node* node, const Lin::Vector<dim>& point, double radius, int depth, std::vector<int>& result) const {
    if (!node) return;

    int axis = depth % dim;
    double dist = (node->point - point).magnitude();

    // Exclude the identical point from the results
    if (dist <= radius && node->point != point) {
        result.push_back(node->index);
    }

    double diff = point[axis] - node->point[axis];
    if (diff < 0) {
        searchNeighbors(node->left, point, radius, depth + 1, result);
        if (fabs(diff) <= radius) {
            searchNeighbors(node->right, point, radius, depth + 1, result);
        }
    } else {
        searchNeighbors(node->right, point, radius, depth + 1, result);
        if (fabs(diff) <= radius) {
            searchNeighbors(node->left, point, radius, depth + 1, result);
        }
    }
}

template <int dim>
std::vector<int> KDTree<dim>::findNearestNeighbors(const Lin::Vector<dim>& point, double radius) const {
    std::vector<int> result;
    searchNeighbors(root, point, radius, 0, result);
    return result;
}

#endif