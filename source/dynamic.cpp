#include <goblin-engineer/detail/dynamic.hpp>

using namespace goblin_engineer;

const dynamic_config dynamic_config::null = dynamic_config::null_t();
const dynamic_config dynamic_config::empty_string = dynamic_config::string_t();
const dynamic_config dynamic_config::empty_array = dynamic_config::array_t();
const dynamic_config dynamic_config::empty_object = dynamic_config::object_t();

goblin_engineer::dynamic_config &dynamic_config::object_t::at(const std::string &key) {
    auto it = find(key);

    if (it == end()) {
        std::string msg;
        msg.append("dynamic_config: key ");
        msg.append(key);
        msg.append(" not found");
        throw std::out_of_range(msg);
    }
    return it->second;
}

const goblin_engineer::dynamic_config &
dynamic_config::object_t::at(const std::string &key) const {
    auto it = find(key);

    if (it == end()) {
        std::string msg;
        msg.append("dynamic_config: key ");
        msg.append(key);
        msg.append(" not found");
        throw std::out_of_range(msg);
    }
    return it->second;
}

goblin_engineer::dynamic_config &dynamic_config::object_t::at(const std::string &key, goblin_engineer::dynamic_config &default_) {
    auto it = find(key);

    if (it == end()) {
        return default_;
    } else {
        return it->second;
    }
}

const goblin_engineer::dynamic_config &dynamic_config::object_t::at(const std::string &key, const goblin_engineer::dynamic_config &default_) const {
    auto it = find(key);

    if (it == end()) {
        return default_;
    } else {
        return it->second;
    }
}

const goblin_engineer::dynamic_config &dynamic_config::object_t::operator[](const std::string &key) const {
    return at(key);
}

struct move_visitor : public boost::static_visitor<> {
    move_visitor(dynamic_config &destination) : m_destination(destination) {}

    template<class T>
    void operator()(T &v) const {
        m_destination = std::move(v);
    }

private:
    dynamic_config &m_destination;
};

struct assign_visitor : public boost::static_visitor<> {
    assign_visitor(dynamic_config &destination) : m_destination(destination) {}

    template<class T>
    void operator()(T &v) const {
        m_destination = v;
    }

private:
    dynamic_config &m_destination;
};

struct equals_visitor : public boost::static_visitor<bool> {
    equals_visitor(const dynamic_config &other) :
            m_other(other) {}

    bool operator()(const dynamic_config::null_t &) const {
        return m_other.is_null();
    }

    bool operator()(const dynamic_config::bool_t &v) const {
        return m_other.is_bool() && m_other.as_bool() == v;
    }

    bool operator()(const dynamic_config::int_t &v) const {
        if (m_other.is_int()) {
            return m_other.as_int();
        } else {
            return m_other.is_uint() && v >= 0 && static_cast<dynamic_config::uint_t>(v) == m_other.as_uint();
        }
    }

    bool operator()(const dynamic_config::uint_t &v) const {
        if (m_other.is_uint()) {
            return m_other.as_uint();
        } else {
            return m_other.is_int() && m_other.as_int() >= 0 && v == m_other.to<dynamic_config::uint_t>();
        }
    }

    bool operator()(const dynamic_config::double_t &v) const {
        return m_other.is_double() && m_other.as_double() == v;
    }

    bool operator()(const dynamic_config::string_t &v) const {
        return m_other.is_string() && m_other.as_string() == v;
    }

    bool operator()(const dynamic_config::array_t &v) const {
        return m_other.is_array() && m_other.as_array() == v;
    }

    bool operator()(const dynamic_config::object_t &v) const {
        return m_other.is_object() && m_other.as_object() == v;
    }

private:
    const dynamic_config &m_other;
};

dynamic_config::dynamic_config() :
        m_value(null_t()) {}

dynamic_config::dynamic_config(const dynamic_config &other) :
        m_value(null_t()) {
    other.apply(assign_visitor(*this));
}

