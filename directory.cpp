#include "directory.h"
#include <unistd.h>
#include <algorithm>

bool directory::isDir(std::string path)
{
    struct stat sb;
    __LOG__(stat(path.c_str(), &sb), log, S_ISDIR(sb.st_mode));
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {

        __LOG__(path, log, "la thu muc");

        return 1;
    }
    __LOG__(path, log, "la file");
    return 0;
}
void directory::listdir(const char *name, int indent)
{
    DIR *dir_;
    struct dirent *entry;

    if (!(dir_ = opendir(name)))
    {
        __LOG__(path, log, (dir_ = opendir(name)));
        return;
    }
    while ((entry = readdir(dir_)) != NULL)
    {

        std::string entry_Name = name + (std::string)entry->d_name;
        makeMockFile *data__ = new makeMockFile(""); //using to use function find...
        int countsignSlash = data__->countLetter(entry_Name, '/', 0, entry_Name.length());
        int *posofsignalSlash = data__->PoshasLetter(entry_Name, '/', 0, entry_Name.length());
        for (int i = 0; i < countsignSlash; i++)
        {
            entry_Name[posofsignalSlash[i]] = '\\';
        }
        entry_Name.insert(entry_Name.length() - strlen(entry->d_name), 1, '\\');

        if (this->isDir(entry_Name))
        {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            entry_Name.insert(entry_Name.length() - strlen(entry->d_name), 1, '\\');
            __LOG__(entry->d_name, log, "la thu muc");

            this->listdir(path, indent + 2);
            __LOG__(entry->d_name, log, "d_name");
            __LOG__(path, log, "path");
        }
        else
        {
            printf("%*s- %s\n", indent, "", entry->d_name);
            __LOG__(entry->d_name, log, "la file");
            this->genmockfile(entry_Name);
            __LOG__(entry_Name, log, "gen this file");
        }
    }
    closedir(dir_);
}
void directory::genmockfile(string namefile)
{
    makeMockFile *mock_ = new makeMockFile(namefile);
    if (!mock_->iscorrectfiletype())
    {
        delete mock_;
        return;
    }
    mock_->removeOldfile();
    mock_->makeFileHeader();
    mock_->readFilename();
    mock_->merfile();
    delete mock_;
}

string getType(string data)
{
    string type = "";
    for (size_t i = data.length(); i > 0; i--)
    {

        if (data[i] == '.')
        {
            for (size_t j = i; j < data.length(); j++)
                type += data[j];
            break;
        }
    }
    return type;
}
string getName(string data)
{
    string name;
    for (size_t i = data.length(); i > 0; i--)
    {

        if (data[i] == '.')
        {
            name = getstring(data, 0, i);
            break;
        }
    }
    return name;
}
struct fileinfor
{
    string name;
    bool choosen;
    int idx;
    fileinfor()
    {
        choosen = 1;
    }
    bool operator<(const fileinfor &a) const
    {
        return name.compare(a.name) > 0;
    }
};
typedef fileinfor fileinfor;
void directory::getlistNeedGenMockFile(string *p, size_t size, string type)
{
    cout << "running--------------------------" << endl;
    vector<string> path;
    vector<fileinfor> Mfileinfor;
    if (p == NULL)
        return;
    for (size_t i = 0; i < size; i++)
    {

        bool ispath = 0;
        bool isnotSpace = 0;
        int isnotSpacePos = 0;
        for (size_t j = p[i].length(); j > 0; j--)
        {
            if (p[i][j] != ' ' && isnotSpace == 0)
            {
                isnotSpace = 1;
                isnotSpacePos = j;
            }
            if ((p[i][j] == '\\' || p[i][j] == '/') && isnotSpace)
            {
                path.push_back(getstring(p[i], 0, j));
                fileinfor file_tmpinfor;
                file_tmpinfor.name = getstring(p[i], j + 1, isnotSpacePos);
                cout << file_tmpinfor.name << endl;
                file_tmpinfor.idx = i;
                Mfileinfor.push_back(file_tmpinfor);
                ispath = 1;
                break;
            }
        }
        if (!ispath)
        {
            path.push_back("");
            fileinfor file_tmpinfor;
            file_tmpinfor.name = p[i];
            file_tmpinfor.idx = i;
            Mfileinfor.push_back(file_tmpinfor);
        }
    }

    std::sort(std::begin(Mfileinfor), std::end(Mfileinfor));

    for (size_t i = 1; i < Mfileinfor.size(); i++)
    {
        string previousString = Mfileinfor[i - 1].name;
        string currentString = Mfileinfor[i].name;

        string previousType = getType(previousString);
        string currentType = getType(currentString);

        string PreviousName = getName(previousString);
        string currentName = getName(currentString);
        cout << previousType << " " << __LINE__ << endl;
        if (!PreviousName.compare(currentName))
        {
            cout << "true" << endl;
            cout << PreviousName << endl;
            cout << currentName << endl;

            cout << "previousType=" << previousType << endl;
            cout << "currentType=" << currentType << endl;
            if (type == ".h" || type == ".H")
            {
                cout << __LINE__ << "dung --" << endl;
                if (currentString.find(".c") != string::npos || currentString.find(".cpp")!= string::npos  || currentString.find(".CPP") != string::npos  || currentString.find(".C")!= string::npos )
                {
                    Mfileinfor[i].choosen = 0;
                }
                if (previousString.find(".c") != string::npos || previousString.find(".cpp")!= string::npos  || previousString.find(".CPP") != string::npos  || previousString.find(".C")!= string::npos )
                {
                    Mfileinfor[i-1].choosen = 0;
                }
            }
            if (type == ".c" || type == ".C" || type == ".cpp" || type == ".CPP")
            {
                cout << __LINE__ << "dung --" << endl;
                if (previousString.find(".h") != string::npos || previousString.find(".H") != string::npos)
                {
                    cout << PreviousName << " " << __LINE__ << endl;
                    Mfileinfor[i - 1].choosen = 0;
                    cout << "------------------------------------" << endl;
                    cout << Mfileinfor[i - 1].name << endl;
                    cout << Mfileinfor[i - 1].choosen << endl;
                    cout << "------------------------------------" << endl;
                }
                if (currentString.find(".h") != string::npos || currentString.find(".H") != string::npos)
                {
                    cout << PreviousName << " " << __LINE__ << endl;
                    Mfileinfor[i].choosen = 0;
                    cout << "------------------------------------" << endl;
                    cout << Mfileinfor[i].name << endl;
                    cout << Mfileinfor[i].choosen << endl;
                    cout << "------------------------------------" << endl;
                }
            }
        }
    }
    for (size_t i = 0; i < Mfileinfor.size(); i++)
    {
        int idx = Mfileinfor[i].idx;
        // cout << Mfileinfor[i].name<<endl;



        if (Mfileinfor[i].choosen == 1)
        {
            string link_ = path[idx] + Mfileinfor[i].name;
            choosenList.push_back(link_);
        }
    }
    for (int i = 0; i < choosenList.size(); i++)
    {
        cout << choosenList[i] << endl;
    }
}
