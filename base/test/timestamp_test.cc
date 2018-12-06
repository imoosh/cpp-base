//
// Created by win on 11/7/18.
//

#include <base/time/timestamp.h>

#include <base/types/types.h>


int main(int argc, const char *argv[])
{
    cout << TimeStamp::now().toFormattedString() << endl;
    cout << TimeStamp::now().toFormattedString(false) << endl;
    cout << TimeStamp::now().toString() << endl;
    cout << TimeStamp::now().toString(true) << endl;
    cout << TimeStamp::now().toStampString() << endl;
    cout << TimeStamp::now().toStampString(true) << endl;

    return 0;
}