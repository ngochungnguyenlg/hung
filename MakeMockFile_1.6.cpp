#include "MakeMockFile.h"
#include "gmockgen.h"

#include <QDir>
#include <QString>
#include <QtDebug>

QString outDir;
std::vector<std::string> exceptionFlags;
BracketInfor checkLevelOfBracket(string str)
{
    BracketInfor ret;
    vector<int> rememberLevel;
    vector<int> pos;
    char previosBracket = '0';
    size_t str_len = str.length();
    bool isFoundFirstBracket = 0;
    int level = 0;
    size_t lenVector;

    for (size_t i = 0; i < str_len; i++)
    {
        if (str[i] == '(' || str[i] == ')')
        {
            if (str[i] == '(' && isFoundFirstBracket == 0)
            {
                isFoundFirstBracket = 1;
                level += 1;
                rememberLevel.push_back(level);
                previosBracket = str[i];
                pos.push_back(i);
                continue;
            }
            if (str[i] == previosBracket && isFoundFirstBracket)
            {
                if (str[i] == '(')
                {
                    level += 1;
                    rememberLevel.push_back(level);
                    previosBracket = str[i];
                    pos.push_back(i);
                }
                else
                {
                    level -= 1;
                    rememberLevel.push_back(level);
                    previosBracket = str[i];
                    pos.push_back(i);
                }
            }
            if (str[i] != previosBracket && isFoundFirstBracket)
            {
                rememberLevel.push_back(level);
                previosBracket = str[i];
                pos.push_back(i);
            }
            lenVector = rememberLevel.size();
            if (rememberLevel[0] == rememberLevel[lenVector - 1])
            {
                break;
            }
        }
    }
    ret.pos=pos;
    ret.rememberLevel=rememberLevel;
    return ret;
}

int countNo(vector<int> data, int number)
{
    size_t len=data.size();
    int count=0;
    for(size_t i=0; i<len; i++)
    {
        if(data[i]==number)
        {
            ++count;
        }
    }
    return count;
}
string makeMockFile::deleteAllBracketAfter1st(string line) const
{
    string line_tmp=line;
    BracketInfor brackinfor= checkLevelOfBracket(line_tmp);
    int counNoTwo=countNo(brackinfor.rememberLevel,2);
    size_t sizeOfAllbracke=brackinfor.rememberLevel.size();

    if(counNoTwo!=0 && brackinfor.rememberLevel[0]==brackinfor.rememberLevel[sizeOfAllbracke-1])
    {
        int firstTwo=0;
        int secondTwo;
        for(size_t i=0; i<sizeOfAllbracke; i++)
        {
            if(firstTwo==0 && brackinfor.rememberLevel[i]==2)
            {
                firstTwo=brackinfor.pos[i];
                continue;
            }
            if(brackinfor.rememberLevel[i]==2&&firstTwo!=0)
            {
                cout << "run"<<endl;
                cout << secondTwo<<endl;
                secondTwo=brackinfor.pos[i];
                cout << secondTwo<<endl;
                for(int j=firstTwo; j<secondTwo+1; j++)
                {
                    line_tmp[j]=' ';
                }
                firstTwo=0;
                secondTwo=0;
            }
        }
    }
    return line_tmp;
}

string getstring(string line, int first, int endpos)
{
    string outline;
    if (line.length() < static_cast<size_t>(endpos))
        endpos = line.length();
    if (0 > first)
        first = 0;
    for (int i = first; i < endpos; i++)
    {
        outline += line[i];
    }
    if (first == endpos)
        outline += line[first];
    return outline;
}
int findstring(string searchregion, string besearch)
{
    int pos = 0;
    int pos_letter = 0;
    int len = besearch.length();
    int lensearhzone = searchregion.length();
    while (1)
    {
        if (searchregion[pos] == besearch[pos_letter])
        {
            pos += 1;
            pos_letter += 1;
        }
        else
        {
            pos += 1;
            pos_letter = 0;
        }
        if (pos_letter == len)
            return true;
        else if (pos + 1 == lensearhzone)
            return false;
    }
    return false;
}
makeMockFile::makeMockFile(string filename)
    : makeMockFile(filename, outDir.toStdString())
{
}

