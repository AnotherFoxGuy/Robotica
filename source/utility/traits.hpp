#pragma once

#include <utility/typedefs.hpp>

#include <type_traits>
#include <tuple>
#include <cstddef>


namespace robotica {
    template <typename Tuple, typename Pred, std::size_t N = 0>
    constexpr inline void expand(const Tuple& tpl, const Pred& pred) {
        if constexpr (std::tuple_size_v<Tuple> > N) pred(std::get<N>(tpl));
        if constexpr (std::tuple_size_v<Tuple> > N + 1) expand<Tuple, Pred, N + 1>(tpl, pred);
    }



    template <typename C, typename T> struct member_variable_info {
        using classname = C;
        using type = T;

        member_variable_info(MemVar<C, T>) {}
    };

    template <typename T> using make_member_variable_info = decltype(member_variable_info(std::declval<T>()));



    template <typename... Ts> struct pack {
        template <template <typename...> typename X> using apply_expand   = X<Ts...>;
        template <template <typename...> typename X> using apply_compress = pack<X<Ts>...>;
    };


    template <template <typename...> typename X, typename T> struct bind_template_front {
        template <typename... Ts> using type = X<T, Ts...>;
    };


    template <template <typename...> typename X, typename T> struct bind_template_back {
        template <typename... Ts> using type = X<Ts..., T>;
    };
}