#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <exception>
#include <stdexcept>
#include "Utils.h"
#include "SymbolTable.h"
#include "InstructionSet.h"

// 汇编器异常类
class AssemblerException : public std::runtime_error
{
public:
    enum ErrorCode
    {
        FILE_READ_ERROR = 1,     // 文件读取失败
        FILE_WRITE_ERROR = 2,    // 文件写入失败
        SYNTAX_ERROR = 3,        // 语法错误
        SEMANTIC_ERROR = 4,      // 语义错误（如未定义的标签）
        INVALID_OPERAND = 5,     // 无效操作数
        INVALID_INSTRUCTION = 6, // 未知指令
        SYMBOL_ERROR = 7,        // 符号表错误（如重复定义）
        VALUE_OUT_OF_RANGE = 8   // 值超出范围
    };

    AssemblerException(ErrorCode code, const std::string &message, int lineNum = -1)
        : std::runtime_error(formatMessage(code, message, lineNum)),
          m_code(code),
          m_lineNum(lineNum),
          m_message(message) {}

    ErrorCode getCode() const { return m_code; }
    int getLineNumber() const { return m_lineNum; }
    const std::string &getMessage() const { return m_message; }

private:
    ErrorCode m_code;
    int m_lineNum;
    std::string m_message;

    static std::string formatMessage(ErrorCode code, const std::string &msg, int lineNum)
    {
        std::string prefix;
        switch (code)
        {
        case FILE_READ_ERROR:
            prefix = "[FILE_READ_ERROR] ";
            break;
        case FILE_WRITE_ERROR:
            prefix = "[FILE_WRITE_ERROR] ";
            break;
        case SYNTAX_ERROR:
            prefix = "[SYNTAX_ERROR] ";
            break;
        case SEMANTIC_ERROR:
            prefix = "[SEMANTIC_ERROR] ";
            break;
        case INVALID_OPERAND:
            prefix = "[INVALID_OPERAND] ";
            break;
        case INVALID_INSTRUCTION:
            prefix = "[INVALID_INSTRUCTION] ";
            break;
        case SYMBOL_ERROR:
            prefix = "[SYMBOL_ERROR] ";
            break;
        case VALUE_OUT_OF_RANGE:
            prefix = "[VALUE_OUT_OF_RANGE] ";
            break;
        default:
            prefix = "[UNKNOWN_ERROR] ";
            break;
        }

        std::string result = prefix + msg;
        if (lineNum > 0)
        {
            result += " (Line " + std::to_string(lineNum) + ")";
        }
        return result;
    }
};

class Assembler
{
public:
    // 汇编方法，可能抛出 AssemblerException
    // 使用 try-catch 来处理异常
    void assemble(std::string inputPath, std::string outputPath)
    {
        try
        {
            // 验证输入文件路径
            if (inputPath.empty())
            {
                throw AssemblerException(AssemblerException::FILE_READ_ERROR,
                                         "Input file path is empty");
            }

            // 读取文件
            std::string sourceCode = Utils::readFile(inputPath);
            if (sourceCode.empty())
            {
                throw AssemblerException(AssemblerException::FILE_READ_ERROR,
                                         "Unable to read input file: " + inputPath);
            }

            // 验证输出文件路径
            if (outputPath.empty())
            {
                throw AssemblerException(AssemblerException::FILE_WRITE_ERROR,
                                         "Output file path is empty");
            }

            // 执行汇编过程
            preprocess(sourceCode);
            firstPass();
            secondPass();

            // 生成输出并写入文件
            std::string output = generateMachineCodeOutput();
            if (!Utils::writeFile(outputPath, output))
            {
                throw AssemblerException(AssemblerException::FILE_WRITE_ERROR,
                                         "Unable to write output file: " + outputPath);
            }
        }
        catch (const AssemblerException &)
        {
            throw; // 重新抛出已知的汇编器异常
        }
        catch (const std::exception &e)
        {
            // 捕获其他标准异常并转换为汇编器异常
            throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                     std::string("Unexpected error: ") + e.what());
        }
    }

    void printSymbolTable() const
    {
        std::cout << "Symbol Table:" << std::endl;
        std::cout << "---" << std::endl;
        for (const auto &entry : symbolTable.getTable())
        {
            int address = entry.second + origAddress;
            std::cout << entry.first << " : x" << std::hex << address << std::dec << std::endl;
        }
    }

    void printMachineCodes() const
    {
        std::cout << "Machine Codes:" << std::endl;
        for (const auto &code : machineCodes)
        {
            int address = origAddress + (&code - &machineCodes[0]);
            std::cout << "x" << std::hex << address << std::dec << ": ";
            std::cout << code << std::endl;
        }
    }

private:
    std::vector<std::string> processedCode;
    SymbolTable symbolTable;
    InstructionSet instructionSet;
    std::vector<std::string> machineCodes;
    int origAddress;
    int currentLineNum; // 用于错误报告的行号跟踪

    void preprocess(const std::string &sourceCode);
    void firstPass();
    void secondPass();
    std::string generateMachineCodeOutput();
};

#endif