#include <iostream>
#include "BDTree.h"

using namespace std;

int main() {
    string formula = "(a&b)|(c&d)";
    BDTree tree(formula);

    cout << tree;

    return 0;
}