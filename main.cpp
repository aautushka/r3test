#include <range/v3/all.hpp>
#include <iostream>

using namespace ranges;

auto to_string()
{
    return view::transform( [](int i) { return view::single(i); });
}

auto to_bar()
{
    return view::transform([](int i) {
        return view::iota(0, 10) | view::transform([i](int j) { return j >= i ? ' ' : '*';} ) | view::reverse;
    });
}

/* auto transpose_bar() */
/* { */
/*     return view::transform([](auto bar) */
/*     { */
/*         return bar | transpose(); */
/*     }; */
/* } */

int main(int argc, char* argv[])
{
    auto r = view::iota(0, 10) | view::take(3);
    auto r2 = view::ints(1) | view::transform([](int i){return i * i;}) | view::take(10);

    auto r3 = view::iota(0, 10) | to_bar();

    for_each(r3, [](auto i)
    {
        std::cout << i << std::endl;
    });
    return 0;
}


