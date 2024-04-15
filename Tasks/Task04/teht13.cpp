#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string s1 = "kissa";
    std::string s2;

    s2 = s1;
    // Molemmat osoittavat samaan muistiosoitteeseen

    s2 = std::move(s1);
    // s2 osoittaa s1:n vanhaan muistiosoitteeseen ja s1 on tyhjä

    std::vector<std::string> string_vector;
    std::string s3 = "koira";

    string_vector.push_back(s3);
    // luodaan kopio s3:sta

    string_vector.push_back(std::move(s3));
    // vektori ottaa s3:n muistiosoitteen ja s3 on tyhjä

    return 0;
}
