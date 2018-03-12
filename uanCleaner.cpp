//File: uanCleaner.cpp
//Author: Adam Blenk
//Created On: 2/22/18

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sstream>

using namespace std;

int main()
{
    string uselessTextElement;
    string usefulElement;
    vector<string> uselessText;
    vector<string> usefulElements;
    string filename;
    string astring;
    string kstring;
    int x;
    int a = 0;
    int b = 0;
    bool fileIsOpen = false;

    for(int k = 1; k <= 5; k++)
    {
        ostringstream convert2;
        convert2 << k;
        kstring = convert2.str();
        filename = kstring + "uan.csv";

        //opens "k"uan.csv
        ifstream file(filename.c_str());

        if(!file.is_open())
        {
            cout << "Error: file did not open" << endl;
        }

        //compiles useless text
        for(int i = 0; i <= 16; i++)
        {
            getline(file, uselessTextElement);
            uselessText.push_back(uselessTextElement);
        }

        //compiles useful elements
        for(int j = 0; j <= 325; j++)
        {
            for(int i = 0; i <= 4; i++)
            {
                getline(file, usefulElement, ' ');
                usefulElements.push_back(usefulElement);
            }
            getline(file, usefulElement, '\n');
            usefulElements.push_back(usefulElement);
        }

        file.close();

        //deletes old file
        remove(filename.c_str());

        //creates new file with the same name
        ofstream file2 (filename.c_str(), ios_base::app | ios_base::out);

        if (file2.is_open())
        {
            for(int i = 1; i <= 1950; i++)
            {
                x = i % 6;
                file2 << usefulElements[i-1];
                file2 << ",";
                if(x == 0 && i != 0)
                {
                    file2 << "\n";
                }
            }

            file2.close();
        }
        else
        {
            cout << "Error: Unable to recreate file.";
        }

        uselessText.clear();
        usefulElements.clear();
    }

    return(0);
}
