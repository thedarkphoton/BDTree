#include <iostream>
#include "BDTree.h"

using namespace std;

int main() {
    string formula = "(A|!B)&(A|!B)";
    BDTree tree(formula);

    cout << tree;

    return 0;
}