#include "Assembler.h"
#include <sstream>
#include <cctype>
#include <bitset>

/*
    功能：预处理
        1. 删除注释和不必要的空白
        2. 将所有行转换为大写
*/
void Assembler::preprocess(const std::string &sourceCode)
{
    std::string line;
    std::istringstream stream(sourceCode);

    while (std::getline(stream, line))
    {
        // 删除注释（分号后的所有内容）
        size_t commentPos = line.find(";");
        if (commentPos != std::string::npos)
        {
            line = line.substr(0, commentPos);
        }

        // 删除尾部空白
        while (!line.empty() && std::isspace(line.back()))
        {
            line.pop_back();
        }

        // 删除前导空白
        size_t start = 0;
        while (start < line.length() && std::isspace(line[start]))
        {
            start++;
        }
        line = line.substr(start);

        // 转换为大写，但需要特殊处理 .STRINGZ
        // .STRINGZ 后面的字符串操作数需要保留原始大小写
        size_t stringzPos = line.find(".STRINGZ");
        if (stringzPos != std::string::npos)
        {
            // 找到 .STRINGZ 的位置
            // 1. 前面部分（包括.STRINGZ）转换为大写
            std::string beforeStringz = line.substr(0, stringzPos + 8); // +8是".STRINGZ"的长度
            for (char &ch : beforeStringz)
            {
                ch = std::toupper(static_cast<unsigned char>(ch));
            }

            // 2. 后面的字符串部分保留原样
            std::string afterStringz = line.substr(stringzPos + 8);

            // 3. 合并
            line = beforeStringz + afterStringz;
        }
        else
        {
            // 非 .STRINGZ 行，全部转换为大写
            for (char &ch : line)
            {
                ch = std::toupper(static_cast<unsigned char>(ch));
            }
        }

        // 将逗号转换为空格
        for (char &ch : line)
        {
            if (ch == ',')
            {
                ch = ' ';
            }
        }

        // 仅添加非空行
        if (!line.empty())
        {
            processedCode.push_back(line);
        }
    }
}

/* 功能：第一遍扫描，构建符号表 */
void Assembler::firstPass()
{
    int instructionAddress = 0; // 存放指令地址与初始地址的差值
    currentLineNum = 0;

    for (const auto &line : processedCode)
    {
        currentLineNum++;
        std::istringstream lineStream(line);
        std::string word;

        while (lineStream >> word)
        {
            if (instructionSet.isInstruction(word) || instructionSet.isPseudoOp(word))
            {
                if (word == ".STRINGZ")
                {
                    // 获取该行剩余的全部内容
                    std::string strOperand;
                    std::getline(lineStream, strOperand);

                    // 去掉引号
                    size_t firstQuote = strOperand.find('\"');
                    size_t lastQuote = strOperand.find_last_of('\"');
                    if (firstQuote == std::string::npos || lastQuote == std::string::npos || firstQuote == lastQuote)
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 "Invalid string literal in .STRINGZ: '" + strOperand + "'",
                                                 currentLineNum);
                    }

                    // 提取引号之间的字符串
                    std::string strContent = strOperand.substr(firstQuote + 1, lastQuote - firstQuote - 1);

                    // 计算实际的字符数（考虑转义序列）
                    int actualLength = 0;
                    for (int i = 0; i < strContent.length(); ++i)
                    {
                        if (strContent[i] == '\\' && i + 1 < strContent.length())
                        {
                            char escaped = strContent[i + 1];
                            if (escaped == 'n' || escaped == 't' || escaped == '\\' || escaped == '\"')
                            {
                                actualLength++;
                                i++; // 跳过转义字符
                            }
                            else
                            {
                                actualLength++;
                            }
                        }
                        else
                        {
                            actualLength++;
                        }
                    }

                    instructionAddress += actualLength + 1; // +1 是 null 终止符
                    break;                                  // 处理完 .STRINGZ，跳出当前行处理
                }
                else if (word == ".BLKW")
                {
                    std::string blockSizeStr;
                    if (lineStream >> blockSizeStr)
                    {
                        try
                        {
                            int blockSize = 0;
                            // 十进制
                            blockSize = std::stoi(blockSizeStr);
                            if (blockSize <= 0)
                            {
                                throw AssemblerException(AssemblerException::VALUE_OUT_OF_RANGE,
                                                         "Invalid .BLKW block size (must be positive): " + blockSizeStr,
                                                         currentLineNum);
                            }

                            instructionAddress += blockSize;
                        }
                        catch (const std::exception &e)
                        {
                            throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                     "Invalid .BLKW operand: '" + blockSizeStr + "'",
                                                     currentLineNum);
                        }
                    }
                    else
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 "Missing operand for .BLKW directive",
                                                 currentLineNum);
                    }
                    break; // 处理完 .BLKW，跳出当前行处理
                }
                else
                {
                    if (word != ".ORIG")
                    {
                        instructionAddress += 1;
                    }
                }
                break;
            }
            else if (!symbolTable.contains(word))
            {
                // symbolTable.addSymbol(word, instructionAddress);
                symbolTable.addSymbol(word, instructionAddress);
            }
            else
            {
                // 重复定义符号 - 抛出异常
                throw AssemblerException(AssemblerException::SYMBOL_ERROR,
                                         "Duplicate symbol definition: '" + word + "'",
                                         currentLineNum);
            }
        }
    }
}

