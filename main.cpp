#include <iostream>
#include "Parser/Parser.h"

int main() {
    Parser *p = new Parser();
    p->parse("/home/mdronski/IET/semestr_6/ADPTO/projekt/test.txt");
    return 0;
}