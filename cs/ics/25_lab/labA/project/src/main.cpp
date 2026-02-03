#include "Assembler.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <cstdlib>

// 辅助函数：将相对路径转换为基于项目根目录的绝对路径
std::string get_path(const std::string& relative_path) {
    return std::string(PROJECT_ROOT) + "/" + relative_path;
}

int main(){
    try {
        Assembler assembler;

        std::string input_file = get_path("test_case/asm/fibonacci.asm");
        std::string output_file = get_path("result/fibonacci.obj");

        std::cout << "================================" << std::endl;
        std::cout << "LC-3 Assembler" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "Input file:  " << input_file << std::endl;
        std::cout << "Output file: " << output_file << std::endl;
        std::cout << "================================" << std::endl;

        // 执行汇编
        assembler.assemble(input_file, output_file);
        
        std::cout << "[+] Assembly completed successfully!" << std::endl;
        std::cout << std::endl;

        // 打印符号表
        assembler.printSymbolTable();
        std::cout << std::endl;
        
        // 选择是否打印机器码
        std::cout << "Enter 'D' to view machine codes, or  Enter 'S' to skip: ";
        char cmd;
        std::cin >> cmd;
        
        if (cmd == 'D' || cmd == 'd') {
            std::cout << std::endl;
            std::cout << "Machine Codes:" << std::endl;
            std::cout << "---" << std::endl;
            assembler.printMachineCodes();
        }
        
        std::cout << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "Assembly finished successfully!" << std::endl;
        std::cout << "================================" << std::endl;
        
        return 0;
    }
    catch (const AssemblerException& e) {
        // 捕获汇编器异常
        std::cerr << std::endl;
        std::cerr << "================================" << std::endl;
        std::cerr << "ASSEMBLY ERROR" << std::endl;
        std::cerr << "================================" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "================================" << std::endl;
        
        // 根据错误类型返回不同的状态码
        return static_cast<int>(e.getCode());
    }
    catch (const std::exception& e) {
        // 捕获所有其他标准异常
        std::cerr << std::endl;
        std::cerr << "================================" << std::endl;
        std::cerr << "UNEXPECTED ERROR" << std::endl;
        std::cerr << "================================" << std::endl;
        std::cerr << "Type: " << typeid(e).name() << std::endl;
        std::cerr << "Message: " << e.what() << std::endl;
        std::cerr << "================================" << std::endl;
        
        return 127;  // 通用错误代码
    }
    catch (...) {
        // 捕获所有未知异常
        std::cerr << std::endl;
        std::cerr << "================================" << std::endl;
        std::cerr << "UNKNOWN ERROR" << std::endl;
        std::cerr << "An unknown error has occurred!" << std::endl;
        std::cerr << "================================" << std::endl;
        
        return 128;
    }
}