#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::string s1 = "kissa";
    std::cout << &s1 << std::endl;
    std::string s2;
    std::cout << &s2 << std::endl;

    int a = 5;
    std::cout << &a << std::endl;

    s2 = s1;
    std::cout << &s2 << std::endl;
    // Molemmat osoittavat samaan muistiosoitteeseen

    s2 = std::move(s1);
    // s2 osoittaa s1:n vanhaan muistiosoitteeseen ja s1 on tyhjä

    std::cout << &s2 << std::endl;

    std::vector<std::string> string_vector;
    std::string s3 = "koira";

    string_vector.push_back(s3);
    // luodaan kopio s3:sta

    string_vector.push_back(std::move(s3));
    // vektori ottaa s3:n muistiosoitteen ja s3 on tyhjä

    return 0;
}