makeMockFile::makeMockFile(string filename, string outpath)
{

    int startNamefile;
    string folder("MockFiles");
    if (filename.find('\\') != std::string::npos)
    {
        int len = filename.length();
        for (int i = len; i > 0; i--)
        {

            if (filename[i] == '\\')
            {
                startNamefile = i;
                break;
            }
        }
        bool iscolon = 0;
        for (size_t i = 0; i < filename.length(); i++)
        {
            if (filename[i] == ':')
            {
                folder = folder + getstring(filename, i + 1, startNamefile);
                iscolon = 1;
                break;
            }
        }
        if (!iscolon)
        {
            folder = folder + getstring(filename, 0, startNamefile);
        }
        this->filename = getstring(filename, startNamefile + 1, len);
        this->readname = filename;
    }
    else
    {
        this->readname = filename;
        this->filename = filename;
    }

    int finddot = 0;
    for (size_t i = 0; i < this->filename.length(); i++)
    {
        if (this->filename[i] == '.')
        {
            finddot = i;
            break;
        }
    }
    if (finddot == -1)
        return;
    QDir dircreator(QString(outpath.c_str()));
    QString folders(folder.c_str());
    dircreator.mkpath(folders);
    dircreator.mkdir("Log");
    dircreator.mkdir("TestScripts");

    this->filetype = getstring(this->filename, finddot, this->filename.length());
    if (filetype == ".cpp" || filetype == ".hpp" || filetype == ".CPP" || filetype == ".HPP")
    {
        this->filenameout = outpath + "\\" + folder + "\\" + getstring(this->filename, 0, this->filename.length() - 4) + "_mock.h";
        this->filenameNotype = getstring(this->filename, 0, this->filename.length() - 4);
        this->isCorrectFileType = 1;
    }
    else if (filetype == ".h" || filetype == ".c" || filetype == ".H" || filetype == ".C")
    {
        this->filenameout = outpath + "\\" + folder + +"\\" + getstring(this->filename, 0, this->filename.length() - 2) + "_mock.h";
        this->filenameNotype = getstring(this->filename, 0, this->filename.length() - 2);
        this->isCorrectFileType = 1;
    }

    this->classname = "Mock" + filenameNotype;
    this->UTtestname = outpath + "/TestScripts/UT_GMOCK_" + filenameNotype + ".cpp";

    this->log = outpath + "/Log/" + filenameNotype + "Log.csv";
    this->bug.open(log, ios::out);
    this->bug.close();
    this->OrginalClass=filenameNotype;
    this->countBracketOfClassIn=0;
    this->countBracketOfClassOut=0;
    this->count_function=0;
    this->AfterSecondBracket=" ";
    this->method0=0;
    __LOG__(filenameout, log, "create filenameout");
}
bool makeMockFile::iscorrectfiletype()
{
    return isCorrectFileType;
}
int makeMockFile::findletter(string line, char data, int frompos)
{
    int len = line.length();
    for (int i = frompos; i < len; i++)
    {
        if (line[i] == data)
            return i;
    }
    return -1;
}
int makeMockFile::findPosIsnotletter(string line, char data)
{
    int len = line.length();
    for (int i = 0; i < len; i++)
    {
        if (line[i] != data)
            return i;
    }
    return -1;
}
int makeMockFile::findPosIsnotletter(string line, char letter, int frompos, int topos)
{
    int len = line.length();
    if (topos > len)
        topos = len;
    for (int i = frompos; i < topos; i++)
    {
        if (line[i] != letter)
            return i;
    }
    return -1;
}
int makeMockFile::checkHasLetter(string line, char letter, int frompos, int topos)
{
    int len = line.length();
    if (topos > len)
        topos = len;
    for (int i = frompos; i < topos; i++)
    {
        if (line[i] == letter)
            return i;
    }
    return -1;
}
int makeMockFile::countLetter(string line, char letter, int frompos, int topos)
{
    int count = 0;
    for (int i = frompos; i < topos; i++)
    {
        if (line[i] == letter)
            count += 1;
    }
    return count;
}
int *makeMockFile::PoshasLetter(string line, char letter, int frompos, int topos)
{
    int count = this->countLetter(line, letter, frompos, topos);
    int *ret = new int[count + 1];
    int has = 0;
    for (int i = frompos; i < topos; i++)
    {
        if (line[i] == letter)
        {
            ret[has] = i;
            has += 1;
        }
    }
    if (has == 0)
        return NULL;
    return ret;
}

void makeMockFile::getOrginalclassName(string line)
{
    if (this->filetype == ".h" || this->filetype == ".H" || this->filetype == ".hpp" || this->filetype == ".HPP")
    {
        int countBracketIn=this->countBracket(line,'}');
        int countBracketOut=this->countBracket(line,'{');
        if (this->belongtoAclass == 1 && this->countBracketOfClassIn==this->countBracketOfClassOut &&this->countBracketOfClassIn!=0)
        {
            this->belongtoAclass = 0;
            this->OrginalClass = filenameNotype;
            this->countBracketOfClassIn=0;
            this->countBracketOfClassOut=0;
        }
        if (line.find("class") != std::string::npos || line.find("struct") != std::string::npos)
        {

            if (line.find("struct") != std::string::npos)
            {
            }
            int firstletter;
            if (line[0] != ' ')
                firstletter = 0;
            else
                firstletter = this->findPosIsnotletter(line, ' ', 0, line.length());
            int isspace = this->checkHasLetter(line, ' ', firstletter, line.length());
            int startClassname = this->findPosIsnotletter(line, ' ', isspace, line.length());
            int checkhasColon = this->checkHasLetter(line, ':', startClassname, line.length());
            int checkhasBrackets = this->checkHasLetter(line, '{', startClassname, line.length());
            string invalid = "/.()=?*&";
            if (line.find("struct") != std::string::npos)
            {
                for (size_t i = 0; i < line.length(); i++)
                    if (this->checkHasLetter(invalid, line[i], 0, invalid.length()) != -1)
                    {
                        __LOG__(line, log, "invalid type name of struct");
                        return;
                    }
            }

            if (isspace == -1 || (getstring(line, firstletter, firstletter + 5) != "class" && getstring(line, firstletter, firstletter + 6) != "struct"))
            {
                return;
            }

            if (checkhasColon == -1 && checkhasBrackets == -1)
            {

                this->OrginalClass = getstring(line, startClassname, line.length());
                this->belongtoAclass = 1;
                this->countBracketOfClassIn+=countBracketIn;
                this->countBracketOfClassOut+=countBracketOut;
                __LOG__(line, log, "get struct name suscess");
                return;
            }
            else if (checkhasColon != -1)
            {

                this->OrginalClass = getstring(line, startClassname, checkhasColon);
                this->belongtoAclass = 1;
                __LOG__(line, log, "get struct name suscess");
                this->countBracketOfClassIn+=countBracketIn;
                this->countBracketOfClassOut+=countBracketOut;
                return;
            }
            else if (checkhasColon == -1 && checkhasBrackets != -1)
            {

                this->OrginalClass = getstring(line, startClassname, checkhasBrackets);
                this->belongtoAclass = 1;
                __LOG__(line, log, "get struct name suscess");
                this->countBracketOfClassIn+=countBracketIn;
                this->countBracketOfClassOut+=countBracketOut;
                return;
            }
        }
        else if (this->isfunction == 1 && this->belongtoAclass != 1)
        {
            this->OrginalClass = this->filenameNotype;
            this->belongtoAclass = 0;
            this->countBracketOfClassIn=0;
            this->countBracketOfClassOut=0;
        }
    }
    if (this->filetype == ".cpp" || this->filetype == ".CPP")
    {
        this->OrginalClass = this->filenameNotype;
        int firstBrackets = this->checkHasLetter(line, '(', 0, line.length());
        int startfunctionname = 0;
        int startclassname;
        bool isletter = 0;
        for (int i = firstBrackets; i > 0; i--)
        {

            if (line[i] == ':' && line[i - 1] == ':' && startfunctionname == 0)
            {
                startfunctionname = i;
            }
            if (startfunctionname != 0)
            {

                if (line[i] != ' ' && isletter == 0)
                {
                    isletter = 1;
                }
                if (isletter == 1 && line[i] == ' ')
                {
                    if (line[i + 1] == '*' || line[i + 1] == '&')
                    {
                        line[i + 1] = ' ';
                    }
                    startclassname = i;
                    this->OrginalClass = getstring(line, startclassname, startfunctionname - 1);
                    this->belongtoAclass = 1;
                    isletter = 0;
                    __LOG__(line, log, "get OrginalClass name suscess");
                    break;
                }
            }
            else
            {
                this->belongtoAclass = 0;
                this->OrginalClass = this->filenameNotype;

            }
        }
    }
    if (this->filetype == ".c")
    {
        this->OrginalClass = this->filenameNotype;
        this->belongtoAclass = 0;
    }
}

