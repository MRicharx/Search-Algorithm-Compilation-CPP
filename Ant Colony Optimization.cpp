#include<iostream>
#include<vector>
#include<math.h>
#include<queue>
#include<time.h>
#include <iomanip>

using namespace std;

void cobasort(vector<int>& n, vector<int>& index) {
	for (int i = 0; i < n.size(); i++)
	{
		for (int j = 0; j < n.size() - 1; j++)
		{
			if (n[j] > n[j + 1]) {
				swap(n[j], n[j + 1]);
				swap(index[j], index[j + 1]);
			}
		}
	}
}

struct Location
{
	int x;
	int y;
};

class List
{
protected:
	vector<Location> coor;
public:
	List()
	{
	}

	void addLocation(int x, int y)
	{
		Location temp;
		temp.x = x;
		temp.y = y;
		coor.push_back(temp);
	}
	void editLocation(int index, int x, int y)
	{
		coor[index].x = x;
		coor[index].y = y;
	}
	void deleteLocation(int index)
	{
		coor.erase(coor.begin() + index);
	}
	void printLocation(int i)
	{
		cout << i + 1 << ". x : " << coor[i].x << "  y : " << coor[i].y << " ";
	}

	int getSize()
	{
		return coor.size();
	}

	Location getLocation(int index)
	{
		return coor[index];
	}
};

class Maps
{
public:
	List listLocation;
	vector<vector<float>> distance;
	vector<vector<float>> pheromone;

	float evaporateDegree = 0.03;

	Maps()
	{
		loadLocation();
	}
	void loadLocation()
	{
		listLocation.addLocation(-4, -4); //0
		listLocation.addLocation(16, -5);
		listLocation.addLocation(22, 8);
		listLocation.addLocation(0, -16);
		listLocation.addLocation(3, -19); //4
		listLocation.addLocation(24, 20);
		listLocation.addLocation(24, -15);
		listLocation.addLocation(16, -1);
		listLocation.addLocation(10, -15);
		listLocation.addLocation(-7, -2); //9
		listLocation.addLocation(13, 10);
		listLocation.addLocation(17, -6);
		listLocation.addLocation(19, 1);
		listLocation.addLocation(19, -16);
		listLocation.addLocation(-8, -8); //14
		listLocation.addLocation(-4, -1);
		listLocation.addLocation(20, -3);
		listLocation.addLocation(17, -17);
		listLocation.addLocation(-2, 9);
		listLocation.addLocation(23, -24); //19
		listLocation.addLocation(31, -26);
		listLocation.addLocation(37, -20);
		listLocation.addLocation(30, -30);
		listLocation.addLocation(26, -22);
		listLocation.addLocation(37, -16); //24
		listLocation.addLocation(25, -18);
		listLocation.addLocation(17, -30);
		listLocation.addLocation(26, -28);
		listLocation.addLocation(27, -15);
		listLocation.addLocation(18, -24);//29
	}
	void generateMatrix()
	{
		//buat matrix awal
		distance.resize(listLocation.getSize());
		pheromone.resize(listLocation.getSize());
		/*for (int i = 0; i < listLocation.getSize(); i++)
		{
			distance[i].reserve(listLocation.getSize());
			pheromone[i].reserve(listLocation.getSize());
		}*/

		//assign default value
		for (int i = 0; i < listLocation.getSize(); i++)
			for (int j = 0; j < listLocation.getSize(); j++)
			{
				distance[i].push_back(0);
				pheromone[i].push_back(1);
			}
	}

	float euclideanDistance(Location a, Location b)
	{
		int x = a.x - b.x;
		int y = a.y - b.y;

		float c = sqrt((float)x * x + (float)y * y);
		return c;
	}
	void calculateDistance()
	{
		//calculate jarak antar semua city
		for (int i = 0; i < listLocation.getSize(); i++)
			for (int j = 0; j < listLocation.getSize(); j++)
				distance[i][j] = euclideanDistance(listLocation.getLocation(i), listLocation.getLocation(j));
	}
	void newLocation(int x, int y)
	{
		//resize ukuran vector
		distance.resize(listLocation.getSize() + 1);
		pheromone.resize(listLocation.getSize() + 1);
		/*for (int i = 0; i < listLocation.getSize(); i++)
		{
			distance[i].resize(listLocation.getSize() + 1);
			pheromone[i].resize(listLocation.getSize() + 1);
		}*/

		//assign default value pheromone & distance
		for (int i = 0; i < listLocation.getSize() + 1; i++)
		{
			distance[i].push_back(0);
			pheromone[i].push_back(1);
			pheromone[listLocation.getSize()].push_back(1);
			distance[listLocation.getSize()].push_back(0);
		}

		listLocation.addLocation(x, y);
		calculateDistance();
	}
	void editLocation(int index, int x, int y)
	{
		listLocation.editLocation(index, x, y);
		for (int i = 0; i < listLocation.getSize(); i++)
		{
			pheromone[index][i] = 1;
			pheromone[i][index] = 1;
		}
		calculateDistance();
	}
	void deleteLocation(int index)
	{
		//delete vector index tertentu
		for (int i = 0; i < listLocation.getSize(); i++)
		{
			distance[i].erase(distance[i].begin() + index);
			pheromone[i].erase(pheromone[i].begin() + index);
		}
		distance.erase(distance.begin() + index);
		pheromone.erase(pheromone.begin() + index);

		//delete location
		listLocation.deleteLocation(index);
		calculateDistance();
	}

	void addPheromone(int x, int y)
	{
		pheromone[x][y]++;
	}
	void updatePheromone()
	{
		for (int i = 0; i < listLocation.getSize(); i++)
			for (int j = 0; j < listLocation.getSize(); j++)
			{
				if (pheromone[i][j] - evaporateDegree > 0.1)
					pheromone[i][j] -= evaporateDegree;
				else
					pheromone[i][j] = 0.1;
			}
	}

