/*
node this sourcode can detect the funtion at type below:
<kind of funtion> functionName(list of args);
 <kind of funtion> functionName(list of
                                 args
                                    ...);
file apcept type: hpp, h and cpp.
*/
#ifdef __DEBUG__
#ifndef __LOG__
#define __LOG__(data,file,comment)\
    bug.open(file,ios::app);\
    bug<<"at LINE "<<__LINE__<<":: variable= "<<#data<< "| value="<<data;\
    bug<<"     ---"<<"comment :"<<comment<<"---"<<std::endl;\
    bug.close();
#endif
#else
#ifndef __LOG__
#define __LOG__(data,file,comment)
#endif
#endif

#ifndef _MakeMockFile_H
#define _MakeMockFile_H
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

struct BracketInfor_
{
    std::vector<int> rememberLevel;
    std::vector<int> pos;
    int pop=0; //second bracket
};
typedef BracketInfor_ BracketInfor;

string getstring(string line, int first, int endpos);
int findstring(string searchregion, string besearch);
class makeMockFile
{
private:
    fstream fb;
    fstream fbout;
    fstream fbout_final;
    fstream fbUTtest;
    fstream bug;

    string checkLine;
    string classname;
    string namefunction;
    string typeofunction;
    string containinbreaket;
    string filenameout;

    string readname;
    string filenameNotype;
    string filename;
    string UTtestname;

    string args;
    string OrginalClass;
    string filetype;
    string log;
    string AfterSecondBracket;

    bool belongtoAclass=0;
    bool isfunction=1;
    bool isvalidTypeofFunction=1;
    bool ismakedUTSheader=0;
    bool isvoid = 0;
    bool isCorrectFileType=0;
    bool method0=0;
    int comman;
    int countBracketIn;
    int countBracketOut;
    int countBracketOfClassIn;
    int countBracketOfClassOut;
    int count_function;


public:
    makeMockFile(string filename, string outpath);
    makeMockFile(string filename);
    ~makeMockFile(){}

    void readFilename();
    int checkisFunction(string line);
    int findletter(string line, char data, int frompos);
    int findPosIsnotletter(string line, char data);
    int checkHasLetter(string line, char letter, int frompos, int topos);
    int findPosIsnotletter(string line, char data, int frompos, int topos);
    int countLetter(string line, char data, int frompos, int topos);
    int * PoshasLetter(string line, char data, int frompos, int topos);
    void getagr();
    void checkvalidargs();
    int merfile();
    void write(string MOCname, string obj);
    void removefile(string filename);
    void removeOldfile(void);
    void makeFileHeader();
    void getOrginalclassName(string line);
    void checkvalidClassname();
    void checktypeofFuntion();
    void makeHeaderfileForUTscript();
    void writeUTtest();
    bool iscorrectfiletype();
    int countBracket(string line, char typeofBracket);
    void deleteSpace(string &str);
    void deleteSpaceBtwWords(string &str);
    void CheckValidStringAfterSecondBracket();
    string deleteAllBracketAfter1st(string line) ;
    bool checkContainInBracket();

};

#endif


