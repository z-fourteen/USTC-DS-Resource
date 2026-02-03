#include "Utils.h"

/* 功能：读取整个文件的内容并返回为字符串 */
std::string Utils::readFile(const std::string &filePath)
{
    /* 打开文件 */
    // FILE *file = fopen(filePath.c_str(), "r");
    FILE *file = fopen(filePath.c_str(), "rb");
    if (!file)
    {
        return "";
    }

    /* 计算文件大小 */
    fseek(file, 0, SEEK_END);
    long length = ftell(file);

    /* 读取文件内容 */
    fseek(file, 0, SEEK_SET);
    std::string content(length, '\0');
    fread(&content[0], 1, length, file);
    fclose(file);
    return content;
}

/* 功能：将给定内容写入文件 */
bool Utils::writeFile(const std::string &filePath, const std::string &content)
{
    FILE *file = fopen(filePath.c_str(), "w");
    if (!file)
    {
        return false;
    }
    fwrite(content.c_str(), 1, content.size(), file);
    fclose(file);
    return true;
}