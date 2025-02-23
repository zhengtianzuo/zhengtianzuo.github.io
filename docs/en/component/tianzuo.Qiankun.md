# tianzuo.Qiankun

![](../img/tianzuo.Qiankun.jpg)

### tianzuo.Qiankun

![](https://img.shields.io/badge/release-1.0.0.0-blue.svg)
![](https://img.shields.io/badge/date-24.1.1-orange.svg)

||||
|--|--|--|
|operating system|![](https://img.shields.io/badge/os-windows_7+-blue.svg) ![](https://img.shields.io/badge/os-macos_10.14+-lightgrey.svg) ![](https://img.shields.io/badge/os-ubuntu_20.04+-orange.svg) ![](https://img.shields.io/badge/os-android_5.0+-green.svg) ![](https://img.shields.io/badge/os-ios_12.0+-lightgrey.svg)||
|compiler|![](https://img.shields.io/badge/c++-11-blue.svg) ![](https://img.shields.io/badge/msvc-14.0-blue.svg) ![](https://img.shields.io/badge/msvc-14.1-blue.svg) ![](https://img.shields.io/badge/msvc-14.2-blue.svg) ![](https://img.shields.io/badge/msvc-14.3-blue.svg) ![](https://img.shields.io/badge/ndk-21.3-green.svg) ![](https://img.shields.io/badge/llvm-10.0-lightgrey.svg) ![](https://img.shields.io/badge/gcc-9.4-orange.svg)||
|programming language|![](../img/C.png) ![](../img/C__.png) ![](../img/Python.png) ![](../img/Java.png) ![](../img/C_.png) ![](../img/JavaScript.png) ![](../img/PHP.png) ![](../img/Visual_Basic.png) ![](../img/Go.png) ![](../img/Rust.png) ![](../img/Ruby.png)||

# introduction

![](../img/alchemy_tianzuo.Qiankun.jpg)

**tianzuo.Qiankun** NoSql database based on drawer file storage can be used for file packaging, data integration, encryption storage and other purposes. The storage system can be easily and quickly built and deployed.

**legend:**
It's owned by Maitreya. It's for storage purposes. With incredible power, there is a strange space inside, a space so large that it seems to contain heaven and earth. There's more in the bag. There's more than enough.

# motivation
Build a NoSql database that can manage data with simple interface calls.

![](../img/tianzuo.QiankunInfo.png)

# features

- 🧩 simple integration
- 📦 integrated simple storage files can be compressed freely
- 🪢 the data is encrypted by 256-bit AES algorithm
- 📒 data can be added, deleted, modified, or queried in batches
- 📃 supports direct access to raw data streams

# screenshot

**tianzuo.Qiankun Test:**

![](../img/tianzuo.Qiankun1.png)


# example
<details>
  <summary>cpp</summary>

```cpp
#include <iostream>
#include <memory>
#include "tianzuo.QiankunInterface.h" // 接口头文件 interface heaher file

int main() {
    // 准备数据 prepare data
    Qiankun_db_verify_struct db_verify("qiankunDataBase.Ztz", "password");
    int integer = 12345678;
    Qiankun_data_type_struct data_type(Qiankun_data_type_integer, -1, "integer");
    Qiankun_user_data_struct user_data(&integer, 0);
    int data_size = 0;
    Qiankun_get_user_data_struct get_user_data(reinterpret_cast<void*&>(integer), &data_size);

    // 初始化接口 initialize the interface
    std::unique_ptr<tianzuo_QiankunInterface> qiankun_interface(new tianzuo_QiankunInterface());

    // 创建实例 create an instance
    tianzuo_QiankunInt* qiankun = qiankun_interface->initialize();

    // 初始化实例 initialize the instance
    int error_code = qiankun->initialize();
    if (error_code != Qiankun_errorcode_success) {
        std::cout << "initialize error: " << error_code;
        goto return_error;
        return -1;
    }
    
    // 创建数据库 create database
    error_code = qiankun->db_create_database(&db_verify, "qiankunDataBase", true);
    if (error_code != Qiankun_errorcode_success) {
        std::cout << "db_create_database error: " << error_code;
        goto return_error;
        return -1;
    }

    // 打开数据库 open database
    error_code = qiankun->db_open_database(&db_verify);
    if (error_code != Qiankun_errorcode_success) {
        std::cout << "db_open_database error: " << error_code;
        goto return_error;
        return -1;
    }

    // 向数据库写入一个整型数据 writes an integer to the database
    error_code = qiankun->data_combine(&db_verify, &data_type, nullptr, &user_data, false, -1);
    if (error_code != Qiankun_errorcode_success) {
        std::cout << "data_combine error: " << error_code;
        goto return_error;
        return -1;
    }


    // 读取刚写入的数据 read the data just written
    integer = -1;
    error_code = qiankun->data_extract(&db_verify, &data_type, nullptr, &get_user_data);
    if (error_code != Qiankun_errorcode_success) {
        std::cout << "data_extract error: " << error_code;
        goto return_error;
        return -1;
    }

    std::cout << "get integer data: " << integer;

    // 关闭数据库 close database
    error_code = qiankun->db_close_database(&db_verify);
    if (error_code != Qiankun_errorcode_success) {
        std::cout << "db_close_database error: " << error_code;
        goto return_error;
        return -1;
    }

return_error:

    // 释放接口 terminate the interface
    if (qiankun_interface != nullptr && qiankun != nullptr) {
        qiankun_interface->terminate(&qiankun);
    }

    std::cout << "test done -------------------";
    
    return 0;
}
```
</details>

<details>
  <summary>python</summary>

```python
from ctypes import *
from enum import Enum
import ctypes

tianzuo_QiankunLib = cdll.LoadLibrary('./tianzuo.Qiankun.dll')

class Qiankun_error_code(Enum):
    Qiankun_errorcode_success           = 1 # 正确的值
    Qiankun_errorcode_normal            = 2 # 错误的值
    Qiankun_errorcode_differ            = 3 # 数据库文件数效验未通过 (The number of database files fails to be verified)
    Qiankun_errorcode_no_rights         = 4 # 数据库密码效验未通过 (The database password verification failed)
    Qiankun_errorcode_db_not_exist      = 5 # 数据库文件不存在 (The database file does not exist)
    Qiankun_errorcode_exist             = 6 # 数据已存在 (Data already exists)
    Qiankun_errorcode_not_exist         = 7 # 数据不存在 (Data does not exist)
    Qiankun_errorcode_out_of_range      = 8 # 超出范围 (out of range)
    Qiankun_errorcode_type              = 9 # 类型不符 (wrong type)
    Qiankun_errorcode_param             = 10 # 参数错误 (parameter error)
    Qiankun_errorcode_compress          = 11 # 压缩错误 (compression error)
    Qiankun_errorcode_get_array         = 12 # 数组错误 (arrary error)
    Qiankun_errorcode_out_of_memory     = 13 # 内存溢出 (memory overflow)
    Qiankun_errorcode_handle            = 14 # 句柄错误 (error handle)
    Qiankun_errorcode_io                = 15 # IO错误 (io error)

class tianzuo_QiankunInt:
    def __init__(self):
        tianzuo_QiankunLib.tianzuo_QiankunInterface_initialize.restype = c_void_p
        self.obj = tianzuo_QiankunLib.tianzuo_QiankunInterface_initialize()

    def __del__(self):
        tianzuo_QiankunLib.tianzuo_QiankunInterface_terminate.argtypes = [c_void_p]
        tianzuo_QiankunLib.tianzuo_QiankunInterface_terminate(self.obj)

    def initialize(self):
        tianzuo_QiankunLib.tianzuo_QiankunInterface_initialize.restype = c_int
        tianzuo_QiankunLib.tianzuo_QiankunInt_initialize.argtypes = [c_void_p]
        return tianzuo_QiankunLib.tianzuo_QiankunInt_initialize(self.obj)

    def db_create_database(self, db_file_name, db_password, db_name, over_write):
        tianzuo_QiankunLib.tianzuo_QiankunInt_db_create_database.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p, c_int]
        return tianzuo_QiankunLib.tianzuo_QiankunInt_db_create_database(self.obj, db_file_name, db_password, db_name, over_write)

    def db_open_database(self, db_file_name, db_password):
        tianzuo_QiankunLib.tianzuo_QiankunInt_db_open_database.argtypes = [c_void_p, c_char_p, c_char_p]
        return tianzuo_QiankunLib.tianzuo_QiankunInt_db_open_database(self.obj, db_file_name, db_password)

    def db_close_database(self, db_file_name, db_password):
        tianzuo_QiankunLib.tianzuo_QiankunInt_db_close_database.argtypes = [c_void_p, c_char_p, c_char_p]
        return tianzuo_QiankunLib.tianzuo_QiankunInt_db_close_database(self.obj, db_file_name, db_password)

    def data_int_combine(self, db_file_name, db_password, data_name, remark, data):
        tianzuo_QiankunLib.tianzuo_QiankunInt_data_int_combine.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int]
        return tianzuo_QiankunLib.tianzuo_QiankunInt_data_int_combine(self.obj, db_file_name, db_password, data_name, remark, data)

    def data_int_extract(self, db_file_name, db_password, data_name, data):
        tianzuo_QiankunLib.tianzuo_QiankunInt_data_int_extract.argtypes = [c_void_p, c_char_p, c_char_p, c_char_p, ctypes.POINTER(ctypes.c_int)]
        return tianzuo_QiankunLib.tianzuo_QiankunInt_data_int_extract(self.obj, db_file_name, db_password, data_name, data)

def main():

    # 初始化接口 initialize the interface
    print("initialize the interface")

    # 创建实例 create an instance
    qiankun = tianzuo_QiankunInt()
    error_code = qiankun.initialize()
    if error_code != Qiankun_error_code.Qiankun_errorcode_success.value:
        print("initialize error:", error_code)
        return

    # 创建数据库 create database
    db_file_name = b"qiankunDataBase.Ztz"
    db_password = b"password"
    error_code = qiankun.db_create_database(db_file_name, db_password, b"qiankunDataBase", True)
    if error_code != Qiankun_error_code.Qiankun_errorcode_success.value:
        print("db_create_database error:", error_code)
        return

    # 打开数据库 open database
    error_code = qiankun.db_open_database(db_file_name, db_password)
    if error_code != Qiankun_error_code.Qiankun_errorcode_success.value:
        print("db_open_database error:", error_code)
        return

    # 向数据库写入一个整型数据 writes an integer to the database
    intInt = 12345678
    error_code = qiankun.data_int_combine(db_file_name, db_password, b"integer", None, intInt)
    if error_code != Qiankun_error_code.Qiankun_errorcode_success.value:
        print("data_combine error:", error_code)
        return

    # 读取刚写入的数据 read the data just written
    return_data = ctypes.c_int()
    error_code = qiankun.data_int_extract(db_file_name, db_password, b"integer", ctypes.byref(return_data))
    if error_code != Qiankun_error_code.Qiankun_errorcode_success.value:
        print("data_extract error:", error_code)
        return

    integer = return_data.value
    print("get integer data: {}".format(integer))

    # 关闭数据库 close database
    error_code = qiankun.db_close_database(db_file_name, db_password)
    if error_code != Qiankun_error_code.Qiankun_errorcode_success.value:
        print("db_close_database error:", error_code)
        return

    print("test done -------------------")

if __name__ == '__main__':
    main()

```
</details>

<details>
  <summary>java</summary>

```java
package cn.camelsoft;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

enum Qiankun_error_code {
    Qiankun_errorcode_success           (0x00000001),   ///<  正确的值
    Qiankun_errorcode_normal            (0x00000002),   ///<  错误的值
    Qiankun_errorcode_differ            (0x00000003),   ///<  数据库文件数效验未通过 (The number of database files fails to be verified)
    Qiankun_errorcode_no_rights         (0x00000004),   ///<  数据库密码效验未通过 (The database password verification failed)
    Qiankun_errorcode_db_not_exist      (0x00000005),   ///<  数据库文件不存在 (The database file does not exist)
    Qiankun_errorcode_exist             (0x00000006),   ///<  数据已存在 (Data already exists)
    Qiankun_errorcode_not_exist         (0x00000007),   ///<  数据不存在 (Data does not exist)
    Qiankun_errorcode_out_of_range      (0x00000008),   ///<  超出范围 (out of range)
    Qiankun_errorcode_type              (0x00000009),   ///<  类型不符 (wrong type)
    Qiankun_errorcode_param             (0x0000000A),   ///<  参数错误 (parameter error)
    Qiankun_errorcode_compress          (0x0000000B),   ///<  压缩错误 (compression error)
    Qiankun_errorcode_get_array         (0x0000000C),   ///<  数组错误 (arrary error)
    Qiankun_errorcode_out_of_memory     (0x0000000D),   ///<  内存溢出 (memory overflow)
    Qiankun_errorcode_handle            (0x0000000E),   ///<  句柄错误 (error handle)
    Qiankun_errorcode_io                (0x0000000F);   ///<  IO错误 (io error)

    private int value;

    Qiankun_error_code(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}

public class Main {

    public interface tianzuo_QiankunLib extends Library {
        tianzuo_QiankunLib instance = (tianzuo_QiankunLib) Native.load("./tianzuo.Qiankun.dll", tianzuo_QiankunLib.class);
        Pointer tianzuo_QiankunInterface_initialize();
        void tianzuo_QiankunInterface_terminate(Pointer qiankun);
        int tianzuo_QiankunInt_initialize(Pointer qiankun);

        int tianzuo_QiankunInt_db_create_database(Pointer qiankun, String db_file_name, String db_password, String db_name, Boolean over_write);
        int tianzuo_QiankunInt_db_open_database(Pointer qiankun, String db_file_name, String db_password);
        int tianzuo_QiankunInt_db_close_database(Pointer qiankun, String db_file_name, String db_password);
        int tianzuo_QiankunInt_data_int_combine(Pointer qiankun, String db_file_name, String db_password, String data_name, String remark, int data);
        int tianzuo_QiankunInt_data_int_extract(Pointer qiankun, String db_file_name, String db_password, String data_name, int[] data);
    }

    public static void main(String[] args) {
        System.setProperty("jna.encoding","UTF-8");

        // 初始化接口 initialize the interface
        System.out.printf("initialize the interface\n");

        // 创建实例 create an instance
        Pointer qiankun = tianzuo_QiankunLib.instance.tianzuo_QiankunInterface_initialize();
        int error_code = tianzuo_QiankunLib.instance.tianzuo_QiankunInt_initialize(qiankun);
        if (error_code != Qiankun_error_code.Qiankun_errorcode_success.getValue()) {
            System.out.printf("tianzuo_QiankunInt_initialize error: %d\n", error_code);
            return;
        }

        // 创建数据库 create database
        String db_file_name = "qiankunDataBase.Ztz";
        String db_password = "password";
        error_code = tianzuo_QiankunLib.instance.tianzuo_QiankunInt_db_create_database(qiankun, db_file_name, db_password, "qiankunDataBase", true);
        if (error_code != Qiankun_error_code.Qiankun_errorcode_success.getValue()) {
            System.out.printf("tianzuo_QiankunInt_db_create_database error: %d\n", error_code);
            return;
        }

        // 打开数据库 open database
        error_code = tianzuo_QiankunLib.instance.tianzuo_QiankunInt_db_open_database(qiankun, db_file_name, db_password);
        if (error_code != Qiankun_error_code.Qiankun_errorcode_success.getValue()) {
            System.out.printf("tianzuo_QiankunInt_db_open_database error: %d\n", error_code);
            return;
        }

        // 向数据库写入一个整型数据 writes an integer to the database
        int intInt = 12345678;
        error_code = tianzuo_QiankunLib.instance.tianzuo_QiankunInt_data_int_combine(qiankun, db_file_name, db_password, "integer", "", intInt);
        if (error_code != Qiankun_error_code.Qiankun_errorcode_success.getValue()) {
            System.out.printf("tianzuo_QiankunInt_data_int_combine error: %d\n", error_code);
            return;
        }

        // 读取刚写入的数据 read the data just written
        int[] return_data = {0};
        error_code = tianzuo_QiankunLib.instance.tianzuo_QiankunInt_data_int_extract(qiankun, db_file_name, db_password, "integer", return_data);
        if (error_code != Qiankun_error_code.Qiankun_errorcode_success.getValue()) {
            System.out.printf("tianzuo_QiankunInt_data_int_extract error: %d\n", error_code);
            return;
        }

        System.out.printf("get int: %d\n", return_data[0]);

        // 关闭数据库 close database
        error_code = tianzuo_QiankunLib.instance.tianzuo_QiankunInt_db_close_database(qiankun, db_file_name, db_password);
        if (error_code != Qiankun_error_code.Qiankun_errorcode_success.getValue()) {
            System.out.printf("tianzuo_QiankunInt_db_close_database error: %d\n", error_code);
            return;
        }

        System.out.printf("test done -------------------\n");

        tianzuo_QiankunLib.instance.tianzuo_QiankunInterface_terminate(qiankun);
    }
}
```
</details>

<details>
  <summary>csharp</summary>

```csharp


using System.Runtime.InteropServices;

public enum Qiankun_error_code
{
    Qiankun_errorcode_success       = 0x00000001,              ///<  正确的值
    Qiankun_errorcode_normal        = 0x00000002,              ///<  错误的值
    Qiankun_errorcode_differ        = 0x00000003,              ///<  数据库文件数效验未通过 (The number of database files fails to be verified)
    Qiankun_errorcode_no_rights     = 0x00000004,              ///<  数据库密码效验未通过 (The database password verification failed)
    Qiankun_errorcode_db_not_exist  = 0x00000005,              ///<  数据库文件不存在 (The database file does not exist)
    Qiankun_errorcode_exist         = 0x00000006,              ///<  数据已存在 (Data already exists)
    Qiankun_errorcode_not_exist     = 0x00000007,              ///<  数据不存在 (Data does not exist)
    Qiankun_errorcode_out_of_range  = 0x00000008,              ///<  超出范围 (out of range)
    Qiankun_errorcode_type          = 0x00000009,              ///<  类型不符 (wrong type)
    Qiankun_errorcode_param         = 0x0000000A,              ///<  参数错误 (parameter error)
    Qiankun_errorcode_compress      = 0x0000000B,              ///<  压缩错误 (compression error)
    Qiankun_errorcode_get_array     = 0x0000000C,              ///<  数组错误 (arrary error)
    Qiankun_errorcode_out_of_memory = 0x0000000D,              ///<  内存溢出 (memory overflow)
    Qiankun_errorcode_handle        = 0x0000000E,              ///<  句柄错误 (error handle)
    Qiankun_errorcode_io            = 0x0000000F,              ///<  IO错误 (io error)
}

class Program {
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern IntPtr tianzuo_QiankunInterface_initialize();
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern void tianzuo_QiankunInterface_terminate(IntPtr qiankun);
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern int tianzuo_QiankunInt_initialize(IntPtr qiankun);
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern int tianzuo_QiankunInt_db_create_database(IntPtr qiankun, string db_file_name, string db_password, string db_name, bool over_write);
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern int tianzuo_QiankunInt_db_open_database(IntPtr qiankun, string db_file_name, string db_password);
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern int tianzuo_QiankunInt_db_close_database(IntPtr qiankun, string db_file_name, string db_password);
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern int tianzuo_QiankunInt_data_int_combine(IntPtr qiankun, string db_file_name, string db_password, string data_name, string remark, int data);
    [DllImport("tianzuo.Qiankun.dll")]
    public static extern int tianzuo_QiankunInt_data_int_extract(IntPtr qiankun, string db_file_name, string db_password, string data_name, ref int data);
    
    static void Main(string[] args) {
        // 初始化接口 initialize the interface
        Console.WriteLine("initialize the interface\n");

        // 创建实例 create an instance
        IntPtr qiankun = tianzuo_QiankunInterface_initialize();
        int error_code = tianzuo_QiankunInt_initialize(qiankun);
        if (error_code != ((int)Qiankun_error_code.Qiankun_errorcode_success))
        {
            Console.WriteLine("tianzuo_QiankunInt_initialize error:" + error_code);
            return;
        }

        // 创建数据库 create database
        string db_file_name = "qiankunDataBase.Ztz";
        string db_password = "password";
        error_code = tianzuo_QiankunInt_db_create_database(qiankun, db_file_name, db_password, "qiankunDataBase", true);
        if (error_code != ((int)Qiankun_error_code.Qiankun_errorcode_success))
        {
            Console.WriteLine("tianzuo_QiankunInt_db_create_database error:" + error_code);
            return;
        }

        // 打开数据库 open database
        error_code = tianzuo_QiankunInt_db_open_database(qiankun, db_file_name, db_password);
        if (error_code != ((int)Qiankun_error_code.Qiankun_errorcode_success)) {
            Console.WriteLine("tianzuo_QiankunInt_db_open_database error:" + error_code);
            return;
        }

        // 向数据库写入一个整型数据 writes an integer to the database
        int intInt = 12345678;
        error_code = tianzuo_QiankunInt_data_int_combine(qiankun, db_file_name, db_password, "integer", "", intInt);
        if (error_code != ((int)Qiankun_error_code.Qiankun_errorcode_success))
        {
            Console.WriteLine("tianzuo_QiankunInt_data_int_combine error:" + error_code);
            return;
        }

        // 读取刚写入的数据 read the data just written
        int return_data = 0;
        error_code = tianzuo_QiankunInt_data_int_extract(qiankun, db_file_name, db_password, "integer", ref return_data);
        if (error_code != ((int)Qiankun_error_code.Qiankun_errorcode_success))
        {
            Console.WriteLine("tianzuo_QiankunInt_data_int_extract error:" + error_code);
            return;
        }

        Console.WriteLine("get int: " + return_data);

        // 关闭数据库 close database
        error_code = tianzuo_QiankunInt_db_close_database(qiankun, db_file_name, db_password);
        if (error_code != ((int)Qiankun_error_code.Qiankun_errorcode_success))
        {
            Console.WriteLine("tianzuo_QiankunInt_db_close_database error:" + error_code);
            return;
        }

        Console.WriteLine("test done -------------------\n");

        tianzuo_QiankunInterface_terminate(qiankun);

    }

}
```
</details>

<details>
  <summary>javascript</summary>

```js

Qiankun_errorcode_success           = 0x00000001;   ///<  正确的值
Qiankun_errorcode_normal            = 0x00000002;   ///<  错误的值
Qiankun_errorcode_differ            = 0x00000003;   ///<  数据库文件数效验未通过 (The number of database files fails to be verified)
Qiankun_errorcode_no_rights         = 0x00000004;   ///<  数据库密码效验未通过 (The database password verification failed)
Qiankun_errorcode_db_not_exist      = 0x00000005;   ///<  数据库文件不存在 (The database file does not exist)
Qiankun_errorcode_exist             = 0x00000006;   ///<  数据已存在 (Data already exists)
Qiankun_errorcode_not_exist         = 0x00000007;   ///<  数据不存在 (Data does not exist)
Qiankun_errorcode_out_of_range      = 0x00000008;   ///<  超出范围 (out of range)
Qiankun_errorcode_type              = 0x00000009;   ///<  类型不符 (wrong type)
Qiankun_errorcode_param             = 0x0000000A;   ///<  参数错误 (parameter error)
Qiankun_errorcode_compress          = 0x0000000B;   ///<  压缩错误 (compression error)
Qiankun_errorcode_get_array         = 0x0000000C;   ///<  数组错误 (arrary error)
Qiankun_errorcode_out_of_memory     = 0x0000000D;   ///<  内存溢出 (memory overflow)
Qiankun_errorcode_handle            = 0x0000000E;   ///<  句柄错误 (error handle)
Qiankun_errorcode_io                = 0x0000000F;   ///<  IO错误 (io error)

let ffi = require('ffi-napi')
const ref = require('ref-napi');

let tianzuo_QiankunLib = ffi.Library("tianzuo.Qiankun.dll", {
    'tianzuo_QiankunInterface_initialize': ['pointer', []],
    'tianzuo_QiankunInterface_terminate':['void',['pointer']],
    'tianzuo_QiankunInt_initialize':['int',['pointer']],
    'tianzuo_QiankunInt_db_create_database':['int',['pointer','string','string','string', 'bool']],
    'tianzuo_QiankunInt_db_open_database':['int',['pointer', 'string', 'string']],
    'tianzuo_QiankunInt_db_close_database':['int',['pointer', 'string', 'string']],
    'tianzuo_QiankunInt_data_int_combine':['int',['pointer','string','string','string', 'string', 'int']],
    'tianzuo_QiankunInt_data_int_extract':['int',['pointer','string','string','string', 'pointer']],
})


// 初始化接口 initialize the interface
console.log("initialize the interface\n");

// 创建实例 create an instance
let qiankun = tianzuo_QiankunLib.tianzuo_QiankunInterface_initialize();
let error_code = tianzuo_QiankunLib.tianzuo_QiankunInt_initialize(qiankun);
if (error_code !== Qiankun_errorcode_success) {
    console.log("tianzuo_QiankunInt_initialize error: %d\n", error_code);
    return;
}

// 创建数据库 create database
let db_file_name = "qiankunDataBase.Ztz";
let db_password = "password";
error_code = tianzuo_QiankunLib.tianzuo_QiankunInt_db_create_database(qiankun, db_file_name, db_password, "qiankunDataBase", true);
if (error_code !== Qiankun_errorcode_success) {
    console.log("tianzuo_QiankunInt_db_create_database error: %d\n", error_code);
    return;
}

// 打开数据库 open database
error_code = tianzuo_QiankunLib.tianzuo_QiankunInt_db_open_database(qiankun, db_file_name, db_password);
if (error_code !== Qiankun_errorcode_success) {
    console.log("tianzuo_QiankunInt_db_open_database error: %d\n", error_code);
    return;
}

// 向数据库写入一个整型数据 writes an integer to the database
let intInt = 12345678;
error_code = tianzuo_QiankunLib.tianzuo_QiankunInt_data_int_combine(qiankun, db_file_name, db_password, "integer", "", intInt);
if (error_code !== Qiankun_errorcode_success) {
    console.log("tianzuo_QiankunInt_data_int_combine error: %d\n", error_code);
    return;
}

// 读取刚写入的数据 read the data just written
return_buf = Buffer.alloc(1 * ref.sizeof.int);
error_code = tianzuo_QiankunLib.tianzuo_QiankunInt_data_int_extract(qiankun, db_file_name, db_password, "integer", return_buf);
if (error_code !== Qiankun_errorcode_success) {
    console.log("tianzuo_QiankunInt_data_int_extract error: %d\n", error_code);
    return;
}

return_data = ref.get(return_buf, 0, ref.types.int);
console.log("get int: %d\n", return_data);

// 关闭数据库 close database
error_code = tianzuo_QiankunLib.tianzuo_QiankunInt_db_close_database(qiankun, db_file_name, db_password);
if (error_code !== Qiankun_errorcode_success) {
    console.log("tianzuo_QiankunInt_db_close_database error: %d\n", error_code);
   return;
}

console.log("test done -------------------\n");

tianzuo_QiankunLib.tianzuo_QiankunInterface_terminate(qiankun);

```
</details>

<details>
  <summary>php</summary>

```php
<?php

// 检查是否加载了 FFI 扩展
if (extension_loaded('ffi')) {
    echo("已启用 FFI 扩展\n");
} else {
    die("未启动 FFI 扩展\n");
}

enum Qiankun_error_code
{
const Qiankun_errorcode_success       = 0x00000001;            ///<  正确的值
const Qiankun_errorcode_normal        = 0x00000002;            ///<  错误的值
const Qiankun_errorcode_differ        = 0x00000003;            ///<  数据库文件数效验未通过 (The number of database files fails to be verified)
const Qiankun_errorcode_no_rights     = 0x00000004;            ///<  数据库密码效验未通过 (The database password verification failed)
const Qiankun_errorcode_db_not_exist  = 0x00000005;            ///<  数据库文件不存在 (The database file does not exist)
const Qiankun_errorcode_exist         = 0x00000006;            ///<  数据已存在 (Data already exists)
const Qiankun_errorcode_not_exist     = 0x00000007;            ///<  数据不存在 (Data does not exist)
const Qiankun_errorcode_out_of_range  = 0x00000008;            ///<  超出范围 (out of range)
const Qiankun_errorcode_type          = 0x00000009;            ///<  类型不符 (wrong type)
const Qiankun_errorcode_param         = 0x0000000A;            ///<  参数错误 (parameter error)
const Qiankun_errorcode_compress      = 0x0000000B;            ///<  压缩错误 (compression error)
const Qiankun_errorcode_get_array     = 0x0000000C;            ///<  数组错误 (arrary error)
const Qiankun_errorcode_out_of_memory = 0x0000000D;            ///<  内存溢出 (memory overflow)
const Qiankun_errorcode_handle        = 0x0000000E;            ///<  句柄错误 (error handle)
const Qiankun_errorcode_io            = 0x0000000F;            ///<  IO错误 (io error)
}

$ffi = FFI::cdef("
    void* tianzuo_QiankunInterface_initialize();
    void tianzuo_QiankunInterface_terminate(void* *QiankunInt_pointer);
    int tianzuo_QiankunInt_initialize(void* QiankunInt_pointer);
    int tianzuo_QiankunInt_db_create_database(void* QiankunInt_pointer, const char* db_file_name, const char* db_password, const char* db_name, bool const over_write);
    int tianzuo_QiankunInt_db_open_database(void* QiankunInt_pointer, const char* db_file_name, const char* db_password);
    int tianzuo_QiankunInt_db_close_database(void* QiankunInt_pointer, const char* db_file_name, const char* db_password);
    int tianzuo_QiankunInt_data_int_combine(void* QiankunInt_pointer, const char* db_file_name, const char* db_password, const char* data_name, const char* remark, int data);
    int tianzuo_QiankunInt_data_int_extract(void* QiankunInt_pointer, const char* db_file_name, const char* db_password, const char* data_name, int* data);
", "tianzuo.Qiankun.dll");

# 初始化接口 initialize the interface
echo "initialize the interface" . "\n";

# 创建实例 create an instance
$qiankun = $ffi->tianzuo_QiankunInterface_initialize();

$error_code = $ffi->tianzuo_QiankunInt_initialize($qiankun);
if ($error_code != Qiankun_error_code::Qiankun_errorcode_success) {
    echo "initialize error: " . $error_code . "\n";
    return;
}

# 创建数据库 create database
$db_file_name = "qiankunDataBase.Ztz";
$db_password = "password";
$error_code = $ffi->tianzuo_QiankunInt_db_create_database($qiankun, $db_file_name, $db_password, "qiankunDataBase", true);
if ($error_code != Qiankun_error_code::Qiankun_errorcode_success) {
    echo "tianzuo_QiankunInt_db_create_database error: " . $error_code . "\n";
    return;
}

# 打开数据库 open database
$error_code = $ffi->tianzuo_QiankunInt_db_open_database($qiankun, $db_file_name, $db_password);
if ($error_code != Qiankun_error_code::Qiankun_errorcode_success) {
    echo "tianzuo_QiankunInt_db_open_database error: " . $error_code . "\n";
    return;
}

# 向数据库写入一个整型数据 writes an integer to the database
$intInt = 12345678;
$error_code = $ffi->tianzuo_QiankunInt_data_int_combine($qiankun, $db_file_name, $db_password, "integer", "", $intInt);
if ($error_code != Qiankun_error_code::Qiankun_errorcode_success) {
    echo "tianzuo_QiankunInt_data_int_combine error: " . $error_code . "\n";
    return;
}

# 读取刚写入的数据 read the data just written
$return_data = FFI::new("int");
$error_code = $ffi->tianzuo_QiankunInt_data_int_extract($qiankun, $db_file_name, $db_password, "integer", FFI::addr($return_data));
if ($error_code != Qiankun_error_code::Qiankun_errorcode_success) {
    echo "tianzuo_QiankunInt_data_int_extract error: " . $error_code . "\n";
    return;
}

echo "get integer data: " . $return_data->cdata . "\n";

# 关闭数据库 close database
$error_code = $ffi->tianzuo_QiankunInt_db_close_database($qiankun, $db_file_name, $db_password);
if ($error_code != Qiankun_error_code::Qiankun_errorcode_success) {
    echo "tianzuo_QiankunInt_db_close_database error: " . $error_code . "\n";
    return;
}

echo "test done -------------------" . "\n";

?>
```
</details>

<details>
  <summary>vb</summary>

```vb

Imports System.Runtime.InteropServices

Enum Qiankun_error_code
    Qiankun_errorcode_success = 1                     '  正确的值
    Qiankun_errorcode_normal = 2                      '  错误的值
    Qiankun_errorcode_differ = 3                      '  数据库文件数效验未通过 (The number Of database files fails To be verified)
    Qiankun_errorcode_no_rights = 4                   '  数据库密码效验未通过 (The database password verification failed)
    Qiankun_errorcode_db_not_exist = 5                '  数据库文件不存在 (The database file does Not exist)
    Qiankun_errorcode_exist = 6                       '  数据已存在 (Data already exists)
    Qiankun_errorcode_not_exist = 7                   '  数据不存在 (Data does Not exist)
    Qiankun_errorcode_out_of_range = 8                '  超出范围 (out Of range)
    Qiankun_errorcode_type = 9                        '  类型不符 (wrong type)
    Qiankun_errorcode_param = 10                      '  参数错误 (parameter Error)
    Qiankun_errorcode_compress = 11                   '  压缩错误 (compression Error)
    Qiankun_errorcode_get_array = 12                  '  数组错误 (arrary Error)
    Qiankun_errorcode_out_of_memory = 13              '  内存溢出 (memory overflow)
    Qiankun_errorcode_handle = 14                     '  句柄错误 (Error handle)
    Qiankun_errorcode_io = 15                         '  IO错误 (io Error)
End Enum

Module Program
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInterface_initialize() As IntPtr
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInterface_terminate(ByRef QiankunInt_pointer As IntPtr)
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInt_initialize(QiankunInt_pointer As IntPtr) As Integer
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInt_db_create_database(QiankunInt_pointer As IntPtr, db_file_name As String, db_password As String, db_name As String, over_write As Boolean) As Integer
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInt_db_open_database(QiankunInt_pointer As IntPtr, db_file_name As String, db_password As String) As Integer
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInt_db_close_database(QiankunInt_pointer As IntPtr, db_file_name As String, db_password As String) As Integer
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInt_data_int_combine(QiankunInt_pointer As IntPtr, db_file_name As String, db_password As String, data_name As String, remark As String, data As Integer) As Integer
    End Function
    <DllImport("tianzuo.Qiankun.dll")>
    Public Function tianzuo_QiankunInt_data_int_extract(QiankunInt_pointer As IntPtr, db_file_name As String, db_password As String, data_name As String, ByRef data As Integer) As Integer
    End Function

    Sub Main(args As String())
        ' 初始化接口 initialize the interface
        Console.WriteLine("initialize the interface")

        ' 创建实例 create an instance
        Dim qiankun As IntPtr = tianzuo_QiankunInterface_initialize()
        Dim error_code As Integer = tianzuo_QiankunInt_initialize(qiankun)
        If (error_code <> Qiankun_error_code.Qiankun_errorcode_success) Then
            Console.WriteLine("tianzuo_QiankunInt_initialize error:" + error_code.ToString)
            Return
        End If

        ' 创建数据库 create database
        Dim db_file_name As String = "qiankunDataBase.Ztz"
        Dim db_password As String = "password"
        error_code = tianzuo_QiankunInt_db_create_database(qiankun, db_file_name, db_password, "qiankunDataBase", True)
        If (error_code <> Qiankun_error_code.Qiankun_errorcode_success) Then
            Console.WriteLine("tianzuo_QiankunInt_db_create_database error:" + error_code.ToString)
            Return
        End If

        ' 打开数据库 open database
        error_code = tianzuo_QiankunInt_db_open_database(qiankun, db_file_name, db_password)
        If (error_code <> Qiankun_error_code.Qiankun_errorcode_success) Then
            Console.WriteLine("tianzuo_QiankunInt_db_open_database error:" + error_code.ToString)
            Return
        End If

        ' 向数据库写入一个整型数据 writes an integer to the database
        Dim intInt As Integer = 12345678
        error_code = tianzuo_QiankunInt_data_int_combine(qiankun, db_file_name, db_password, "integer", "", intInt)
        If (error_code <> Qiankun_error_code.Qiankun_errorcode_success) Then
            Console.WriteLine("tianzuo_QiankunInt_data_int_combine error:" + error_code.ToString)
            Return
        End If

        ' 读取刚写入的数据 read the data just written
        Dim return_data As Integer = 0
        error_code = tianzuo_QiankunInt_data_int_extract(qiankun, db_file_name, db_password, "integer", return_data)
        If (error_code <> Qiankun_error_code.Qiankun_errorcode_success) Then
            Console.WriteLine("tianzuo_QiankunInt_data_int_extract error:" + error_code.ToString)
            Return
        End If

        Console.WriteLine("get int: " + return_data.ToString)

        ' 关闭数据库 close database
        error_code = tianzuo_QiankunInt_db_close_database(qiankun, db_file_name, db_password)
        If (error_code <> Qiankun_error_code.Qiankun_errorcode_success) Then
            Console.WriteLine("tianzuo_QiankunInt_db_close_database error:" + error_code.ToString)
            Return
        End If

        Console.WriteLine("test done -------------------")

    End Sub
End Module

```
</details>

<details>
  <summary>go</summary>

```go
package main

/*
#include "tianzuo.QiankunInterface.h"
*/
import "C"
import (
	"fmt"
	"syscall"
	"unsafe"
)

const (
	Qiankun_errorcode_success       = 1  //  正确的值
	Qiankun_errorcode_normal        = 2  //  错误的值
	Qiankun_errorcode_differ        = 3  //  数据库文件数效验未通过 (The number Of database files fails To be verified)
	Qiankun_errorcode_no_rights     = 4  //  数据库密码效验未通过 (The database password verification failed)
	Qiankun_errorcode_db_not_exist  = 5  //  数据库文件不存在 (The database file does Not exist)
	Qiankun_errorcode_exist         = 6  //  数据已存在 (Data already exists)
	Qiankun_errorcode_not_exist     = 7  //  数据不存在 (Data does Not exist)
	Qiankun_errorcode_out_of_range  = 8  //  超出范围 (out Of range)
	Qiankun_errorcode_type          = 9  //  类型不符 (wrong type)
	Qiankun_errorcode_param         = 10 //  参数错误 (parameter Error)
	Qiankun_errorcode_compress      = 11 //  压缩错误 (compression Error)
	Qiankun_errorcode_get_array     = 12 //  数组错误 (arrary Error)
	Qiankun_errorcode_out_of_memory = 13 //  内存溢出 (memory overflow)
	Qiankun_errorcode_handle        = 14 //  句柄错误 (Error handle)
	Qiankun_errorcode_io            = 15 //  IO错误 (io Error)
)

func main() {
	// 初始化接口 initialize the interface
	dll := syscall.MustLoadDLL("tianzuo.Qiankun.dll")

	// 创建实例 create an instance
	tianzuo_QiankunInterface_initialize := dll.MustFindProc("tianzuo_QiankunInterface_initialize")
	qiankun, _, _ := tianzuo_QiankunInterface_initialize.Call()

	tianzuo_QiankunInt_initialize := dll.MustFindProc("tianzuo_QiankunInt_initialize")
	error_code, _, _ := tianzuo_QiankunInt_initialize.Call(qiankun)
	if error_code != Qiankun_errorcode_success {
		fmt.Println("tianzuo_QiankunInt_initialize error: ", error_code)
	}

	// 创建数据库 create database
	db_file_name := C.CString("qiankunDataBase.Ztz")
	db_password := C.CString("password")
	tianzuo_QiankunInt_db_create_database := dll.MustFindProc("tianzuo_QiankunInt_db_create_database")
	error_code, _, _ = tianzuo_QiankunInt_db_create_database.Call(qiankun, uintptr(unsafe.Pointer(db_file_name)), uintptr(unsafe.Pointer(db_password)), uintptr(unsafe.Pointer(C.CString("qiankunDataBase"))), 1)
	if error_code != Qiankun_errorcode_success {
		fmt.Println("tianzuo_QiankunInt_db_create_database error: ", error_code)
	}

	// 打开数据库 open database
	tianzuo_QiankunInt_db_open_database := dll.MustFindProc("tianzuo_QiankunInt_db_open_database")
	error_code, _, _ = tianzuo_QiankunInt_db_open_database.Call(qiankun, uintptr(unsafe.Pointer(db_file_name)), uintptr(unsafe.Pointer(db_password)))
	if error_code != Qiankun_errorcode_success {
		fmt.Println("tianzuo_QiankunInt_db_open_database error: ", error_code)
	}

	// 向数据库写入一个整型数据 writes an integer to the database
	intInt := C.int(12345678)
	tianzuo_QiankunInt_data_int_combine := dll.MustFindProc("tianzuo_QiankunInt_data_int_combine")
	error_code, _, _ = tianzuo_QiankunInt_data_int_combine.Call(qiankun, uintptr(unsafe.Pointer(db_file_name)), uintptr(unsafe.Pointer(db_password)), uintptr(unsafe.Pointer(C.CString("integer"))), uintptr(unsafe.Pointer(C.CString(""))), uintptr(intInt))
	if error_code != Qiankun_errorcode_success {
		fmt.Println("tianzuo_QiankunInt_data_int_combine error: ", error_code)
	}

	// 读取刚写入的数据 read the data just written
	return_int := C.int(0)
	tianzuo_QiankunInt_data_int_extract := dll.MustFindProc("tianzuo_QiankunInt_data_int_extract")
	error_code, _, _ = tianzuo_QiankunInt_data_int_extract.Call(qiankun, uintptr(unsafe.Pointer(db_file_name)), uintptr(unsafe.Pointer(db_password)), uintptr(unsafe.Pointer(C.CString("integer"))), uintptr(unsafe.Pointer(&return_int)))
	if error_code != Qiankun_errorcode_success {
		fmt.Println("tianzuo_QiankunInt_data_int_extract error: ", error_code)
	}

	fmt.Println("get integer data: ", return_int)

	// 关闭数据库 close database
	tianzuo_QiankunInt_db_close_database := dll.MustFindProc("tianzuo_QiankunInt_db_close_database")
	error_code, _, _ = tianzuo_QiankunInt_db_close_database.Call(qiankun, uintptr(unsafe.Pointer(db_file_name)), uintptr(unsafe.Pointer(db_password)))
	if error_code != Qiankun_errorcode_success {
		fmt.Println("tianzuo_QiankunInt_db_close_database error: ", error_code)
	}

	fmt.Println("test done -------------------")
}

```
</details>

<details>
  <summary>rust</summary>

```rust

use std::os::raw::c_char;
use std::ffi::{c_int};
use std::ffi::CString;
use std::ptr::null;

#[link(name = "tianzuo.Qiankun")]
extern {
    fn tianzuo_QiankunInterface_initialize() -> *mut std::ffi::c_void;
    fn tianzuo_QiankunInterface_terminate(obj: *mut std::ffi::c_void);
    fn tianzuo_QiankunInt_initialize(obj: *mut std::ffi::c_void) -> c_int;
    fn tianzuo_QiankunInt_db_create_database(obj: *mut std::ffi::c_void, db_file_name: *const c_char, db_password: *const c_char, db_name: *const c_char, over_write: c_int) -> c_int;
    fn tianzuo_QiankunInt_db_open_database(obj: *mut std::ffi::c_void, db_file_name: *const c_char, db_password: *const c_char) -> c_int;
    fn tianzuo_QiankunInt_db_close_database(obj: *mut std::ffi::c_void, db_file_name: *const c_char, db_password: *const c_char) -> c_int;
    fn tianzuo_QiankunInt_data_int_combine(obj: *mut std::ffi::c_void, db_file_name: *const c_char, db_password: *const c_char, data_name: *const c_char, remark: *const c_char, data: c_int) -> c_int;
    fn tianzuo_QiankunInt_data_int_extract(obj: *mut std::ffi::c_void, db_file_name: *const c_char, db_password: *const c_char, data_name: *const c_char, data: *const c_int) -> c_int;
}

fn main() {
    unsafe {
        // 初始化接口 initialize the interface
        println!("initialize the interface");

        // 创建实例 create an instance
        let qiankun = tianzuo_QiankunInterface_initialize();
        let error_code = tianzuo_QiankunInt_initialize(qiankun);
        if 1 != error_code {
            println!("initialize error: {:?}", error_code);
        }

        // 创建数据库 create database
        let db_file_name = CString::new(String::from("qiankunDataBase.Ztz")).expect("Failed to create CString");
        let db_password = CString::new(String::from("password")).expect("Failed to create CString");
        let db_name = CString::new(String::from("qiankunDataBase")).expect("Failed to create CString");
        let error_code = tianzuo_QiankunInt_db_create_database(qiankun, db_file_name.as_ptr(), db_password.as_ptr(), db_name.as_ptr(), 1);
        if 1 != error_code {
            println!("db_create_database error: {:?}", error_code);
        }

        // 打开数据库 open database
        let error_code = tianzuo_QiankunInt_db_open_database(qiankun, db_file_name.as_ptr(), db_password.as_ptr());
        if 1 != error_code {
            println!("db_open_database error: {:?}", error_code);
        }

        // 向数据库写入一个整型数据 writes an integer to the database
        let int = 12345678;
        let data_name = CString::new(String::from("integer")).expect("Failed to create CString");
        let error_code = tianzuo_QiankunInt_data_int_combine(qiankun, db_file_name.as_ptr(), db_password.as_ptr(), data_name.as_ptr(), null(), int);
        if 1 != error_code {
            println!("data_combine error: {:?}", error_code);
        }

        // 读取刚写入的数据 read the data just written
        let mut return_data: c_int = 0;
        let error_code = tianzuo_QiankunInt_data_int_extract(qiankun, db_file_name.as_ptr(), db_password.as_ptr(), data_name.as_ptr(), &mut return_data);
        if 1 != error_code {
            println!("data_extract error: {:?}", error_code);
        }

        println!("get integer data: {:?}", return_data);

        // 关闭数据库 close database
        let error_code = tianzuo_QiankunInt_db_close_database(qiankun, db_file_name.as_ptr(), db_password.as_ptr());
        if 1 != error_code {
            println!("db_close_database error: {:?}", error_code);
        }

        println!("test done -------------------")
    }
}

```
</details>

<details>
  <summary>ruby</summary>

```ruby
require 'fiddle'

lib = Fiddle::dlopen('C:/Users/zhengtianzuo/RubymineProjects/tianzuo.QiankunTest/tianzuo.Qiankun.dll')
tianzuo_QiankunInterface_initialize = Fiddle::Function.new(lib['tianzuo_QiankunInterface_initialize'], [], Fiddle::TYPE_VOIDP)
tianzuo_QiankunInterface_terminate = Fiddle::Function.new(lib['tianzuo_QiankunInterface_terminate'], [Fiddle::TYPE_VOIDP], Fiddle::TYPE_VOID)
tianzuo_QiankunInt_initialize = Fiddle::Function.new(lib['tianzuo_QiankunInt_initialize'], [Fiddle::TYPE_VOIDP], Fiddle::TYPE_INT)
tianzuo_QiankunInt_db_create_database = Fiddle::Function.new(lib['tianzuo_QiankunInt_db_create_database'], [Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_INT], Fiddle::TYPE_INT)
tianzuo_QiankunInt_db_open_database = Fiddle::Function.new(lib['tianzuo_QiankunInt_db_open_database'], [Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP], Fiddle::TYPE_INT)
tianzuo_QiankunInt_data_int_combine = Fiddle::Function.new(lib['tianzuo_QiankunInt_data_int_combine'], [Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_INT], Fiddle::TYPE_INT)
tianzuo_QiankunInt_data_int_extract = Fiddle::Function.new(lib['tianzuo_QiankunInt_data_int_extract'], [Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP], Fiddle::TYPE_INT)
tianzuo_QiankunInt_db_close_database = Fiddle::Function.new(lib['tianzuo_QiankunInt_db_close_database'], [Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP, Fiddle::TYPE_VOIDP], Fiddle::TYPE_INT)

# 初始化接口 initialize the interface
print("initialize the interface\n")

# 创建实例 create an instance
qiankun = tianzuo_QiankunInterface_initialize.call()
error_code = tianzuo_QiankunInt_initialize.call(qiankun)
if error_code != 1
  print("initialize error:", error_code, "\n")
  return
end

# 创建数据库 create database
db_file_name = "qiankunDataBase.Ztz"
db_password = "password"
error_code = tianzuo_QiankunInt_db_create_database.call(qiankun, db_file_name, db_password, "qiankunDataBase", 1)
if error_code != 1
  print("db_create_database error:", error_code, "\n")
  return
end

# 打开数据库 open database
error_code = tianzuo_QiankunInt_db_open_database.call(qiankun, db_file_name, db_password)
if error_code != 1
  print("db_open_database error:", error_code, "\n")
return
end

# 向数据库写入一个整型数据 writes an integer to the database
intInt = 12345678
error_code = tianzuo_QiankunInt_data_int_combine.call(qiankun, db_file_name, db_password, "integer", "", intInt)
if error_code != 1
  print("data_combine error:", error_code, "\n")
return
end

# 读取刚写入的数据 read the data just written
return_data = Fiddle::Pointer.malloc(Fiddle::SIZEOF_INT)
error_code = tianzuo_QiankunInt_data_int_extract.call(qiankun, db_file_name, db_password, "integer", return_data)
if error_code != 1
  print("data_extract error:", error_code, "\n")
return
end

integer = return_data[0, Fiddle::SIZEOF_INT].unpack('I')[0]
print("get integer data: ", integer, "\n")

# 关闭数据库 close database
error_code = tianzuo_QiankunInt_db_close_database.call(qiankun, db_file_name, db_password)
if error_code != 1
  print("db_close_database error:", error_code)
return
end

print("test done -------------------")

```
</details>

# download

|||||
|--|--|--|--|
|[![download](../img/com_btnGitHub.svg)](https://github.com/zhengtianzuo/tianzuo.Qiankun/releases)|[![download](../img/com_btnGitee.svg)](https://gitee.com/zhengtianzuo/tianzuo.Qiankun/releases)|[![download](../img/down_baidu.svg)](https://pan.baidu.com/s/1ecnWXCHtFUT0edNqCLwFbQ?pwd=1234)|[![download](../img/down_weiyun.svg)](https://share.weiyun.com/TVxOYtJr)|
