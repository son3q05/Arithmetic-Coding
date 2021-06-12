//Drive program for Arithmetic Coding
//By Nguyen Thai Son - 931924

#include<iostream>
#include "ArithmeticWithScaling.cpp"

using namespace std;

int main(){
    cout<<"Arithmetic Codeing - By Nguyen Thai Son"<<endl<<endl;

    string input,output;

    cout<<"Encode!"<<endl;
    cout<<"Input your Plaintext file name: ";
    cin>>input;
	const char* inputFile = input.c_str();

    cout<<"Input your Destination File name: ";
	cin>>output;
	output.append(".arth");
    const char* outputFile=output.c_str();

    Arithmetic EncodeObj;
    EncodeObj.Encode(inputFile,outputFile);

    cout<<"Decode!"<<endl;
    cout<<"Input your encoded filename: ";
    cin>>input;
	inputFile = input.c_str();

    cout<<"Input your Destination File name: ";
	cin>>output;
    outputFile=output.c_str();

    Arithmetic DecodeObj;
    DecodeObj.Decode(inputFile,outputFile);

    return 0;
}
