//
// Created by darkphoton on 16/10/15.
//

#include <regex>

#include "BDTNode.h"

void merge_nodes(vector<shared_ptr<BDTNode> >& removed_nodes, shared_ptr<BDTNode>& internal_node, shared_ptr<BDTNode>& node){
    vector<BDTParent> parents = internal_node->getParents();
    for (int j = 0; j < parents.size(); ++j) {
        shared_ptr<BDTNode> p = parents[j].getParent();
        if (parents[j].getBranch()){
            p->getRight()->removeParentRecursive(p);
            p->setRight(node);
            node->addParent(p, true);
        }
        else{
            p->getLeft()->removeParentRecursive(p);
            p->setLeft(node);
            node->addParent(p, false);
        }
    }
    removed_nodes.push_back(internal_node);
}

BDTNode::BDTNode(string& name)
        : _value(name), _left(nullptr), _right(nullptr), _self(nullptr) { }

BDTNode::BDTNode(BDTParent parent, string& name)
        : _value(name), _left(nullptr), _right(nullptr), _self(nullptr) {

    _parents.push_back(parent);
}

void BDTNode::generateChildren(string formula, shared_ptr<BDTNode>& self, string& names, shared_ptr<BDTNode>& leaf_left, shared_ptr<BDTNode>& leaf_right, vector<shared_ptr<BDTNode> >& internal_nodes) {
    _self = self;

    if (names != ""){
        string name = names.substr(0, 1);
        string new_names = names.substr(1, names.size() - 1);

        string leftFormula = substituteVariable(formula, _value, "0");
        shared_ptr<BDTNode> left(new BDTNode(BDTParent(_self, false), name));
        setLeft(left);
        left->generateChildren(leftFormula, getLeft(), new_names, leaf_left, leaf_right, internal_nodes);

        string rightFormula = substituteVariable(formula, _value, "1");
        shared_ptr<BDTNode> right(new BDTNode(BDTParent(_self, true), name));
        setRight(right);
        right->generateChildren(rightFormula, getRight(), new_names, leaf_left, leaf_right, internal_nodes);
    }
    else{
        string leftFormula = substituteVariable(formula, _value, "0");
        string leftResult = evaluateFormula(leftFormula);
        if (leftResult == "0"){
            setLeft(leaf_left);
            leaf_left->addParent(_self, false);
        }
        else if (leftResult == "1"){
            setLeft(leaf_right);
            leaf_right->addParent(_self, false);
        }

        string rightFormula = substituteVariable(formula, _value, "1");
        string rightResult = evaluateFormula(rightFormula);
        if (rightResult == "0"){
            setRight(leaf_left);
            leaf_left->addParent(_self, true);
        }
        else if (rightResult == "1"){
            setRight(leaf_right);
            leaf_right->addParent(_self, true);
        }
    }

    minimize(internal_nodes);
    if (!isTerminal())
        internal_nodes.push_back(_self);
}

void BDTNode::setSelf(shared_ptr<BDTNode>& self) {
    _self = self;
}

BDTNode::~BDTNode() { }

bool BDTNode::setLeft(shared_ptr<BDTNode>& child) {
    _left = child;
    return true;
}
shared_ptr<BDTNode>& BDTNode::getLeft() {
    return _left;
}

bool BDTNode::setRight(shared_ptr<BDTNode>& child) {
    _right = child;
    return true;
}
shared_ptr<BDTNode>& BDTNode::getRight() {
    return _right;
}

vector<BDTParent>& BDTNode::getParents() {
    return _parents;
}
bool BDTNode::addParent(shared_ptr<BDTNode>& node, bool branch) {
    BDTParent parent(node, branch);
    if (!(find(_parents.begin(), _parents.end(), parent) != _parents.end()))
        _parents.push_back(parent);
    return true;
}
bool BDTNode::removeParent(shared_ptr<BDTNode>& node, bool branch) {
    BDTParent parent(node, branch);
    _parents.erase(remove(_parents.begin(), _parents.end(), parent), _parents.end());
    return false;
}
bool BDTNode::removeParent(shared_ptr<BDTNode>& node) {
    BDTParent parent_a(node, false);
    _parents.erase(remove(_parents.begin(), _parents.end(), parent_a), _parents.end());

    BDTParent parent_b(node, true);
    _parents.erase(remove(_parents.begin(), _parents.end(), parent_b), _parents.end());

    return false;
}
bool BDTNode::removeParentRecursive(shared_ptr<BDTNode>& node) {
    BDTParent parent_a(node, false);
    _parents.erase(remove(_parents.begin(), _parents.end(), parent_a), _parents.end());

    BDTParent parent_b(node, true);
    _parents.erase(remove(_parents.begin(), _parents.end(), parent_b), _parents.end());

    if (!isTerminal() && _parents.size() == 0){
        _left->removeParentRecursive(_self);
        _right->removeParentRecursive(_self);
    }

    return false;
}

