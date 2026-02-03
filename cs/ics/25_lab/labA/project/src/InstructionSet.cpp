#include "InstructionSet.h"

InstructionSet::InstructionSet() {
    initializeInstructions();
    initializePseudoOps();
}

void InstructionSet::initializeInstructions() {
    // ADD: 操作码 0001，支持寄存器模式和立即数模式
    // 格式: [OPCODE(4)|DR(3)|SR1(3)|00|SR2(3)]  或  [OPCODE(4)|DR(3)|SR1(3)|1|IMM5(5)]
    instructions["ADD"] = {
        "ADD", 
        0x1,
        {
            // 寄存器模式：ADD R1, R2, R3
            {
                3, 
                {OperandType::REGISTER, OperandType::REGISTER, OperandType::REGISTER},
                {
                    {"OPCODE", 12, 4},   // 位12-15
                    {"DR", 9, 3},        // 位9-11
                    {"SR1", 6, 3},       // 位6-8
                    {"SR2", 0, 3}        // 位0-2，位3-4是00
                }
            },
            // 立即数模式：ADD R1, R2, #5
            {
                3,
                {OperandType::REGISTER, OperandType::REGISTER, OperandType::IMMEDIATE},
                {
                    {"OPCODE", 12, 4},   // 位12-15
                    {"DR", 9, 3},        // 位9-11
                    {"SR1", 6, 3},       // 位6-8
                    {"IMM5", 0, 5}       // 位0-4，位5是1
                }
            }
        }
    };
    
    // AND: 操作码 0101，支持寄存器模式和立即数模式
    instructions["AND"] = {
        "AND",
        0x5,
        {
            // 寄存器模式：AND R1, R2, R3
            {
                3,
                {OperandType::REGISTER, OperandType::REGISTER, OperandType::REGISTER},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"SR1", 6, 3},
                    {"SR2", 0, 3}
                }
            },
            // 立即数模式：AND R1, R2, #5
            {
                3,
                {OperandType::REGISTER, OperandType::REGISTER, OperandType::IMMEDIATE},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"SR1", 6, 3},
                    {"IMM5", 0, 5}
                }
            }
        }
    };
    
    // NOT: 操作码 1001
    // 格式: [OPCODE(4)|DR(3)|SR(3)|111111]
    instructions["NOT"] = {
        "NOT",
        0x9,
        {
            {
                2,
                {OperandType::REGISTER, OperandType::REGISTER},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"SR", 6, 3}
                }
            }
        }
    };
    
    // LD: 操作码 0010
    // 格式: [OPCODE(4)|DR(3)|OFFSET9(9)]
    instructions["LD"] = {
        "LD",
        0x2,
        {
            {
                2,
                {OperandType::REGISTER, OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };

    // LEA: 操作码 1110
    // 格式: [OPCODE(4)|DR(3)|OFFSET9(9)]
    instructions["LEA"] = {
        "LEA",
        0xE,
        {
            {
                2,
                {OperandType::REGISTER, OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    // LDR: 操作码 0110
    // 格式: [OPCODE(4)|DR(3)|BaseR(3)|OFFSET6(6)]
    instructions["LDR"] = {
        "LDR",
        0x6,
        {
            {
                3,
                {OperandType::REGISTER, OperandType::REGISTER, OperandType::IMMEDIATE},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"BaseR", 6, 3},
                    {"OFFSET6", 0, 6}
                }
            }
        }
    };

    // LDI: 操作码 1010
    // 格式: [OPCODE(4)|DR(3)|OFFSET9(9)]
    instructions["LDI"] = {
        "LDI",
        0xA,
        {
            {
                2,
                {OperandType::REGISTER, OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"DR", 9, 3},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };

    // ST: 操作码 0011
    // 格式: [OPCODE(4)|SR(3)|OFFSET9(9)]
    instructions["ST"] = {
        "ST",
        0x3,
        {
            {
                2,
                {OperandType::REGISTER, OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"SR", 9, 3},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    // STR: 操作码 0111
    // 格式: [OPCODE(4)|SR(3)|BaseR(3)|OFFSET6(6)]
    instructions["STR"] = {
        "STR",
        0x7,
        {
            {
                3,
                {OperandType::REGISTER, OperandType::REGISTER, OperandType::IMMEDIATE},
                {
                    {"OPCODE", 12, 4},
                    {"SR", 9, 3},
                    {"BaseR", 6, 3},
                    {"OFFSET6", 0, 6}
                }
            }
        }
    };

    // STI: 操作码 1011
    // 格式: [OPCODE(4)|SR(3)|OFFSET9(9)]
    instructions["STI"] = {
        "STI",
        0xB,
        {
            {
                2,
                {OperandType::REGISTER, OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"SR", 9, 3},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    // BR: 操作码 0000 (条件分支)
    // 格式: [OPCODE(4)|N,Z,P(3)|OFFSET9(9)]
    instructions["BR"] = {
        "BR",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRN"] = {
        "BRn",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRZ"] = {
        "BRz",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRP"] = {
        "BRp",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRNZ"] = {
        "BRnz",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRNP"] = {
        "BRnp",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRZP"] = {
        "BRzp",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    instructions["BRNZP"] = {
        "BRnzp",
        0x0,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET9", 0, 9}
                }
            }
        }
    };
    
    // JSR: 操作码 0100
    // 格式: [OPCODE(4)|1|11|OFFSET11(11)]
    instructions["JSR"] = {
        "JSR",
        0x4,
        {
            {
                1,
                {OperandType::OFFSET},
                {
                    {"OPCODE", 12, 4},
                    {"OFFSET11", 0, 11}
                }
            }
        }
    };
    
    // JSRR: 操作码 0100
    // 格式: [OPCODE(4)|0|00|BaseR(3)|000000]
    instructions["JSRR"] = {
        "JSRR",
        0x4,
        {
            {
                1,
                {OperandType::REGISTER},
                {
                    {"OPCODE", 12, 4},
                    {"BaseR", 6, 3}
                }
            }
        }
    };
    
    // RET: 操作码 1100
    // 格式: [OPCODE(4)|000|111|000000]（实际是JMP R7）
    instructions["RET"] = {
        "RET",
        0xC,
        {
            {
                0,
                {},
                {
                    {"OPCODE", 12, 4}
                }
            }
        }
    };

    // JMP: 操作码 1100
    // 格式: [OPCODE(4)|000|BaseR(3)|000000]
    instructions["JMP"] = {
        "JMP",
        0xC,
        {
            {
                1,
                {OperandType::REGISTER},
                {
                    {"OPCODE", 12, 4},
                    {"BaseR", 6, 3}
                }
            }
        }
    };
    
    // TRAP: 操作码 1111
    // 格式: [OPCODE(4)|0000|TRAPVECT(8)]
    instructions["TRAP"] = {
        "TRAP",
        0xF,
        {
            {
                1,
                {OperandType::IMMEDIATE},
                {
                    {"OPCODE", 12, 4},
                    {"TRAPVECT", 0, 8}
                }
            }
        }
    };
    
    // HALT: 伪指令，实际是 TRAP x25
    instructions["HALT"] = {
        "HALT",
        0xF,
        {
            {
                0,
                {},
                {
                    {"OPCODE", 12, 4},
                    {"TRAPVECT", 0, 8}
                }
            }
        }
    };

    //PUTS: 伪指令，实际是 TRAP x22
    instructions["PUTS"] = {
        "PUTS",
        0xF,
        {
            {
                0,
                {},
                {
                    {"OPCODE", 12, 4},
                    {"TRAPVECT", 0, 8}
                }
            }
        }
    };
    
    // RTI: 操作码 1000
    // 格式: [OPCODE(4)|000000000000]
    instructions["RTI"] = {
        "RTI",
        0x8,
        {
            {
                0,
                {},
                {
                    {"OPCODE", 12, 4}
                }
            }
        }
    };
}

void InstructionSet::initializePseudoOps() {
    pseudoOps[".ORIG"] = true;
    pseudoOps[".END"] = true;
    pseudoOps[".FILL"] = true;
    pseudoOps[".BLKW"] = true;
    pseudoOps[".STRINGZ"] = true;
}

bool InstructionSet::isInstruction(const std::string &word) const {
    return instructions.find(word) != instructions.end();
}

bool InstructionSet::isPseudoOp(const std::string &word) const {
    return pseudoOps.find(word) != pseudoOps.end();
}

const InstructionInfo* InstructionSet::getInstructionInfo(const std::string &mnemonic) const {
    auto it = instructions.find(mnemonic);
    if (it != instructions.end()) {
        return &it->second;
    }
    return nullptr;
}

const OperandMode* InstructionSet::getOperandMode(const std::string &mnemonic,
                                                          const std::vector<OperandType>& operandTypes) const {
    const InstructionInfo* info = getInstructionInfo(mnemonic);
    if (info != nullptr) {
        return info->findMode(operandTypes);
    }
    return nullptr;
}

int InstructionSet::getOpcode(const std::string &mnemonic) const {
    const InstructionInfo* info = getInstructionInfo(mnemonic);
    if (info != nullptr) {
        return info->opcode;
    }
    return -1;
}

const BitField* InstructionSet::getBitField(const OperandMode* mode, const std::string& fieldName) const {
    if (mode == nullptr) return nullptr;
    
    for (const auto& field : mode->bitFields) {
        if (field.name == fieldName) {
            return &field;
        }
    }
    return nullptr;
}