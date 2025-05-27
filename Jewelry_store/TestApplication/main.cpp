#include <iostream>
#include "date.h"
#include "FIO.h"
#include "ctime.h"
#include "docs.h"
#include "tvector.h"
#include <windows.h>

void set_color(int text_color, int bg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | text_color);
}

namespace TestSystem {
    int count_success = 0, count_failed = 0;

    void start_test(bool(*test)(), const char* name_of_test) {
        set_color(2, 0);
        std::cout << "[ RUN      ]";
        set_color(7, 0);
        std::cout << name_of_test << std::endl;

        bool status = test();

        if (status == true) {
            set_color(2, 0);
            std::cout << "[       OK ]" << std::endl;
            count_success++;
        }
        else {
            set_color(4, 0);
            std::cout << "[  FAILED  ]" << std::endl;
            count_failed++;
        }
        set_color(7, 0);
    }

    template <class T>
    bool check(const T& expected, const T& actual) {
        if (expected == actual) {
            return true;
        }
        else {
            std::cerr << "Expected result is " << expected << ", but actual is " << actual << "." << std::endl;
            return false;
        }
    }

    void print_init_info() {
        set_color(2, 0);
        std::cout << "[==========] " << std::endl;
        set_color(7, 0);
    }

    void print_final_info() {
        set_color(2, 0);
        std::cout << "[==========] ";
        set_color(7, 0);
        std::cout << count_success + count_failed << " test" << (count_success + count_failed > 1 ? "s" : "") << " ran." << std::endl;
        set_color(2, 0);
        std::cout << "[  PASSED  ] ";
        set_color(7, 0);
        std::cout << count_success << " test" << (count_success > 1 ? "s" : "") << std::endl;
        if (count_failed > 0) {
            set_color(4, 0);
            std::cout << "[  FAILED  ] ";
            set_color(7, 0);
            std::cout << count_failed << " test" << (count_failed > 1 ? "s." : ".") << std::endl;
        }
    }
};

bool test_1_default_constructor() {
    TVector<int> vec;
    return TestSystem::check((size_t)0, vec.size()) &&
        TestSystem::check(true, vec.is_empty());
}
bool test_2_size_constructor() {
    TVector<int> vec(7);
    if (vec.size() != 7) {
        return false;
    }
    if (vec.capacity() != 15) {
        return false;
    }
    for (size_t i = 0; i < vec.capacity(); i++) {
        if (vec.states()[i] != empty) {
            return false;
        }
    }
    return true;
}
bool test_3_size_null_constructor() {
    TVector<int> vec(0);
    return TestSystem::check((size_t)0, vec.size()) &&
        TestSystem::check(true, vec.is_empty());
}
bool test_4_copy_constructor() {
    int arr[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    TVector<int> vec1(arr, 9);
    TVector<int> vec2(vec1);
    if (vec2.size() != 9) {
        return false;
    }
    if (vec2.capacity() != 15) {
        return false;
    }
    for (size_t i = 0; i < vec2.size(); i++) {
        if (vec1.data()[i] != vec2.data()[i]) {
            return false;
        }
    }
    return true;
}

bool test_5_copy_null_constructor() {
    TVector<int> vec1(0);
    TVector<int> vec2(vec1);
    return TestSystem::check((size_t)0, vec2.capacity()) &&
        TestSystem::check((size_t)0, vec2.size()) &&
        TestSystem::check(true, vec2.is_empty());
}
bool test_6_mass_constructor() {
    int arr[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    TVector<int> vec(arr, 9);
    if (vec.size() != 9) {
        return false;
    }
    if (vec.capacity() != 15) {
        return false;
    }
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec.data()[i] != arr[i]) {
            return false;
        }
        if (vec.states()[i] != busy) {
            return false;
        }
    }
    return true;
}
bool test_7_mass_null_constructor() {
    int* arr = nullptr;
    TVector<int> vec(arr, 0);
    return TestSystem::check((size_t)0, vec.capacity()) &&
        TestSystem::check((size_t)0, vec.size()) &&
        TestSystem::check(true, vec.is_empty());
}
bool test_8_destructor() {
    TVector<int> vec;
    vec.push_back_elem(111); 

    vec.~TVector();

    if (vec.size() != 0) {
        return false;
    }
    if (vec.capacity() != 0) {
        return false;
    }
    if (vec.data() != nullptr) {
        return false;
    }
    if (vec.states() != nullptr) {
        return false;
    }
    return true;
}
bool test_9_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec(arr, 14);
    int expected_res = 1;
    int actual_res = vec.front();
    return TestSystem::check(expected_res, actual_res);
}
bool test_10_back_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec(arr, 14);
    int expected_res = 14;
    int actual_res = vec.back();
    return TestSystem::check(expected_res, actual_res);
}
bool test_11_begin_vector_with_size() {
    TVector<int> vec(6);
    return TestSystem::check(&vec[0], vec.begin());
}
bool test_12_begin_empty_vector() {
    TVector<int> empty_vec;
    return TestSystem::check(empty_vec.end(), empty_vec.begin());
}
bool test_13_end_vector_with_size() {
    int size = 6;
    TVector<int> vec(size);
    return TestSystem::check(&vec[size], vec.end());
}
bool test_14_end_empty_vector() {
    TVector<int> empty_vec;
    return TestSystem::check(empty_vec.end(), empty_vec.begin());
}
bool test_t_assign() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec(arr, 14);
    vec.assign(4, 444);
    int expected_res[4] = { 444, 444, 444, 444 };
    for (size_t i = 0; i < 4; i++) {
        if (vec[i] != expected_res[i]) {
            return false;
        }
    }
    return true;
}

