
#include <iostream>
#include <string>

#include "BfCode.h"

using namespace std;

int main() {
    string caminho = "Codigos_para_teste\\";
    string filename;

    cout << " - 99beer.bf\n";
    cout << " - hello_world.bf\n";
    cout << " - squares.bf\n";
    cout << " - mandelbrot.bf\n";
    cout << " - triangle.bf\n\n";
    cin >> filename;

    BfCode code(caminho, filename);
    code.Interpret();
    code.Bf_to_Cpp();
    code.Createlog();

    cin.ignore();
    cin.get();
    return 0;
}
