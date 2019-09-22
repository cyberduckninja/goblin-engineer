#pragma once

#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <set>

namespace goblin_engineer {

template<>
struct dynamic_converter<dynamic_config> {
    typedef const dynamic_config& result_type;

    static inline
    const dynamic_config&
    convert(const dynamic_config& from) {
        return from;
    }

    static inline
    bool
    convertible(const dynamic_config&) {
        return true;
    }
};

template<>
struct dynamic_converter<bool> {
    typedef bool result_type;

    static inline result_type convert(const dynamic_config& from) {
        return from.as_bool();
    }

    static inline bool convertible(const dynamic_config& from) {
        return from.is_bool();
    }
};

template<class To>
struct dynamic_converter<
    To,
    typename std::enable_if<std::is_arithmetic<To>::value>::type
>
{
    typedef To result_type;

    static inline result_type convert(const dynamic_config& from) {
        if(from.is_int()) {
            return from.as_int();
        } else if(from.is_uint()) {
            return from.as_uint();
        } else {
            return from.as_double();
        }
    }

    static inline bool convertible(const dynamic_config& from) {
        return from.is_int() || from.is_uint() || from.is_double();
    }
};

template<class To>
struct dynamic_converter<
    To,
    typename std::enable_if<std::is_enum<To>::value>::type
>
{
    typedef To result_type;
    typedef typename std::underlying_type<To>::type underlying_type;

    static inline result_type convert(const dynamic_config& from) {
        return static_cast<result_type>(dynamic_converter<underlying_type>::convert(from));
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        return dynamic_converter<underlying_type>::convertible(from);
    }
};

template<>
struct dynamic_converter<std::string> {
    typedef const std::string& result_type;

    static inline
    result_type convert(const dynamic_config& from) {
        return from.as_string();
    }

    static inline bool convertible(const dynamic_config& from) {
        return from.is_string();
    }
};

template<>
struct dynamic_converter<const char*> {
    typedef const char *result_type;

    static inline result_type convert(const dynamic_config& from) {
        return from.as_string().c_str();
    }

    static inline bool convertible(const dynamic_config& from) {
        return from.is_string();
    }
};

template<>
struct dynamic_converter<std::vector<dynamic_config>> {
    typedef const std::vector<dynamic_config>& result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        return from.as_array();
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        return from.is_array();
    }
};

template<class T>
struct dynamic_converter<std::vector<T>> {
    typedef std::vector<T> result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        std::vector<T> result;
        const dynamic_config::array_t& array = from.as_array();

        for(size_t i = 0; i < array.size(); ++i) {
            result.emplace_back(array[i].to<T>());
        }

        return result;
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        return from.is_array() && std::all_of(from.as_array().begin(), from.as_array().end(),
            std::bind(&dynamic_config::convertible_to<T>, std::placeholders::_1)
        );
    }
};

template<class T>
struct dynamic_converter<std::set<T>> {
    typedef std::set<T> result_type;

    static inline result_type convert(const dynamic_config& from) {
        std::set<T> result;
        const dynamic_config::array_t& array = from.as_array();

        for(size_t i = 0; i < array.size(); ++i) {
            result.insert(array[i].to<T>());
        }

        return result;
    }

    static inline bool convertible(const dynamic_config& from) {
        return from.is_array() && std::all_of(from.as_array().begin(), from.as_array().end(),
            std::bind(&dynamic_config::convertible_to<T>, std::placeholders::_1)
        );
    }
};

template<class... Args>
struct dynamic_converter<std::tuple<Args...>> {
    typedef std::tuple<Args...> result_type;

    static inline result_type convert(const dynamic_config& from) {
        if(sizeof...(Args) == from.as_array().size()) {
            if(sizeof...(Args) == 0) {
                return result_type();
            } else {
                return range_applier<sizeof...(Args) - 1>::convert(from.as_array());
            }
        } else {
            throw std::bad_cast();
        }
    }

    static inline bool convertible(const dynamic_config& from) {
        if(from.is_array() && sizeof...(Args) == from.as_array().size()) {
            if(sizeof...(Args) == 0) {
                return true;
            } else {
                return range_applier<sizeof...(Args) - 1>::is_convertible(from.as_array());
            }
        } else {
            return false;
        }
    }

private:
    template<size_t... Idxs>
    struct range_applier;

    template<size_t First, size_t... Idxs>
    struct range_applier<First, Idxs...> :
        range_applier<First - 1, First, Idxs...>
    {
        static inline
        bool
        is_convertible(const dynamic_config::array_t& from) {
            return from[First].convertible_to<typename std::tuple_element<First, result_type>::type>() &&
                   range_applier<First - 1, First, Idxs...>::is_convertible(from);
        }
    };

    template<size_t... Idxs>
    struct range_applier<0, Idxs...> {
        static inline
        result_type
        convert(const dynamic_config::array_t& from) {
            return std::tuple<Args...>(
                from[0].to<typename std::tuple_element<0, result_type>::type>(),
                from[Idxs].to<typename std::tuple_element<Idxs, result_type>::type>()...
            );
        }

        static inline
        bool
        is_convertible(const dynamic_config::array_t& from) {
            return from[0].convertible_to<typename std::tuple_element<0, result_type>::type>();
        }
    };
};

template<class First, class Second>
struct dynamic_converter<std::pair<First, Second>> {
    typedef std::pair<First, Second> result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        if(from.as_array().size() == 2) {
            return std::make_pair(from.as_array()[0].to<First>(), from.as_array()[1].to<Second>());
        } else {
            throw std::bad_cast();
        }
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        if(from.is_array() && from.as_array().size() == 2) {
            return from.as_array()[0].convertible_to<First>() &&
                   from.as_array()[1].convertible_to<Second>();
        } else {
            return false;
        }
    }
};

template<>
struct dynamic_converter<dynamic_config::object_t> {
    typedef const dynamic_config::object_t& result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        return from.as_object();
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        return from.is_object();
    }
};

template<>
struct dynamic_converter<std::map<std::string, dynamic_config>> {
    typedef const std::map<std::string, dynamic_config>& result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        return from.as_object();
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        return from.is_object();
    }
};

template<class T>
struct dynamic_converter<std::map<std::string, T>> {
    typedef std::map<std::string, T> result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        result_type result;
        const dynamic_config::object_t& object = from.as_object();

        for(auto it = object.begin(); it != object.end(); ++it) {
            result.insert(typename result_type::value_type(it->first, it->second.to<T>()));
        }

        return result;
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        if(!from.is_object()) {
            return false;
        }

        const dynamic_config::object_t& object = from.as_object();

        for(auto it = object.begin(); it != object.end(); ++it) {
            if(!it->second.convertible_to<T>()) {
                return false;
            }
        }

        return true;
    }
};

template<class T>
struct dynamic_converter<std::unordered_map<std::string, T>> {
    typedef std::unordered_map<std::string, T> result_type;

    static inline
    result_type
    convert(const dynamic_config& from) {
        result_type result;
        const dynamic_config::object_t& object = from.as_object();

        for(auto it = object.begin(); it != object.end(); ++it) {
            result.insert(typename result_type::value_type(it->first, it->second.to<T>()));
        }

        return result;
    }

    static inline
    bool
    convertible(const dynamic_config& from) {
        if(!from.is_object()) {
            return false;
        }

        const dynamic_config::object_t& object = from.as_object();

        for(auto it = object.begin(); it != object.end(); ++it) {
            if(!it->second.convertible_to<T>()) {
                return false;
            }
        }

        return true;
    }
};

}

