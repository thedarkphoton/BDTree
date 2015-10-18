//
// Created by darkphoton on 16/10/15.
//

#include <regex>

#include "BDTNode.h"

BDTNode::BDTNode(string& name)
        : _value(name), _left(nullptr), _right(nullptr), _self(nullptr) { }

BDTNode::BDTNode(BDTParent parent, string& name)
        : _value(name), _left(nullptr), _right(nullptr), _self(nullptr) {

    _parents.push_back(parent);
}

void BDTNode::generateChildren(string formula, shared_ptr<BDTNode>& self, string& names, vector<shared_ptr<BDTNode> >& leafs) {
    _self = self;

    if (names != ""){
        string name = names.substr(0, 1);
        string new_names = names.substr(1, names.size() - 1);

        string leftFormula = substituteVariable(formula, _value, "0");
        shared_ptr<BDTNode> left(new BDTNode(BDTParent(_self, false), name));
        setLeft(left);
        left->generateChildren(leftFormula, getLeft(), new_names, leafs);

        string rightFormula = substituteVariable(formula, _value, "1");
        shared_ptr<BDTNode> right(new BDTNode(BDTParent(_self, true), name));
        setTrue(right);
        right->generateChildren(rightFormula, getTrue(), new_names, leafs);
    }
    else{
        string leftFormula = substituteVariable(formula, _value, "0");
        string leftName = evaluateFormula(leftFormula);
        shared_ptr<BDTNode> left(new BDTNode(BDTParent(_self, false), leftName));
        setLeft(left);
        left->setSelf(getLeft());
        leafs.push_back(getLeft());

        string rightFormula = substituteVariable(formula, _value, "1");
        string rightName = evaluateFormula(rightFormula);
        shared_ptr<BDTNode> right(new BDTNode(BDTParent(_self, true), rightName));
        setTrue(right);
        right->setSelf(getTrue());
        leafs.push_back(getTrue());
    }
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

bool BDTNode::setTrue(shared_ptr<BDTNode>& child) {
    _right = child;
    return true;
}
shared_ptr<BDTNode>& BDTNode::getTrue() {
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

pair<string, bool> BDTNode::minimize(shared_ptr<BDTNode>& falseChild, shared_ptr<BDTNode>& trueChild) {
    if (isTerminal())
        return make_pair(_value, true);
    else{
        pair<string, bool> a = getLeft()->minimize(falseChild, trueChild);
        pair<string, bool> b = getTrue()->minimize(falseChild, trueChild);

        if (a.second && b.second && a.first == b.first){
            for (int i = 0; i < _parents.size(); ++i) {
                shared_ptr<BDTNode> parent = _parents[i].getParent();
                bool branch = _parents[i].getBranch();

                shared_ptr<BDTNode> null(nullptr);
                setTrue(null);
                setLeft(null);

                if (a.first == "0"){
                    falseChild->removeParent(_self, true);
                    falseChild->removeParent(_self, false);
                    if (branch)
                        parent->setTrue(falseChild);
                    else
                        parent->setLeft(falseChild);
                    falseChild->addParent(parent, _parents[i].getBranch());
                }
                else if (a.first == "1"){
                    trueChild->removeParent(_self, true);
                    trueChild->removeParent(_self, false);
                    if (branch)
                        parent->setTrue(trueChild);
                    else
                        parent->setLeft(trueChild);
                    trueChild->addParent(parent, _parents[i].getBranch());
                }
            }

            if (isRoot()){
                shared_ptr<BDTNode> null(nullptr);
                setLeft(null);
                setTrue(null);

                if (a.first == "0"){
                    falseChild->removeParent(_self, true);
                    falseChild->removeParent(_self, false);
                    falseChild->addParent(_self, false);
                }
                else if (a.first == "1"){
                    trueChild->removeParent(_self, true);
                    trueChild->removeParent(_self, false);
                    trueChild->addParent(_self, false);
                }
            }

            return make_pair(a.first, true);
        }
        else {
            if (a.second){
                if (a.first == "0"){
                    setLeft(falseChild);
                    falseChild->addParent(_self, false);
                }
                else if (a.first == "1"){
                    setLeft(trueChild);
                    trueChild->addParent(_self, false);
                }
            }

            if (b.second){
                if (b.first == "0"){
                    setTrue(falseChild);
                    falseChild->addParent(_self, true);
                }
                else if (b.first == "1"){
                    setTrue(trueChild);
                    trueChild->addParent(_self, true);
                }
            }
            return make_pair(_value, false);
        }
    }
}

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