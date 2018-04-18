#include <iostream>
#include "utc_to_string.h"

using namespace std;
using namespace boost::chrono;

int main()
{
    system_clock::time_point tp = system_clock::now();
    string str = utc_to_string(tp);
    cout << "now: " << str << endl;

    return 0;
}
