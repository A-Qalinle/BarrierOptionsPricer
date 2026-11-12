#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
using namespace std;

class Option {
public:
    double strike_price;
    double maturity;
    double barrier;
    Option(double a, double b, double c) : strike_price(a), maturity(b), barrier(c) {}
};

class Market_Price {
public:
    double interest_rate;
    double volatility;
    double current_stock_price;
    Market_Price(double a, double b, double c): interest_rate(a), volatility(b), current_stock_price(c) {}
};

class Monte_Carlo_Pricer {
public:
    int number_runs;
    mt19937 gen;
    normal_distribution<> dist;

    Monte_Carlo_Pricer(int a): number_runs(a), gen(random_device{}()), dist(0.0, 1.0) {}

        double simulation(double maturity, double strike_price, double interest_rate, double volatility, double current_stock_price, double barrier, vector<double>* path_out = nullptr) {
        double dt = 0.01;
        int state = 0;
        double sqrt_dt = sqrt(dt);
        int steps = static_cast<int>(maturity / dt);
        vector<double> price(steps + 1);
        price[0] = current_stock_price;

        for (int i = 0; i < steps; i++) {
            double Z = dist(gen);
            price[i + 1] = price[i] * exp((interest_rate - 0.5 * volatility * volatility) * dt + volatility * sqrt_dt * Z);
            if (price[i+1] >= barrier && state == 0) state = 1;
        }
        if (path_out) *path_out = price;
        return state * max(price.back() - strike_price, 0.0);
    }

    double monte_carlo_pricer(double maturity, double strike_price, double interest_rate, double volatility, double current_stock_price, double barrier) {
        double sum_payoffs = 0.0;
        for (int i = 0; i < number_runs; i++) {
            sum_payoffs += simulation(maturity, strike_price, interest_rate, volatility, current_stock_price, barrier);
        }
        return exp(-interest_rate * maturity) * (sum_payoffs / number_runs);
    }
};

int main() {
    Option option1(100, 1.0, 120); // Strike, maturity (years), barrier
    Market_Price market_price1(0.02, 0.05, 100); // r, sigma, S0
    Monte_Carlo_Pricer pricer(1000000);

    double option_price = pricer.monte_carlo_pricer(option1.maturity, option1.strike_price,  market_price1.interest_rate, market_price1.volatility, market_price1.current_stock_price, option1.barrier);

    cout << "Up-and-in Call Option Price: " << option_price << endl;

    ofstream out("data/paths.csv");
        for (int p = 0; p < 100; p++) {
        vector<double> path;
        pricer.simulation(option1.maturity, option1.strike_price, market_price1.interest_rate, market_price1.volatility, market_price1.current_stock_price, option1.barrier, &path);
        for (size_t i = 0; i < path.size(); i++) out << path[i] << (i + 1 < path.size() ? "," : "\n");
}


    return 0;
}