void Assembler::secondPass()
{
    int currentAddress = 0;
    currentLineNum = 0;

    for (const auto &line : processedCode)
    {
        currentLineNum++;
        std::istringstream lineStream(line);
        std::string word;

        while (lineStream >> word)
        {
            if (instructionSet.isInstruction(word))
            {
                std::string operand;
                std::vector<std::string> operands;
                std::vector<OperandType> operandTypes;

                // 收集操作数并识别类型
                while (lineStream >> operand)
                {
                    operands.push_back(operand);
                    if (operand.empty())
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 "Empty operand detected", currentLineNum);
                    }

                    if (operand[0] == 'R' && isdigit(operand[1])) // 寄存器
                    {
                        operandTypes.push_back(OperandType::REGISTER);
                    }
                    else if (operand[0] == '#' || operand[0] == 'X') // 立即数
                    {
                        operandTypes.push_back(OperandType::IMMEDIATE);
                    }
                    else // 标签或偏移量
                    {
                        operandTypes.push_back(OperandType::OFFSET);
                    }
                }

                // 查询指令模式
                const OperandMode *mode = instructionSet.getOperandMode(word, operandTypes);
                if (mode == nullptr)
                {
                    throw AssemblerException(AssemblerException::INVALID_OPERAND,
                                             "Invalid operands for instruction '" + word + "'",
                                             currentLineNum);
                }

                // 生成机器码
                uint16_t machineCode = 0;
                const InstructionInfo *info = instructionSet.getInstructionInfo(word);
                if (info == nullptr)
                {
                    throw AssemblerException(AssemblerException::INVALID_INSTRUCTION,
                                             "Unknown instruction: '" + word + "'",
                                             currentLineNum);
                }

                // 设置操作码
                const BitField *opcodeField = instructionSet.getBitField(mode, "OPCODE");
                if (opcodeField == nullptr)
                {
                    throw AssemblerException(AssemblerException::INVALID_INSTRUCTION,
                                             "Missing OPCODE field for instruction '" + word + "'",
                                             currentLineNum);
                }
                machineCode |= (info->opcode << opcodeField->startBit);

                // 设置NZP位
                if (word == "BR" || word == "BRN" || word == "BRZ" || word == "BRP" ||
                    word == "BRNZ" || word == "BRNP" || word == "BRZP" || word == "BRNZP")
                {

                    const BitField *nzpField = instructionSet.getBitField(mode, "NZP");

                    int nzpBits = 0;
                    if (word.find('N') != std::string::npos)
                        nzpBits |= 0b100;
                    if (word.find('Z') != std::string::npos)
                        nzpBits |= 0b010;
                    if (word.find('P') != std::string::npos)
                        nzpBits |= 0b001;
                    if(word == "BR")
                        nzpBits = 0b111; // BR 等同于 BRNZP

                    machineCode |= (nzpBits << 9);
                }

                // 设置标识位
                if (word == "ADD" || word == "AND")
                {
                    int startBit = 5; // 立即数标志位位置
                    if (operandTypes.size() >= 3 && operandTypes[2] == OperandType::IMMEDIATE)
                    {
                        machineCode |= (1 << startBit);
                    }
                }
                if(word == "JSR" )
                {
                    int startBit = 11; // JSR/JSRR 标志位位置
                    machineCode |= (1 << startBit);
                }
                if(word == "RET"){
                    machineCode = 49600;   // RET 的固定机器码
                
                }
                if(word == "NOT"){
                    machineCode = 36927 ;  // NOT 的机器码固定部分
                }

                // 设置trap的vector位
                if (word == "PUTS" || word == "IN" || word == "OUT" || word == "HALT" || word == "GETC")
                {
                    const BitField *vectorField = instructionSet.getBitField(mode, "TRAPVECT");
                    if (vectorField == nullptr)
                    {
                        throw AssemblerException(AssemblerException::INVALID_INSTRUCTION,
                                                 "Missing VECTOR8 field for instruction '" + word + "'",
                                                 currentLineNum);
                    }
                    int vectorNum = 0;
                    if (word == "GETC")
                        vectorNum = 0x20;
                    else if (word == "OUT")
                        vectorNum = 0x21;
                    else if (word == "PUTS")
                        vectorNum = 0x22;
                    else if (word == "IN")
                        vectorNum = 0x23;
                    else if (word == "PUTSP")
                        vectorNum = 0x24;
                    else if (word == "HALT")
                        vectorNum = 0x25;

                    machineCode |= (vectorNum << vectorField->startBit);
                }

                // 根据操作数设置其他字段
                for (size_t i = 0; i < operands.size(); ++i)
                {
                    if (i + 1 >= mode->bitFields.size())
                    {
                        throw AssemblerException(AssemblerException::INVALID_OPERAND,
                                                 "Too many operands for instruction '" + word + "'",
                                                 currentLineNum);
                    }

                    const BitField *field = instructionSet.getBitField(mode, mode->bitFields[i + 1].name);
                    if (field == nullptr)
                    {
                        throw AssemblerException(AssemblerException::INVALID_OPERAND,
                                                 "Invalid field definition for operand " + std::to_string(i + 1),
                                                 currentLineNum);
                    }

                    if (operandTypes[i] == OperandType::REGISTER)
                    {
                        try
                        {
                            int regNum = std::stoi(operands[i].substr(1));
                            if (regNum < 0 || regNum > 7)
                            {
                                throw AssemblerException(AssemblerException::VALUE_OUT_OF_RANGE,
                                                         "Register number out of range: " + operands[i] + " (must be R0-R7)",
                                                         currentLineNum);
                            }
                            machineCode |= (regNum << field->startBit);
                        }
                        catch (const std::invalid_argument &e)
                        {
                            throw AssemblerException(AssemblerException::INVALID_OPERAND,
                                                     "Invalid register format: '" + operands[i] + "'",
                                                     currentLineNum);
                        }
                    }
                    else if (operandTypes[i] == OperandType::IMMEDIATE)
                    {
                        try
                        {
                            int min = -(1 << (field->length - 1));
                            int max = (1 << (field->length - 1)) - 1;
                            int immValue;

                            if (operands[i][0] == '#')
                            {
                                immValue = std::stoi(operands[i].substr(1));
                            }
                            else // 十六进制
                            {
                                immValue = std::stoi(operands[i].substr(1), nullptr, 16);
                            }

                            // 检查范围
                            if (immValue < min || immValue > max)
                            {
                                throw AssemblerException(AssemblerException::VALUE_OUT_OF_RANGE,
                                                         "Immediate value out of range: " + operands[i] +
                                                             " (valid range: " + std::to_string(min) + " to " + std::to_string(max) + ")",
                                                         currentLineNum);
                            }

                            machineCode |= ((immValue & ((1 << field->length) - 1)) << field->startBit);
                        }
                        catch (const AssemblerException &)
                        {
                            throw;
                        }
                        catch (const std::exception &e)
                        {
                            throw AssemblerException(AssemblerException::INVALID_OPERAND,
                                                     "Invalid immediate value: '" + operands[i] + "'",
                                                     currentLineNum);
                        }
                    }
                    else if (operandTypes[i] == OperandType::OFFSET)
                    {
                        if (!symbolTable.contains(operands[i]))
                        {
                            throw AssemblerException(AssemblerException::SEMANTIC_ERROR,
                                                     "Undefined symbol: '" + operands[i] + "'",
                                                     currentLineNum);
                        }

                        int offsetAddress = symbolTable.getAddress(operands[i]);
                        int pcOffset = offsetAddress - (currentAddress + 1);

                        // 检查偏移量范围
                        int maxOffset = (1 << (field->length - 1)) - 1;
                        int minOffset = -(1 << (field->length - 1));
                        if (pcOffset > maxOffset || pcOffset < minOffset)
                        {
                            throw AssemblerException(AssemblerException::VALUE_OUT_OF_RANGE,
                                                     "Branch offset out of range for label '" + operands[i] +
                                                         "' (offset: " + std::to_string(pcOffset) + ")",
                                                     currentLineNum);
                        }

                        machineCode |= ((pcOffset & ((1 << field->length) - 1)) << field->startBit);
                    }
                }

                currentAddress += 1;
                std::bitset<16> bs(machineCode);
                machineCodes.push_back(bs.to_string());
                break;
            }
            else if (instructionSet.isPseudoOp(word))
            {
                if (word == ".ORIG" || word == ".FILL")
                {
                    std::string value;
                    if (!(lineStream >> value))
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 word + " requires an argument",
                                                 currentLineNum);
                    }

                    try
                    {
                        int valuenum;
                        if (value[0] == 'X')
                        {
                            valuenum = std::stoi(value.substr(1), nullptr, 16);
                        }
                        else if (value[0] == '#')
                        {
                            valuenum = std::stoi(value.substr(1));
                        }
                        else
                        { 
                            valuenum = symbolTable.getAddress(value) + origAddress;
                        }

                        std::bitset<16> bs(valuenum);
                        machineCodes.push_back(bs.to_string());

                        if (word == ".ORIG")
                        {
                            origAddress = valuenum;
                        }
                        else
                        {
                            currentAddress += 1;
                        }
                    }
                    catch (const std::exception &e)
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 "Invalid " + word + " argument: '" + value + "'",
                                                 currentLineNum);
                    }
                }
                else if (word == ".END")
                {
                    return; // 正常结束汇编
                }
                else if (word == ".BLKW")
                {
                    std::string countStr;
                    if (!(lineStream >> countStr))
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 ".BLKW requires a count argument",
                                                 currentLineNum);
                    }

                    try
                    {
                        int count = std::stoi(countStr.substr(0));
                        if (count < 0)
                        {
                            throw AssemblerException(AssemblerException::VALUE_OUT_OF_RANGE,
                                                     ".BLKW count must be positive",
                                                     currentLineNum);
                        }

                        for (int i = 0; i < count; ++i)
                        {
                            machineCodes.push_back("0000000000000000");
                        }
                        currentAddress += count;
                    }
                    catch (const AssemblerException &)
                    {
                        throw;
                    }
                    catch (const std::exception &e)
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR,
                                                 "Invalid .BLKW argument: '" + countStr + "'",
                                                 currentLineNum);
                    }
                }
                else if (word == ".STRINGZ")
                {
                    // 获取该行剩余的全部内容
                    std::string remainder;
                    std::getline(lineStream, remainder);

                    // 去掉引号
                    size_t firstQuote = remainder.find('\"');
                    size_t lastQuote = remainder.find_last_of('\"');

                    if (firstQuote == std::string::npos || lastQuote == std::string::npos || firstQuote == lastQuote)
                    {
                        throw AssemblerException(AssemblerException::SYNTAX_ERROR, "Invalid string", currentLineNum);
                    }
                    std::string content = remainder.substr(firstQuote + 1, lastQuote - firstQuote - 1);

                    // 处理转义字符
                    for (int i = 0; i < content.length(); ++i)
                    {
                        char ch = content[i];
                        if (ch == '\\' && i + 1 < content.length())
                        {
                            char escaped = content[i + 1];
                            if (escaped == 'n')
                            {
                                ch = '\n';
                                i++;
                            }
                        }

                        // 存入机器码
                        std::bitset<16> bs(static_cast<unsigned char>(ch));
                        machineCodes.push_back(bs.to_string());
                        currentAddress += 1;
                    }

                    // 补上 NULL (x0000) 结束符
                    machineCodes.push_back("0000000000000000");
                    currentAddress += 1;
                }
            }
        }
    }
}

std::string Assembler::generateMachineCodeOutput()
{
    std::string output;
    for (const auto &code : machineCodes)
    {
        output += code + "\n";
    }
    return output;
}