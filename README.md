[[中文版]](https://github.com/Kori-Sama/mygo-management/blob/main/README-CN.md)
## A Distributed HTTP Server Written in C++ 

This server cannot run independently, it is a service in a distributed system, and it depends on its [main server](https://github.com/Kori-Sama/mygo).

### Third-Party Dependencies

- gRPC (for remote calls to the main server)
- cJSON (for serializing data to JSON)
- googletest (unit test framework)

### Links to Other Parts of the Project

- [GUI](https://github.com/Kori-Sama/mygo-gui-management)
- [Main Server](https://github.com/Kori-Sama/mygo)

### Main Implementations
- Based on TCP, manually implemented HTTP protocol parsing and sending, and built a simple HTTP server.
- Based on the producer-consumer model, manually implemented a thread pool to improve the concurrent performance of the server.
- Based on gRPC, implemented a gRPC client for exchanging data with the main server.
