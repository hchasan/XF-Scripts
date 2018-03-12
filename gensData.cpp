//File: gensData.cpp
//Author: Adam Blenk
//Created On: 1/18/18

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

int main ()
{
    vector<string> uselessText;
    vector<string> handshakes;
    vector<string> handshooks;
    vector<double> handshooksDouble;
    string uselessTextElement;
    string handshakesElement;
    string handshooksElement;
    double handshooksDoubleElement;
    double maxHandshook;

    //Creates files if not already created
    ifstream testFile("gensData.csv");

    if(!testFile.is_open())
    {
        ofstream makeFile("gensData.csv");

        if(makeFile.is_open())
        {
            makeFile.close();
        }
        else
        {
            cout << "Error: Failed to create file gensData.csv" << endl;
        }
    }
    else
    {
        testFile.close();
    }


    ifstream testFile2("maxHandshooks.csv");

    if(!testFile2.is_open())
    {
        ofstream makeFile2("maxHandshooks.csv");

        if(makeFile2.is_open())
        {
            makeFile2.close();
        }
        else
        {
            cout << "Error: Failed to create file maxHandshooks.csv" << endl;
        }
    }
    else
    {
        testFile2.close();
    }


    //opens handshake.csv and reads data in
    ifstream file("handshake.csv");

    if(!file.is_open())
    {
        cout << "Error: handshake.csv did not open" << endl;
    }

    for(int i = 0; i < 8; i++)
    {
        getline(file, uselessTextElement);
        uselessText.push_back(uselessTextElement);
    }

    for(int i = 0; i < 5; i++)
    {
        getline(file, handshakesElement);
        handshakes.push_back(handshakesElement);
    }

    file.close();


    //opens handshook, reads data in, and finds maximum
    ifstream file2("handshook.csv");

    if(!file2.is_open())
    {
        cout << "Error: handshook.csv did not open" << endl;
    }

    for(int i = 0; i < 2; i++)
    {
        getline(file2, uselessTextElement);
        uselessText.push_back(uselessTextElement);
    }

    for(int i = 0; i < 5; i++)
    {
        getline(file2, handshooksElement);
        handshooks.push_back(handshooksElement);
    }

    for(int i = 0; i < 5; i++)
    {
        handshooksDoubleElement = atof(handshooks[i].c_str());
        handshooksDouble.push_back(handshooksDoubleElement);
    }

    maxHandshook = handshooksDouble[0];
    for(int i = 1; i < 5; i++)
    {
        if(handshooksDouble[i] > maxHandshook)
        {
            maxHandshook = handshooksDouble[i];
        }
    }

    file2.close();


    //opens gensData and writes data from handshook and handshake to it
    ofstream file3 ("gensData.csv", ios_base::app | ios_base::out);

    if (file3.is_open())
    {
        for(int i = 0; i < 5; i++)
        {
            file3 << handshakes[i] << "\n";
        }

        file3.close();
    }
    else
    {
        cout << "Error: Unable to open file gensData.csv";
    }

    ofstream file4 ("maxHandshooks.csv", ios_base::app | ios_base::out);

    if (file4.is_open())
    {
        file4 << maxHandshook << "\n";

        file4.close();
    }
    else
    {
        cout << "Error: Unable to open file maxHandshooks.csv";
    }

    return(0);
}
