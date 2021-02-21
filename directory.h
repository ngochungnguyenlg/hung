#ifndef _DIRECTORY_H
#define _DIRECTORY_H


#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include "MakeMockFile.h"


#ifndef __LOG__
#define __LOG__(data,file,comment)\
    bug.open(file,std::ios::app);\
    bug<<"at LINE "<<__LINE__<<":: variable= "<<#data<< "| value="<<data;\
    bug<<"     ---"<<"comment :"<<comment<<"---"<<std::endl;\
    bug.close();
#endif
class directory
{

    std::string path;
    DIR *dir;
    std::string log="log.txt";
    std::fstream bug;
    vector<string> listFileName;
    vector<string> choosenList;
    // makeMockFile *mock_;
public:
    void showfile();
    directory(std::string path)
    {
        this->path = path;
        bug.open(log.c_str(), std::ios::out);
    }
    bool isDir(std::string path);
    void listdir(const char *name, int indent);
    void genmockfile(string namefile);
    void getlistNeedGenMockFile(string * p, size_t size, string type);
    void dosomething();
    ~directory(){}

};


#endif
