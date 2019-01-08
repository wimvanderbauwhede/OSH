#include "abstract_system_alu.h"
#include <iostream>

int main() {
    for (int i=0;i<9;i++) {
        std::cout << alu[i](6,7) << "\n";
    }
    exit(0);
}
