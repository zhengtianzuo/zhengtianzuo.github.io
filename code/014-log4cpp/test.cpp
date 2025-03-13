#include <iostream>
#include <log4cpp/Category.hh> 
#include <log4cpp/FileAppender.hh> 
#include <log4cpp/RollingFileAppender.hh> 
#include <log4cpp/PatternLayout.hh> 
#include <log4cpp/PropertyConfigurator.hh> 
#include <log4cpp/NDC.hh>
#include <log4cpp/OstreamAppender.hh>

using namespace std;
using namespace log4cpp;

// 基础配置初始化 
void initBasicLog() {
    // 创建格式化器
    PatternLayout* layout = new PatternLayout();
    layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S.%l} [%p] %m%n");

    // 控制台输出 
    Appender* consoleApp = new OstreamAppender("console", &std::cout);
    consoleApp->setLayout(layout);

    // 创建格式化器
    PatternLayout* layout1 = new PatternLayout();
    layout1->setConversionPattern("%d{%Y-%m-%d %H:%M:%S.%l} [%p] %m%n");

    // 文件输出（自动滚动）
    RollingFileAppender* fileApp = new RollingFileAppender("fileApp", "demo.log",  10*1024*1024, 5);
    fileApp->setLayout(layout1);

    // 配置主日志器
    Category& root = Category::getRoot();
    root.setPriority(Priority::DEBUG); 
    root.addAppender(consoleApp); 
    root.addAppender(fileApp); 
}

// 配置文件加载 
void initConfigLog() {
    try {
        log4cpp::PropertyConfigurator::configure("log4cpp.conf"); 
    } catch(log4cpp::ConfigureFailure& f) {
        std::cerr << "配置加载失败: " << f.what()  << std::endl;
    }
}

int main() {
  // 初始化方式任选其一 
  initBasicLog();    // 代码初始化 
  // initConfigLog(); // 文件配置初始化 

  log4cpp::Category& logger = log4cpp::Category::getInstance("MainModule");
  
  // 多级别日志记录 
  logger.debug(" 调试信息: 变量x=%d", 42);
  logger.info(" 系统启动完成");
  logger.warn(" 磁盘空间剩余10%%");
  logger.error(" 数据库连接失败");
  
  // NDC诊断上下文（多线程追踪）
  log4cpp::NDC::push("RequestID:1001");
  logger << log4cpp::Priority::INFO << "处理用户请求";
  log4cpp::NDC::pop();

  for (int i = 0; i < 10; i++) {
    std::string strError;
    std::ostringstream oss;
    oss << i << " 这是一条Error信息...";
    strError = oss.str();
    logger.error(strError);
  }

  for (int i = 0; i < 10; i++) {
    string strError;
    ostringstream oss;
    oss << i << " 这是一条Info信息...";
    strError = oss.str();
    logger.info(strError);
  }

  for (int i = 0; i < 10; i++) {
    string strError;
    ostringstream oss;
    oss << i << " 这是一条Debug信息...";
    strError = oss.str();
    logger.debug(strError);
  }

  log4cpp::Category::shutdown();
  return 0;
}
