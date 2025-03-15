#include <cpprest/http_listener.h>
#include <cpprest/json.h>
 
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
 
json::value GetUserList() {
    return json::value::string(U("user1,user2,user3"));
}

class RestServer {
public:
    RestServer(const utility::string_t& url) : listener(url) {
        listener.support(methods::GET,  std::bind(&RestServer::HandleGet, this, std::placeholders::_1));
        listener.support(methods::POST,  std::bind(&RestServer::HandlePost, this, std::placeholders::_1));
    }
 
    void Start() { listener.open().wait();  }
    void Stop() { listener.close().wait();  }
 
private:
    void HandleGet(http_request req) {
        auto path = uri::decode(req.relative_uri().path()); 
        
        if (path == U("/users")) {
            json::value users = GetUserList(); // 业务逻辑实现 
            req.reply(status_codes::OK, users);
        } else {
            req.reply(status_codes::NotFound); 
        }
    }
 
    void HandlePost(http_request req) {
        req.extract_json() 
            .then([=](json::value body) {
                // 数据验证与存储逻辑 
                req.reply(status_codes::Created); 
            });
    }
 
    http_listener listener;
};

int main() {
    RestServer server(U("http://localhost:8080/"));
    server.Start();
    std::cin.get();
    server.Stop();
    std::cout << "RestServer exit" << std::endl;
    return 0;
}
