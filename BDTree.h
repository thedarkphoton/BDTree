//
// Created by darkphoton on 16/10/15.
//

#ifndef BDT_GENERATOR_BDTREE_H
#define BDT_GENERATOR_BDTREE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class BDTNode;
class BDTree {
    shared_ptr<BDTNode> _root;
    vector<shared_ptr<BDTNode>> _leafs;

    static const string OPS;
    string _formula;
    string _names;

protected:

public:
    BDTree(string& _formula);
    ~BDTree();

    void minimize();

    shared_ptr<BDTNode>& getRoot();
    vector<shared_ptr<BDTNode> >& getLeafs();

    string toString();
};


#endif //BDT_GENERATOR_BDTREE_H