bool test_t_assign_in_empty_vec() {
    TVector<int> vec;
    vec.assign(4, 444);
    int expected_res[4] = { 444, 444, 444, 444 };
    for (size_t i = 0; i < 4; i++) {
        if (vec[i] != expected_res[i]) {
            return false;
        }
    }
    return true;
}

bool test_assign_zero_count() {
    TVector<int> vec;
    vec.assign(0, 42);
    return vec.size() == 0 && vec.capacity() == 0;
}

bool test_assign_large_count() {
    TVector<int> vec;
    int huge = 1000000; // либо size_t
    vec.assign(huge, 42);

    if (vec.size() != huge) {
        return false;
    }

    for (size_t i = 0; i < huge; i += 1000) { // проверяем с шагом 1000 чтобы не занимало много времени
        if (vec[i] != 42) {
            return false;
        }
    }
    return true;
}

bool test_repeated_assign() {
    TVector<int> vec;
    vec.assign(10, 1);
    vec.assign(5, 2);

    if (vec.size() != 5) {
        return false;
    }

    for (size_t i = 0; i < 5; ++i) {
        if (vec[i] != 2) {
            return false;
        }
    }
    return true;
}
bool test_n_push_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 4, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_front_elem(4);

    if (vec1.size() != 15) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }
    return true;
}

bool test_n_push_back_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 4 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_back_elem(4);

    if (vec1.size() != 15) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }
    return true;

}

bool test_n_insert_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 1, 2, 999, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.insert_elem(999, 3);

    if (vec1.size() != 15) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_n_push_front_elem_in_empty_mass() {
    int expected_result[1] = { 1 };
    TVector<int> vec1, vec2(expected_result, 1);
    vec1.push_front_elem(1);

    if (vec1.size() != 1) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }

    return true;
}
bool test_n_push_back_elem_in_empty_mass() {
    int expected_result[1] = { 1 };
    TVector<int> vec1, vec2(expected_result, 1);
    vec1.push_back_elem(1);

    if (vec1.size() != 1) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }

    return true;
}
bool test_n_insert_elem_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.insert_elem(555, 1);
    }

    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_n_push_front_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[18] = { 555, 666, 777, 888, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int mass[4] = { 555, 666, 777, 888 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 18);
    vec1.push_front_elems(mass, 4);

    if (vec1.size() != 18) {
        return false;
    }


    if (vec1 != vec2) {
        return false;

    }

    return true;
}

bool test_n2_push_back_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 555, 666, 777, 888 };
    int mass[4] = { 555, 666, 777, 888 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 18);
    vec1.push_back_elems(mass, 4);

    if (vec1.size() != 18) {
        return false;
    }


    if (vec1 != vec2) {
        return false;

    }

    return true;
}

