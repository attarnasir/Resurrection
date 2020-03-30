#include<iostream>
#include<fstream>
#include<string>
#include<cmath>

using namespace std;

int main(){
	int number[26][26],d;
	int a=0, b=0;
	ifstream File;
	File.open("batman1.txt");
	
	while( a<26){
		while( b<26 && File>>d){
		
		number[a][b++]=d;	
	
	}
	b=0;
	a++;
	}
	
	for(int i=0; i<26; i++){
		for(int j=0; j<26; j++){
		
		if(number[i][j]==0){
			cout<<"  ";
		}
		else if(number[i][j]==1){
			cout<<" *";
		}
	}
	
	cout<<endl;}
	File.close();

	 
	 return 0;
}