void makeMockFile::checkvalidClassname()
{
    string invalid = "-=<>/\\';^%$#@#!~";
    if (this->checkHasLetter(invalid, this->OrginalClass[0], 0, invalid.length()) != -1)
    {
        this->OrginalClass[0] = ' ';
    }
    bool isnotSpace = 0;
    for (size_t i = 0; i < this->OrginalClass.length(); i++)
    {
        if (OrginalClass[i] != ' ' && isnotSpace == 0)
        {
            isnotSpace = 1;
            if (this->checkHasLetter(invalid, this->OrginalClass[i], 0, invalid.length()) != -1)
            {
                this->OrginalClass[i] = ' ';
            }
        }
        if (isnotSpace == 1)
        {
            if (this->checkHasLetter(invalid, this->OrginalClass[i], 0, invalid.length()) != -1)
            {
                cout << "invalid class name" << endl;
                cout << "OrginalClass name="
                     << "filename" << endl;
                this->OrginalClass = this->filename;
            }
        }
    }
}

int makeMockFile::countBracket(string line, char typeofBracket)
{
    size_t len = line.length();
    int count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (line[i] == typeofBracket)
            ++count;
    }
    return count;
}

int makeMockFile::checkisFunction(string line)
{
    __LOG__("check is function is starting", log, "notify");
    __LOG__(line, log, "show value");
    string invalid1stletter = "//\\@,:1234567890'.,*&^^%$#@!~";
    string out;
    size_t _line_length = line.length();
    int isnotspace;
    this->countBracketOut = 0;
    this->countBracketIn = 0;
    if (line[0] == ' ')
    {
        isnotspace = findPosIsnotletter(line, ' ', 0, line.length());
        if (isnotspace == -1)
        {
            __LOG__("NullLINE", log, "ignore because line is empty!");
            this->isfunction = 0;
            return 0;
        }
    }
    else if (line[0] == 9)
    {
        int startbyatab = findPosIsnotletter(line, '\t', 0, line.length());
        isnotspace = findPosIsnotletter(line, ' ', startbyatab, line.length());
    }
    else
    {
        isnotspace = 0;
    }


    if(this->belongtoAclass)
    {
        this->countBracketOfClassIn+=this->countBracket(line,'}');
        this->countBracketOfClassOut+=this->countBracket(line,'{');
    }



    string virtual_ = getstring(line, isnotspace, isnotspace + 7);
    // if (checkHasLetter(line, '=', 0, line.length()) != -1 && virtual_ != "virtual")
    // {
    //     this->isfunction = 0;
    //     __LOG__(line,log);
    // }
    if (virtual_ == "virtual")
    {
        int ss = checkHasLetter(line, '=', 0, line.length());

        if (ss != -1){
            //            string tmpline=getstring(line,ss,line.length());
            int filebracketAfterEqualSignal=this->checkHasLetter(line,')',ss,line.length());
            if(filebracketAfterEqualSignal==-1){
                line = getstring(line, isnotspace + 7, ss);
                _line_length = line.length();

            }
        }
        else
        {
            line = getstring(line, isnotspace + 7, line.length());
            _line_length = line.length();
        }
        isnotspace = 0;
    }


    if (getstring(line, isnotspace, isnotspace + 4) == "void")
    {
        this->isvoid = 1;
        this->typeofunction = "void";
        bool IspaceAfterVoid=0;
        for(size_t i=isnotspace+5; i<_line_length; i++)
        {
            if(line[i]!=' ' && IspaceAfterVoid==0)
            {
                IspaceAfterVoid =1;
            }
            if(IspaceAfterVoid && (line[i]=='*'||line[i]=='&'))
            {
//                this->typeofunction+=line[i];
                this->isvoid = 0;
                break;
            }
        }
    }
    else
    {
        this->isvoid = 0;
    }

    if (line.find("/*") != string::npos && line.find("*/") == string::npos)
    {
        return 3;
    }
    if (findletter(invalid1stletter, line[isnotspace], 0) != -1)
    {
        this->isfunction = 0;
        __LOG__(line, log, "invalid line");
        return 0;
    }

    int firstSpace = findletter(line, ' ', isnotspace); //dau cach sau kieu cua ham

    int firstbreaket = findletter(line, '(', 0); // dau ngoac dau tien
    if(firstbreaket>0 &&(line[firstbreaket-1]=='\t'||line[firstbreaket-1]==' '))
    {   __LOG__(line[firstbreaket-1],log,"notify");
        __LOG__(line[firstbreaket],log,"notify");
        bool isSpace_checkSpaceBeforeBracket=0;
        for(size_t i=firstbreaket-1; i>0; i--)
        {
            if((line[i]==' '||line[firstbreaket-1]=='\t')&&isSpace_checkSpaceBeforeBracket==0)
            {
                isSpace_checkSpaceBeforeBracket=1;
            }
            if((line[i]!=' '&&line[i]!='\t') &&isSpace_checkSpaceBeforeBracket)

            {
                line.erase(line.begin()+i+1,line.begin()+firstbreaket);
                __LOG__(line,log,"show value after delete");
                _line_length=line.length();
                firstbreaket =findletter(line, '(', 0);
                break;
            }
        }
    }

    this->countBracketIn += this->countBracket(line, ')');
    this->countBracketOut += this->countBracket(line, '(');
    if (countBracketOut == 0)
    {
        this->isfunction = 0;
        __LOG__(line, log, "invalid line");
        return 0;
    }

    int letterAfter1stSpace = findPosIsnotletter(line, ' ', firstSpace + 1, firstbreaket); // after the first space cannot be "("

    if (firstSpace > firstbreaket || firstSpace == -1 ||
            letterAfter1stSpace == -1 || line[letterAfter1stSpace] == '(' || firstbreaket == -1)
    {
        this->isfunction = 0;
        __LOG__(line, log, "invalid line");
        return 0;
    }

    int startFunctionname = 0; //check the first letter by coundown from first "(" to the nearest space that is the function name.
    int checkispointer = 0;
    __LOG__(this->countBracketIn, log, "show value");
    __LOG__(this->countBracketOut, log, "show value");

    if (firstSpace != -1)
    {
        for (int i = firstbreaket; i > 0; i--)
        {
            if (line[i] == ' ')
            {
                startFunctionname = i;
                checkispointer = i + 1;
                break;
            }
        }
        if (!isvoid)
            this->typeofunction = getstring(line, isnotspace, startFunctionname);
        //getstring from start is not space to befor the startFunctionname that is fuction name.
    }

    int secondbreaket = 0;
    // finding the second the breaket.
    int checkHasSpecialBracket = this->checkHasLetter(line, '{', 0, _line_length);

    if (this->countBracketIn < this->countBracketOut)
    {
        //if has the first and no sencond breaket return 2 to merger this line with next line.
        __LOG__(line, log, "cannot find sencond bracket!");
        return 2;
    }
    else if (this->countBracketIn == this->countBracketOut)
    {
        size_t checkFrom__ = 0;
        if (checkHasSpecialBracket != -1)
        {
            checkFrom__ = checkHasSpecialBracket - 1;
        }
        else
        {
            checkFrom__ = _line_length;
        }

        for (size_t i = checkFrom__; i > 0; i--)
        {
            if (line[i] == ')')
            {
                __LOG__(line[i],log,"show value");
                secondbreaket = i;
                break;
            }
        }
    }
    if(secondbreaket!=0)
    {
        int isnotSpaceAfterSecondBracket=this->findPosIsnotletter(line,' ',secondbreaket+1,line.length());
        string line_PushToGetAfterBracket;
        if(checkHasSpecialBracket!=-1){
            line_PushToGetAfterBracket=getstring(line,0,checkHasSpecialBracket-1);
        }
        else
        {
            line_PushToGetAfterBracket=line;
        }
        if(isnotSpaceAfterSecondBracket!=-1)
        {
            if(line[isnotSpaceAfterSecondBracket]!=';')
            {
                int findComma=this->checkHasLetter(line_PushToGetAfterBracket,';',isnotSpaceAfterSecondBracket,line_PushToGetAfterBracket.length());
                if(findComma!=-1)
                    this->AfterSecondBracket=getstring(line_PushToGetAfterBracket,isnotSpaceAfterSecondBracket,findComma);
                else
                {
                    this->AfterSecondBracket=getstring(line_PushToGetAfterBracket,isnotSpaceAfterSecondBracket,line_PushToGetAfterBracket.length());
                }
            }

        }
    }
    __LOG__(secondbreaket, log, "show value");

    //change all the letter from 2nd level of bracket
    string lineTemp;
    lineTemp=this->deleteAllBracketAfter1st(line);
    __LOG__(lineTemp,log,"show value");
    //check has comma or not, after that check that is no arg or not.
    int checkComma = checkHasLetter(lineTemp, ',', firstbreaket, secondbreaket);
    int allspace = findPosIsnotletter(line, ' ', firstbreaket, secondbreaket);
    //separate the classname::functionname to get funtionname
    int checkiscppfile = checkHasLetter(line, ':', startFunctionname, firstbreaket);
    if (checkiscppfile == -1)
    {
        this->namefunction = getstring(line, startFunctionname, firstbreaket);
    }
    else
    {
        this->namefunction = getstring(line, checkiscppfile + 2, firstbreaket);
        // cout << LINE << "true" << endl;
    }

    //check valid function name
    string InvalidFC = "-<>//\\.,:;)(*&^%$#@!)\"~{}[]=";
    int fisnul = findPosIsnotletter(this->namefunction, ' ', 0, this->namefunction.length());

    if (fisnul == -1)
    {
        this->isfunction = 0;
        __LOG__(line, log, "function name is empty!");
        return 0;
    }
    for (size_t i = fisnul; i < this->namefunction.length(); i++)
    {
        int ss = this->checkHasLetter(InvalidFC, this->namefunction[i], 0, InvalidFC.length());

        if ((int)i == fisnul)
            if ((this->namefunction[fisnul] == '&' || this->namefunction[fisnul] == '*'))
            {
                this->namefunction[fisnul] = ' ';
                continue;
            }
        if (ss != -1)
        {
            this->isfunction = 0;
            __LOG__(this->namefunction, log, "Function name is invalid!");
            return 0;
        }
    }

    if (/*checkPointerPos != -1||*/ (line[checkispointer] == '&' || line[checkispointer] == '*') && fisnul != -1)
    {
        this->typeofunction += line[checkispointer];
        line[checkispointer] = ' ';
    }
    // check the structure in the () whether is list of args or not.
    if (checkComma != -1)
    {
        int check1stArg = findPosIsnotletter(line, ' ', firstbreaket + 1, checkComma);
        this->comman = this->countLetter(line, ',', firstbreaket, line.length());
        int checkhasSpaceFrom1stbreaketocomma = checkHasLetter(line, ' ', check1stArg, checkComma);

        if (firstbreaket != -1 && checkhasSpaceFrom1stbreaketocomma != -1)
        {
            this->containinbreaket = getstring(line, firstbreaket, secondbreaket + 1);
            this->isfunction = 1;

            return 1;
        }
    }
    else if (allspace != -1)
    {
        this->containinbreaket = getstring(line, firstbreaket, secondbreaket + 1);
//        __LOG__(line.length(),log,"show value");
//        __LOG__(line,log, "show value");
//        __LOG__(firstbreaket,log, "show value");
//        __LOG__(secondbreaket+1,log, "show value");
        if(firstbreaket+1==secondbreaket)
        {
            __LOG__(this->containinbreaket,log,"in bracket NULL");
        }
        if(firstbreaket+1<secondbreaket)
        {
            string containTmp=getstring(line,firstbreaket+1,secondbreaket);
            __LOG__(containTmp,log,"containTmp");
            this->deleteSpace(containTmp);
            if(this->checkHasLetter(containTmp,' ',0,containTmp.length())!=-1)
            {
                this->comman = 0;
                this->isfunction = 1;
                __LOG__(this->containinbreaket,log,"show value");
                return 1;
            }
            else
            {
                if(containTmp.compare("void")==0)
                {
                    this->comman = 0;
                    this->isfunction = 1;
//                    __LOG__(this->containinbreaket,log,"show value");
                    this->method0=1;
                    return 1;
                }
                __LOG__(this->containinbreaket,log,"show value");
                this->isfunction = 0;
                return 0;
            }
        }else
        {
            this->comman = 0;
            this->isfunction = 1;
            __LOG__(this->containinbreaket,log,"show value");
            return 1;
        }


    }
    //in the () is null
    else
    {
        //the other case
        int StartLetterAfterBreaket = findPosIsnotletter(line, ' ', firstbreaket, line.length());
        int ispace = 0;
        for (int i = StartLetterAfterBreaket; i < secondbreaket; i++)
        {
            if (line[i] == ' ')
                ispace = 1;
            if (ispace == 1 && line[i] != ' ')
            {
                this->containinbreaket = getstring(line, firstbreaket, secondbreaket + 1);
                this->comman = 1;
                this->isfunction = 1;

                return 1;
            }
        }
    }
    this->isfunction = 0;
    __LOG__(line, log, "cannot check it is function is not!");

    return 0;
}
void makeMockFile::getagr()
{
    // get the name of args separate with it's type
    // ex: int a -> a
    // line_ =(int a) -> (a)
    string line_ = this->containinbreaket;
    // get list of all pos of comma
    string tmpLine=this->deleteAllBracketAfter1st(line_);
    int *posofcomma = this->PoshasLetter(line_, ',', 0, line_.length());
    int finalLetter = checkHasLetter(line_, ')', 0, line_.length());
    bool isfinal = 0;

    string dataout = "(";
    int isallSpace = this->findPosIsnotletter(line_, ' ', 0, line_.length());
    if (line_.length() == 2)
        isallSpace = -1;
    int *posHasEqual = this->PoshasLetter(line_, '=', 0, line_.length());
    if (posHasEqual != NULL)
    {
        int countEqual = this->countLetter(line_, '=', 0, line_.length());
        for (int i = 0; i < countEqual; i++)
        {
            for (size_t j = posHasEqual[i]; j < line_.length(); j++)
            {
                if (line_[j] == ',' || line_[j] == ')')
                    break;
                line_[j] = ' ';
            }
        }
        this->containinbreaket = line_;
    }

    BracketInfor thisLineInfor=checkLevelOfBracket(this->containinbreaket);
    string containInbracketAterRemove=this->deleteAllBracketAfter1st(this->containinbreaket);
    //count the position of each comma:
    int *PosCommas=this->PoshasLetter(containInbracketAterRemove,',',0,containInbracketAterRemove.length());
    //đang thực hiện doạn code phát hiện con trỏ hàm trong args
//    if(countNo(thisLineInfor.rememberLevel,2)>0)
//    {
//        for(int i=0; i<this->comman; i++)
//        {
//            for
//        }
//    }

    if (this->comman > 0 || (isallSpace != -1&&!this->method0))
    {
        for (int i = 0; i < this->comman + 1; i++)
        {
            int isspace = 1;
            int pos1st;
            int pos2nd;
            int j=0;
            string out;
            if (i == this->comman)
            {
                j = finalLetter - 1;
                isfinal = 1;
            }
            else
            {
                j = posofcomma[i];
            }

            for (j; j > 0; j--)
            {

                if (line_[j] != ' ' && isspace == 1)
                {
                    isspace = 0;
                    pos1st = j;
                    continue;
                }
                if (isspace == 0 && (line_[j] == ' ' || line_[j] == '(') && isfinal == 0)
                {
                    pos2nd = j + 1;
                    dataout += getstring(line_, pos2nd, pos1st) + ", ";
                    isspace = 0;
                    break;
                }
                else if (isspace == 0 && (line_[j] == ' ' || line_[j] == '(') && isfinal == 1)
                {
                    pos2nd = j;
                    dataout += getstring(line_, pos2nd, pos1st + 1) + ")";
                    isspace = 0;
                    break;
                }
            }
        }
    }
    else if(this->method0)
    {
        dataout += ")";
    }

    this->args = dataout;
}
void makeMockFile::makeFileHeader()
{

    this->fbout.open(filenameout.c_str(), ios::out);
    this->fbout << "class ";
    this->fbout << classname;
    this->fbout << "{" << endl
                << "public:" << endl;
    this->fbout.close();
}