	void print()
	{
		cout << "Location : " << endl;
		for (int i = 0; i < listLocation.getSize(); i++)
		{
			listLocation.printLocation(i);
			cout << endl;
		}
	}
	int pickRoute(int start)
	{
		vector<int> probability;
		vector<int> index;
		int totalProb = 0;
		float pick; float counter = 0;
		bool stat = 0;

		for (int i = 0; i < listLocation.getSize(); i++)
		{
			float temp = pheromone[start][i] / distance[start][i];
			if (distance[start][i] != 0)
			{
				// tambah kondisi if kalau sudah ada di taken path ga jalan
				probability.push_back((int)(temp * 100));
				index.push_back(counter);
				totalProb += probability[counter];
				counter++;
			}
		}

		cobasort(probability, index);
		counter = 0;
		pick = rand() % (int)(totalProb);
		for (int i = 0; i < probability.size(); i++)
		{
			counter += probability[i];
			if (pick < counter)
				return index[i];
		}
	}
};

class _aco
{
protected:
	Maps map;
	vector<vector<int>> takenPath; //kota yang sudah dilewati
	vector<int> start;
	int menu;

public:
	_aco()
	{
		menu = 9;
		takenPath.resize(map.listLocation.getSize());
		start.resize(map.listLocation.getSize());

		for (int i = 0; i < takenPath.size(); i++)
		{
			start.push_back(i);
		}
	}

	bool checkTaken(int ant, int index)
	{
		if (takenPath.empty())
			return false;
		else
			for (int i = 0; i < takenPath[ant].size(); i++)
				if (takenPath[ant][i] == index)
					return true;
		return false;
	}

	void selectPath(int index)
	{
		bool pickCity = 0; int choice;
		while (pickCity == 0)
		{
			choice = map.pickRoute(start[index]);
			if (takenPath[index].size() == map.listLocation.getSize() - 1) {
				takenPath[index].push_back(index);
				map.addPheromone(start[index], index);
				map.addPheromone(index, start[index]);
				start[index] = index;
				pickCity = 1;

			}
			if (checkTaken(index, choice) == 0)
			{
				takenPath[index].push_back(choice);
				map.addPheromone(start[index], choice);
				map.addPheromone(choice, start[index]);
				start[index] = choice;
				pickCity = 1;
			}

		}
	}

	void acoAlgo()
	{
		map.calculateDistance();
		for (int i = 0; i < map.listLocation.getSize(); i++)
		{
			takenPath[i].push_back(i);
			for (int c = 0; c < map.listLocation.getSize() - 1; c++)
			{
				selectPath(i);
			}

			cout << i + 1 << ". Path Result: ";
			for (int j = 0; j < takenPath.size(); j++) {
				cout << takenPath[i][j];
				if (j < takenPath.size() - 1) {
					cout << " - ";
				}
			}

			cout << endl;
		}
		map.updatePheromone();

	}

	void reset() {
		takenPath.erase(takenPath.begin(), takenPath.end());
		start.erase(start.begin(), start.end());

		takenPath.resize(map.listLocation.getSize());
		start.resize(map.listLocation.getSize());

		for (int i = 0; i < takenPath.size(); i++)
		{
			start.push_back(i);
		}
	}


	void menuLoop()
	{
		map.generateMatrix();
		while (menu != 0)
		{
			cout << "--MENU--" << endl
				<< "1. Add Location" << endl
				<< "2. Edit Location" << endl
				<< "3. Delete Location" << endl
				<< "4. Calculate path" << endl
				<< "5. Show All Location" << endl
				<< "0. Exit" << endl
				<< ">>";
			cin >> menu;

			int _x, _y; int index = 0;

			switch (menu)
			{
			case 1: //addLocation
				cout << "Input Koor X: ";
				cin >> _x;
				cout << "Input Koor Y: ";
				cin >> _y;
				map.newLocation(_x, _y);

				//tambah ant
				start.resize(start.size() + 1);
				takenPath.resize(takenPath.size() + 1);

				cout << "Index Added" << endl;
				break;
			case 2: //editLocation
				cout << "Input Index Kota: ";
				cin >> index;
				if (index < map.listLocation.getSize())
				{
					cout << "Input Koor X: ";
					cin >> _x;
					cout << "Input Koor Y: ";
					cin >> _y;
					map.editLocation(index, _x, _y);
					cout << "Index Edited" << endl;
				}
				else
					cout << "Index Out of Bound" << endl;
				break;
			case 3: //deleteLocation
				cout << "Input Index Kota: ";
				cin >> index;
				if (index < map.listLocation.getSize())
				{
					//hapus ant
					start.erase(start.begin() + index);
					takenPath.erase(takenPath.begin() + index);

					map.deleteLocation(index);
					cout << "Index Deleted" << endl;
				}
				else
					cout << "Index Out of Bound" << endl;
				break;
			case 4: //calculate Path
				acoAlgo();
				reset();
				break;
			case 5: //showAll location
				map.print();
				break;
			case 0: //exit
				break;
			}

			system("pause");
			system("CLS");
		}
	}
};

void main()
{
	srand(time(NULL));

	_aco postman;
	postman.menuLoop();

	/*vector<int> v = { 10, 9, 8, 6, 7, 2, 5, 1 };
	vector<int> index = { 0,1,2,3,4,5,6,7 };
	cout << "Elements before sorting" << endl;
	for (const auto& i : v)
		cout << i << ' ';
	cout << "Elements after sorting" << endl;
	cobasort(v, index);
	for (const auto& i : v)
		cout << i << ' ';

	cout << endl;
	for (const auto& i : index)
		cout << i << ' ';*/
}