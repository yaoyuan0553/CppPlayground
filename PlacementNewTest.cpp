//
// Created by yuan on 2/12/22.
//

#include <new>
#include <type_traits>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    aligned_storage_t<sizeof(string), alignof(string)> buf[10];

    cout << sizeof(buf) << '\n';
    cout << &buf[0] << '\n';
    cout << &buf[1] << '\n';

    auto* str1 = new (&buf[0]) string("blahasfasdfasdfasdqwerqwer");
    cout << *str1 << '\n';
    new (&buf[0]) string("okaykzz");
    destroy_at(static_cast<string*>(static_cast<void*>(&buf[0])));

    cout << *str1 << '\n';
}
