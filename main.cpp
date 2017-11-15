#include <range/v3/all.hpp>
#include <iostream>

using namespace ranges;

auto draw_char(int bar_length)
{
    return view::transform([bar_length](int position)
    {
        return position >= bar_length ? '.' : '*';
    });
}

auto draw_bar(int max_length)
{
    return view::transform([max_length](int bar_length) 
    {
        return view::iota(0, max_length) | draw_char(bar_length) | view::reverse;
    });
}

template <class Ranges>
class interleave_view : public view_facade<interleave_view<Ranges>>
{
public:
    interleave_view() = default;

    explicit interleave_view(Ranges range)
        : ranges_(std::move(range))
    {
    }

private:
    friend range_access;
    std::vector<range_value_type_t<Ranges>> ranges_;

    struct cursor
    {
        decltype(auto) read() const
        {
            return *iterators[n];
        }

        void next()
        {
            if (0 == ((++n) %= iterators.size()))
            {
                for_each(iterators, [](auto& i) { ++i; });
            }
        }

        bool equal(default_sentinel) const
        {

            iterator_t<range_value_type_t<Ranges>> i = ranges::end(range->front());
            return *iterators.begin() == i;
        }

        bool equal(const cursor& other) const
        {
            return (other.range == nullptr && equal(default_sentinel{}))
                || (other.range == nullptr && range == nullptr)
                || (n == other.n && iterators == other.iterators); 
        }

        size_t n;
        const std::vector<range_value_type_t<Ranges>>* range;
        std::vector<iterator_t<range_value_type_t<Ranges>>> iterators;
    };

    cursor begin_cursor() const
    {
        return {0, &ranges_, view::transform(ranges_, ranges::begin)};
    }

    cursor end_cursor() const
    {
        return {0, nullptr};
    }
};


auto interleave()
{
    return make_pipeable([](auto&& ranges)
    {
        using ranges_type = decltype(ranges);

        return interleave_view<view::all_t<ranges_type>>(
                view::all(std::forward<ranges_type>(ranges)));
    });
}

void print(const auto& range)
{
    for_each(range, [](auto i)
    {
        std::cout << i << std::endl;
    });
}


int main(int argc, char* argv[])
{
    auto sample = view::repeat(4) | view::take(10);

    print(sample | draw_bar(10)); 
    print(sample | draw_bar(10) | interleave() | view::chunk(10));

    return 0;
}


