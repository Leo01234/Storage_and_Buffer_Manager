# Storage_and_Buffer_Manager

## 运行环境

- Windows SDK Version: 10.0

- Platform Toolset: Visual Studio 2022 (v143)

- C++ Language Standard: /std:c++latest

## 注意事项

- 当不存在`data.dbf`时，运行时将生成正确的`data.dbf`数据库文件，并退出；
  当存在`data.dbf`时，认为该文件已正确生成，将进行Lab的内容。

- 默认buffer大小为1024，可通过命令行参数调节。