dynamic_config::dynamic_config(dynamic_config &&other) :
        m_value(null_t()) {
    other.apply(move_visitor(*this));
}

dynamic_config &dynamic_config::operator=(const dynamic_config &other) {
    other.apply(assign_visitor(*this));
    return *this;
}

dynamic_config &dynamic_config::operator=(dynamic_config &&other) {
    other.apply(move_visitor(*this));
    return *this;
}

bool dynamic_config::operator==(const dynamic_config &other) const {
    return other.apply(equals_visitor(*this));
}

bool dynamic_config::operator!=(const dynamic_config &other) const {
    return !other.apply(equals_visitor(*this));
}

dynamic_config::bool_t dynamic_config::as_bool() const {
    return get<bool_t>();
}

dynamic_config::int_t dynamic_config::as_int() const {
    return get<int_t>();
}

dynamic_config::uint_t dynamic_config::as_uint() const {
    return get<uint_t>();
}

dynamic_config::double_t dynamic_config::as_double() const {
    return get<double_t>();
}

const dynamic_config::string_t &dynamic_config::as_string() const {
    return get<string_t>();
}

const dynamic_config::array_t &dynamic_config::as_array() const {
    return get<incomplete_wrapper<array_t>>().get();
}

const dynamic_config::object_t &dynamic_config::as_object() const {
    return get<incomplete_wrapper<object_t>>().get();
}

dynamic_config::string_t &dynamic_config::as_string() {
    if (is_null()) {
        *this = string_t();
    }

    return get<string_t>();
}

dynamic_config::array_t &dynamic_config::as_array() {
    if (is_null()) {
        *this = array_t();
    }

    return get<incomplete_wrapper<array_t>>().get();
}

dynamic_config::object_t &dynamic_config::as_object() {
    if (is_null()) {
        *this = object_t();
    }

    return get<incomplete_wrapper<object_t>>().get();
}

bool dynamic_config::is_null() const {
    return is<null_t>();
}

bool dynamic_config::is_bool() const {
    return is<bool_t>();
}

bool dynamic_config::is_int() const {
    return is<int_t>();
}

bool dynamic_config::is_uint() const {
    return is<uint_t>();
}

bool dynamic_config::is_double() const {
    return is<double_t>();
}

bool dynamic_config::is_string() const {
    return is<string_t>();
}

bool dynamic_config::is_array() const {
    return is<incomplete_wrapper<array_t>>();
}

bool dynamic_config::is_object() const {
    return is<incomplete_wrapper<object_t>>();
}

struct to_string_visitor : public boost::static_visitor<std::string> {
    std::string operator()(const dynamic_config::null_t &) const {
        return "null";
    }

    std::string operator()(const dynamic_config::bool_t &v) const {
        return v ? "true" : "false";
    }

    template<class T>
    std::string operator()(const T &v) const {
        return boost::lexical_cast<std::string>(v);
    }

    std::string operator()(const dynamic_config::string_t &v) const {
        return "\"" + v + "\"";
    }

    std::string operator()(const dynamic_config::array_t &v) const {
        std::string result = "[";

        size_t index = 0;

        if (!v.empty()) {
            result += v[index++].apply(*this);
        }

        while (index < v.size()) {
            result += ", ";
            result += v[index++].apply(*this);
        }

        return result + "]";
    }

    std::string operator()(const dynamic_config::object_t &v) const {
        std::string result = "{";

        dynamic_config::object_t::const_iterator it = v.begin();

        if (it != v.end()) {
            result += "\"" + it->first + "\":" + it->second.apply(*this);
            ++it;
        }

        for (; it != v.end(); ++it) {
            result += ", ";
            result += "\"" + it->first + "\":" + it->second.apply(*this);
        }

        return result + "}";
    }
};

template<>
std::string boost::lexical_cast<std::string, goblin_engineer::dynamic_config>(const goblin_engineer::dynamic_config &v) {
    return v.apply(to_string_visitor());
}
