

#include <ostream>
#include <fstream>

#include <iostream>

using namespace std;

typedef struct
{
	float x,y,z;
}VertexType;

typedef struct
{
	int vIndex1,vIndex2,vIndex3;
	int tIndex1,tIndex2,tIndex3;
	int nIndex1,nIndex2,nIndex3;
}FaceType;

void GetModelFilename(char*);
bool ReadFileCounts(char*,int&,int&,int&,int&);
bool LoadDataStructures(char*,int,int,int,int);


int main()
{
	bool result;
	char filename[256];
	int vertexCount,textureCount,normalCount,faceCount;
	char garbage;

	GetModelFilename(filename);

	result = ReadFileCounts(filename,vertexCount,textureCount,normalCount,faceCount);
	if(!result)
	{
		return -1;
	}

	cout << endl;
	cout << "Vertices: " <<vertexCount << endl;
	cout << "UVs:      " << textureCount <<endl;
	cout << "Normals:  " << normalCount <<endl;
	cout << "Faces:    " << faceCount << endl;

	result = LoadDataStructures(filename,vertexCount,textureCount,normalCount,faceCount);
	if(!result)
	{
		return -1;
	}

	cout << "\nFile has been converted." << endl;
	cout << "\nDo you wish to exit(y/n)? ";
	cin >> garbage;

	return 0;

}

void GetModelFilename(char* filename)
{
	bool done;
	ifstream fin;

	done = false;
	while(!done)
	{
		cout << "Enter model filename : ";

		cin >> filename;

		fin.open(filename);

		if(fin.good())
		{
			done = true;
		}
		else
		{
			fin.clear();
			cout << endl;
			cout << "File " <<filename << " could not be opened. "<< endl << endl;

		}
	}
}

bool ReadFileCounts(char* filename,int& vcount,int& tcount,int& ncount,int& fcount)
{
	ifstream fin;
	char input;

	vcount = 0;
	tcount = 0;
	ncount = 0;
	fcount = 0;

	fin.open(filename);

	if(fin.fail() == true)
	{
		return false;
	}

	fin.get(input);
	while(!fin.eof())
	{
		if(input == 'v')
		{
			fin.get(input);
			if(input == ' ')  {vcount++;}
			if(input == 't') {tcount++;}
			if(input == 'n') {ncount++;}
		}

		if(input == 'f')
		{
			fin.get(input);
			if(input == ' ') {fcount++;}
		}

		while(input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;
}