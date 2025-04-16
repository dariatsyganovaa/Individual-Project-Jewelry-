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

    int arr[6] = {1, 2, 3, 4, 5, 6};
    TVector<int> vec3(arr, 6);
    std::cout << "vec3 (size constructor = 6):\n";
    std::cout << " - Capacity: " << vec3.capacity() << "\n";
    std::cout << " - Size: " << vec3.size() << "\n";
    std::cout << " - Is empty? " << (vec3.is_empty() ? "Yes" : "No") << "\n\n";

    std::cout << " The element at the beginning - " << vec3.front() << "\n";
    std::cout << " The element at the end - " << vec3.back() << "\n";
    std::cout << " address data " << vec3.begin() << "\n";
    std::cout << " address data " << vec3.end() << "\n";
	system("pause");
	return 0;
}