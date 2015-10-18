//
// Created by darkphoton on 16/10/15.
//

#include <stack>

#include "BDTree.h"
#include "BDTNode.h"

const string BDTree::OPS("()|&!>=");

BDTree::BDTree(string& formula)
        : _root(nullptr), _formula(formula) {

    for (unsigned long i = 0; i < _formula.size(); ++i){
        string str(_formula.substr(i, 1));
        if (OPS.find(str) == string::npos && _names.find(str) == string::npos)
            _names += str;
    }

    string name = _names.substr(0, 1);
    string names = _names.substr(1, _names.size() - 1);
    _root = shared_ptr<BDTNode>(new BDTNode(name));

    string nil = "0";
    string one = "1";

    shared_ptr<BDTNode> leaf_left(new BDTNode(nil));
    leaf_left->setSelf(leaf_left);
    shared_ptr<BDTNode> leaf_right(new BDTNode(one));
    leaf_right->setSelf(leaf_right);

    _root->generateChildren(formula, _root, names, leaf_left, leaf_right);

    if (leaf_left->getParentCount() > 0)
        _leafs.push_back(leaf_left);

    if (leaf_right->getParentCount() > 0)
        _leafs.push_back(leaf_right);
}

BDTree::~BDTree() { }

shared_ptr<BDTNode>& BDTree::getRoot() {
    return _root;
}

vector<shared_ptr<BDTNode> >& BDTree::getLeafs() {
    return _leafs;
}

void BDTree::minimize() {
    string nil = "0";
    string one = "1";

    shared_ptr<BDTNode> nilNode(new BDTNode(nil));
    nilNode->setSelf(nilNode);
    shared_ptr<BDTNode> oneNode(new BDTNode(one));
    oneNode->setSelf(oneNode);
    _root->minimize(nilNode, oneNode);

    _leafs.clear();

    if (nilNode->getParentCount() > 0)
        _leafs.push_back(nilNode);

    if (oneNode->getParentCount() > 0)
        _leafs.push_back(oneNode);
}

string BDTree::toString(){
    string s_paths = "";
    for (int i = 0; i < _leafs.size(); ++i) {
        vector<vector<BDTParent> > paths;
        vector<BDTParent> path;
        _leafs[i]->paths_from_root(paths, path);

        string s_path = _leafs[i]->getValue() == "0" ? "false:{\n" : "true:{\n";
        for (int j = 0; j < paths.size(); ++j) {
            path = paths[j];
            s_path += "\t case " + to_string(j + 1) + ": ";
            for (int k = 0; k < path.size(); ++k) {
                BDTParent node = path[k];
                if (!node.getParent()->isTerminal())
                    s_path += "(" + node.getParent()->getValue() + to_string(node.getBranch()) + ")";
            }
            s_path += "\n";
        }
        s_path += "}\n";

        s_paths += s_path;
    }

    return s_paths;
}

ostream& operator<<(ostream& os, BDTree& t) {
    os << t.toString();
    return os;
}
