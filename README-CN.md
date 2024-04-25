## 一个用C++编写的分布式HTTP服务器

这个服务无法独自运行, 它是一个分布式系统中的服务, 依赖于它的[主服务器](https://github.com/Kori-Sama/mygo)

### 第三方依赖

- gRPC (对主服务器进行远程调用)
- cJSON (将数据序列化为json)

### 项目其他部分链接

- [GUI](https://github.com/Kori-Sama/mygo-gui-management)
- [主服务器](https://github.com/Kori-Sama/mygo)

### 项目主要实现
- 基于TCP, 手动实现了HTTP协议的解析和发送, 构建了一个简易的HTTP服务器
- 基于生产者消费者模型, 手动实现了一个线程池, 提高服务器的并发性能
- 基于gRPC, 实现了一个用于与主服务器交换数据的gRPC客户端