void makeMockFile::write(string MOCname, string obj)
{
    if (this->isfunction == 0)
        return;
    this->getagr();
    this->checkvalidargs();

    string MOCK;

    // cout << "\n\nwrite is starting with infor below:" << endl;

    this->deleteSpaceBtwWords(this->containinbreaket);
    this->deleteSpaceBtwWords(this->args);
    this->deleteSpaceBtwWords(this->namefunction);
    this->deleteSpaceBtwWords(this->typeofunction);
    this->deleteSpaceBtwWords(this->OrginalClass);
    this->CheckValidStringAfterSecondBracket();



    bool has_space=this->checkContainInBracket();
    if (this->containinbreaket.length() == 2)
    {
        MOCK = MOCname + to_string(0);
    }
    else if(!has_space)
    {
        MOCK = MOCname + to_string(0);
    }
    else if (has_space)
    {
        MOCK = MOCname + to_string(this->comman + 1);
    }
    else
    {
        MOCK = MOCname + to_string(0);
    }
    fbout << "\t" << MOCK << "(" << this->namefunction << "," << this->typeofunction << this->containinbreaket << ");" << endl;
    cout << "-------------------" << endl;
    cout << "check OrginalClass" << endl;
    cout << this->OrginalClass << endl;
    cout << "belongtoAclass " << belongtoAclass << endl;
    cout << "write infor: Function name=" << this->namefunction << " |type= " << this->typeofunction << "| agr=" << this->containinbreaket << endl;
    if (this->belongtoAclass)
    {
        fbout_final << this->typeofunction << " " << this->OrginalClass << "::"
                    << this->namefunction << this->containinbreaket <<" "<<this->AfterSecondBracket<< "{" << endl;
    }
    else
    {
        fbout_final << this->typeofunction << " "
                    << this->namefunction << this->containinbreaket <<" "<<this->AfterSecondBracket<< "{" << endl;
    }

    if (this->isvoid == 1)
    {
        fbout_final << "\t"
                    << obj << "->" << this->namefunction << this->args << ";" << endl;
    }
    else
    {

        fbout_final << "\t"
                    << "return " << obj << "->" << this->namefunction << this->args << ";" << endl;
    }
    fbout_final << "}\n" << endl;
    this->writeUTtest();
    cout << "writing is finised with function: "
         << this->namefunction
         << "\n-------------------------"
         << "\n\n\n"
         << endl;
    //reset the number of bracket
    this->countBracketIn = 0;
    this->countBracketOut = 0;
    this->count_function+=1;
    this->AfterSecondBracket=" ";
    __LOG__(this->namefunction,log, "writing is finished");
}
void makeMockFile::checktypeofFuntion()
{
    string invalid = "-!@#$%^.,//\"\'<<+{}=;";
    for (size_t i = 0; i < this->typeofunction.length(); i++)
    {
        int ss = this->checkHasLetter(invalid, this->typeofunction[i], 0, invalid.length());
        if (ss != -1)
        {
            this->isvalidTypeofFunction = 0;
            __LOG__(this->typeofunction[i], log, "invalid type of function!");
            return;
        }
    }
    this->deleteSpaceBtwWords(this->namefunction);
    string invalidKey[] = {"break", "else", "else if", "auto", "case", "typedef", "return", "}else"};
    int lenOfInvalidKey = sizeof(invalidKey) / sizeof(string);
    __LOG__(this->typeofunction, log, "show value");

    for (int i = 0; i < lenOfInvalidKey; i++)
    {
//        __LOG__(this->typeofunction.compare(invalidKey[i]), log, "show value");

        if (this->typeofunction.compare(invalidKey[i]) == 0)
        {
            this->isvalidTypeofFunction = 0;
            __LOG__(this->typeofunction, log, "invalid type of function!");
            return;
        }
    }
    this->isvalidTypeofFunction = 1;
}
void makeMockFile::readFilename()
{
    this->fb.open(this->readname.c_str(), ios::in);
    __LOG__(this->readname, log, "show value");
    if (this->fb.fail())
        cout << "read failed" << endl;
    string line;
    string MOCname = "MOCK_METHOD";
    this->fbout.open(this->filenameout, ios::app);
    this->fbout_final.open(this->filenameout + "_.h", ios::app);
    fbout_final << "};";
    string obj = " M_" + getstring(this->classname, 4, this->classname.length());
    fbout_final << "\n\n"
                << this->classname << "* " << obj << ";\n" << endl;
    string supLine;
    bool isMissingaBreaket = 0;
    bool comment = 0;

    while (getline(this->fb, line))
    {

        int checkAllSpace = this->findPosIsnotletter(line, ' ', 0, line.length());
//        __LOG__(line,log,"line before delete unspecify space");
//        this->deleteSpaceBtwWords(line);
//        __LOG__(line,log,"line after delete unspecify space");
        if (line.length() == 0 || checkAllSpace == -1)
        {
            __LOG__(line, log, "line is empty or all space!");
            continue;
        }
        if (line.find("//") != std::string::npos)
        {
            __LOG__(line, log, "this is comment, before remove");
            line.erase(line.find("//"), line.length());
            if (line.length() == 0)
                continue;
            __LOG__(line, log, "this is comment, before remove");
        }
        size_t comment_start = line.find("/*");
        size_t comment_end = line.find("*/");
        if (comment_start != std::string::npos && comment_end !=  std::string::npos)
        {
            __LOG__(line, log, "before removing comment type: /**/");
            if (comment_end > comment_start)
                line.erase(line.begin()+comment_start, line.begin()+comment_end+2);
            __LOG__(line, log, "after removing comment type: /**/");
        }

        this->AfterSecondBracket=" "; // reset after second bracket to " "
        this->method0=0;
        int checkisFunction = this->checkisFunction(line);
        __LOG__(checkisFunction, log, "showvalue");
        //get class name
        if (checkisFunction == 0 && comment != true)
        {
            __LOG__(line, log, "checked is not function and is not comment");
            comment = 0;
        }


        this->getOrginalclassName(line);
        if(checkisFunction==1)
        {
            this->checktypeofFuntion();
        }
        if (!this->ismakedUTSheader && checkisFunction == 1)
        {
            this->makeHeaderfileForUTscript();
            this->ismakedUTSheader = 1;
        }

        if (!this->isvalidTypeofFunction && checkisFunction == 1 && comment!=1 && isMissingaBreaket!=1)
        {
            __LOG__(line, log, "invalid type of function and break, checkisFunction!=0");
            continue;
        }
        __LOG__(comment,log,"show value");
        __LOG__(isMissingaBreaket,log,"show value");
        if (checkisFunction == 1 && isMissingaBreaket == 0 && comment == 0)
        {
            __LOG__("check is valid class name is starting",log,"notify");
            this->checkvalidClassname();
            __LOG__("writing is starting",log, "notify");
            this->write(MOCname, obj);
        }
        else if (checkisFunction == 2 || (isMissingaBreaket == 1 && comment == 0))
        {
            supLine += line;
            isMissingaBreaket = 1;
            int findSecondBreaket = this->checkHasLetter(line, ')', 0, line.length());
            if(findSecondBreaket!=-1)
            {
                int checkIsFuncionResultInMultipLine=this->checkisFunction(supLine);
                __LOG__(supLine,log,"show supline");
                if (this->countBracketIn==this->countBracketOut)
                {
                    __LOG__("bracket equal",log," missing bracket was found");
                    if(checkIsFuncionResultInMultipLine==1)
                    {
                        this->checktypeofFuntion();
                        if(!this->ismakedUTSheader)
                        {
                            this->getOrginalclassName(supLine);
                            __LOG__("make header file for UT script",log," request make header");
                            this->makeHeaderfileForUTscript();
                            this->ismakedUTSheader=1;
                        }
                    }
                    if(this->isvalidTypeofFunction){
                      this->write(MOCname, obj);
                    }
                    supLine = "";
                    isMissingaBreaket = 0;
                }
            }
        }
        else if (checkisFunction == 3 || comment == true)
        {
            supLine += line;
            __LOG__("checking is comment is stating",log,"notify");
            if (supLine.find("*/") != string::npos)
            {
                __LOG__("the end of comment is found",log,"notify");
                comment = 0;
                supLine = "";
            }
            else
            {
                __LOG__("still in comment",log,"notify");
                comment = true;
            }
        }
    }
    this->fbout_final.close();
    this->fbout.close();
    __LOG__(this->count_function,log,"show total function was written");
}

