#include "Property.h"

class User {
public:
    User(const std::string& name, int age) 
        : m_name(name), m_age(age) {}

    Property<User, std::string> Name{
        this, 
        &User::get_name,
        &User::set_name 
    };

    Property<User, int>::ReadOnly Age{
        this,
        &User::get_age 
    };

private:
    std::string m_name;
    int m_age;

    std::string get_name() const { return m_name; }
    void set_name(const std::string& val) { m_name = val; }
    int get_age() const { return m_age; }
};

int main() {
    User user("A", 18);
    user.Name = "zhengtianzuo";
    std::cout << "Name: " << user.Name << std::endl;
    std::cout << "Age: " << user.Age << std::endl;
    // user.Age = 20; // 编译错误（只读属性）
    return 0;
}