unsigned long BDTNode::getParentCount() {
    return _parents.size();
}
BDTParent& BDTNode::getParent(int index) {
    return _parents[index];
}

string BDTNode::getValue() {
    return _value;
}

void BDTNode::paths_from_root(vector<vector<BDTParent> >& paths, vector<BDTParent>& path) {
    if (isTerminal() && !isNull())
        path.push_back(BDTParent(_self, false));

    for (int i = 0; i < _parents.size(); ++i) {
        if (!_parents[i].getParent()->isNull())
            path.push_back(_parents[i]);

        if (!isRoot())
            _parents[i].getParent()->paths_from_root(paths, path);
    }

    if (isRoot()){
        vector<BDTParent> finished_path;
        for (unsigned long i = path.size() - 1; i < path.size(); --i) {
            finished_path.push_back(path[i]);
        }
        paths.push_back(finished_path);
    }

    if (!isNull())
        path.pop_back();

    if (isTerminal() && !isNull()){
        path = paths[0];
        for (int i = 1; i < paths.size(); ++i) {
            if (path.size() > paths[i].size())
                path = paths[i];
        }
    }
}

vector<BDTParent> BDTNode::shortest_path() {
    vector<vector<BDTParent> > paths;
    vector<BDTParent> path;
    paths_from_root(paths, path);
    return path;
}

void BDTNode::parseFormula(string formula) {
    vector<BDTParent> path = shortest_path();

    for (int i = 0; i < path.size(); ++i) {
        BDTParent parent = path[i];
        replace(
                formula.begin(),
                formula.end(),
                parent.getParent()->getValue().substr(0, 1).c_str()[0],
                to_string(parent.getBranch()).substr(0, 1).c_str()[0]);
    }

    string KEEP("()|&!>=01");
    for (unsigned long j = 0; j < formula.size(); ++j) {
        if (KEEP.find(formula[j]) == string::npos){
            formula.replace(j, 1, "0");
        }
    }

    _value = evaluateFormula(formula);
}

string BDTNode::evaluateFormula(string formula) {
    stack<unsigned long> brackets_stack;
    for (unsigned long i = 0; i < formula.size(); ++i) {
        if (formula[i] == '('){
            brackets_stack.push(i);
        }
        else if (formula[i] == ')'){
            unsigned long start = brackets_stack.top() + 1;
            unsigned long length = i - start;
            brackets_stack.pop();
            string result = evaluateFormula(formula.substr(start, length));
            if (result == "1")
                formula.replace(start - 1, length + 2, "1");
            else
                formula.replace(start - 1, length + 2, "0");
            i -= length + 1;
        }
    }

    for (unsigned long i = 0; i < formula.size(); ++i) {
        if (formula[i] == '!'){
            unsigned long start = i;
            unsigned long length = 2;
            if (formula[i+1] == '0')
                formula.replace(start, length, "1");
            else
                formula.replace(start, length, "0");
        }
    }

    for (unsigned long i = 0; i < formula.size(); ++i) {
        if (formula[i] == '&'){
            bool val1 = formula[i - 1] == '1';
            bool val2 = formula[i + 1] == '1';

            unsigned long start = i - 1;
            unsigned long length = 3;
            if (val1 && val2)
                formula.replace(start, length, "1");
            else
                formula.replace(start, length, "0");
            --i;
        }
        else if (formula[i] == '|'){
            bool val1 = formula[i - 1] == '1';
            bool val2 = formula[i + 1] == '1';

            unsigned long start = i - 1;
            unsigned long length = 3;
            if (val1 || val2)
                formula.replace(start, length, "1");
            else
                formula.replace(start, length, "0");
            --i;
        }
    }

    for (unsigned long i = 0; i < formula.size(); ++i) {
        if (formula[i] == '>'){
            bool val1 = formula[i - 1] == '1';
            bool val2 = formula[i + 1] == '1';

            unsigned long start = i - 1;
            unsigned long length = 3;
            if (!val1 || val2)
                formula.replace(start, length, "1");
            else
                formula.replace(start, length, "0");
            --i;
        }
        else if (formula[i] == '='){
            bool val1 = formula[i - 1] == '1';
            bool val2 = formula[i + 1] == '1';

            unsigned long start = i - 1;
            unsigned long length = 3;
            if ((!val1 || val2) && (!val2 || val1))
                formula.replace(start, length, "1");
            else
                formula.replace(start, length, "0");
            --i;
        }
    }

    return formula.substr(0, 1);
}

