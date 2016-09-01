#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <limits>
#include <windows.h>
#include <conio.h>

using namespace std;

int main()
{
    double *n = new double[10000000];
    int cnt=16;
    long sample=0;
    long sampleOne=0;
    long i=0;
    double x=0;
    long j=0;
    int y=0;
    char q='q';

    ifstream myfile1;
    ofstream myfile2;

    myfile1.open ("test1.txt");

    if(!myfile1.is_open()){
        cout<<"Sorry, \"test1.txt\" does not exist"<<endl<<"Press any key to exit...";
        getch();
        return 0;}

    myfile2.open ("temp.txt");

    myfile1 >>cnt;

    switch(cnt){
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            break;
        default:
            cout<<"Sorry, number of channels is invalid"<<endl<<"Press any key to exit...";
            getch();
            return 0;
            }


    while(!myfile1.eof()){
        myfile1>>n[i];
        sample++;
        i++;
    }

    sampleOne=sample/cnt;


    for(long f=0;f<sampleOne;f++){
        for(int k=0;k<cnt;k++){
                x=n[j]*3276.8+32768;
                y=x;
                if(y>=65536)
                y=65535;
                if(y<=0)
                    y=0;
                myfile2<<uppercase<<hex<<setw(4)<<y<<endl;
                j=j+sampleOne;
                            }

        for(int m=cnt;m<16;m++){
        myfile2<<"8000"<<endl;
                               }

        if(j!=0)
            j=j-(cnt)*sampleOne+1;
                                 }

    myfile2<<"X";

    myfile1.close();
    myfile2.close();



    ifstream myfile3;
    ofstream myfile4;

    myfile3.open ("temp.txt");
    myfile4.open ("test.txt");


    while(myfile3.good()){

       myfile3.get(q);
        if(q==' ')
            myfile4<<"0";
        else if(q=='X')
            {
            myfile4<<"X";
            break;}
        else
            myfile4<<q;
                        }

    myfile3.close();
    myfile4.close();


   cout<<sampleOne<<" Samples each channel"<<endl<<cnt<<" Channels"<<endl;



    cout<<"%";
    for(int p=1;p<=100;p++){
    cout<<p;
    Sleep(10);
    if(p>67)
        Sleep(20);
    if(p>77)
        Sleep(30);
    cout<<"\b";
    if(p>9)
        cout<<"\b";
    }






    delete[] n;

    return 0;
}