bool test_n3_insert_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[18] = { 1, 2, 3, 4, 5, 555, 666, 777, 888, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int mass[4] = { 555, 666, 777, 888 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 18);
    vec1.insert_elems(6, mass, 4);

    if (vec1.size() != 18) {
        return false;
    }


    if (vec1 != vec2) {
        return false;

    }

    return true;
}
bool test_n_push_front_elems_in_empty_mass() {
    int expected_result[4] = { 555, 666, 777, 888 };
    int mass[4] = { 555, 666, 777, 888 };
    TVector<int> vec1, vec2(expected_result, 4);
    vec1.push_front_elems(mass, 4);

    if (vec1.size() != 4) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }


    return true;
}
bool test_n_push_back_elems_in_empty_mass() {
    int expected_result[4] = { 555, 666, 777, 888 };
    int mass[4] = { 555, 666, 777, 888 };
    TVector<int> vec1, vec2(expected_result, 4);
    vec1.push_back_elems(mass, 4);

    if (vec1.size() != 4) {
        return false;
    }

    if (vec1 != vec2) {
        return false;
    }


    return true;
}

bool test_n_insert_elems_in_empty_mass() {
    TVector<int> vec1;
    int mass[4] = { 555, 666, 777, 888 };
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.insert_elems(1, mass, 4);
    }

    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_n3_insert_elem_error() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.insert_elem(5, 16);
    }

    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_n3_insert_elems_error() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int mass[4] = { 555, 666, 777, 888 };

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.insert_elems(18, mass, 4);
    }

    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}

bool test_n_pop_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_front_elem();
    int expected_result[14] = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 13) {
        return false;
    }

    for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_n_pop_back_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_back_elem();
    int expected_result[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };

    if (vec1.size() != 13) {
        return false;
    }

    for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_n_erase_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.erase_elem(4);
    int expected_result[14] = { 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 13) {
        return false;
    }

    for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_n_pop_front_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_front_elems(2);
    int expected_result[14] = { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 12) {
        return false;
    }

    for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_pop_back_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_back_elems(2);
    int expected_result[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };

    if (vec1.size() != 12) {
        return false;
    }

    for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_n_erase_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.erase_elems(4, 2);
    int expected_result[14] = { 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 12) {
        return false;
    }

    for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}

bool test_n_pop_front_elem_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_front_elem();
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_n_pop_back_elem_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_back_elem();
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_n_erase_elem_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.erase_elem(4);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}

bool test_n_erase_elem_error() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.erase_elem(15);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}


bool test_n_pop_front_elems_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_front_elems(2);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_n_pop_back_elems_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_back_elems(2);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_n_erase_elems_in_empty_mass() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.erase_elems(4, 2);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}

bool test_n_pop_front_elems_error() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_front_elems(15);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}

bool test_n_pop_back_elems_error() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_back_elems(15);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}


bool test_n_erase_elems_error() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.erase_elems(4, 15);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}

bool test_n_erase_elems_pos_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.erase_elems(-1, 10);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_k_find_first_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_first_elem(vec1, 6);
    int expected_res = 5;
    return TestSystem::check(actual_res, expected_res);
}

bool test_k_not_find_first_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_first_elem(vec1, 111);
    int expected_res = -1;
    return TestSystem::check(actual_res, expected_res);
}

bool test_k_not_find_last_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_last_elem(vec1, 111);
    int expected_res = -1;
    return TestSystem::check(actual_res, expected_res);
}

bool test_k_not_find_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 10, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int count = 0;
    int* expected_res = find_elems(vec1, 111, count);
    bool actual_res = true;
    if (expected_res != nullptr || count != 0) {
        actual_res = false;
    }
    return true;
}

bool test_k2_find_last_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 10, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_last_elem(vec1, 10);
    int expected_res = 9;
    return TestSystem::check(actual_res, expected_res);
}

bool test_k2_find_all_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 10, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int count = 0;
    int* actual_res = find_elems(vec1, 10, count);
    int expected_res[2] = { 5, 9 };
    for (int i = 0; i < 2; i++) {
        if (actual_res[i] != expected_res[i]) {
            return false;
        }
    }
    return true;
}