void BDTNode::minimize(vector<shared_ptr<BDTNode> >& internal_nodes) {
    vector<shared_ptr<BDTNode> > removed_nodes;
    string nodea = _left->toString();
    string nodeb = _right->toString();
    if (nodea == nodeb){
        for (int i = 0; i < _parents.size(); ++i) {
            shared_ptr<BDTNode> parent = _parents[i].getParent();
            bool branch = _parents[i].getBranch();

            _right->removeParent(_self);

            if (branch)
                parent->setRight(_right);
            else
                parent->setLeft(_right);
            _right->addParent(parent, _parents[i].getBranch());

            _left->removeParentRecursive(_self);
            removed_nodes.push_back(_right);
        }

        if (isRoot()){
            _left->removeParent(_self, true);
            _left->removeParent(_self, false);
            _left->addParent(_self, false);
        }
        else {
            shared_ptr<BDTNode> null(nullptr);
            setRight(null);
            setLeft(null);
        }
    }
    else{
        for (int i = 0; i < internal_nodes.size(); ++i) {
            string node = internal_nodes[i]->toString();
            if (internal_nodes[i] != _left && node == nodea)
                merge_nodes(removed_nodes, internal_nodes[i], _left);
            else if (internal_nodes[i] != _right && node == nodeb)
                merge_nodes(removed_nodes, internal_nodes[i], _right);
        }
    }

    for (int j = 0; j < removed_nodes.size(); ++j) {
        internal_nodes.erase(
                remove(
                        internal_nodes.begin(),
                        internal_nodes.end(),
                        removed_nodes[j]
                ),
                internal_nodes.end()
        );
    }
}

//pair<string, bool> BDTNode::minimize(shared_ptr<BDTNode>& falseChild, shared_ptr<BDTNode>& trueChild) {
//    if (isTerminal())
//        return make_pair(_value, true);
//    else{
//        pair<string, bool> a = getLeft()->minimize(falseChild, trueChild);
//        pair<string, bool> b = getTrue()->minimize(falseChild, trueChild);
//
//        if (a.second && b.second && a.first == b.first){
//            for (int i = 0; i < _parents.size(); ++i) {
//                shared_ptr<BDTNode> parent = _parents[i].getParent();
//                bool branch = _parents[i].getBranch();
//
//                shared_ptr<BDTNode> null(nullptr);
//                setRight(null);
//                setLeft(null);
//
//                if (a.first == "0"){
//                    falseChild->removeParent(_self, true);
//                    falseChild->removeParent(_self, false);
//                    if (branch)
//                        parent->setRight(falseChild);
//                    else
//                        parent->setLeft(falseChild);
//                    falseChild->addParent(parent, _parents[i].getBranch());
//                }
//                else if (a.first == "1"){
//                    trueChild->removeParent(_self, true);
//                    trueChild->removeParent(_self, false);
//                    if (branch)
//                        parent->setRight(trueChild);
//                    else
//                        parent->setLeft(trueChild);
//                    trueChild->addParent(parent, _parents[i].getBranch());
//                }
//            }
//
//            if (isRoot()){
//                shared_ptr<BDTNode> null(nullptr);
//                setLeft(null);
//                setRight(null);
//
//                if (a.first == "0"){
//                    falseChild->removeParent(_self, true);
//                    falseChild->removeParent(_self, false);
//                    falseChild->addParent(_self, false);
//                }
//                else if (a.first == "1"){
//                    trueChild->removeParent(_self, true);
//                    trueChild->removeParent(_self, false);
//                    trueChild->addParent(_self, false);
//                }
//            }
//
//            return make_pair(a.first, true);
//        }
//        else {
//            if (a.second){
//                if (a.first == "0"){
//                    setLeft(falseChild);
//                    falseChild->addParent(_self, false);
//                }
//                else if (a.first == "1"){
//                    setLeft(trueChild);
//                    trueChild->addParent(_self, false);
//                }
//            }
//
//            if (b.second){
//                if (b.first == "0"){
//                    setRight(falseChild);
//                    falseChild->addParent(_self, true);
//                }
//                else if (b.first == "1"){
//                    setRight(trueChild);
//                    trueChild->addParent(_self, true);
//                }
//            }
//            return make_pair(_value, false);
//        }
//    }
//}

bool BDTNode::isTerminal() {
    return _left == nullptr && _right == nullptr;
}
bool BDTNode::isInternal() {
    return _left == nullptr || _right == nullptr;
}
bool BDTNode::isRoot() {
    return _parents.size() == 0;
}
bool BDTNode::isNull() {
    return isRoot() && isTerminal();
}

string BDTNode::substituteVariable(string formula, string var, string sub) {
    replace(
        formula.begin(),
        formula.end(),
        var.c_str()[0],
        sub.c_str()[0]
    );

    return formula;
}

string BDTNode::toString() {
    if (isTerminal()){
        return "["+getValue()+"]";
    }
    else{
        return "{" + _left->toString() + "," + _right->toString() + "}";
    }
}
