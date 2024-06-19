#include <iostream>
#include "pacman.hh"
using namespace std;

int main()
{
    position p1(10,33);
    position p2(11,23);
    if (p1 == p2){
        cout<<"bonjour"<<"\n";
    }else {
        cout<<"non"<<"\n";
    }
    cout<<p1;
    cout << "Hello World!" << endl;
    return 0;
}
