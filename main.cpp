#include <iostream>
#include "BDTree.h"

using namespace std;

int main() {
    string formula = "a&b&c&d&e&f&g&h&i&j&k&l&m&n&o&p&q&r&s&t";
    BDTree tree(formula);
    tree.minimize();

    cout << tree.toString();

    return 0;
}