#include "fetcher.h"
#include <iostream>
using namespace std;
int main()
{
    Fetcher fetcher;
    string url = "https://gcdharamshala.in/";
    string res = fetcher.fetch(url);
    cout << res << endl;
}