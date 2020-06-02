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




    template <typename C, typename T> struct member_variable_info {
        using classname = C;
        using type = T;

        member_variable_info(MemVar<C, T> const) {}
    };


    template <typename C, typename T, typename... A> struct member_function_info {
        using classname = C;
        using return_type = T;
        using arguments = pack<A...>;

        member_function_info(MemFn<C, T, A...>) {}
        member_function_info(CMemFn<C, T, A...>) {}
    };


    template <typename T> using make_member_variable_info = decltype(member_variable_info(std::declval<T>()));
    template <typename T> using make_member_function_info = decltype(member_function_info(std::declval<T>()));
}