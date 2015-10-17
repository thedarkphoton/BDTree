//
// Created by darkphoton on 17/10/15.
//

#ifndef BDT_GENERATOR_BDTPARENT_H
#define BDT_GENERATOR_BDTPARENT_H

#include <memory>

using namespace std;

class BDTNode;
class BDTParent {
    shared_ptr<BDTNode> _parent;
    bool _branch;

public:
    BDTParent(shared_ptr<BDTNode>& parent, bool branch);
    BDTParent(const BDTParent& source);

    BDTParent& operator=(const BDTParent& source);
    bool operator==(const BDTParent& b);

    const shared_ptr<BDTNode>& getParent() const;
    const bool& getBranch() const;
};


#endif //BDT_GENERATOR_BDTPARENT_H
