#ifndef KDTREE_HH
#define KDTREE_HH

#include <vector>
#include <cmath>
#include "../Math/vectorMath.hh"

template <int dim>
class KDTree {
public:
    KDTree(const std::vector<Lin::Vector<dim>>& points);

    std::vector<int> findNearestNeighbors(const Lin::Vector<dim>& point, double radius) const;

private:
    struct Node {
        Lin::Vector<dim> point;
        int index;
        Node* left;
        Node* right;
        Node(const Lin::Vector<dim>& p, int i) : point(p), index(i), left(nullptr), right(nullptr) {}
    };

    Node* buildTree(std::vector<Node>& nodes, int depth);
    void searchNeighbors(Node* node, const Lin::Vector<dim>& point, double radius, int depth, std::vector<int>& result) const;

    Node* root;
    std::vector<Lin::Vector<dim>> points;
};

#include "kdTree.cc"

#endif // KDTREE_HH
