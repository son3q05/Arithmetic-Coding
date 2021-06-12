//Project Arithmetic Coding
//Nguyen Thai Son - 931924
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>

using namespace std;

struct Table{
    char c;
    int num=0;
    double freq;
    double low;
    double high;
};

class Arithmetic{
    string data;
    Table* Interval;
    int UsedChar,max_num,data_len;
public:
	~Arithmetic() {
		delete [] Interval;
	};
    void Encode(const char* ifname, const char* ofname);
    void Decode(const char* ifname, const char* ofname);
    void EncodeInput(const char* filename);
    void DecodeInput(const char* filename);
    void E1(double &x1,double &x2);
    void E2(double &x1,double &x2);
    //void E3(double &x1,double &x2);
    double value(string str, int t);
    string ConvertStringToBit(const string codestring, int& remainbit);
    string ConvertBitToString(const string bitstring, int remainbit);
    void PrintTable();
    void CreateTable();
};

int log2(int x){
    int ret; double temp;
    ret=log(x)/log(2);
    temp=log(x)/log(2);
    if ((temp-ret)>0) ret++;
    return ret;
}

double Arithmetic::value(string str,int t){
    double ret=0;
    int cap=256;
    if (t<256) cap=t;
    for (int i=0;i<cap;i++){
        ret+=(str[i]-'0')*pow(2,-i-1);
    }
    return ret;
}

void Arithmetic::E1(double &x1,double &x2){     //Scaling Rule E1: [) in [0;0,5)
    x1=x1*2;
    x2=x2*2;
}

void Arithmetic::E2(double &x1,double &x2){     //Scaling Rule E2: [) in [0.5;1)
    x1=2*x1-1;
    x2=2*x2-1;
}

void Arithmetic::EncodeInput(const char* filename){
    ifstream ifs;
	ifs.open(filename);
	if (!ifs.is_open()) {cout<<"Can't open this file"<<endl; return;}
	char c; int count=0,c_num=0;
    Table temp[256]; //work as map to read data
	//Read Data
	while (ifs.get(c)) {
		data+=c;
		temp[c].num ++;
	}

	for (int i=0;i<256;i++){
        if(temp[i].num!=0){
            count++;
            temp[i].freq=(double)temp[i].num/data.length();
            if (c_num<temp[i].num) c_num=temp[i].num;
        }
    }
    UsedChar=count;
    max_num=c_num;
    data_len=data.length();
    Interval = new Table [UsedChar];

    //Get data from temp into table Interval ;
    int j=0,i=0;
    while (j<256){
        if (temp[j].num!=0) {
            Interval[i].c=(char)j;
            Interval[i].freq=temp[j].freq;
            Interval[i].num=temp[j].num;
            i++;
        }
        j++;
    }
	ifs.close();
	cout<<"Data:"<<data<<endl;
}

void Arithmetic::CreateTable(){
    Interval[0].low=0;
    Interval[0].high=Interval[0].freq;
    for(int i=1;i<UsedChar;i++){
        Interval[i].low=Interval[i-1].high;
        Interval[i].high=Interval[i].low+ Interval[i].freq;
    }
    cout<<"Table has been Created!"<<endl;
}

void Arithmetic::PrintTable(){
    cout<<endl<<"Table:\t\tfreq\t\tlow\t\thigh"<<endl;
    for (int i=0;i<UsedChar;i++){
        cout<<fixed<<setprecision(10)<<Interval[i].c<<"\t"<<Interval[i].freq<<"\t"<<Interval[i].low<<"\t"<<Interval[i].high<<endl;
    }
}

string Arithmetic::ConvertStringToBit(const string codestring, int& remainbit){
	char temp;
	string ret;
	int m;
	int nbit=codestring.length();
	if (nbit%8==0)  m=nbit/8; else m=nbit/8+1;
	int index=0;
	for(int part=0;part<m-1;part++){
		temp=0;
        for(int bit=0;bit<8;bit++){
            index=(part*8+bit);
            temp |= ((codestring[index] - '0') << bit);
        }
		ret.push_back(temp);
	}
	temp=0;
	remainbit=(m*8-nbit);
	for(int bit=0;bit<8-remainbit;bit++){
            index=((m-1)*8+bit);
            temp |= ((codestring[index] - '0') << bit);
    }
	temp=temp<<remainbit;
	ret.push_back(temp);
	return ret;
}

string Arithmetic::ConvertBitToString(const string bitstring, int remainbit) {
	char temp;
	string ret;
	int len = bitstring.length();

    for (int part = 0; part < len - 1; part++){
        for (int bit=0;bit<8;bit++) {
			temp=((bitstring[part]>>bit&1)+'0');
			ret.push_back(temp);
		}
    }
	char last=bitstring[len-1]>>remainbit;
	for (int bit=0;bit<8-remainbit;bit++) {
			temp=((last>>bit&1)+'0');
			ret.push_back(temp);
		}
    return ret;
}

