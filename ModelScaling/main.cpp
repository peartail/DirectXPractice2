#include <ostream>
#include <fstream>

#include <iostream>
#include <math.h>

using namespace std;

void GetModelFilename(char* filename, std::string& outfilename);
bool LoadTextStructure(char* savename,std::string filename);

int maxsize = 1;

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
	float tx, ty, tz;
	float bx, by, bz;
};

int main()
{
	char filename[256];
	string out;

	GetModelFilename(filename, out);

	cout << "MAX Size : ";

	int size;

	cin >> size;
	
	if (size > 0)
		maxsize = size;

	LoadTextStructure(filename,out);

	return 0;

}

void GetModelFilename(char* filename,std::string& outfilename)
{
	bool done;
	ifstream fin;

	done = false;
	while (!done)
	{
		cout << "Enter model filename : ";

		cin >> filename;

		string currentDirectory = ".\\Model\\";
		string outputFilename = currentDirectory + filename+".txt";

		

		fin.open(outputFilename);

		if (fin.good())
		{
			done = true;
			outfilename = outputFilename;
		}
		else
		{
			outfilename = "";
			fin.clear();
			cout << endl;
			cout << "File " << filename << " could not be opened. " << endl << endl;
		}
	}

}


bool LoadTextStructure(char* savename, std::string filename)
{
	int count;

	float absmax = 0;
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> count;



	ModelType* models = new ModelType[count];
	if (!models)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i<count; i++)
	{
		fin >> models[i].x >> models[i].y >> models[i].z;
		fin >> models[i].tu >> models[i].tv;
		fin >> models[i].nx >> models[i].ny >> models[i].nz;

		if (absmax < abs(models[i].x))
		{
			absmax = abs(models[i].x);
		}
		if (absmax < abs(models[i].y))
		{
			absmax = abs(models[i].y);
		}
		if (absmax < abs(models[i].z))
		{
			absmax = abs(models[i].z);
		}
		
		//cout << models[i].x << models[i].y << models[i].z;
	}

	absmax = absmax / maxsize;
	
	for (i = 0; i<count; i++)
	{
		models[i].x = models[i].x / absmax;
		models[i].y = models[i].y / absmax;
		models[i].z = models[i].z / absmax;
		
		cout << models[i].x << ' ' << models[i].y << ' ' << models[i].z << endl;
	}

	fin.close();

	ofstream fout;

	string filestring = savename;

	string currentDirectory = ".\\Model\\";
	string outputFilename = currentDirectory + (filestring + "_normal.txt").c_str();
	//printf(outputFilename);

	fout.open(outputFilename);

	fout << "Vertex Count: " << count << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	for (int i = 0; i < count; ++i)
	{
		fout << models[i].x << ' ' << models[i].y << ' ' << models[i].z << ' ' << models[i].tu << ' ' << models[i].tv << ' ' << models[i].nx << ' ' << models[i].ny
			<< ' ' << models[i].nz << endl;
	}


	fout.close();
	return true;


}