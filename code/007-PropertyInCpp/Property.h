#include <iostream>
#include <functional>

template<typename HostType, typename ValueType>
class Property {
    using Getter = ValueType (HostType::*)() const;
    using Setter = void (HostType::*)(const ValueType&);

public:
    Property(HostType* h, Getter g, Setter s = nullptr)
        : m_host(h), m_getter(g), m_setter(s) {}

    operator ValueType() const {
        if (!m_getter) throw std::logic_error("Getter not defined");
        return (m_host->*m_getter)();
    }

    ValueType operator=(const ValueType& value) {
        if (!m_setter) throw std::logic_error("Setter not defined");
        (m_host->*m_setter)(value);
        return value;
    }

    class ReadOnly {
    public:
        ReadOnly(const HostType* h, Getter g) : m_host(h), m_getter(g) {}
        operator ValueType() const { return (m_host->*m_getter)(); }
    private:
        Getter m_getter;
        const HostType* m_host;
    };

    friend std::ostream& operator<<(std::ostream& os, const Property& prop) {
        os << static_cast<ValueType>(prop);
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const ReadOnly& prop) {
        os << static_cast<ValueType>(prop);
        return os;
    }

private:
    HostType* m_host;
    Getter m_getter;
    Setter m_setter;
};