int makeMockFile::merfile()
{
    fbout_final.open(this->filenameout + "_.h", ios::in);
    fbout.open(this->filenameout, ios::app);
    if (fbout.fail() || fbout_final.fail())
        return -1;
    string line;
    while (getline(fbout_final, line))
    {
        fbout << line << endl;
    }
    fbout_final.close();
    fbout.close();
    string commandrm = this->filenameout + "_.h";
    this->removefile(commandrm);
    return 1;
}
void makeMockFile::checkvalidargs()
{
    string invalid = "*&";
    for (size_t i = 0; i < this->args.length(); i++)
    {
        int valid = this->checkHasLetter(invalid, args[i], 0, invalid.length());
        if (valid != -1)
            args[i] = ' ';
    }
}
void makeMockFile::removefile(string filename)
{
    cout << "remove file: " << filename << " is starting" << endl;
    int ret = remove(filename.c_str());
    if (ret != 0)
        cout << "remove file: " << filename << " is failed" << endl;
    else
    {
        cout << "remove file: " << filename << " is successful" << endl;
    }
}
void makeMockFile::removeOldfile()
{
    cout << "remove file: " << this->filenameout << " is starting" << endl;
    int ret = remove(this->filenameout.c_str());
    if (ret != 0)
        cout << "remove file: " << this->filenameout << " is failed" << endl;
    else
    {
        cout << "remove file: " << this->filenameout << " is successful" << endl;
    }
}
void makeMockFile::writeUTtest()
{
    fbUTtest.open(this->UTtestname.c_str(), ios::app);
    if (fbUTtest.fail())
    {
        cout << "cannot gen UT script" << endl;
        __LOG__("cannot gen UT script",log,"notify-error, cannot open file");
        return;
    }
    string invalid = "*& ";
    string FCname;
    string orclassname;
    for (size_t i = 0; i < this->namefunction.length(); i++)
    {
        int ss = this->checkHasLetter(invalid, this->namefunction[i], 0, invalid.length());
        if (ss == -1)
        {
            FCname += this->namefunction[i];
        }
    }
    for (size_t i = 0; i < this->OrginalClass.length(); i++)
    {
        int ss = this->checkHasLetter(invalid, this->OrginalClass[i], 0, invalid.length());
        if (ss == -1)
        {
            orclassname += this->OrginalClass[i];
        }
    }
    fbUTtest << "\n/**\n"
                "*   @brief This is a test script for the "
             << FCname << " function\n"
                          "*   @classID "
             << orclassname << "\n"
                               "*   @methodID "
             << orclassname << "_" << FCname << "\n"
                                                "*   @paramList \n"
                                                "*   @priority P2\n"
                                                "*   @resolution_Method Equivalence Partitioning\n"
                                                "*   @test_condition \n"
                                                "*   @test_Coverage_Item \n"
                                                "*   @test_case_ID "
             << orclassname << "_" << FCname << "\n"
                                                "*   @test_type functionality\n"
                                                "*   @test_objective \n"
                                                "*   @test_precon \n"
                                                "*   @test_input \n"
                                                "*   @test_expected_result \n"
                                                "*   @test_module \n"
                                                "*   @design_id \n"
                                                "*/\n";
    fbUTtest << "TEST_F(" << orclassname << "Test, " << orclassname << "_" << FCname << "_TC001){" << endl;
    fbUTtest << "// Please make test script!!" << endl;
    fbUTtest << "#if 0 \t// Please enable and make test code here!!" << endl;
    fbUTtest << "\t// Please check source file, make mock files and insert EXPECT_CALL for callee functions.\n\n\n"
             << endl;
    fbUTtest << "#endif\n}";
    fbUTtest.close();
}
void makeMockFile::makeHeaderfileForUTscript()
{
    fbUTtest.open(this->UTtestname.c_str(), ios::out);
    if (fbUTtest.fail())
    {
        cout << "cannot gen UT script" << endl;
        return;
    }
    string classnamewirtetoheader = "";
    for (size_t i = 0; i < this->OrginalClass.length(); i++)
    {
        if (this->OrginalClass[i] != ' ')
            classnamewirtetoheader += this->OrginalClass[i];
    }
    __LOG__(classnamewirtetoheader,log,"show value");
    __LOG__(this->OrginalClass,log,"show value");
    if(classnamewirtetoheader=="")
    {
        this->deleteSpace(filenameNotype);
        classnamewirtetoheader=this->filenameNotype;

    }
    string include = "#include";
    fbUTtest << include + " \"gtest/gtest.h\"" << endl;
    fbUTtest << include + " \"gmock/gmock.h\"" << endl;

    fbUTtest << "// Include Source File for testing!!" << endl
             << "/*" << endl
             << "*Define Mock/Mock function" << endl
             << "*/" << endl;
    fbUTtest << "//class " << classnamewirtetoheader << "{" << endl;
    fbUTtest << "//public:\n"
             << "//virtual ~" << classnamewirtetoheader << "() {}";
    fbUTtest << "\n//};";
    fbUTtest << "\n//class Mock" << classnamewirtetoheader << ": public " << classnamewirtetoheader << "{" << endl;
    fbUTtest << "//public:\n//};" << endl;
    fbUTtest << "//Mock" << classnamewirtetoheader << " * M_" << classnamewirtetoheader << ";" << endl;

    fbUTtest << "using ::testing::_;\n"
                "using ::testing::A;\n"
                "using ::testing::AllOf;\n"
                "using ::testing::Args;\n"
                "using ::testing::AtLeast;\n"
                "using ::testing::Combine;\n"
                "using ::testing::DoAll;\n"
                "using ::testing::Return;\n"
                "using ::testing::ReturnRef;\n"
                "using ::testing::SaveArg;\n"
                "using ::testing::SaveArgPointee;\n"
                "using ::testing::SetArgPointee;\n"
                "using ::testing::SetArrayArgument;\n"
                "using ::testing::Values;\n";

    // fbUTtest << this->namefunction <<this->args<<";"<<endl;
    fbUTtest << "class " << classnamewirtetoheader << "Test : public::testing::Test{" << endl;
    fbUTtest << "protected:\n"
             << "\tvirtual void SetUp() {\n\n\n\t}" << endl;
    fbUTtest << "\t virtual void TearDown(){\n\n\n\t}\n};";
    fbUTtest.close();
}

