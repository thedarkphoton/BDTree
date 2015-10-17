//
// Created by darkphoton on 17/10/15.
//

#include "BDTParent.h"

BDTParent::BDTParent(shared_ptr<BDTNode>& parent, bool branch)
        : _parent(parent), _branch(branch) {
}

BDTParent::BDTParent(const BDTParent& source)
        : _parent(source.getParent()), _branch(source.getBranch()) {
}

BDTParent& BDTParent::operator=(const BDTParent& source) {
    _parent = source.getParent();
    _branch = source.getBranch();
    return *this;
}

bool BDTParent::operator==(const BDTParent& b) {
    return _parent == b.getParent() && _branch == b.getBranch();
}

const shared_ptr<BDTNode>& BDTParent::getParent() const {
    return _parent;
}

const bool& BDTParent::getBranch() const {
    return _branch;
}
