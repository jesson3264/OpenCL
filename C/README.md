
scl 目录结构  

## build     
linux 环境下的 make 文件 
## scl-win   
windows 下的 sln 工程文件
## include   
scl 库头文件
## src       
scl 源码文件
## test      
test 源代码
## README.md 
这个文件


## 当前目录
当前目录为项目下的 C/ 下目录

## 编译

cd source/build
cmake ..
make install

## 编译测试程序

cd test
cd build
cmake ..
make install 
cd ../bin
./SCL_TEST
