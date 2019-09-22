#pragma once

#include <tuple>
#include <unordered_map>

namespace goblin_engineer {

template<>
struct dynamic_constructor<dynamic_config::null_t> {
    static const bool enable = true;

    static inline
    void
    convert(const dynamic_config::null_t& from, dynamic_config::value_t& to) {
        to = from;
    }
};

template<>
struct dynamic_constructor<bool> {
    static const bool enable = true;

    static inline
    void
    convert(bool from, dynamic_config::value_t& to) {
        to = dynamic_config::bool_t(from);
    }
};

template<class From>
struct dynamic_constructor<
    From,
    typename std::enable_if<std::is_integral<From>::value && std::is_unsigned<From>::value>::type
>
{
    static const bool enable = true;

    static inline
    void
    convert(From from, dynamic_config::value_t& to) {
        to = dynamic_config::uint_t(from);
    }
};

template<class From>
struct dynamic_constructor<
    From,
    typename std::enable_if<std::is_integral<From>::value && std::is_signed<From>::value>::type
>
{
    static const bool enable = true;

    static inline
    void
    convert(From from, dynamic_config::value_t& to) {
        to = dynamic_config::int_t(from);
    }
};

template<class From>
struct dynamic_constructor<
    From,
    typename std::enable_if<std::is_enum<From>::value>::type
>
{
    static const bool enable = true;

    static inline
    void
    convert(const From& from, dynamic_config::value_t& to) {
        if (std::is_signed<typename std::underlying_type<From>::type>::value) {
            to = dynamic_config::int_t(from);
        } else {
            to = dynamic_config::uint_t(from);
        }
    }
};

template<class From>
struct dynamic_constructor<
    From,
    typename std::enable_if<std::is_floating_point<From>::value>::type
>
{
    static const bool enable = true;

    static inline
    void
    convert(From from, dynamic_config::value_t& to) {
        to = dynamic_config::double_t(from);
    }
};

template<size_t N>
struct dynamic_constructor<char[N]> {
    static const bool enable = true;

    static inline
    void
    convert(const char* from, dynamic_config::value_t& to) {
        to = dynamic_config::string_t();
        boost::get<dynamic_config::string_t>(to).assign(from, N - 1);
    }
};

template<>
struct dynamic_constructor<std::string> {
    static const bool enable = true;

    static inline
    void
    convert(const std::string& from, dynamic_config::value_t& to) {
        to = from;
    }

    static inline
    void
    convert(std::string&& from, dynamic_config::value_t& to) {
        to = dynamic_config::string_t();
        boost::get<dynamic_config::string_t>(to) = std::move(from);
    }
};

template<>
struct dynamic_constructor<dynamic_config::array_t> {
    static const bool enable = true;

    template<class Array>
    static inline
    void
    convert(Array&& from, dynamic_config::value_t& to) {
        to = incomplete_wrapper<dynamic_config::array_t>();
        boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).set(std::forward<Array>(from));
    }
};

template<class T>
struct dynamic_constructor<
    std::vector<T>,
    typename std::enable_if<!std::is_same<T, dynamic_config>::value>::type
>
{
    static const bool enable = true;

    static inline
    void
    convert(const std::vector<T>& from, dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::array_t>::convert(dynamic_config::array_t(), to);

        auto& array = boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).get();
        array.reserve(from.size());

        for(size_t i = 0; i < from.size(); ++i) {
            array.emplace_back(from[i]);
        }
    }

    static inline
    void
    convert(std::vector<T>&& from, dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::array_t>::convert(dynamic_config::array_t(), to);

        auto& array = boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).get();
        array.reserve(from.size());

        for(size_t i = 0; i < from.size(); ++i) {
            array.emplace_back(std::move(from[i]));
        }
    }
};

template<class T, size_t N>
struct dynamic_constructor<T[N]> {
    static const bool enable = true;

    static inline
    void
    convert(const T (&from)[N], dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::array_t>::convert(dynamic_config::array_t(), to);

        auto& array = boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).get();
        array.reserve(N);

        for(size_t i = 0; i < N; ++i) {
            array.emplace_back(from[i]);
        }
    }

    static inline
    void
    convert(T (&&from)[N], dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::array_t>::convert(dynamic_config::array_t(), to);

        auto& array = boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).get();
        array.reserve(N);

        for(size_t i = 0; i < N; ++i) {
            array.emplace_back(std::move(from[i]));
        }
    }
};

