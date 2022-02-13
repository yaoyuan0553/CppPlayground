//
// Created by yuan on 2/3/22.
//

#include <functional>
#include <string>
#include <iostream>
#include <memory>

struct Foo {
    int bar;

    void printBar() const { printf("bar = %d\n", bar); }
};

void test()
{
    auto str = std::make_unique<std::string>();
    std::cin >> *str;
//    std::string str;
//    std::cin >> str;

    auto invokeAndPrintResult = [](auto&&... args) {
        std::cout << std::invoke(std::forward<decltype(args)>(args)...) << '\n';
        std::cout << __PRETTY_FUNCTION__ << '\n';
    };

    invokeAndPrintResult(&decltype(str)::element_type::length, str);
    invokeAndPrintResult(printf, "blah %d", 3);

    Foo foo{4};
    invokeAndPrintResult(&decltype(foo)::bar, foo);
}


int main()
{
    test();
}
