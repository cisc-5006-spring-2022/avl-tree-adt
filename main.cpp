#include <iostream>
#include "TreeType.h"

using namespace std;

int main() {
    cout << "I'm a tree" << endl;

    TreeType<int> t;

    t.PutItem(10);
    t.PutItem(15);
    t.PutItem(12);

    t.PrintTree();
}