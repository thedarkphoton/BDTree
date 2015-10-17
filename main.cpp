#include <iostream>
#include "BDTree.h"

using namespace std;

int main() {
    string formula = "(a&b|c)|!(a&b|c)";
    BDTree tree(formula);
    tree.minimize();

    cout << tree.toString();

    return 0;
}