void makeMockFile::deleteSpace(string &str)
{
    size_t __leng_str = str.length();
    int start = 0;
    int end = __leng_str;
    if (str[0] == ' '||str[0] == '\t' )
    {
        for (size_t i = 0; i < __leng_str; i++)
        {
            if (str[i] != ' '&&str[i] != '\t' )
            {
                start = i;
                break;
            }
        }
    }
    if (str[__leng_str] == ' '||str[0] == '\t')
    {
        for (size_t i = __leng_str; i > 0; i++)
        {
            if (str[i] != ' '&& str[i] != '\t')
            {
                end = i;
                break;
            }
        }
    }
    str = getstring(str, start, end);
}
void makeMockFile::deleteSpaceBtwWords(string &str)
{
    this->deleteSpace(str);

    size_t __leng_str = str.length();
    bool isspace1st = 0;
    int startSpace = 0;
    __LOG__(str, log, "show value");
    int count = 0;
    size_t i = 0;
//    char berforSpace = '0';
    for (i; i < __leng_str; i++)
    {
        if (str[i] == ' ' && !isspace1st)
        {
            isspace1st = 1;
            startSpace = i;
//            __LOG__(str[i], log, "show value");
//            berforSpace = str[i - 1];
            ++count;
        }
        else if (isspace1st && str[i] == ' ')
        {
            ++count;
//            __LOG__(str[i], log, "show value");
//            __LOG__(count, log, "show value");
        }
        else if (isspace1st && str[i] != ' ' && count >= 2)
        {
//            __LOG__(str[i], log, "show value");

            size_t step_first = startSpace;
            size_t step_second = step_first + count;
//            __LOG__(step_first, log, "show value");
//            __LOG__(step_second, log, "show value");

//            __LOG__(str, log, "show value");
//            if (berforSpace == ',')
//            {
//                str.erase(str.begin() + step_first + 1, str.begin() + step_second);
//            }
//            else
//            {
//                str.erase(str.begin() + step_first+1, str.begin() + step_second);
//            }
            str.erase(str.begin() + step_first+1, str.begin() + step_second);
//            __LOG__(str, log, "show value");

            isspace1st = 0;
            count = 0;
            __leng_str = str.length();
            i = 0;
        }
        else if (str[i] != ' ' && count < 2 && isspace1st != 0)
        {
//            __LOG__(str[i], log, "show value");
            count = 0;
            isspace1st = 0;
            if (i == 2 || i == __leng_str - 1)
            {
                str.erase(str.begin() + i - 1);
                __leng_str = str.length();
                i = 0;
            }
        }
    }
    __LOG__(str, log, "show value");
}
void makeMockFile::CheckValidStringAfterSecondBracket()
{
    string invalidKey={"{},'\"~!@#$%^&*`:"};
    size_t len=invalidKey.length();
    size_t lenAfterSecondBracket=this->AfterSecondBracket.length();

    for(size_t i=0; i<lenAfterSecondBracket; i++)
    {
        if(this->checkHasLetter(invalidKey,this->AfterSecondBracket[i],0,len)!=-1)
        {
            this->AfterSecondBracket=" ";
            break;
        }
    }
}
bool makeMockFile::checkContainInBracket()
{
    this->deleteSpaceBtwWords(this->containinbreaket);
    size_t containLength=this->containinbreaket.length();
    size_t letterBeforeSecondBracket=0;
    size_t letterAfterFirstBracket=0;
    bool secondBracketFound=0;
    bool firstBracketFound=0;

    bool has_space=0;
    for(size_t i=containLength; i>0; i--)
    {
        if(!secondBracketFound && this->containinbreaket[i]==')')
        {
            secondBracketFound =1;
            continue;
        }
        if(secondBracketFound)
        {
            if(this->containinbreaket[i]!=' ')
            {
                letterBeforeSecondBracket=i;
                break;
            }
        }
    }
    for(size_t i=0; i<containLength; i++)
    {
        if(!firstBracketFound && this->containinbreaket[i]=='(')
        {
            firstBracketFound =1;
            continue;
        }
        if(firstBracketFound)
        {
            if(this->containinbreaket[i]!=' ')
            {
                letterAfterFirstBracket=i;
                break;
            }
        }
    }
    int checkHasSpace=this->checkHasLetter(this->containinbreaket,' ',letterAfterFirstBracket,letterBeforeSecondBracket);
    __LOG__(getstring(this->containinbreaket,letterAfterFirstBracket,letterBeforeSecondBracket),log,"show value");
    if(checkHasSpace!=-1)
    {
        has_space=1;
    }
    else
    {
        has_space=0;
    }
    return has_space;
}
