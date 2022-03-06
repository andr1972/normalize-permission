/*goal: correct Linux attributes after copying to usb drive
 * files: no write by others, no executes
 * directories: no write by others
 * */
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <string>
#include <algorithm>
#include <filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
namespace fs = std::filesystem;

char* permissions(const char *filename){
    struct stat st;
    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
    if(stat(filename, &st) == 0){
        mode_t perm = st.st_mode;
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        modeval[9] = '\0';
        return modeval;
    }
    else{
        return "bad";
    }
}


void listDirectory(string currentDir) {
    fs::path dirPath(currentDir);
    string pwd = canonical(dirPath).string();
    std::vector<string> fileNames;
    std::vector<string> dirNames;
    for (const auto &file: fs::directory_iterator(pwd)) {
        string name = file.path().filename().string();
        if (file.is_directory())
            dirNames.push_back(name);
        else
            fileNames.push_back(name);
    }
    std::sort(dirNames.begin(), dirNames.end());
    std::sort(fileNames.begin(), fileNames.end());
    for (const string& name: fileNames) {
        string path =  pwd+ "/" + name;
        cout << path << "  " << permissions(path.c_str()) << endl;
    }
    for (const string& name: dirNames) {
        string path =  pwd+ "/" + name;
        listDirectory(path);
        cout << path << "  " << permissions(path.c_str()) << endl;
    }
}

int main() {
    listDirectory("..");
    return EXIT_SUCCESS;
}
