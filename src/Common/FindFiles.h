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

    //���ڵݹ���ã�������Ҫ����������֮��ı���
	std::vector<std::tstring> findFiles(std::tstring lpPath, std::vector<std::tstring> dotextNames = {},bool enterSubFolder=true);

private:
    std::vector<std::tstring> file_lists;
};
