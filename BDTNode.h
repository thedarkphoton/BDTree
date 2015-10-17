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
    shared_ptr<BDTNode> _falseChild;
    shared_ptr<BDTNode> _trueChild;
    string _value;

protected:

public:
    BDTNode(string& name);
    BDTNode(BDTParent parent, string& name);
    ~BDTNode();

    void generateChildren(shared_ptr<BDTNode>& self, string& names, vector<shared_ptr<BDTNode> >& leafs);
    void setSelf(shared_ptr<BDTNode>& self);

    void paths_from_root(vector<vector<BDTParent> >& paths, vector<BDTParent>& path);
    vector<BDTParent> shortest_path();
    void parseFormula(string formula);
    string evaluateFormula(string formula);
    pair<string, bool> minimize(shared_ptr<BDTNode>& falseChild, shared_ptr<BDTNode>& trueChild);

    string getValue();

    vector<BDTParent>& getParents();
    bool addParent(shared_ptr<BDTNode>& node, bool branch);
    bool removeParent(shared_ptr<BDTNode>& node, bool branch);
    unsigned long getParentCount();
    BDTParent& getParent(int index);

    bool setFalseChild(shared_ptr<BDTNode>& child);
    shared_ptr<BDTNode>& getFalseChild();

    bool setTrueChild(shared_ptr<BDTNode>& child);
    shared_ptr<BDTNode>& getTrueChild();

    bool isTerminal();
    bool isInternal();
    bool isRoot();
    bool isNull();
};


#endif //BDT_GENERATOR_BDTNODE_H