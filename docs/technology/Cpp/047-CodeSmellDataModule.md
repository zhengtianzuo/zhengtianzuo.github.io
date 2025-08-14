---
icon: simple/sellfy
---

# 代码味道-数据模块

![](img/CodeSmellDataModule.png)

以下是关于**代码坏味道-Data Module（数据模块）**的技术解析与实践指南，包含定义、特征分析、重构方案及完整的C++代码示例：

一、Data Module 的定义与核心特征
1. 定义
Data Module（又称数据模块）是一种典型的代码坏味道，表现为仅包含数据字段和简单访问方法（如 getter/setter），却缺乏业务逻辑和职责的类。它本质上是“哑数据容器”，违背了面向对象设计中“数据与行为绑定”的原则。

2. 特征与问题

|特征|	问题分析|
|-|-|
|数据暴露	|字段公开或仅通过 getter/setter 暴露，破坏封装性。|
|行为缺失	|无业务逻辑方法，操作数据的逻辑散落在其他类中（依恋情结）。|
|高耦合性	|外部类频繁访问其字段，修改数据结构需同步修改多处。|
|贫血模型	|导致系统设计为“贫血域模型”，数据与行为分离。|

二、重构解决方案
重构原则：封装数据 + 搬移行为
1. 封装字段（Encapsulate Field）
将字段设为私有，通过方法控制访问。
2. 搬移函数（Move Method）
将与数据紧密相关的逻辑移入数据模块中。
3. 移除设值函数（Remove Setting Method）
对不应外部修改的字段移除 setter。

三、C++ 示例：重构前后对比
重构前：典型的 Data Module
~~~cpp
// EmployeeData.h（数据模块）
class EmployeeData {
public:
    std::string name;
    int age;
    double salary;
};
 
// 业务逻辑散落在外部类
class ReportGenerator {
public:
    void generateReport(const EmployeeData& emp) {
        std::cout << "Name: " << emp.name  
                  << ", Age: " << emp.age  
                  << ", Salary: $" << emp.salary; 
    }
};
~~~
问题分析：

EmployeeData 仅有数据字段，无行为逻辑。
generateReport 方法在外部类中，直接访问 EmployeeData 的内部字段（依恋情结）。
重构后：内聚行为的数据模块
~~~cpp
// Employee.h（封装数据与行为）
class Employee {
private:  // 封装字段
    std::string name;
    int age;
    double salary;
 
public:
    Employee(std::string name, int age, double salary) 
        : name(std::move(name)), age(age), salary(salary) {}
 
    // 搬移行为到数据模块
    void generateReport() const {
        std::cout << "Name: " << name 
                  << ", Age: " << age 
                  << ", Salary: $" << salary;
    }
 
    // 只读访问（移除不必要的setter）
    std::string getName() const { return name; }
    double getSalary() const { return salary; }
};
~~~
优化点：

字段私有化，通过构造函数初始化7；
业务逻辑 generateReport 内聚到类中；
移除 age 的 setter，避免随意修改。

四、重构过程流程图
~~~mermaid
graph TD
    A[识别Data Module] --> B{检查字段暴露}
    B -->|字段公开| C[封装字段为私有]
    B -->|依赖外部逻辑| D[查找依恋情结方法]
    D --> E[搬移方法到数据模块]
    C --> F[移除不必要setter]
    E --> G[验证行为内聚性]
    G --> H[重构完成]
~~~

五、类结构变化图
重构前结构
~~~mermaid
classDiagram 
    class EmployeeData {
        +string name 
        +int age
        +double salary 
    }
    class ReportGenerator {
        +generateReport(EmployeeData)
    }
    EmployeeData <.. ReportGenerator : 依赖 
~~~

重构后结构
~~~mermaid
classDiagram
    class Employee {
        -string name 
        -int age
        -double salary 
        +generateReport()
        +getName()
        +getSalary()
    }
``` }
~~~

关键改进：

数据与行为绑定，消除跨类依赖；
类职责单一化，符合高内聚原则。

六、分析与总结
为什么重构有效？
1. 降低耦合：外部类无需感知数据细节；
2. 提高内聚：数据操作逻辑集中在同一类中；
3. 增强可维护性：修改数据格式只需调整一个类。

何时使用 Data Module？
仅当类需作为纯数据传输对象（DTO） 时（如 API 接口传参），可保留简单结构。其他场景应避免6。

核心准则：面向对象设计中，数据与操作数据的行为应共存于同一抽象层次。

如需完整代码或扩展案例，可参考：

重构实践：Data Class 的深度解析
重构手法：搬移函数与封装字段