编译完毕后在终端中输入 ./Servant 8080开始运行服务器程序，再打开浏览器输入localhost:8080访问我们写的HTTP服务器。

服务器在Servant.cpp中定义的mian函数中监听到了客户浏览器发送的连接请求，主函数accept客户连接并选择一个线程将客户的已连接套接字注册到此线程的EventLoop中。

EventLoop是一个事件循环，每个循环都是在试图从其中的epoll中获取活动的套接字描述符并交给Handler类中处理。

Handler类是处理客户HTTP请求的类，它首先将客户的原始请求转发给Parser类处理，从而获取客户请求的解析结果，Parser类将解析后的结果存入HTTPRequest类型的结构体中。Handler类根据解析后的结果首先测试客户请求的文件是否存在，如果不存在将返回错误。如果文件存在但客户的权限不允许那么也返回客户错误信息。

每个客户请求处理完毕后就关闭此套接字然后EventLoop继续循环。

## 解析HTTP请求
HTTP服务器的一个重要任务是解析HTTP请求，源代码中Parser.h和Parser.cpp文件中定义的Parser类就是干这个的。为了表示解析后的结果我们定义了

一个结构体HTTPRequest结构存储解析后的结果，定义如下：
```c++
// 解析请求后的数据存储在HTTPRequest结构体中
typedef struct
{
    std::string method;     // 请求的方法
    std::string uri;        // 请求的uri
    std::string version;    // HTTP版本
    std::string host;       // 请求的主机名
    std::string connection; // Connection首部
} HTTPRequest;
```

Parser类的构造函数接受一个字符串类型的参数，解析后的值存储在_parseResult结构体中，并提供一个接口getParseResult函数访问解析后的结果。解析的顺序如下：

首先parseLine函数按照`\r\n`作为分隔解析出每一行请求，并把结果存储在_lines中，其中每一个元素是一行请求。

然后调用parseRequestLine函数解析请求行，函数按空格解析各个字段，并把解析得到的方法、URI和HTTP版本存入HTTPRequest类型的结构体中。

最后调用parseHeaders函数解析其他头部字段，并将结果存入HTTPRequest类型的结构体中。

## 线程池
要想提高服务器的性能，使用线程池是一种很好的方法，它避免了单线程的低效率，并且避免了每次创建一个线程的额外开销。

线程池中线程的数目由主函数设置。线程的数目不宜过多也不宜过少，过多的话会增加CPU的调度开销；过少的话不能发挥多核CPU的性能。所以线程池中常驻线程的数目应该等于CPU核心数，以尽量减少任务切换带来的额外开销并充分发挥处理器的性能。


## 其他模块的作用
由于我们写的是非阻塞的HTTP服务器，所以缓冲区是必须要有的。在读取客户浏览器的请求和发送服务器的响应时我们会先将不完整的请求和响应暂存到缓冲区中，等到数据全部读完或者写完后再一并发送或者交给其他模块处理。所以Buffer.cpp和Buffer.h文件中定义的Buffer类就显得非常有用了。
