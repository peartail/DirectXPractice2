

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

bool LoadDataStructures(char* filename, int vcount, int tcount, int ncount, int fcount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texIndex, normIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	vertices = new VertexType[vcount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[tcount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[ncount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[fcount];
	if (!faces)
	{
		return false;
	}

	vertexIndex = 0;
	texIndex = 0;
	normIndex = 0;
	faceIndex = 0;

	fin.open(filename);

	if (fin.fail() == true)
	{

		return false;
	}

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			if (input == 't')
			{
				fin >> texcoords[texIndex].x >> texcoords[texIndex].y;

				texcoords[texIndex].y = 1.0f - texcoords[texIndex].y;
				texIndex++;
			}

			if (input == 'n')
			{
				fin >> normals[normIndex].x >> normals[normIndex].y >> normals[normIndex].z;

				normals[normIndex].z = normals[normIndex].z * -1.0f;
				normIndex++;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}
		while (input != '\n')
		{
			fin.get(input);
		}
		fin.get(input);
	}

	fin.close();
	string filestring = filename;
	int fileIndex = filestring.rfind('.');
	string fname = filestring.substr(0,fileIndex);

	string currentDirectory = ".\\Model\\";
	string outputFilename = currentDirectory + (fname + ".txt").c_str();
	//printf(outputFilename);

	fout.open(outputFilename);

	fout << "Vertex Count: " << (fcount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	for (int i = 0; i < faceIndex; ++i)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' ' << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' ' << normals[nIndex].x << ' ' << normals[nIndex].y
			<< ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' ' << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' ' << normals[nIndex].x << ' ' << normals[nIndex].y
			<< ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' ' << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' ' << normals[nIndex].x << ' ' << normals[nIndex].y
			<< ' ' << normals[nIndex].z << endl;
	}

	fout.close();

	if (vertices)
	{
		delete[] vertices;
		vertices = NULL;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = NULL;
	}

	if (normals)
	{
		delete[] normals;
		normals = NULL;
	}

	if (faces)
	{
		delete[] faces;
		faces = NULL;
	}

	return true;




}