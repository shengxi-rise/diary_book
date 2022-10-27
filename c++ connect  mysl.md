# 连接方式
1. 难:使用connector/ODBC
2. 易:使用官方提供的mysql_connect_c++,基于JDBC设计的,[官方download](https://dev.mysql.com/downloads/connector/cpp/)
3. so on......

# [ODBC和JDBC区别](https://blog.csdn.net/qq_42374697/article/details/117946358)
* *简单来说*： JDBC比ODBC容易理解,容易编写. JDBC是本来是java用来连接数据库使用的API,后来拓展到了面向对象. JDBC可以在任意平台上使用. 而ODBC只能在windows系统上使用(网上说的). JDBC代表着java数据库连接, 而ODBC代表着开放式数据库连接. ODBC可以以统一方式管理所有数据库.

# mysql_connect_c++的环境配置
1. 下载官方文件connector/c++[download](https://dev.mysql.com/downloads/connector/cpp/)
2. 下载boost库[download](http://www.boost.org/)
3. 配置VS:
    * 在VS中新建项目
     <div align=center><img src = "https://s1.ax1x.com/2022/08/28/vWNIMR.png"/></div>

    * > 打开对应项目的属性界面,在图中所示位置,添加 **boost** 的目录以及mysql_connect_c++里面的 **include** 目录.
    ---

    <div align=center><img src = "https://s1.ax1x.com/2022/08/28/vWUtyR.png"/></div>

    * > 然后在另一个位置,如图所示. 添加 **mysqlcppconn.lib** 文件
    * 最后,将 **mysqlcppconn-9-vs14.dll** 文件放在exe生成的目录下(也有可能不是这个,看报错就行)

4. *示例代码*：
   ```c++
    #include <mysql\jdbc.h>
    #pragma comment(lib, "mysqlcppconn.lib")

    #include<iostream>
    using namespace std;

    int main()
    {
        //连接字符串
        const char* host = "tcp://127.0.0.1/test";
        //后面的test是指数据库
        const char* user = "root";//用户名
        const char* passwd = "b555";//密码

        try
        {
            //获取驱动实例
            sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

            //connect连接数据库，返回Connection对象指针
            sql::Connection* conn = driver->connect(host, user, passwd);

            if (conn != nullptr && conn->isClosed() == 0) {
                cout << "数据库连接成功！" << endl;
            }
            else {
                cout << "数据库连接失败" << endl;
            }

            //设置客户端地连接编码格式为GBK
            sql::Statement* stmt = conn->createStatement();
            stmt->execute("SET character set gbk");



            //插入数据
            if (true)
            {
                //创建PreparedStatement准备sql语句
                const sql::SQLString  sql = "INSERT  INTO Student VALUES(?,?,?);";
                sql::PreparedStatement* pstmt = conn->prepareStatement(sql);

                //填充？参数
                pstmt->setInt(1, 1000);
                pstmt->setString(2, "学习");
                pstmt->setInt(3, 22);

                //执行sql语句
                int ret = pstmt->executeUpdate(); // executeUpdate适用于UPDATE  INSERT  DELETE操作
                if (ret == 1) {
                    cout << "插入数据成功！" << endl;
                }
                else {
                    cout << "数据插入失败" << endl;
                }
                //关闭语句
                pstmt->close();

            }
            //查询数据
            if (true)
            {
                const sql::SQLString  sql = "SELECT  *  FROM   Student ;";
                sql::ResultSet* rs = stmt->executeQuery(sql);// executeQuery适用于 SELECT

                while (rs != nullptr && rs->next()) {

                    int id = rs->getInt("id");//按 列名获取数据
                    sql::SQLString  name = rs->getString(2);//按序号方式获取列数据
                    int age = rs->getInt(3);

                    cout << id << "\t" << name.c_str() << "\t" << age << endl;
                }

                //关闭结果集
                rs->close();
            }
            //修改数据
            if (true)
            {
                const sql::SQLString  sql = "UPDATE  Student  SET  name='天天向上'  WHERE id =1000;";
                int  effectRows = stmt->executeUpdate(sql);// executeUpdate适用于 UPDATE INSERT  DELETE
                cout << "修改数据成功" << effectRows << "行受影响！" << endl;
            }

            //
            //删除数据
            if (true)
            {
                /*写法一
                const sql::SQLString  sql = "DELETE  FROM Student WHERE id =1000;";
                int  effectRows = stmt->executeUpdate(sql);// executeUpdate适用于 UPDATE INSERT  DELETE
                cout << "删除数据成功" << effectRows << "行受影响！" << endl;
            */


                const sql::SQLString  sql = "DELETE  FROM Student WHERE id =1000;";
                // const sql::SQLString  sql = "SELECT  * FROM Student WHERE id =1000;";
                bool  ret = stmt->execute(sql);// execute适用于 UPDATE INSERT  DELETE  SELECT
                if (ret)
                {//代表是SELECT 操作，可以获取结果集
                    sql::ResultSet* rs = stmt->getResultSet();
                    cout << "查询操作，返回了数据集" << rs << endl;
                }
                else
                {//代表是UPDATE INSERT  DELETE操作，可以获取受影响的行数
                    int  effectRows = stmt->getUpdateCount();
                    cout << "删除数据成功" << effectRows << "行受影响！" << endl;
                }


            }
            //


            //关闭语句
            stmt->close();

            //关闭数据库连接
            conn->close();
            cout << "关闭数据库连接成功！" << endl;

            delete conn;
        }
        catch (sql::SQLException& e) {
            std::cout << "错误: " << e.getErrorCode() << "  " << e.what();
        }
    }
   ```
* 使用之前还要自己创建一个 **test** 的数据库和 **student** 的数据表
  ```sql
    create database test;
    CREATE TABLE IF NOT EXISTS `student` ( 
        `id` INT(12) UNSIGNED AUTO_INCREMENT COMMENT '学生id',
        `name` VARCHAR ( 255 ) NOT NULL COMMENT '学生姓名', 
        `age` INT ( 12 ) COMMENT '年龄', 
        PRIMARY KEY ( `id` ) 
    ) ENGINE = INNODB DEFAULT CHARSET = gbk;
  ```