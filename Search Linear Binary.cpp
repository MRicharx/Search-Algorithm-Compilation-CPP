#include<iostream>
#include<fstream>
#include<chrono>
#include<math.h>
#include<list>
#include<stdlib.h>
#include<time.h>

using namespace std;

bool linearSearch(int arr[], int length, int x)
{
	for (int i = 0; i < length; i++)
		if (arr[i] == x)
			return true;
	return false;
}

//bool binarySearch(int arr[],int n,int low, int high, int x)
//{
//	if (low > high)
//		return -1;
//
//	int middle = arr[high / 2];
//
//	if (x == middle)
//		return n / 2;
//	else if (x > middle)
//		return binarySearch(arr,n, n / 2 + 1, high, x);
//	else
//		return binarySearch(arr,n, low, n / 2 + 1, x);
//}

bool binarySearch(int arr[], int low, int high, int x)
{
	if (low > high)
		return -1;

	int middle = arr[high / 2];

	if (x == middle)
		return high / 2;
	else if (x > middle)
		return binarySearch(arr, high / 2 + 1, high, x);
	else
		return binarySearch(arr, low, high / 2 + 1, x);
}


int getMax(int arr[], int n)
{
	int mx = arr[0];
	for (int i = 1; i < n; i++)
		if (arr[i] > mx)
			mx = arr[i];
	return mx;
}

void countSort(int arr[], int n, int exp)
{
	int *output;
	output = new int(n);// output array 
	int i, count[10] = { 0 };

	// Store count of occurrences in count[] 
	for (i = 0; i < n; i++)
		count[(arr[i] / exp) % 10]++;

	// Change count[i] so that count[i] now contains actual 
	//  position of this digit in output[] 
	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];

	// Build the output array 
	for (i = n - 1; i >= 0; i--)
	{
		output[count[(arr[i] / exp) % 10] - 1] = arr[i];
		count[(arr[i] / exp) % 10]--;
	}

	// Copy the output array to arr[], so that arr[] now 
	// contains sorted numbers according to current digit 
	for (i = 0; i < n; i++)
		arr[i] = output[i];
}

// Radix Sort 
void radixSort(int n, int arr[])
{
	// Find the maximum number to know number of digits 
	int m = getMax(arr, n);

	for (int exp = 1; m / exp > 0; exp *= 10)
		countSort(arr, n, exp);
}

void main()
{
	int length;
	int* arr;
	char temp;
	string address;

	for (int i= 0; i <= 1; i+=3)
	{
		//USE 48 TO TEST FROM ARRAY 0
		temp = i + 49;
		string address = "Array";
		address += temp;
		address += ".txt";

		//READ FILE
		fstream read(address, ios::in);
		if (read.is_open())
		{
			cout << "Percobaan Searching Dari Array ke-" << i + 1 << endl;
			read >> length;
			arr = new int[length];
			for (int j = 0; j < length; j++)
			{
				read >> arr[j];
			}
		}
		else
		{
			cout << "File Gagal Dibuka" << endl;
		}

		radixSort(length, arr);

		srand(time(NULL));
		int x = rand() % 1000000;
		x++;

		//VALUE TEST
		x = 888812;

		auto start = chrono::high_resolution_clock::now();

		cout << "Search: " << x;
		//LINEAR SEARCH
		linearSearch(arr, length, x);

		//BINARY SEARCH

		//INTERPOLATION SEARCH

		chrono::duration<double> elapsed = chrono::high_resolution_clock::now() - start;
		cout << "Duration Taken: " << elapsed.count() << " s" << endl;
		read.close();
	}
}