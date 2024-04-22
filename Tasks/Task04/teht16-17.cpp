#include <iostream>

int main()
{
    try
    {
        double size = 10'000'000'000'000;

        std::cout << "Trying to allocate " << size * sizeof(int) << " bytes of memory" << std::endl;

        int* array = new int[size];

        std::cout << "Allocated" << std::endl;

        for (int i = 0; i < size; i++)
        {
            array[i] = i;
        }
        std::cout << "Filled" << std::endl;

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