template<class... Args>
struct dynamic_constructor<std::tuple<Args...>> {
    static const bool enable = true;

    template<size_t N, size_t I, class... Args2>
    struct copy_tuple_to_vector {
        static inline
        void
        convert(const std::tuple<Args2...>& from, dynamic_config::array_t& to) {
            to.emplace_back(std::get<I - 1>(from));
            copy_tuple_to_vector<N, I + 1, Args2...>::convert(from, to);
        }
    };

    template<size_t N, class... Args2>
    struct copy_tuple_to_vector<N, N, Args2...> {
        static inline
        void
        convert(const std::tuple<Args2...>& from, dynamic_config::array_t& to) {
            to.emplace_back(std::get<N - 1>(from));
        }
    };

    template<class... Args2>
    struct copy_tuple_to_vector<0, 1, Args2...> {
        static inline
        void
        convert(const std::tuple<Args2...>&, dynamic_config::array_t&) {
            // Empty.
        }
    };

    template<size_t N, size_t I, class... Args2>
    struct move_tuple_to_vector {
        static inline
        void
        convert(std::tuple<Args2...>& from, dynamic_config::array_t& to) {
            to.emplace_back(std::move(std::get<I - 1>(from)));
            move_tuple_to_vector<N, I + 1, Args2...>::convert(from, to);
        }
    };

    template<size_t N, class... Args2>
    struct move_tuple_to_vector<N, N, Args2...> {
        static inline
        void
        convert(std::tuple<Args2...>& from, dynamic_config::array_t& to) {
            to.emplace_back(std::move(std::get<N - 1>(from)));
        }
    };

    template<class... Args2>
    struct move_tuple_to_vector<0, 1, Args2...> {
        static inline
        void
        convert(std::tuple<Args2...>&, dynamic_config::array_t&) {
            // Empty.
        }
    };

    static inline
    void
    convert(const std::tuple<Args...>& from, dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::array_t>::convert(dynamic_config::array_t(), to);

        auto& array = boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).get();
        array.reserve(sizeof...(Args));

        copy_tuple_to_vector<sizeof...(Args), 1, Args...>::convert(from, array);
    }

    static inline
    void
    convert(std::tuple<Args...>&& from, dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::array_t>::convert(dynamic_config::array_t(), to);

        auto& array = boost::get<incomplete_wrapper<dynamic_config::array_t>>(to).get();
        array.reserve(sizeof...(Args));

        move_tuple_to_vector<sizeof...(Args), 1, Args...>::convert(from, array);
    }
};

template<>
struct dynamic_constructor<dynamic_config::object_t> {
    static const bool enable = true;

    template<class Object>
    static inline
    void
    convert(Object&& from, dynamic_config::value_t& to) {
        to = incomplete_wrapper<dynamic_config::object_t>();
        boost::get<incomplete_wrapper<dynamic_config::object_t>>(to).set(std::forward<Object>(from));
    }
};

struct dynamic_map_constructor {
    static const bool enable = true;

    template<class From>
    using direct_conversion = std::is_same<typename std::decay<From>::type, std::map<std::string, dynamic_config>>;

    template<class From>
    static inline typename std::enable_if<direct_conversion<From>::value>::type convert(From&& from, dynamic_config::value_t& to) {
        to = incomplete_wrapper<dynamic_config::object_t>();
        boost::get<incomplete_wrapper<dynamic_config::object_t>>(to).set(std::forward<From>(from));
    }

    template<class From>
    static inline typename std::enable_if<!direct_conversion<From>::value>::type convert(From&& from, dynamic_config::value_t& to) {
        dynamic_constructor<dynamic_config::object_t>::convert(dynamic_config::object_t(), to);

        auto& object = boost::get<incomplete_wrapper<dynamic_config::object_t>>(to).get();

        for(auto it = from.begin(); it != from.end(); ++it) {
            object.emplace(boost::lexical_cast<std::string>(it->first), std::move(it->second));
        }
    }
};

template<class Key, class Value>
struct dynamic_constructor<std::map<Key, Value>> : public dynamic_map_constructor {};

template<class Key, class Value>
struct dynamic_constructor<std::unordered_map<Key, Value>> : public dynamic_map_constructor {};

template<class T>
struct dynamic_constructor<std::reference_wrapper<T>> {
        static const bool enable = true;

        static inline void convert(std::reference_wrapper<T> from, dynamic_config::value_t& to) {
            dynamic_constructor<typename std::remove_const<T>::type>::convert(from.get(), to);
        }
};


}

