#define _CRT_SECURE_NO_WARNINGS

#include "FindFiles.h"

#include <iostream>
#include <algorithm>

using namespace std;

#ifdef _WIN32

#include <Windows.h>
//#include <direct.h>

std::vector<std::tstring> FindFiles::findFiles(std::tstring lpPath, std::vector<std::tstring> dotextNames, bool enterSubFolder)
{
	tstring szFile;
    WIN32_FIND_DATA FindFileData;

	tstring szFind = lpPath + TEXT("\\*");

    HANDLE hFind=::FindFirstFile(szFind.c_str(),&FindFileData);

    if(INVALID_HANDLE_VALUE == hFind)
    {
        std::tcout << "Invalid folder:"<<szFind << std::endl;
        return std::vector<std::tstring>();
    }

    do
    {
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
			//是目录
            if(FindFileData.cFileName[0]!= TEXT('.'))//不是以.开头的目录
            {
                if (enterSubFolder)
                {
                    //进入子目录
                    szFile = lpPath + TEXT("\\") + FindFileData.cFileName;
                    findFiles(szFile, dotextNames);
                }
            }
        }
        else
        {
			//是文件
			std::tstring filePath = lpPath + TEXT("\\") + FindFileData.cFileName;

			if (dotextNames.empty())
			{
				file_lists.push_back(filePath);
			}
			else
			{
				auto tstolower = [](tstring s)->tstring
				{
					tstring temp(s);
					for_each(temp.begin(), temp.end(), [](TCHAR& c) {c = tolower(c); });
					return temp;
				};

				tstring filename(tstolower(FindFileData.cFileName));
				for (auto& dotext : dotextNames)
				{
                    if (filename.substr(filename.length() - dotext.length(), dotext.length()) == dotext)//文件名后n位和扩展名相同
                    {
						file_lists.push_back(filePath);
                    }
				}
			}
    //        if ( szFile.empty() )
    //        {
				////szFile为空，为当前目录
    //            std::string filePath = szFile+FindFileData.cFileName;
    //            file_lists.push_back(filePath);
    //        }
    //        else
    //        {
				////非当前目录，输出全路径
    //            std::string filePath = lpPath+"\\"+FindFileData.cFileName;
    //            file_lists.push_back(filePath);
    //        }
        }

    }while(::FindNextFile(hFind,&FindFileData));

    ::FindClose(hFind);
    return file_lists;
}

#else

#include <dirent.h>
#include <string.h>
#include <queue>

std::vector<std::string> FindFiles::findFiles( const char *lpPath, const char *secName /*= ".*" */ )
{
    (void)secName;

    std::vector<std::string> result;
    std::queue<std::string> queue;
    std::string dirname;

    DIR *dir;
    if ( !(dir = opendir ( lpPath )) ) {
        return result;
    }
    queue.push( lpPath );

    struct dirent *ent;
    while ( !queue.empty() )
    {

        dirname = queue.front();
        dir = opendir( dirname.c_str() );
        queue.pop();
        if ( !dir ) { continue; }

        while( ent = readdir( dir ) )
        {

            if ( strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0 )
            {
                continue;
            }
            if ( ent->d_type == DT_DIR )
            {
                queue.push( dirname+"/"+ent->d_name );
            }
            else
            {
                result.push_back( dirname+"/"+ent->d_name );
            }

        }

        closedir( dir );

    }

    return result;
}

#endif
