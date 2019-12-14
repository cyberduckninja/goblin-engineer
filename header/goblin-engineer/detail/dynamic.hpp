#pragma once

#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>


namespace goblin_engineer {

        template<class T>
    class incomplete_wrapper {
        std::unique_ptr<T> m_data;

    public:

        incomplete_wrapper() = default;

        incomplete_wrapper(const incomplete_wrapper&) {

        }

        incomplete_wrapper& operator=(const incomplete_wrapper&) {
            return *this;
        }

        T& get() {
            return *m_data;
        }

        const T& get() const {
            return *m_data;
        }

        template<class... Args>
        void set(Args&&... args) {
            m_data.reset(new T(std::forward<Args>(args)...));
        }
    };

    template<class Visitor, class Result>
    struct dynamic_visitor_applier : public boost::static_visitor<Result> {
        dynamic_visitor_applier(Visitor v) :
                m_visitor(v)
        { }

        template<class T>
        Result operator()(T& v) const {
            return m_visitor(static_cast<const T&>(v));
        }

        template<class T>
        Result operator()(incomplete_wrapper<T>& v) const {
            return m_visitor(v.get());
        }

    private:
        Visitor m_visitor;
    };

    template<class ConstVisitor, class Result>
    struct const_visitor_applier : public boost::static_visitor<Result> {
        const_visitor_applier(ConstVisitor v) : m_const_visitor(v) { }

        template<class T>
        Result operator()(T& v) const {
            return m_const_visitor(static_cast<const T&>(v));
        }

        template<class T>
        Result operator()(incomplete_wrapper<T>& v) const {
            return m_const_visitor(static_cast<const T&>(v.get()));
        }

    private:
        ConstVisitor m_const_visitor;
    };

    template<class From, class = void>
    struct dynamic_constructor {
        static const bool enable = false;
    };

    template<class To, class = void>
    struct dynamic_converter {
    };

    template<class T>
    struct pristine {
        using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    };

    class dynamic_config final {
    public:
        using bool_t =  bool;
        using int_t = int64_t;
        using uint_t = uint64_t;
        using double_t = double;
        using string_t = std::string;
        using array_t = std::vector<dynamic_config>;

        struct null_t final {
            bool operator==(const null_t &) const {
                return true;
            }
        };

        class object_t;

        using value_t =  boost::variant<
                null_t,
                bool_t,
                int_t,
                uint_t,
                double_t,
                string_t,
                incomplete_wrapper<array_t>,
                incomplete_wrapper<object_t>
        >;

        static const dynamic_config null;
        static const dynamic_config empty_string;
        static const dynamic_config empty_array;
        static const dynamic_config empty_object;

    public:
        dynamic_config();

        dynamic_config(const dynamic_config &other);

        dynamic_config(dynamic_config &&other);

        template<class T>
        dynamic_config(T &&from, typename std::enable_if<dynamic_constructor<typename pristine<T>::type>::enable>::type * = 0) : m_value(null_t()) {
            dynamic_constructor<typename pristine<T>::type>::convert(std::forward<T>(from), m_value);
        }

        dynamic_config &operator=(const dynamic_config &other);

        dynamic_config &operator=(dynamic_config &&other);

        template<class T>
        typename std::enable_if<dynamic_constructor<typename pristine<T>::type>::enable, dynamic_config &>::type
        operator=(T &&from){
            dynamic_constructor<typename pristine<T>::type>::convert(std::forward<T>(from), m_value);
            return *this;
        }

        bool operator==(const dynamic_config &other) const;

        bool operator!=(const dynamic_config &other) const;

        template<class Visitor>
        typename Visitor::result_type
        apply(const Visitor &visitor) {
            return boost::apply_visitor(
                    dynamic_visitor_applier<const Visitor &, typename Visitor::result_type>(visitor),
                    m_value
            );
        }

        template<class Visitor>
        typename Visitor::result_type
        apply(Visitor &visitor) {
            return boost::apply_visitor(
                    dynamic_visitor_applier<Visitor &, typename Visitor::result_type>(visitor),
                    m_value
            );
        }

        template<class Visitor>
        typename Visitor::result_type
        apply(const Visitor &visitor) const {
            return boost::apply_visitor(
                    const_visitor_applier<const Visitor &, typename Visitor::result_type>(visitor),
                    m_value
            );
        }

        template<class Visitor>
        typename Visitor::result_type
        apply(Visitor &visitor) const {
            return boost::apply_visitor(
                    const_visitor_applier<Visitor &, typename Visitor::result_type>(visitor),
                    m_value
            );
        }

        bool is_null() const;

        bool is_bool() const;

        bool is_int() const;

        bool is_uint() const;

        bool is_double() const;

        bool is_string() const;

        bool is_array() const;

        bool is_object() const;

        bool_t as_bool() const;

        int_t as_int() const;

        uint_t as_uint() const;

        double_t as_double() const;

        const string_t &as_string() const;

        const array_t &as_array() const;

        const object_t &as_object() const;

        string_t &as_string();

        array_t &as_array();

        object_t &as_object();

        template<class T>
        bool convertible_to() const{
            return dynamic_converter<typename pristine<T>::type>::convertible(*this);
        }

        template<class T>
        typename dynamic_converter<typename pristine<T>::type>::result_type to() const {
            return dynamic_converter<typename pristine<T>::type>::convert(*this);
        }

    private:
        template<class T>
        T &get() {
            try {
                return boost::get<T>(m_value);
            } catch (const boost::bad_get &e) {
                throw std::out_of_range("failed to get node valu");
            }
        }

        template<class T>
        const T &get() const {
            return const_cast<dynamic_config *>(this)->get<T>();
        }

        template<class T>
        bool is() const {
            return static_cast<bool>(boost::get<T>(&m_value));
        }

    private:
        value_t mutable m_value;
    };

}

#include "goblin-engineer/detail/object.hpp"
#include "constructors.hpp"
#include "converters.hpp"

namespace boost {

    template<>
    std::string lexical_cast<std::string, goblin_engineer::dynamic_config>(const goblin_engineer::dynamic_config &);

}

