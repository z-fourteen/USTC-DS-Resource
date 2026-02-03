# LC-3 汇编器输出对比脚本
# 使用方法: powershell -ExecutionPolicy Bypass -File compare_results.ps1

$ErrorActionPreference = "Continue"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$resultDir = Join-Path $scriptDir "result"
$answerDir = Join-Path $scriptDir "test_case" "bin"

Write-Host "🔍 LC-3 汇编器输出对比工具`n" -ForegroundColor Cyan

# 验证目录
if (-not (Test-Path $resultDir)) {
    Write-Host "❌ 错误: 找不到 result 目录: $resultDir" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path $answerDir)) {
    Write-Host "❌ 错误: 找不到答案目录: $answerDir" -ForegroundColor Red
    exit 1
}

# 获取所有 .obj 文件
$objFiles = Get-ChildItem -Path $resultDir -Filter "*.obj" | Sort-Object Name

if ($objFiles.Count -eq 0) {
    Write-Host "❌ 在 $resultDir 中没有找到 .obj 文件" -ForegroundColor Red
    exit 1
}

Write-Host "📁 找到 $($objFiles.Count) 个输出文件`n"
Write-Host ("=" * 80)

$totalFiles = 0
$passedFiles = 0
$failedFiles = 0

foreach ($objFile in $objFiles) {
    $totalFiles++
    $objName = $objFile.BaseName  # 文件名不含扩展名
    $answerFile = Join-Path $answerDir "$objName.asm.bin"
    
    if (-not (Test-Path $answerFile)) {
        Write-Host "⚠️  $($objFile.Name): 未找到对应的答案文件 $objName.asm.bin" -ForegroundColor Yellow
        $failedFiles++
        continue
    }
    
    # 读取文件
    try {
        $resultLines = @(Get-Content -Path $objFile.FullName)
        $answerLines = @(Get-Content -Path $answerFile)
    }
    catch {
        Write-Host "❌ $($objFile.Name): 读取文件失败 - $_" -ForegroundColor Red
        $failedFiles++
        continue
    }
    
    # 对比行数
    if ($resultLines.Count -ne $answerLines.Count) {
        Write-Host "❌ $($objFile.Name): 行数不匹配" -ForegroundColor Red
        Write-Host "   输出行数: $($resultLines.Count), 答案行数: $($answerLines.Count)"
        $failedFiles++
        continue
    }
    
    # 逐行对比
    $mismatchCount = 0
    $firstMismatchLine = -1
    
    for ($i = 0; $i -lt $resultLines.Count; $i++) {
        if ($resultLines[$i] -ne $answerLines[$i]) {
            $mismatchCount++
            if ($firstMismatchLine -eq -1) {
                $firstMismatchLine = $i + 1  # 行号从1开始
            }
        }
    }
    
    if ($mismatchCount -eq 0) {
        Write-Host "✅ $($objFile.Name): 完全匹配 ($($resultLines.Count) 行)" -ForegroundColor Green
        $passedFiles++
    }
    else {
        Write-Host "❌ $($objFile.Name): 有 $mismatchCount 行不匹配 (共 $($resultLines.Count) 行)" -ForegroundColor Red
        Write-Host "   第一个不匹配在第 $firstMismatchLine 行:"
        
        # 显示第一个不匹配的行
        $idx = $firstMismatchLine - 1
        Write-Host "   您的输出: $($resultLines[$idx])" -ForegroundColor Yellow
        Write-Host "   标准答案: $($answerLines[$idx])" -ForegroundColor Yellow
        
        # 计算二进制差异
        if ($resultLines[$idx].Length -eq 16 -and $answerLines[$idx].Length -eq 16) {
            try {
                $resultVal = [Convert]::ToInt32($resultLines[$idx], 2)
                $answerVal = [Convert]::ToInt32($answerLines[$idx], 2)
                $xorVal = $resultVal -bxor $answerVal
                Write-Host "   差异位置: 0x$($xorVal.ToString('X4')) (十六进制)"
            }
            catch { }
        }
        
        $failedFiles++
    }
    
    Write-Host ""
}

# 打印总结
Write-Host ("=" * 80)
Write-Host "`n📊 总结:" -ForegroundColor Cyan
Write-Host "   总文件数: $totalFiles"
Write-Host "   ✅ 通过: $passedFiles" -ForegroundColor Green
Write-Host "   ❌ 失败: $failedFiles" -ForegroundColor Red
Write-Host "   成功率: $(([math]::Round(100 * $passedFiles / $totalFiles, 1)))%"

if ($passedFiles -eq $totalFiles) {
    exit 0
}
else {
    exit 1
}
