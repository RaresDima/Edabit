#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using std::cout;
using std::string;
using std::vector;
using std::stringstream;


class FactorialTable
{
private:
    vector<long long int> table;

    void compute_up_to(int n)
    {
        for (int i = highest_computed() + 1; i <= n; i++)
            table.push_back(table[i - 1] * i);
    }

public:
    FactorialTable(int n = 0)
    {
        table.push_back(1);
        compute_up_to(n);
    }

    int highest_computed()
    {
        return table.size() - 1;
    }

    long long int operator()(int k)
    {
        compute_up_to(k);
        return table[k];
    }

    long long int operator()(int a, int b)
    {
        long long int result = 1;
        for (int i = a; i <= b; i++)
            result *= i;
        return result;
    }

    void print()
    {
        cout << "FactorialTable(" << table.size() - 1 << ") [\n";
        for (size_t i = 0; i < table.size(); i++)
        {
            cout << "    " << i << " -> " << table[i] << "\n";
        }
        cout << "]\n";
    }
};

class CombinationsOfNTable
{
private:
    FactorialTable factorial;
    vector<long long int> table;
    int n;

public:
    CombinationsOfNTable(int n, int k = 0) : n(n), table(n + 1, -1), factorial(n) {}

    bool is_computed(int k)
    {
        return table[k] != -1;
    }

    long long int operator()(int k)
    {
        if (not is_computed(k))
        {
            int max_div, min_div;
            if (k > n / 2)
            {
                max_div = k;
                min_div = n - k;
            }
            else
            {
                max_div = n - k;
                min_div = k;
            }
            int start_term = max_div + 1;

            long long int result = factorial(start_term, n) / factorial(min_div);

            //table[k] = factorial(n) / factorial(k) / factorial(n - k);
            table[k] = result;
        }
        return table[k];
    }
};


string render_term(int coeff, string term, int power)
{
    stringstream rendered_term;
    
    switch (coeff)
    {
    case 0:
        return "";

    case 1:
        // nothing
        break;

    default:  // coeff > 1
        rendered_term << coeff;
    }

    switch (power)
    {
    case 0:
        // nothing
        break;

    case 1:
        rendered_term << term;
        break;

    default:
        rendered_term << term << "^" << power;
    }

    return rendered_term.str();
}

std::string formula(int n) 
{
    if (n == 0)
        return "1";

    bool negative_power = n < 0;
    if (negative_power)
        n *= -1;

    CombinationsOfNTable cn(n);

    stringstream formula;

    if (negative_power)
        formula << "1/(";

    formula << render_term(1, "a", n);

    for (int i = 1; i < n; i++)
    {
        formula << "+" << render_term(cn(i), "a", n - i) << render_term(1, "b", i);
    }

    formula << "+" << render_term(1, "b", n);

    if (negative_power)
        formula << ")";

    return formula.str();
}

int main()
{
    FactorialTable fac(27);
    CombinationsOfNTable c(13);
    cout << c(1) << " Hello World!\n";
    cout << formula(27);
    fac.print();
}