bool test_k_find_first_elem_in_empty_mass() {
    TVector<int> vec1;

    bool expected_result = true;
    bool actual_result = false;
    try {
        find_first_elem(vec1, 6);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}

bool test_k2_find_last_elem_in_empty_mass() {
    TVector<int> vec1;

    bool expected_result = true;
    bool actual_result = false;
    try {
        find_last_elem(vec1, 6);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_k2_find_all_elem_in_empty_mass() {
    TVector<int> vec1;

    bool expected_result = true;
    bool actual_result = false;
    try {
        int count = 0;
        find_elems(vec1, 6, count);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_s_emplace() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.emplace(6, 11037);
    int expected_res[14] = { 1, 2, 3, 4, 5, 6, 11037, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec2(expected_res, 14);

    if (vec1 != vec2) {
        return false;
    }
    return true;
}

bool test_s_randomize() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    randomize(vec1);

    if (vec1.size() != 14 || vec1.capacity() != 15) {
        return false;
    }
    return true;
}
bool test_s_randomize_empty_mass() {
    TVector<int> vec1;
    randomize(vec1);

    if (vec1.size() != 0 || vec1.capacity() != 0) {
        return false;
    }
    return true;
}
bool test_s_randomize_one_elem() {
    TVector<int> vec1(1);
    randomize(vec1);

    if (vec1.size() != 1 || vec1.capacity() != 15) {
        return false;
    }
    return true;
}
//bool test_s2_emplace() {
//    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
//    TVector<int> vec1(arr, 14);
//    int mass[3] = { 444, 555, 666 };
//    vec1.emplace(6, mass);
//    int expected_res[17] = { 1, 2, 3, 4, 5, 6, 444, 555, 666, 8, 9, 10, 11, 12, 13, 14 };
//    TVector<int> vec2(expected_res, 14);
//
//    if (vec1 != vec2) {
//        return false;
//    }
//    return true;
//}


bool test_clear() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec(arr, 14);
    vec.clear();
    if (vec.size() != 0) return false;
    for (size_t i = 0; i < vec.capacity(); ++i) {
        if (vec.states()[i] != empty) return false;
    }
    return true;
}

bool test_null_clear() {
    TVector<char> vec;
    vec.clear();
    if (vec.size() != 0) return false;
    if (vec.capacity() != 0) return false;
    return true;
}

bool test_reserve_return() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec(arr, 14);
    vec.reserve(13);
    if (vec.capacity() != 15) return false;
    return true;
}

bool test_reserve() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec(arr, 14);
    vec.reserve(17);
    if (vec.capacity() != 17) return false;
    return true;
}

bool test_reserve_null_arr() {
    TVector<int> vec;
    vec.reserve(5);
    if (vec.capacity() != 5) return false;
    if (vec.size() != 0) return false;
    if (vec.data() == nullptr) return false;
    for (size_t i = 0; i < vec.capacity(); i++) {
        if (vec.states()[i] != empty) return false;
    }
    return true;
}

bool test_shrink_to_fit_null_size() {
    TVector<int> vec;
    vec.shrink_to_fit();
    if (vec.capacity() != 0) return false;
    if (vec.size() != 0) return false;
    if (vec.data() != nullptr) return false;
    if (vec.states() != nullptr) return false;
    return true;
}

bool test_shrink_to_fit() {
    int arr[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec(arr, 16);
    vec.shrink_to_fit();
    if (vec.capacity() != 16) return false;
    for (size_t i = 0; i < vec.capacity(); i++) {
        if (vec.states()[i] != busy) return false;
    }

    return true;
}

//bool test_shrink_to_fit() {
//    TVector<int> vec;
//    vec.reserve(5);
//    return true;
//}
bool test_n_pop_front_elem_after_push_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 2, 1, 1, 1, 1, 1,1, 1, 1, 1,1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_front_elem(4);
    vec1.pop_front_elem();

    if (vec1.size() != 14) {
        return false;
    }

    std::cout << std::endl;

    for (size_t i = 0; i < 15; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_pop_back_elem_after_push_back_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_back_elem(4);
    vec1.pop_back_elem();

    if (vec1.size() != 14) {
        return false;
    }

    for (size_t i = 0; i < 15; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_erase_elem_after_insert_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.insert_elem(4, 4);
    vec1.erase_elem(4);

    if (vec1.size() != 14) {
        return false;
    }

    for (size_t i = 0; i < 15; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_pop_front_elems_after_push_front_elems() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int arr2[2] = { 1, 2 };
    int expected_result[18] = { 2, 2, 1, 1, 1, 1, 1, 1,1, 1, 1, 1,1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr1, 16), vec2(expected_result, 18);
    vec1.push_front_elems(arr2, 2);
    vec1.pop_front_elems(2);

    if (vec1.size() != 16) {
        return false;
    }
    for (size_t i = 0; i < 18; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_pop_back_elems_after_push_back_elems() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int arr2[2] = { 1, 2 };
    int expected_result[18] = { 1, 1, 1, 1, 1, 1,1, 1, 1, 1,1, 1, 1, 1, 1, 1, 0, 0 };
    TVector<int> vec1(arr1, 16), vec2(expected_result, 18);
    vec1.push_back_elems(arr2, 2);
    vec1.pop_back_elems(2);

    if (vec1.size() != 16) {
        return false;
    }
    for (size_t i = 0; i < 18; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_erase_elems_after_insert_elems() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int arr2[2] = { 1, 2 };
    int expected_result[18] = { 1, 1, 2, 2, 1, 1,1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr1, 16), vec2(expected_result, 18);
    vec1.insert_elems(3, arr2, 2);
    vec1.erase_elems(3, 2);

    if (vec1.size() != 16) {
        return false;
    }
    for (size_t i = 0; i < 18; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_n_pop_front_elem_after_emplace_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(0, 5);
    vec1.pop_front_elem();

    if (vec1.size() != 13) {
        return false;
    }

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_pop_back_elem_after_emplace_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(13, 5);
    vec1.pop_back_elem();

    if (vec1.size() != 13) {
        return false;
    }

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_erase_elem_after_emplace_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(2, 5);
    vec1.erase_elem(3);

    if (vec1.size() != 13) {
        return false;
    }

    for (int i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;

}
bool test_n_found_elem_after_pop_elem() {
    int arr[14] = { 1, 2, 3, 4, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result = 2;
    TVector<int> vec1(arr, 14);
    vec1.erase_elems(1, 2);
    int actual_result = find_first_elem(vec1, 1);
    return TestSystem::check(expected_result, actual_result);

}
int main() {
    TestSystem::print_init_info();

    TestSystem::start_test(test_1_default_constructor, "TVector.test_1_default_constructor");
    TestSystem::start_test(test_2_size_constructor, "TVector.test_2_size_constructor");
    TestSystem::start_test(test_3_size_null_constructor, "TVector.test_3_size_null_constructor");
    TestSystem::start_test(test_4_copy_constructor, "TVector.test_4_copy_constructor");
    TestSystem::start_test(test_5_copy_null_constructor, "TVector.test_5_copy_null_constructor");
    TestSystem::start_test(test_6_mass_constructor, "TVector.test_6_mass_constructor");
    TestSystem::start_test(test_7_mass_null_constructor, "TVector.test_7_mass_null_constructor");
    TestSystem::start_test(test_8_destructor, "TVector.test_8_destructor");

    TestSystem::start_test(test_9_front_elem, "TVector.test_9_front_elem");
    TestSystem::start_test(test_10_back_elem, "TVector.test_10_back_elem");
    TestSystem::start_test(test_11_begin_vector_with_size, "TVector.test_11_begin_vector_with_size");
    TestSystem::start_test(test_12_begin_empty_vector, "TVector.test_12_begin_empty_vector");
    TestSystem::start_test(test_13_end_vector_with_size, "TVector.test_13_end_vector_with_size");
    TestSystem::start_test(test_14_end_empty_vector, "TVector.test_14_end_empty_vector");
    TestSystem::start_test(test_t_assign, "TVector.test_t_assign");
    TestSystem::start_test(test_t_assign_in_empty_vec, "TVector.test_t_assign_in_empty_vec");
    TestSystem::start_test(test_assign_zero_count, "TVector.test_assign_zero_count");
    TestSystem::start_test(test_assign_large_count, "TVector.test_assign_large_count");





    //вставка
    TestSystem::start_test(test_n_push_front_elem, "TVector.test_n_push_front_elem");
    TestSystem::start_test(test_n_push_back_elem, "TVector.test_n_back_front_elem");
    TestSystem::start_test(test_n_insert_elem, "TVector.test_n_insert_elem");
    TestSystem::start_test(test_n_push_front_elem_in_empty_mass, "TVector.test_n_push_front_elem_in_empty_mass");
    TestSystem::start_test(test_n_push_back_elem_in_empty_mass, "TVector.test_n_back_front_elem_in_empty_mass");
    TestSystem::start_test(test_n_insert_elem_in_empty_mass, "TVector.test_n_insert_elem_in_empty_mass");
    TestSystem::start_test(test_n_push_front_elems, "TVector.test_n_push_front_elems");
    TestSystem::start_test(test_n2_push_back_elems, "TVector.test_n_push_back_elems");
    TestSystem::start_test(test_n3_insert_elems, "TVector.test_n_insert_elems");
    TestSystem::start_test(test_n_push_front_elems_in_empty_mass, "TVector.test_n_push_front_elems_in_empty_mass");
    TestSystem::start_test(test_n_push_back_elems_in_empty_mass, "TVector.test_n_back_front_elems_in_empty_mass");
    TestSystem::start_test(test_n_insert_elems_in_empty_mass, "TVector.test_n_insert_elems_in_empty_mass");
    TestSystem::start_test(test_n3_insert_elems_error, "TVector.test_n3_insert_elems_error");
    TestSystem::start_test(test_n3_insert_elem_error, "TVector.test_n_insert_elem_error");
    TestSystem::start_test(test_n3_insert_elems_error, "TVector.test_n_insert_elems_error");

    //удаление
    TestSystem::start_test(test_n_pop_front_elem, "TVector.test_n_pop_front_elem");
    TestSystem::start_test(test_n_pop_front_elems, "TVector.test_n_pop_front_elems");
    TestSystem::start_test(test_n_pop_front_elem_in_empty_mass, "TVector.test_n_pop_front_elem_in_empty_mass");
    TestSystem::start_test(test_n_pop_front_elems_in_empty_mass, "TVector.test_n_pop_front_elems_in_empty_mass");
    TestSystem::start_test(test_n_pop_back_elems_error, "TVector.test_n_pop_back_elems_error");
    TestSystem::start_test(test_n_pop_back_elem, "TVector.test_n_pop_back_elem");
    TestSystem::start_test(test_n_pop_back_elems, "TVector.test_n_pop_back_elems");
    TestSystem::start_test(test_n_pop_back_elem_in_empty_mass, "TVector.test_n_pop_back_elem_in_empty_mass");
    TestSystem::start_test(test_n_pop_back_elems_in_empty_mass, "TVector.test_n_pop_back_elems_in_empty_mass");
    TestSystem::start_test(test_n_pop_back_elems_error, "TVector.test_n_pop_back_elems_error");
    TestSystem::start_test(test_n_erase_elem, "TVector.test_n_erase_elem");
    TestSystem::start_test(test_n_erase_elems, "TVector.test_n_erase_elems");
    TestSystem::start_test(test_n_erase_elem_in_empty_mass, "TVector.test_n_erase_elem_in_empty_mass");
    TestSystem::start_test(test_n_erase_elems_in_empty_mass, "TVector.test_n_erase_elems_in_empty_mass");
    TestSystem::start_test(test_n_erase_elem_error, "TVector.test_n_erase_elem_error");
    TestSystem::start_test(test_n_erase_elem_error, "TVector.test_n_erase_elems_error");
    TestSystem::start_test(test_n_erase_elems_pos_out_of_range, "TVector.test_n_erase_elems_pos_out_of_range");

    

    //поиск
    TestSystem::start_test(test_k_find_first_elem, "TVector.test_k_find_first_elem");
    TestSystem::start_test(test_k_not_find_first_elem, "TVector.test_k_not_find_first_elem");
    TestSystem::start_test(test_k2_find_last_elem, "TVector.test_k2_find_last_elem");
    TestSystem::start_test(test_k_not_find_last_elem, "TVector.test_k_not_find_last_elem");
    TestSystem::start_test(test_k2_find_all_elem, "TVector.test_k2_find_all_elem");
    TestSystem::start_test(test_k_not_find_elems, "TVector.test_k_not_find_elems");
    TestSystem::start_test(test_k_find_first_elem_in_empty_mass, "TVector.test_k_find_first_elem_in_empty_mass");
    TestSystem::start_test(test_k2_find_last_elem_in_empty_mass, "TVector.test_k2_find_last_elem_in_empty_mass");
    TestSystem::start_test(test_k2_find_all_elem_in_empty_mass, "TVector.test_k2_find_all_elem_in_empty_mass");

    //замена
    TestSystem::start_test(test_s_emplace, "TVector.test_s_emplace");

    //память
    TestSystem::start_test(test_clear, "TVector.test_clear");
    TestSystem::start_test(test_null_clear, "TVector.test_null_clear");
    TestSystem::start_test(test_reserve_return, "TVector.test_reserve_return");
    TestSystem::start_test(test_reserve, "TVector.test_reserve");
    TestSystem::start_test(test_reserve_null_arr, "TVector.test_reserve_null_arr");
    TestSystem::start_test(test_shrink_to_fit_null_size, "TVector.test_shrink_to_fit_null_size");
    TestSystem::start_test(test_shrink_to_fit, "TVector.test_shrink_to_fit");

    //at
    //перемешивание
    TestSystem::start_test(test_s_randomize, "TVector.test_s_randomize");
    TestSystem::start_test(test_s_randomize_empty_mass, "TVector.test_s_randomize_empty_mass");
    TestSystem::start_test(test_s_randomize_one_elem, "TVector.test_s_randomize_one_elem");
    //двойные
    TestSystem::start_test(test_n_pop_front_elem_after_push_front_elem, "TVector.test_n_pop_front_elem_after_push_front_elem");
    TestSystem::start_test(test_n_pop_back_elem_after_push_back_elem, "TVector.test_n_pop_back_elem_after_push_back_elem");
    TestSystem::start_test(test_n_erase_elem_after_insert_elem, "TVector.test_n_erase_elem_after_insert_elem");
    TestSystem::start_test(test_n_pop_front_elems_after_push_front_elems, "TVector.test_n_pop_front_elems_after_push_front_elems");
    TestSystem::start_test(test_n_pop_back_elems_after_push_back_elems, "TVector.test_n_pop_back_elems_after_push_back_elems");
    TestSystem::start_test(test_n_erase_elems_after_insert_elems, "TVector.test_n_erase_elems_after_insert_elems");
    //поиск после удаления 9 (3 поиска на 3 удаления) и (1 поиск и 3 удалени я нескольких)
    TestSystem::start_test(test_n_found_elem_after_pop_elem, "TVector.test_n_found_elem_after_pop_elem");

    TestSystem::start_test(test_n_pop_front_elem_after_emplace_elem, "TVector.test_n_pop_front_elem_after_emplace_elem");
    TestSystem::start_test(test_n_pop_back_elem_after_emplace_elem, "TVector.test_n_pop_back_elem_after_emplace_elem");
    TestSystem::start_test(test_n_erase_elem_after_emplace_elem, "TVector.test_n_erase_elem_after_emplace_elem");
    //удаление после замены нескольких


    //замена после удаления 3
    //вставка после замена
    //замена после вставки
    //поиск после замены 3 
   //вставка после clear
    //удаление после clear 

    //деструктор
    int arr[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 111, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    TVector<int> vec1(arr, 32);
    int mass[4] = { 555, 666, 777, 888 };
    auto it = vec1.begin();
    std::cout << *(it) << std::endl;
    vec1.print_elems();
    vec1.print_states();
    TestSystem::print_final_info();

    TVector<int> vec111;
    std::cout << "vec111 (default constructor):\n";
    std::cout << " - Capacity: " << vec111.capacity() << "\n";
    std::cout << " - Size: " << vec111.size() << "\n";
    std::cout << " - Is empty? " << (vec111.is_empty() ? "Yes" : "No") << "\n\n";

    TVector<int> vec222(5);
    std::cout << "vec222 (size constructor = 5):\n";
    std::cout << " - Capacity: " << vec222.capacity() << "\n";
    std::cout << " - Size: " << vec222.size() << "\n";
    std::cout << " - Is empty? " << (vec222.is_empty() ? "Yes" : "No") << "\n\n";

    int arr3[9] = { 6, 2, 6, 4, 5, 6, 7, 6, 9 };
    TVector<int> vec333(arr3, 9);
    std::cout << "vec333 (size constructor = 9):\n";
    std::cout << " - Capacity: " << vec333.capacity() << "\n";
    std::cout << " - Size: " << vec333.size() << "\n";
    std::cout << " - Is empty? " << (vec333.is_empty() ? "Yes" : "No") << "\n\n";

    vec222.push_front_elem(3);
    std::cout << " The element at the end vec222 - " << vec222.back() << "\n";
    std::cout << " address data " << vec333.begin() << "\n";
    std::cout << " address data " << vec333.end() << "\n";

    std::cout << " data mass " << vec333.data() << "\n";


    std::cout << " The element at the end - " << vec333.back() << "\n";

    vec333.print_elems();
    //vec333.push_back_elem(505);
    //vec333.insert_elem(6, 3);
    //vec333.push_front_elem(1);
    vec333.erase_elem(4);
    vec333.erase_elem(4);

    int first = find_first_elem(vec333, 6);
    // Поиск последнего вхождения
    int last = find_last_elem(vec333, 6);

    vec333.print_elems();
    vec333.print_states();

    system("pause");
    return 0;
} // доделать хоара 