void Arithmetic::Encode(const char* ifname, const char* ofname){
    cout<<"Arithmetic Coding - Encode!"<<endl;

    //Read Data
    EncodeInput(ifname);
    cout<<endl<<"Input has been read"<<endl;

    //Create Table
    CreateTable();
    cout<<"Table has been created"<<endl;
    PrintTable();

    //Building Header
    //Header will contain (bit_len)(UsedChar)[num0,num1,...](char0)(char1)....
    ofstream out;
	out.open(ofname,ios::trunc);
	if (!out.is_open()) {cout<<"Can't open this file"<<endl; return;}
    char bit_len=log2(max_num);
    out<<bit_len;
    out<<(char)UsedChar;

    //Create String of 1 and 0
    string HeaderStr;
	for (int i=0; i<UsedChar;i++){
		for (int j=bit_len-1;j>=0;j--){
			HeaderStr+=(char) ('0'+(Interval[i].num>>j&1));
		}
	}

    int remain;
    //convert to the bit type
	string HeaderFinal;
	HeaderFinal=ConvertStringToBit(HeaderStr,remain);

    //Add char;
    for (int i=0;i<UsedChar;i++) {HeaderFinal+=(char)Interval[i].c;}

    //Write into output
    out<<HeaderFinal;
    cout<<endl<<"Header had been writen to Output file!"<<endl;

    //Into the main Encode
    double low=0,high=1,range=high-low,tmp;
    int check=0;
    string tag_str;
    for (int i=0;i<data.length();i++){
        for (int j=0;j<UsedChar;j++){
            if (data[i]==Interval[j].c) {
                range = high-low;
                tmp=low;
                low = tmp + Interval[j].low*range;
                high = tmp + Interval[j].high*range;
                while((high<0.5)||(low>=0.5)){
                    if (high<0.5) {E1(low,high); tag_str+='0';}
                    else if (low>=0.5) {E2(low,high); tag_str+='1';}
                }
                break;
            }
        }
    }

    cout<<"Data has been encoded!"<<endl;

    //Convert 0,1 in string into real bit. At the end add the remain to re-convert string
    string tag;
    tag=ConvertStringToBit(tag_str,remain);
    tag+=(remain+'0');
    out<<tag;
    out.close();
}

void Arithmetic::DecodeInput(const char* filename){
    ifstream ifs;
	ifs.open(filename);
	if (!ifs.is_open()) {cout<<"Can't open this file"<<endl; return;}

    char tmp;

    //Read bit_len
    ifs.get(tmp);
    int bit_len= (int)tmp;

    //Read UsedChar
    ifs.get(tmp);
    UsedChar=(int)tmp;

    //Read the Header
    int len;
	if ((UsedChar*bit_len)%8==0) {len=UsedChar*bit_len/8;} else {len=UsedChar*bit_len/8+1;}
    string HeaderBitcode;
    for(int i=0;i<len;i++) {
			ifs.get(tmp);
			HeaderBitcode+=tmp;
		}
	cout<<"Bitcode has been readed!"<<endl;

    string HeaderStringCode;
	HeaderStringCode=ConvertBitToString(HeaderBitcode,len*8-UsedChar*bit_len);
	cout<<"Convert Bitcode to String succesfully!"<<endl;

    //Convert the string of 0,1 in to Interval.num
    Interval= new Table [UsedChar];
    int index=0;
    for (int i=0;i<UsedChar;i++) {
		Interval[i].num=0;
		for (int j=0;j<bit_len;j++) {
			Interval[i].num|=(HeaderStringCode[index]-'0')<<(bit_len-1-j);
			index++;
		}
	}

    //Read the Char into Interval.c and get sum of Interval.num
    data_len=0;
    for (int i=0;i<UsedChar;i++) {ifs.get(Interval[i].c); data_len+=Interval[i].num;}
    cout<<"Header has been read"<<endl;

    //Re-calculate Frequency & Rebuild Table
    for (int i=0;i<UsedChar;i++) {Interval[i].freq=(double)Interval[i].num/data_len;}
    CreateTable();
    cout<<"Table has been Rebuilt!"<<endl;
    PrintTable();

    while(ifs.get(tmp)) data+=tmp;
	cout<<endl<<"Main code has been readed!"<<endl;
    cout<<data;
	ifs.close();
}

void Arithmetic::Decode(const char* ifname, const char* ofname){
    cout<<"Arithmetic Coding - Decode!"<<endl<<endl;

     //Read Data
    DecodeInput(ifname);
    cout<<endl<<"Input has been read successfully"<<endl;

    //Get remain
    int remain=data[data.length()-1]-'0';
    data=data.substr(0,data.length()-1);

    //Convert tag to string 1,0
    string tag_str;
    tag_str=ConvertBitToString(data,remain);
    //Decode double to string with the help of table;
    int tag_len=tag_str.length();

    //Create new table for decoding
    Table Table_tmp[UsedChar];
    for (int i=0;i<UsedChar;i++) {
        Table_tmp[i].c=Interval[i].c;
        Table_tmp[i].low=Interval[i].low;
        Table_tmp[i].high=Interval[i].high;
        Table_tmp[i].freq=Interval[i].freq;
;
    }

    double tag,low,high,range;  string plain;
    tag=value(tag_str,tag_len);

    for (int i=0;i<data_len;i++){
        for (int j=0;j<UsedChar;j++){
            if (tag>=Table_tmp[j].low && tag<Table_tmp[j].high) {
                plain+=Interval[j].c;
                low=Table_tmp[j].low; high=Table_tmp[j].high;
                //scaling
                while((high<0.5)||(low>=0.5)){
                    if (high<0.5) {E1(low,high); tag_str=tag_str.substr(1,tag_len-1);tag_str+='0';}
                    else if (low>=0.5) {E2(low,high); tag_str=tag_str.substr(1,tag_len-1); tag_str+='1';}
                }
                //Re_arrange Table_tmp
                range=high-low;
                for (int k=0;k<UsedChar;k++){
                    Table_tmp[k].low=low+range*Interval[k].low;
                    Table_tmp[k].high=low+range*Interval[k].high;
                }
                tag=value(tag_str,tag_len);
                break;
            }
        }
    }



    cout<<"Decode successfully: "<<plain<<endl;
    
    ofstream ofs;
    ofs.open(ofname,ios::trunc);
    ofs<<plain;
    ofs.close();
    cout<<"Plain text has been writen to Output!"<<endl;

}

