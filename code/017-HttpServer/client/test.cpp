#include <cpprest/http_client.h>
#include <cpprest/json.h>
 
using namespace web;
using namespace web::http;
using namespace web::http::client;
 
class RestClient {
public:
    RestClient(const utility::string_t& url) : client(url) {}
 
    // GET请求（带查询参数）
    pplx::task<json::value> GetUsers(int page = 1) {
        uri_builder builder(U("/users"));
        builder.append_query(U("page"),  page);
        return client.request(methods::GET,  builder.to_string()) 
            .then([](http_response response) {
                return response.extract_json(); 
            });
    }
 
    // POST创建资源 
    pplx::task<http_response> CreateUser(const json::value& user) {
        return client.request(methods::POST,  U("/users"), user);
    }
 
    // PUT更新资源 
    pplx::task<http_response> UpdateUser(int id, const json::value& data) {
        uri_builder builder(U("/users"));
        builder.append_path((const std::string)std::to_string(id)); 
        return client.request(methods::PUT,  builder.to_string(),  data);
    }
 
    // DELETE删除资源 
    pplx::task<http_response> DeleteUser(int id) {
        uri_builder builder(U("/users"));
        builder.append_path((const std::string)std::to_string(id)); 
        return client.request(methods::DEL,  builder.to_string()); 
    }
 
private:
    http_client client;
};

int main() {
    RestClient client(U("http://localhost:8080/"));
    pplx::task<json::value> requestTask = client.GetUsers();
    json::value users = requestTask.get();
    std::cout << "client get users: " << users.to_string() << std::endl;
    return 0;
}
