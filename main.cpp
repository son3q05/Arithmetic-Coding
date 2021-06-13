//Drive program for Arithmetic Coding
//By Nguyen Thai Son - 931924

#include<iostream>
#include "ArithmeticWithScaling.cpp"

using namespace std;

int main(){
    cout<<"Arithmetic Codeing - By Nguyen Thai Son"<<endl<<endl;

    string input,output;

    char check;
	cout<<"Encode Phase!"<<endl;

	cout<<"Do you want to Encode?<y/n> ";
	cin>>check;
	if((check=='Y'||check=='y')){
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
    }
    
    //Decode Phase
    cout<<"Decode!"<<endl;
    cout<<"Do you want to Decode?<y/n> ";
	cin>>check;
	if(!(check=='Y'||check=='y')) return 0;

    cout<<"Input your encoded filename: ";
    cin>>input;
	const char* inputFile = input.c_str();


    cout<<"Input your Destination File name: ";
	cin>>output;
    const char* outputFile=output.c_str();

    Arithmetic DecodeObj;
    DecodeObj.Decode(inputFile,outputFile);

    return 0;
}
