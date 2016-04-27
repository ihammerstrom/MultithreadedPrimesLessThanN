#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <future>
#include <thread>
#include <vector>

using namespace std;

/*
Program which finds the quantity of numbers between 0 and a specified number using a specified number of threads.
Also finds the quantity sequentially to find the percent speed change using threads.

Ian Hammerstrom
11/28/15
*/



int numPrimesRange(long int start, long int end);
int isPrime(long int num);

double get_wallTime() {
	struct timeval tp;
	gettimeofday(&tp,NULL);
	return (double) (tp.tv_sec + tp.tv_usec/1000000.0);
}


int main(int argc, const char *argv[]){

	if(argc!=3){
		cout<< "Usage: ./primesLessThan [num] [numThreads]"<<endl;
		return 1;
	}

	long int num= (long int)atoi(argv[1]);
	int numThreads = atoi(argv[2]);

	int numOfPrimes = 0;
	vector<future<int>> outputVector;

	
	double startTime = get_wallTime();
	cout << "Calculating number of primes between 0 and "<< num<<endl;
	numOfPrimes = numPrimesRange(0, num);
	double endTime = get_wallTime();
	cout<< "(nonThreaded) The number of primes less than "<< num<< " is "<< numOfPrimes<<endl;
	double sequentialTime = endTime - startTime;
	cout << "(nonThreaded) Time in seconds needed to calculate numPrimes less than "<< num <<" without threading: "<< sequentialTime << "\n\n\n";


	numOfPrimes = 0;
	startTime = get_wallTime();
	
	
	long int workSize = num/numThreads;
	for(long int i=0; i< numThreads; i++){
		cout << "Calculating number of primes between "<< i*workSize<< " and "<< (i+1)*workSize<<endl;
		outputVector.push_back(async(launch::async, numPrimesRange, i*workSize, (i+1)*workSize));
	}

	int temp;
	for(auto& t : outputVector){
		int temp = t.get();
		cout<< " - returned: "<<temp<<endl;
		numOfPrimes += temp;
	}


	cout << "Calculating number of primes between "<< num - num%numThreads <<" and "<< num<<endl;
	temp = numPrimesRange(num - num%numThreads, num); //clean up last remaining if number of threads doesnt evenly divide into num
	cout<< " - returned: "<<temp<<endl;
	numOfPrimes += temp;

	endTime = get_wallTime();
	cout<< "(threaded) The number of primes less than "<< num<< " is "<< numOfPrimes<<endl;
	cout << "(threaded) Time in seconds needed to calculate numPrimes less than "<< num <<" with "<<numThreads<< " threads: "<< endTime - startTime << endl;

	cout << "\nThe percentage of numbers that are primes from 0 to " <<num<< " is "<< ((double)numOfPrimes/(double)num)*100.0 << "%"<<endl; 
	cout << "The threaded calculation runs "<<sequentialTime/(endTime-startTime)<<" times faster than the sequential calculation."<<endl;
	return 0;
}

int numPrimesRange(long int start, long int end){
	int numOfPrimes=0;
	for(; start < end; start++){
		if(isPrime(start)){
			numOfPrimes++;
		}
	}
	return numOfPrimes;
}

int isPrime(long int num){
	if(num<=1){
		return false;
	}else if(num<=3){
		return true;
	}else if(num%2 == 0 || num%3 == 0){
		return false;
	}
	long int i=5;
	while(i*i <= num){
		if(num%i == 0 || num %(i+2)==0){
			return false;
		}
		i+=6;
	}
	return true;
}



























