#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from pathlib import Path

def compare_files(result_dir, answer_dir):
    """
    对比 result 文件夹中的输出和 test_case/bin 中的标准答案
    """
    result_path = Path(result_dir)
    answer_path = Path(answer_dir)
    
    # 获取所有的 .obj 文件
    obj_files = sorted(result_path.glob("*.obj"))
    
    if not obj_files:
        print(f"❌ 在 {result_dir} 中没有找到 .obj 文件")
        return
    
    print(f"📁 找到 {len(obj_files)} 个输出文件\n")
    print("=" * 80)
    
    total_files = 0
    passed_files = 0
    failed_files = 0
    
    for obj_file in obj_files:
        total_files += 1
        obj_name = obj_file.stem  # 获取文件名不含扩展名
        
        # 查找对应的标准答案文件
        # fibonacci.obj -> fibonacci.asm.bin
        answer_file = answer_path / f"{obj_name}.asm.bin"
        
        if not answer_file.exists():
            print(f"⚠️  {obj_name}.obj: 未找到对应的答案文件 {obj_name}.asm.bin")
            failed_files += 1
            continue
        
        # 读取两个文件
        try:
            with open(obj_file, 'r', encoding='utf-8') as f:
                result_lines = [line.rstrip('\n') for line in f.readlines()]
            
            with open(answer_file, 'r', encoding='utf-8') as f:
                answer_lines = [line.rstrip('\n') for line in f.readlines()]
        except Exception as e:
            print(f"❌ {obj_name}.obj: 读取文件失败 - {e}")
            failed_files += 1
            continue
        
        # 对比行数
        if len(result_lines) != len(answer_lines):
            print(f"❌ {obj_name}.obj: 行数不匹配")
            print(f"   输出行数: {len(result_lines)}, 答案行数: {len(answer_lines)}")
            failed_files += 1
            continue
        
        # 逐行对比
        mismatch_count = 0
        first_mismatch_line = -1
        
        for i, (result_line, answer_line) in enumerate(zip(result_lines, answer_lines)):
            if result_line != answer_line:
                mismatch_count += 1
                if first_mismatch_line == -1:
                    first_mismatch_line = i + 1  # 行号从1开始
        
        if mismatch_count == 0:
            print(f"✅ {obj_name}.obj: 完全匹配 ({len(result_lines)} 行)")
            passed_files += 1
        else:
            print(f"❌ {obj_name}.obj: 有 {mismatch_count} 行不匹配 (共 {len(result_lines)} 行)")
            print(f"   第一个不匹配在第 {first_mismatch_line} 行:")
            
            # 显示第一个不匹配的行
            idx = first_mismatch_line - 1
            print(f"   您的输出: {result_lines[idx]}")
            print(f"   标准答案: {answer_lines[idx]}")
            
            # 计算二进制差异
            if len(result_lines[idx]) == 16 and len(answer_lines[idx]) == 16:
                try:
                    result_val = int(result_lines[idx], 2)
                    answer_val = int(answer_lines[idx], 2)
                    xor_val = result_val ^ answer_val
                    print(f"   差异位置: 0x{xor_val:04X} (十六进制)")
                except:
                    pass
            
            failed_files += 1
        
        print()
    
    # 打印总结
    print("=" * 80)
    print(f"\n📊 总结:")
    print(f"   总文件数: {total_files}")
    print(f"   ✅ 通过: {passed_files}")
    print(f"   ❌ 失败: {failed_files}")
    print(f"   成功率: {100 * passed_files / total_files:.1f}%")
    
    return passed_files == total_files


def main():
    # 设置路径
    script_dir = Path(__file__).parent
    result_dir = script_dir / "result"
    answer_dir = script_dir / "test_case" / "bin"
    
    print("🔍 LC-3 汇编器输出对比工具\n")
    
    # 验证目录存在
    if not result_dir.exists():
        print(f"❌ 错误: 找不到 result 目录: {result_dir}")
        sys.exit(1)
    
    if not answer_dir.exists():
        print(f"❌ 错误: 找不到答案目录: {answer_dir}")
        sys.exit(1)
    
    # 执行对比
    success = compare_files(result_dir, answer_dir)
    
    # 返回退出码
    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
