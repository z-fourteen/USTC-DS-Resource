#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// 指令操作数类型
enum class OperandType {
    REGISTER,      
    IMMEDIATE,     
    OFFSET,        
    NONE           // 无操作数
};

// 位字段信息：描述二进制指令中字段的位置和大小
struct BitField {
    std::string name;     
    int startBit;         // 起始位（从右到左，0为最低位）
    int length;           // 字段长度（位数）
};

// 指令模式：描述特定操作数类型下的指令格式
struct OperandMode {
    int operandCount;           // 操作数个数
    std::vector<OperandType> operandTypes;  // 每个操作数的类型
    std::vector<BitField> bitFields;        // 二进制字段布局信息
};

// 指令信息
struct InstructionInfo {
    std::string name;           
    int opcode;                 
    std::vector<OperandMode> modes;  // 支持的操作数模式
    
    /* 内置函数： 根据操作数类型匹配找到对应的模式 */
    const OperandMode* findMode(const std::vector<OperandType>& operandTypes) const {
        for (const auto& mode : modes) {
            if (mode.operandTypes == operandTypes) {
                return &mode;
            }
        }
        return nullptr;
    }
};

class InstructionSet {
public:
    InstructionSet();
    
    bool isInstruction(const std::string &word) const;
    bool isPseudoOp(const std::string &word) const;
    
    const InstructionInfo* getInstructionInfo(const std::string &mnemonic) const;
    
    const OperandMode* getOperandMode(const std::string &mnemonic, 
                                               const std::vector<OperandType>& operandTypes) const;
    
    const BitField* getBitField(const OperandMode* mode, const std::string& fieldName) const;
    
    int getOpcode(const std::string &mnemonic) const;
    
private:
    // 指令集映射
    std::unordered_map<std::string, InstructionInfo> instructions;
    
    // 伪指令集合
    std::unordered_map<std::string, bool> pseudoOps;
    
    // 初始化指令集
    void initializeInstructions();
    void initializePseudoOps();
};
