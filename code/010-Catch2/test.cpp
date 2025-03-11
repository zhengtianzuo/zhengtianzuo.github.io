
#include <catch2/catch_all.hpp> 

int add(int a, int b) {
    return a + b;
}

TEST_CASE("test add", "[add]") {
    REQUIRE(add(1,2) == 3);
    REQUIRE(add(384,28347) == 28731);
    REQUIRE(add(32,63543) == 63575);
}

int factorial(int n) {
    if(n <= 0) return 1;
    return n * factorial(n - 1);
}

TEST_CASE("阶乘基础测试", "[math][factorial]") {
    SECTION("正整数值验证") {
        REQUIRE(factorial(5) == 120);
        CHECK(factorial(6) == 720);  // 非致命断言 
    }
    
    SECTION("边界条件验证") {
        REQUIRE(factorial(0) == 1);
        REQUIRE(factorial(-1) == 1);
    }
}

bool authenticate(std::string username, std::string password) {
    if (username.compare("test_user") == 0 && password.compare("P@ssw0rd") == 0)
        return true;
    return false;
}

SCENARIO("用户登录流程验证", "[auth][bdd]") {
    GIVEN("已注册用户凭证") {
        std::string username = "test_user";
        std::string password = "P@ssw0rd";
        
        WHEN("输入正确密码") {
            bool result = authenticate(username, password);
            
            THEN("应返回认证成功") {
                REQUIRE(result == true);
            }
        }
        
        WHEN("输入错误密码") {
            bool result = authenticate(username, "wrong_pass");
            
            THEN("应返回认证失败") {
                REQUIRE_FALSE(result);
            }
        }
    }
}

// // 异常测试 
// TEST_CASE("异常处理验证", "[exceptions]") {
//     REQUIRE_THROWS_AS(throw std::runtime_error("错误"), std::runtime_error);
//     CHECK_THROWS_WITH(throw "Error!", ("Error"));
// }

// 浮点近似比较 
TEST_CASE("精度计算验证", "[math][approx]") {
    double pi = 3.14159265359;
    REQUIRE(pi == Catch::Approx(3.14159).epsilon(0.0001));
}

TEST_CASE("性能基准测试", "[!benchmark]") {
    BENCHMARK("向量插入 1000 元素") {
        std::vector<int> v;
        for(int i = 0; i < 1000; ++i) {
            v.push_back(i); 
        }
    };
}

template<typename T, size_t S>
struct Bar {
    size_t size() { return S; }
};

TEMPLATE_PRODUCT_TEST_CASE_SIG("A Template product test case with array signature", "[template][product][nttp]", ((typename T, size_t S), T, S), (std::array, Bar), ((int, 9), (float, 42))) {
    TestType x;
    REQUIRE(x.size() > 0);
}

int main( int argc, char* argv[] ) {
    return Catch::Session().run( argc, argv );
}
