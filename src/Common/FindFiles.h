/*
 *   Find and generate a file list of the folder.
**/
#pragma once

#include <vector>
#include <tstring.h>

class FindFiles
{
public:
    FindFiles(){}
    ~FindFiles(){}

    //存在递归调用，所以需要函数作用域之外的变量
	std::vector<std::tstring> findFiles(std::tstring lpPath, std::vector<std::tstring> dotextNames = {},bool enterSubFolder=true);

private:
    std::vector<std::tstring> file_lists;
};
