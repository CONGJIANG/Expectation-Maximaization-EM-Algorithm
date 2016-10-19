// Advanced Topics in Statistics -- EM Algorithm

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
#define pi  3.141592653589793238463

// flip the coin function
bool flip1() {
	std::mt19937 eng(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int<int> dist(0, 1);
	return dist(eng);
}

// pdf 1
double pdf11(double xi, double mean1, double var1) {
	return (1 / (sqrt(2 * pi)))*exp(-pow((xi - mean1), 2) / 2 * var1);
}

// pdf 2
double pdf22(double xi, double mean2, double var2) {
	return (1 / (sqrt(2 * pi)))*exp(-pow((xi - mean2), 2) / 2 * var2);
}


// Function for Mean 1 
double mean_1(double p, const std::vector<double> & xval, double mean1, double mean2, double var1, double var2) {
	double sum = 0;
	double dum1 = 0;
	for (int i = 0; i <= (xval.size() - 1); i++) {
		sum += (((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2)))*xval[i]);
		dum1 += ((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2)));
	}
	return sum / dum1;
}

// Function for Mean 2
double mean_2(double p, const std::vector<double> & xval, double mean1, double mean2, double var1, double var2) {
	double sum = 0;
	double dum2 = 0;
	for (int i = 0; i <= (xval.size() - 1); i++) {
		sum += ((1 - ((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2)))) * xval[i]);
		dum2 += (1 - ((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2))));
	}
	return sum / dum2;
}

// Function for Probability 
double probability_1(double p, const std::vector<double> & xval, double mean1, double mean2, double var1, double var2) {
	double sum = 0;
	for (int i = 0; i <= (xval.size() - 1); i++) {
		sum += (pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2));
	}
	return (sum / (xval.size()));
}

// Function for Variance 1 
double variance_1(double p, const std::vector<double> & xval, double mean1, double mean2, double var1, double var2) {
	double sum = 0;
	double dum1 = 0;
	for (int i = 0; i <= (xval.size() - 1); i++) {
		sum += (((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2)))*pow((xval[i] - mean1), 2));
		dum1 += ((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2)));
	}
	return sum / dum1;
}

// Function for Variance 2
double variance_2(double p, const std::vector<double> & xval, double mean1, double mean2, double var1, double var2) {
	double sum = 0;
	double dum2 = 0;
	for (int i = 0; i <= (xval.size() - 1); i++) {
		sum += (((1 - ((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2)))))*pow((xval[i] - mean2), 2));
		dum2 += (1 - ((pdf11(xval[i], mean1, var1)*p) / ((pdf11(xval[i], mean1, var1)*p) + (1 - p)*pdf22(xval[i], mean2, var2))));
	}
	return sum / dum2;
}


int main() {


	// Variables 
	double sigma1 = 0;
	double sigma2 = 0;
	double mu1 = 0;
	double mu2 = 0;
	double p = 0;
	long int N = 0;

	cout << "Initialize mean 1: ";					cin >> mu1;
	cout << "Initialize variance 1: ";				cin >> sigma1;
	cout << "Initialize mean 2: ";					cin >> mu2;
	cout << "Initialize variance 2: ";				cin >> sigma2;
	cout << "Initialize probability: ";				cin >> p;
	cout << "Pick a size of the sample size: ";		cin >> N;
	cout << "\n" << endl;


	// Vectors that we are going to store the values in each iteration
	vector<double> meanvec1;
	meanvec1.push_back(mu1);

	vector<double> meanvec2;
	meanvec2.push_back(mu2);

	vector<double> variancevec1;
	variancevec1.push_back(sigma1);

	vector<double> variancevec2;
	variancevec2.push_back(sigma2);

	vector<double> probability;
	probability.push_back(p);

	vector<double> xval(N);
	
	std::normal_distribution<double> dist1(0.0, 1);
	std::normal_distribution<double> dist2(0.5, 0.7);

	// Initialize the vector from the two distributions
	for (int i = 0; i < N; i++) {

		// Mersenne Twister random engine 
		std::mt19937 eng(std::chrono::system_clock::now().time_since_epoch().count());

		if (flip1()) {
			xval[i] = dist1(eng);
		}
		else xval[i] = dist2(eng);
	}
	
	double epsilon = 0.1;
	static int i = 0;
	bool indicator = true;

	// Enter recurrence

	do {

		probability.push_back(probability_1(probability[i], xval, meanvec1[i], meanvec2[i], variancevec1[i], variancevec2[i])); // probabbility 

		meanvec1.push_back(mean_1(probability[probability.size() - 1], xval, meanvec1[i], meanvec2[i], variancevec1[i], variancevec2[i]));	// mean 1

		variancevec1.push_back(variance_1(probability[probability.size() - 1], xval, meanvec1[i + 1], meanvec2[i + 1], variancevec1[i], variancevec2[i])); // variance 1

		meanvec2.push_back(mean_2(probability[probability.size() - 1], xval, meanvec1[i], meanvec2[i], variancevec1[i], variancevec2[i])); // mean 2

		variancevec2.push_back(variance_2(probability[probability.size() - 1], xval, meanvec1[i + 1], meanvec2[i + 1], variancevec1[i], variancevec2[i])); // variance 2 

		i++;	// Increment the dummy counter 

		// exit the loop if...
		if ((abs(probability[i] - probability[i - 1]) < epsilon) &&
			(abs(meanvec1[i] - meanvec1[i - 1]) < epsilon) &&
			(abs(meanvec2[i] - meanvec2[i - 1]) < epsilon) &&
			(abs(variancevec1[i] - variancevec1[i - 1]) < epsilon) &&
			(abs(variancevec2[i] - variancevec2[i - 1]) < epsilon)) {
			indicator = false;
		}

	} while (indicator);
	
	cout << "\n" << endl;
	cout << "PROBABILITY MLE: " << probability[i] << endl;
	cout << "\n" << endl;
	cout << "MEAN 1 MLE: " << meanvec1[i] << endl;
	cout << "VARIANCE 1 MLE: " << variancevec1[i] << endl;
	cout << "\n" << endl;
	cout << "MEAN 2 MLE: " << meanvec2[i] << endl;
	cout << "VARIANCE 2 MLE: " << variancevec2[i] << endl;
	cout << "\n" << endl;
	cout << "\n" << endl;
	
	return 0;
}
