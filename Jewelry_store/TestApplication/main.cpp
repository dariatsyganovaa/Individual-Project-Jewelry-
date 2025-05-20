#include <iostream>
#include "date.h"
#include "FIO.h"
#include "ctime.h"
#include "docs.h"
#include "tvector.h"

int main() {
    TVector<int> vec1;
    std::cout << "vec1 (default constructor):\n";
    std::cout << " - Capacity: " << vec1.capacity() << "\n";
    std::cout << " - Size: " << vec1.size() << "\n";
    std::cout << " - Is empty? " << (vec1.is_empty() ? "Yes" : "No") << "\n\n";

    TVector<int> vec2(5);
    std::cout << "vec2 (size constructor = 5):\n";
    std::cout << " - Capacity: " << vec2.capacity() << "\n";
    std::cout << " - Size: " << vec2.size() << "\n";
    std::cout << " - Is empty? " << (vec2.is_empty() ? "Yes" : "No") << "\n\n";

    int arr[9] = {6, 2, 6, 4, 5, 6, 7, 6, 9};
    TVector<int> vec3(arr, 9);
    std::cout << "vec3 (size constructor = 9):\n";
    std::cout << " - Capacity: " << vec3.capacity() << "\n";
    std::cout << " - Size: " << vec3.size() << "\n";
    std::cout << " - Is empty? " << (vec3.is_empty() ? "Yes" : "No") << "\n\n";

    vec2.push_front_elem(3);
    std::cout << " The element at the end vec2 - " << vec2.back() << "\n";
    std::cout << " address data " << vec3.begin() << "\n";
    std::cout << " address data " << vec3.end() << "\n";

    std::cout << " data mass " << vec3.data() << "\n";
    

    std::cout << " The element at the end - " << vec3.back() << "\n";

    vec3.print_elems();
    //vec3.push_back_elem(505);
    //vec3.insert_elem(6, 3);
    //vec3.push_front_elem(1);
    vec3.erase_elem(4);
    vec3.erase_elem(4);

    int first = find_first_elem(vec3, 6);
    // Поиск последнего вхождения
    int last = find_last_elem(vec3, 6);
    
    vec3.print_elems();
    vec3.print_states();
    
    

	system("pause");
	return 0;
}