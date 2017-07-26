#include <string>
#include <cstdio>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

string project_name;
vector<string> files;
unsigned char separator[12] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};

void getProjectName(char *argv[])
{
    int left, right = 0;
    for (int i = strlen(argv[0]) - 1; i >= 0; i--)
    {
        if (argv[0][i] == '\\')
        {
            if (right == 0)
                right = i;
            else
            {
                left = i;
                break;
            }
        }
    }

    for (int i = left + 1; i < right; i++)
        project_name.push_back(argv[0][i]);

    cout << project_name << '\n';
}

void extractFiles()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("data")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if (strlen(ent->d_name) > 3)
                files.push_back(ent->d_name);
        }
        closedir (dir);

        sort(files.begin(), files.end());
    }
    else
    {
        perror ("");
        exit(1);
    }
}

void copyFile(const char *file_name, FILE *output_file)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        cerr << "ERROR: Could not open loader.gba\n";
        exit(2);
    }

    long file_size;
    unsigned char *buffer;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    buffer = (unsigned char*)malloc(file_size);
    if (buffer == NULL)
    {
        cerr << "Memory error\n";
        exit(10);
    }

    fread(buffer, 1, file_size, file);
    fclose(file);

    fwrite(buffer, 1, file_size, output_file);
    fwrite(separator, 1, 12, output_file);

    free(buffer);
}

void copyData(FILE *output_file)
{

}

int main(int argc, char *argv[])
{
    extractFiles();

    getProjectName(argv);

    string output_name = project_name + ".gba";
    FILE *output_file = fopen(output_name.c_str(), "wb");

    copyFile("loader.gba", output_file);
    string bin_name = project_name + ".bin";
    copyFile(bin_name.c_str(), output_file);

    fclose(output_file);

    return 0;
}
