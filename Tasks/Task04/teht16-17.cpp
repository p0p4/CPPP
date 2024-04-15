#include <iostream>

int main()
{
    try
    {
        int* array = new int[10000000000000];

        delete[] array;
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << e.what() << '\n';
        // std::bad_alloc
    }

    // 16 Pitäisikö muistinvarauksen epäonnistumiseen varautua ohjelmoitaessa?
        // Parempi varautua koska muistinvaraus voi aiheuttaa erilaisia ongelmia

    return 0;
}

// 17 Kannattaako poikkeuksia käyttää C++:ssa? Miksi tai miksi ei?
    // Argumentteja puolesta on, että se on helpommin luettavaa, ymmärrettävää.
    // Argumentteja vastaan on, että sitä käyttäessä aiheuttaa helposti ongelmia
    // jotkut kääntäjät eivät tue poikkeuksia
    // 