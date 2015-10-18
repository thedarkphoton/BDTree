//
// Created by darkphoton on 16/10/15.
//

#ifndef BDT_GENERATOR_BDTNODE_H
#define BDT_GENERATOR_BDTNODE_H

#include <string>
#include <vector>
#include <memory>
#include "BDTParent.h"

using namespace std;

class BDTParent;
class BDTNode {
    vector<BDTParent> _parents;
    shared_ptr<BDTNode> _self;
    shared_ptr<BDTNode> _left;
    shared_ptr<BDTNode> _right;
    string _value;

    string substituteVariable(string formula, string var, string sub);
    void minimize(vector<shared_ptr<BDTNode> >& internal_nodes);

protected:

public:
    BDTNode(string& name);
    BDTNode(BDTParent parent, string& name);
    ~BDTNode();

    void generateChildren(string formula, shared_ptr<BDTNode>& self, string& names, shared_ptr<BDTNode>& leaf_left, shared_ptr<BDTNode>& leaf_right, vector<shared_ptr<BDTNode> >& internal_nodes);
    void setSelf(shared_ptr<BDTNode>& self);

    void paths_from_root(vector<vector<BDTParent> >& paths, vector<BDTParent>& path);
    vector<BDTParent> shortest_path();
    void parseFormula(string formula);
    string evaluateFormula(string formula);
//    pair<string, bool> minimize(shared_ptr<BDTNode>& falseChild, shared_ptr<BDTNode>& trueChild);

    string getValue();

    vector<BDTParent>& getParents();
    bool addParent(shared_ptr<BDTNode>& node, bool branch);
    bool removeParent(shared_ptr<BDTNode>& node, bool branch);
    bool removeParent(shared_ptr<BDTNode>& node);
    bool removeParentRecursive(shared_ptr<BDTNode>& node);
    unsigned long getParentCount();
    BDTParent& getParent(int index);

    bool setLeft(shared_ptr<BDTNode>& child);
    shared_ptr<BDTNode>& getLeft();

    bool setRight(shared_ptr<BDTNode>& child);
    shared_ptr<BDTNode>& getRight();

    bool isTerminal();
    bool isInternal();
    bool isRoot();
    bool isNull();

    string toString();
};


#endif //BDT_GENERATOR_BDTNODE_H
