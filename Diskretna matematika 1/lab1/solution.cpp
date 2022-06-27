#include <iostream>
#include <math.h>
using namespace std;

double recursion(double λ_1, double λ_2, double a_0, double a_1, unsigned n)
{
    // 𝑛 ∈ 𝐍
    if (n == 0)
        return a_0;
    if (n == 1)
        return a_1;

    double arr[2] = {0.0};
    arr[0] = a_0;
    arr[1] = a_1;
    double a_2;

    for (int i = 1; i < n; i++)
    { // eg. n == 2
        a_2 = λ_1 * arr[1] + λ_2 * arr[0];
        arr[0] = arr[1];
        arr[1] = a_2;
    }
    return a_2;
}

double formula(double λ_1, double λ_2, double a_0, double a_1, unsigned n)
{
    double discriminant_root = sqrt(λ_1 * λ_1 + 4 * λ_2);
    double x_1 = (λ_1 + discriminant_root) / 2;
    double x_2 = (λ_1 - discriminant_root) / 2;

    double A;
    double B;
    if (abs(x_1 - x_2) < 1e-5)
    {
        // x_1 == x_2: -> a_n = A * (x_1)^n + B * n(x_2)^n
        // a_0 = A + 0
        // a_1 = A + B
        A = a_0;
        B = a_1 - A;
        // cout << A << " " << B;

        return A * pow((x_1), n) + B * n * pow((x_2), n);
    }
    else
    {
        // x_1 != x_2: -> a_n = A * (x_1)^n + B * (x_2)^n
        // a_0 = A + B              / * (-x_2)
        // a_0 = A * x_1 + B * x_2
        A = (a_1 - a_0 * x_2) / (x_1 - x_2);
        B = a_0 - A;
        return A * pow((x_1), n) + B * pow((x_2), n);
    }
}

int main()
{
    // input: 2 -1 2 5 4 (x_1 = x_2) <---> -1 6 0 5 4 (x_1 != x_2) <---> 2 -5 2 5 4 (discriminant_root ∈ I, recursion will produce result)

    double λ_1;
    cout << "Unesite prvi koeficijent λ_1 rekurzivne relacije: ";
    cin >> λ_1;

    double λ_2;
    cout << "Unesite drugi koeficijent λ_2 rekurzivne relacije: ";
    cin >> λ_2;

    double a_0;
    cout << "Unesite vrijednost nultog clana niza a_0: ";
    cin >> a_0;

    double a_1;
    cout << "Unesite vrijednost prvog clana niza a_1: ";
    cin >> a_1;

    unsigned n;
    cout << "Unesite redni broj n trazenog clana niza: ";
    cin >> n;

    cout << "Vrijednost n-tog clana niza pomocu formule: " << formula(λ_1, λ_2, a_0, a_1, n) << endl;
    cout << "Vrijednost n-tog clana niza iz rekurzije: " << recursion(λ_1, λ_2, a_0, a_1, n) << endl;

    return 0;
}
