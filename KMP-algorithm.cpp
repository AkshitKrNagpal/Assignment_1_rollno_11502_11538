#include <iostream>
#include <fstream>
using namespace std;

#define max_length 50

// Some helper functions for strings and characters
 
bool isAlnum(char ch)  // checks if given 'char' is alphanumberic 
{
	if( ( ch >= 'A' && ch <= 'Z') || ( ch >= 'a' && ch <= 'z' ) || ( ch >= '0' && ch <= '9') )
		return true;
	return false; 
}
void trim(char * string) // remove special characters, spaces from a string
{
	int i=0,j=0;
	while(string[j]!='\0')
	{
		if( isAlnum(string[j]) )
			string[i++]=string[j];
		j++;
	}
	string[i]='\0';
}
int strlen(char * s) // returns length of string
{
	int i;
	for(i=0;s[i]!='\0';i++);
	return i;
}

// KMP algorithm functions

void LPSArray(int LPS[],char * pattern,int size) // this function generates LPS values 
{
	int i=0;
	int length=0; // length of previous longest prefix suffix 

	LPS[i++]=0; // LPS value of first char is always 0
	while( i < size )
	{
		if(pattern[i] == pattern[length]) 
		{
			length++;
			LPS[i++]=length;
		}
		else if( length == 0 ) 
			LPS[i++] = 0;
		else
			length = LPS[length-1];
	}
}
bool search(char * text,char * pattern) // return true if string 'text' contains string 'pattern' 
{
	int patternLength = strlen(pattern);
	int textLength = strlen(text);

	int * LPS = new int[patternLength];
	LPSArray(LPS,pattern,patternLength);
	bool found=false; // pattern found 
	int j=0;
	for(int i=0;i<textLength;)
	{
		if(pattern[j] == text[i]) // checking for pattern
			i++ , j++;
		if( j == patternLength )
		{	// pattern matched
			found=true;
			break;
		}
		else if ( i<textLength && pattern[j] != text[i] )
		{
			if(j!=0)
				j = LPS[j-1];
			else
				i++;
		}
	}
	delete LPS; // to avoid memory leak
	return found;
}


// Menu options functions

void numberOfPara(char * filename)
{
	if(strlen(filename)==0)
		cout<<"  No file selected. Please select file and try again \n";
	else
	{
		ifstream f(filename);
		char prev2=' ',prev=' ',ch;
		int para=0;
		while( f.get(ch) )
		{
			if( isAlnum(ch) && prev2 == '\n' && prev == '\n')
				para++;
			prev2=prev;
			prev=ch;
		}
		if(para>0)
			para++;
		cout<<" Number of paragraphs : "<<para<<endl;
		f.close();
	}
}
void numberOfWords(char * filename)
{
	if(strlen(filename)==0)
		cout<<"  No file selected. Please select file and try again \n";
	else
	{
		ifstream f(filename);
		char prev=' ',ch,temp[max_length];
		int words=0;
		while( !f.eof() )
		{
			f>>temp;
			words++;
		}
		cout<<" Number of words : "<<words<<endl;
		f.close();
	}			
}
void searchInFile(char * filename)
{
	if(strlen(filename)==0)
		cout<<"   No file selected. Please select file and try again \n";
	else
	{
		ifstream f(filename);
		char query[max_length],temp[max_length];
		cout<<"\n Enter word to be searched  => ";
		cin>>query;
		cout<<"\n Displaying results \n\n";
		int index=1;
		int indexOfFile=0;
		while( !f.eof() )
		{
			f >> temp;
			indexOfFile+=strlen(temp)+1;
			trim(temp);
			if( search(temp,query) )
			{
				cout<<" "<<index++<<". "<<temp<<"  (at index value "<<indexOfFile<<")"<<endl;
			}
		}
		if( index == 1 )
			cout<<"\n\n No results found \n";
	}
}

int main()
{
	char filename[max_length]="";
	int choice;
	do {
		ifstream f;
		cout<<"\n\n\t\tMenu\n\n";
		(strlen(filename)==0) ? cout<<"    No file Selected. " : cout<<"    Selected file : "<<filename;
		cout<<"\n\n";
		(strlen(filename)==0) ? cout<<" 1. Select File \n" : cout<<" 1. Change File\n";
		cout<<" 2. Display number of paragraphs in file\n";
		cout<<" 3. Display number of words in file\n";
		cout<<" 4. Search word in file \n";
		cout<<" 5. Exit \n";
		cout<<"\n Enter your choice => ";
		cin>>choice;
		cin.ignore();

		switch(choice)
		{
			case 1 :
				cout<<" Enter filename : ";
				cin>>filename;
				f.open(filename);
				if(!f.is_open())
				{
					cout<<" \n Error opening file.\n\n";
					filename[0]='\0';
				}
				else
					f.close();
				break;
			case 2 :
				numberOfPara(filename);
				break;
			case 3 :
				numberOfWords(filename);
				break;
			case 4 :
				searchInFile(filename);
				break;
			case 5 :
				cout<<"\n\n   Thank you for using this program.  \n";
				break;
			default:
				cout<<"\n\n   EEhhh. Please enter a valid choice \n";
				break;
		}
	} while(choice !=5);
}

