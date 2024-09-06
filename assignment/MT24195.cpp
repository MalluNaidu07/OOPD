#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> // For std::exit

using namespace std;

const int NUM_YEARS = 29; // Number of years from 2001 to 2029

class Data {
public:
    int year;
    double in_rate;
    double growth_price;

    Data() : year(0), in_rate(0.0), growth_price(0.0) {}
    Data(int year, double in_rate, double growth_price) 
        : year(year), in_rate(in_rate), growth_price(growth_price) {}
};

class InflationRate {
public:
    Data rates[NUM_YEARS];

    void readCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open file!" << endl;
            return;
        }

        string line;
        int index = 0;
        getline(file, line); // Skip header line
        while (getline(file, line) && index < NUM_YEARS) {
            stringstream ss(line);
            string yearstr, in_rate_str, growth_price_str;
            getline(ss, yearstr, ',');
            getline(ss, growth_price_str, ',');
            getline(ss, in_rate_str, ',');

            yearstr = yearstr.substr(0, 4);
            rates[index] = Data(stoi(yearstr), stod(in_rate_str), stod(growth_price_str));
            ++index;
        }

        file.close();
    }
};

class TaxCalculator {
public:
    double initialPrice;
    int purchaseYear;
    int sellingYear;

    TaxCalculator(double price, int pYear, int sYear)
        : initialPrice(price), purchaseYear(pYear), sellingYear(sYear) {}

    double calculateSellingPrice(const Data rates[]) {
        double sellingPrice = initialPrice;
        for (int i = 0; i < NUM_YEARS; ++i) {
            if (rates[i].year > purchaseYear && rates[i].year <= sellingYear) {
                double growthFactor = (1 + ((rates[i].growth_price) / 100.0));
                sellingPrice *= growthFactor;
            }
        }
        return sellingPrice;
    }

    

    double calculateLTCG(double sellingPrice) {
        double gain = sellingPrice - initialPrice;
        return gain > 0 ? 1.25 * gain : 0; // 20% tax on the gain
    }

    
};

int main() {
    string path;
    double initialPrice = 50.0; // in lakhs
    int purchaseYear = 2010;
    int sellingYear;

    cout << "Enter year of selling: ";
    cin >> sellingYear;  

    if (sellingYear < purchaseYear || sellingYear > 2029) {
        cerr << "Invalid selling year!" << endl;
        return 1;
    }

    InflationRate inflationRate;
    inflationRate.readCSV("price-inflation.csv");

    TaxCalculator taxCalculator(initialPrice, purchaseYear, sellingYear);
    double sellingPrice = taxCalculator.calculateSellingPrice(inflationRate.rates);
    double LTCG = taxCalculator.calculateLTCG(sellingPrice);

    cout << "Estimated Selling Price: Rs " << sellingPrice << " lakhs" << endl;
    cout << "Long-term Capital Gains Tax (LTCG): Rs " << LTCG << " lakhs" << endl;

    

    return 0;
}
