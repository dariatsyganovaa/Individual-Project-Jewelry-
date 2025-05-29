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
bool test_15_push_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 111, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_front_elem(111);

    if (vec1.size() != 15) {
        return false;
    }
    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_16_push_front_elems() {
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
bool test_17_push_back_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 111 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_back_elem(111);

    if (vec1.size() != 15) {
        return false;
    }
    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_18_push_back_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int mass[4] = { 555, 666, 777, 888 };
    int expected_result[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 555, 666, 777, 888 };
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
bool test_19_insert_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 1, 2, 111, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.insert_elem(111, 3);

    if (vec1.size() != 15) {
        return false;
    }
    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_20_insert_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int mass[4] = { 555, 666, 777, 888 };
    int expected_result[18] = { 1, 2, 555, 666, 777, 888, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 18);
    vec1.insert_elems(3, mass, 4);

    if (vec1.size() != 18) {
        return false;
    }
    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_21_push_front_elem_in_empty_vec() {
    int expected_result[1] = { 111 };
    TVector<int> vec1, vec2(expected_result, 1);
    vec1.push_front_elem(111);

    if (vec1.size() != 1) {
        return false;
    }
    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_22_push_front_elems_in_empty_vec() {
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
bool test_23_push_back_elem_in_empty_vec() {
    int expected_result[1] = { 111 };
    TVector<int> vec1, vec2(expected_result, 1);
    vec1.push_back_elem(111);

    if (vec1.size() != 1) {
        return false;
    }
    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_24_push_back_elems_in_empty_vec() {
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
bool test_25_insert_elem_in_empty_vec() {
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
bool test_26_insert_elems_in_empty_vec() {
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
bool test_27_insert_elem_pos_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.insert_elem(555, 16);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_28_insert_elems_pos_out_of_range() {
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
bool test_29_pop_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_front_elem();
    int expected_result[14] = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 13) {
        return false;
    }
    for (size_t i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_30_pop_front_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_front_elems(2);
    int expected_result[14] = { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 12) {
        return false;
    }
    for (size_t i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_31_pop_front_elem_in_empty_vec() {
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
bool test_32_pop_front_elems_in_empty_vec() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_front_elems(1);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_33_pop_front_elems_count_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_front_elems(16);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_34_pop_back_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_back_elem();
    int expected_result[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };

    if (vec1.size() != 13) {
        return false;
    }
    for (size_t i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_35_pop_back_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_back_elems(5);
    int expected_result[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

    if (vec1.size() != 9) {
        return false;
    }
    for (size_t i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_36_pop_back_elem_in_empty_vec() {
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
bool test_37_pop_back_elems_in_empty_vec() {
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
bool test_38_pop_back_elems_count_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.pop_back_elems(16);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_39_erase_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.erase_elem(4);
    int expected_result[14] = { 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    if (vec1.size() != 13) {
        return false;
    }
    for (size_t i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_40_erase_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    vec1.erase_elems(4, 4);
    int mass[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 }; //перевыделение памяти
    if (vec1.size() != 10) {
        return false;
    }
    vec1.erase_elems(4, 2);
    int expected_result[14] = { 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
    if (vec1.size() != 8) {
        return false;
    }

    /*for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;*/

    for (size_t i = 0; i < 14; i++) {
        if (vec1.states()[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_41_erase_elem_in_empty_vec() {
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
bool test_42_erase_elems_in_empty_vec() {
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
bool test_43_erase_elem_pos_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.erase_elem(16);
    }
    catch (const std::exception& ex) {
        actual_result = true;
    }

    return TestSystem::check(expected_result, actual_result);
}
bool test_44_erase_elems_pos_out_of_range() {
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
bool test_45_emplace() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.emplace(6, 111);
    int expected_res[14] = { 1, 2, 3, 4, 5, 111, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec2(expected_res, 14);

    if (vec1 != vec2) {
        return false;
    }
    return true;
}
bool test_46_emplace_in_empty_vec() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.emplace(1, 111);
    }
    catch (const std::out_of_range& ex) {
        actual_result = true;
    }
    return true;
}
bool test_47_emplace_pos_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.emplace(15, 111);
    }
    catch (const std::out_of_range& ex) {
        actual_result = true;
    }
    return true;
}
bool test_48_emplace_not_busy() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_front_elem();
    vec1.emplace(1, 111);
    int mass[13] = { 111, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec2(mass, 13);
    vec1.print_elems();
    bool expected_res = true;
    bool actual_res = (vec1 == vec2);
    return TestSystem::check(expected_res, actual_res);
}
bool test_49_assign() {
    TVector<int> vec1(40);
    vec1.assign(4, 111);
    int expected_res[4] = { 111, 111, 111, 111 };
    for (size_t i = 0; i < 4; i++) {
        if (vec1[i] != expected_res[i]) {
            return false;
        }
    }
    return true;
}
bool test_50_assign_in_empty_vec() {
    TVector<int> vec1; 
    vec1.assign(4, 111);
    int expected_res[4] = { 111, 111, 111, 111 };
    for (size_t i = 0; i < 4; i++) {
        if (vec1[i] != expected_res[i]) {
            return false;
        }
    }
    return true;
}
bool test_51_assign_with_null_size() {
    TVector<int> vec1(20);
    vec1.assign(0, 111);

    if (vec1.size() != 0) {
        return false;
    }
    if (vec1.capacity() != 0) {
        return false;
    }
    return true;
}

bool test_52_assign_to_large_vec() {
    TVector<int> vec1;
    vec1.assign(1000000, 42);

    if (vec1.size() != 1000000) {
        return false;
    }
    for (size_t i = 0; i < 1000000; i += 1000) {
        if (vec1[i] != 42) {
            return false;
        }
    }
    return true;
}
bool test_53_repeat_assign() {
    TVector<int> vec1(5);
    vec1.assign(10, 222);
    vec1.assign(4, 111);

    if (vec1.size() != 4) {
        return false;
    }
    int expected_res[4] = { 111, 111, 111, 111 };
    for (size_t i = 0; i < 4; i++) {
        if (vec1[i] != expected_res[i]) {
            return false;
        }
    }
    return true;
}
bool test_54_at() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int expected_res = 1;
    int actual_res = vec1.at(0);
    return TestSystem::check(actual_res, expected_res);
}
bool test_55_at_index_out_of_range() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.at(15);
    }
    catch (const std::out_of_range& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_56_at_with_an_invalid_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.erase_elems(5, 10);

    /*for (size_t i = 0; i < 14; i++) {
        std::cout << vec1.states()[i] << " ";
    }
    std::cout << std::endl;*/

    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.at(9);
    }
    catch (const std::logic_error& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_57_at_in_empty_vec() {
    TVector<int> vec1;
    bool expected_result = true;
    bool actual_result = false;
    try {
        vec1.at(0);
    }
    catch (const std::out_of_range& ex) {
        actual_result = true;
    }
    return TestSystem::check(expected_result, actual_result);
}
bool test_58_clear() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.pop_front_elems(2);
    vec1.clear();
    if (vec1.size() != 0) {
        return false;
    }
    if (vec1.capacity() != 0) {
        return false;
    }
    return true;
}
bool test_59_shrink_to_fit() {
    TVector<int> vec1(16);
    vec1.resize(7);
    vec1.shrink_to_fit();
    if (vec1.capacity() != 7) {
        return false;
    }   
    return true;
}
bool test_60_shrink_to_fit_with_null_size() {
    TVector<int> vec1;
    vec1.shrink_to_fit();
    if (vec1.capacity() != 0) {
        return false;
    }
    if (vec1.size() != 0) {
        return false;
    }
    if (vec1.data() != nullptr) {
        return false;
    }
    if (vec1.states() != nullptr) {
        return false;
    }
    return true;
}
bool test_61_reserve() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.reserve(17);
    if (vec1.capacity() != 17) {
        return false;
    }
    return true;
}
bool test_62_reserve_return() {
    TVector<int> vec1(40);
    vec1.reserve(16);
    if (vec1.capacity() != 45) {
        return false;
    }
    return true;
}
bool test_63_reserve_in_empty_vec() {
    TVector<int> vec1;
    vec1.reserve(5);
    if (vec1.capacity() != 5) {
        return false;
    }
    if (vec1.data() == nullptr) {
        return false;
    }
    return true;
}
bool test_64_resize_increasing_the_size() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.resize(25); //заполняет нулями
    if (vec1.size() != 25) {
        return false;
    }
    return true;
}
bool test_65_resize_reducing_the_size() {
    int arr[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec1(arr, 16);
    vec1.resize(5);
    if (vec1.size() != 5) {
        return false;
    }
    return true;
}
bool test_66_resize_increasing_the_size_with_value() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    vec1.resize(25, 111); 
    if (vec1.size() != 25) {
        return false;
    }
    return true;
}
bool test_67_resize_in_empty_vec() {
    TVector<int> vec1;
    vec1.resize(5);
    if (vec1.size() != 5) {
        return false;
    }
    return true;
}
bool test_68_resize_in_empty_vec_with_value() {
    TVector<int> vec1;
    vec1.resize(5, 111);
    if (vec1.size() != 5) {
        return false;
    }
    for (size_t i = 0; i < 5; i++) {
        if (vec1.data()[i] != 111) {
            return false;
        }
    }
    return true;
}
bool test_69_the_assignment_operator() {
    int arr[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec1(20);
    TVector<int> vec2(arr, 16);
    vec1 = vec2;
    if (vec1.size() != 16) {
        return false;
    }
    for (size_t i = 0; i < 16; i++) {
        if (vec1.data()[i] != arr[i]) {
            return false;
        }
    }
    for (size_t i = 0; i < 16; i++) {
        if (vec1.states()[i] != busy) {  
            return false;
        }
    }
    return true;
}
bool test_70_the_assignment_operator_for_empty_vec() {
    int arr[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec1(arr, 16);
    TVector<int> vec2;
    vec1 = vec2;
    if (vec1.size() != 0) {
        return false;
    }
    return true;
}
bool test_71_comparison_operator() {
    int arr[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec1(arr, 16);
    TVector<int> vec2(vec1);
    bool expected_res = true;
    bool actual_res = (vec1 == vec2);
    return TestSystem::check(expected_res, actual_res);
}
bool test_72_comparison_operator_for_empty_vec() {
    TVector<int> vec1;
    TVector<int> vec2;
    return vec1 == vec2;
}
bool test_73_comparison_operator_after_erase() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec1(arr1, 16);
    int arr2[15] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec2(arr2, 15);
    vec1.pop_front_elem();
    bool expected_res = true;
    bool actual_res = (vec1 == vec2);
    return TestSystem::check(expected_res, actual_res);
}
bool test_74_indexing_operator() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec1(arr1, 16);
    int expected_res = 1;
    int actual_res = vec1[0];
    //std::cout << actual_res;
    return (expected_res == actual_res);
}
bool test_75_indexing_operator_modification() {
    int arr1[3] = { 1, 2, 3 };
    TVector<int> vec1(arr1, 3);
    vec1[1] = 100; 
    return vec1[0] == 1 && vec1[1] == 100 && vec1[2] == 3;
}
bool test_76_find_first_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_first_elem(vec1, 6);
    int expected_res = 5;
    return TestSystem::check(actual_res, expected_res);
}
bool test_77_not_find_first_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_first_elem(vec1, 111);
    int expected_res = -1;
    return TestSystem::check(actual_res, expected_res);
}
bool test_78_find_last_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 10, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_last_elem(vec1, 10);
    int expected_res = 9;
    return TestSystem::check(actual_res, expected_res);
}
bool test_79_not_find_last_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int actual_res = find_last_elem(vec1, 111);
    int expected_res = -1;
    return TestSystem::check(actual_res, expected_res);
}
bool test_80_find_elems() {
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
bool test_81_not_find_elems() {
    int arr[14] = { 1, 2, 3, 4, 5, 10, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    int count = 0;
    int* actual_res = find_elems(vec1, 111, count);
    bool expected_res = true;
    if (actual_res != nullptr || count != 0) {
        expected_res = false;
    }
    return true;
}
bool test_82_find_first_elem_in_empty_vec() {
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
bool test_83_find_last_elem_in_empty_vec() {
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
bool test_84_find_elems_in_empty_vec() {
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
bool test_85_randomize() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14);
    randomize(vec1);

    if (vec1.size() != 14 || vec1.capacity() != 15) {
        return false;
    }
    return true;
}
bool test_86_randomize_in_empty_vec() {
    TVector<int> vec1;
    randomize(vec1);

    if (vec1.size() != 0 || vec1.capacity() != 0) {
        return false;
    }
    return true;
}
bool test_87_randomize_one_elem() {
    TVector<int> vec1(1);
    randomize(vec1);

    if (vec1.size() != 1 || vec1.capacity() != 15) {
        return false;
    }
    return true;
}
bool test_88_pop_front_elem_after_push_front_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[15] = { 2, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 15);
    vec1.push_front_elem(4);
    vec1.pop_front_elem();

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
bool test_89_pop_back_elem_after_push_back_elem() {
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
bool test_90_erase_elem_after_insert_elem() {
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
bool test_91_pop_front_elems_after_push_front_elems() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int arr2[2] = { 1, 2 };
    int expected_result[18] = { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
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
bool test_92_pop_back_elems_after_push_back_elems() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int arr2[2] = { 1, 2 };
    int expected_result[18] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
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
bool test_93_erase_elems_after_insert_elems() {
    int arr1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int arr2[2] = { 1, 2 };
    int expected_result[18] = { 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
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
bool test_94_pop_front_elem_after_emplace_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(1, 5);
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
bool test_95_pop_back_elem_after_emplace_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(14, 5);
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
bool test_96_erase_elem_after_emplace_elem() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(3, 5);
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
bool test_97_find_elems_after_pop_elem() {
    int arr[20] = { 1, 2, 3, 4, 1, 6, 7, 8, 9, 10, 11, 12, 1, 14, 15, 18, 1, 21, 11, 1 };
    TVector<int> vec1(arr, 20);
    vec1.erase_elems(4, 3);

    int expected_result[4] = { 0, 9, 13, 16};
    int count = 0;
    int* actual_result = find_elems(vec1, 1, count);
    if (count != 4) {
        return false;
    }
    for (int i = 0; i < count; i++) {
        if (actual_result[i] != expected_result[i]) {
            return false;
        }
    }
    return true;
}
bool test_98_emplace_after_erase_elem() {
    int arr[20] = { 1, 2, 3, 4, 1, 6, 7, 8, 9, 10, 11, 12, 1, 14, 15, 18, 1, 21, 11, 1 };
    TVector<int> vec1(arr, 20);
    vec1.erase_elems(5, 2);
    vec1.emplace(5, 111);
    int expected_result[18] = { 1, 2, 3, 4, 111, 8, 9, 10, 11, 12, 1, 14, 15, 18, 1, 21, 11, 1 };
    TVector<int> vec2(expected_result, 18);
    
    return (vec1 == vec2);
}
bool test_99_push_front_elem_after_pop_front_elem() {
    int arr[20] = { 1, 2, 3, 4, 1, 6, 7, 8, 9, 10, 11, 12, 1, 14, 15, 18, 1, 21, 11, 1 };
    TVector<int> vec1(arr, 20);
    vec1.pop_front_elem();
    vec1.push_front_elem(111);
    if (vec1.size() != 20) { 
        return false;
    }
    if (vec1[0] != 111) {
        return false;
    }
    return true;
}
bool test_100_find_elems_after_emplace() {
    int arr[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    int expected_result[14] = { 1, 2, 111, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
    TVector<int> vec1(arr, 14), vec2(expected_result, 14);
    vec1.emplace(3, 111);
    int count = 0;
    int* actual_res = find_elems(vec1, 3, count);
    bool expected_res = true;
    if (actual_res != nullptr || count != 0) {
        expected_res = false;
    }
    return true;
}
bool test_101_accessing_an_elem_using_an_iterator() {
    int arr[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 111, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    TVector<int> vec1(arr, 32);
    int mass[4] = { 555, 666, 777, 888 };
    auto it = vec1.begin();
    return  (*(it) == 1);
}
bool test_102_hoara_sort() {
    int arr[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 111, 222, 333, 444 };

    TVector<int> vec1(arr, 32);
    randomize(vec1);
    hoara_sort(vec1);

    for (size_t i = 1; i < 32; i++) {
        if (vec1.states()[i] == busy && vec1.states()[i - 1] == busy) {
            if (vec1[i] < vec1[i - 1]) {
                return false;
            }
        }
    }
    return true;
}
bool test_103_hoara_sort_after_erase_elem() {
    int arr[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 111, 222, 333, 444 };

    TVector<int> vec1(arr, 32);
    vec1.erase_elem(5);
    randomize(vec1);
    hoara_sort(vec1);

    if (vec1.size() != 31) {
        return false;
    }

    for (size_t i = 1; i < 31; i++) {
        if (vec1.states()[i] == busy && vec1.states()[i - 1] == busy) {
            if (vec1[i] < vec1[i - 1]) {
                return false;
            }
        }
    }
    return true;
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

    TestSystem::start_test(test_15_push_front_elem, "TVector.test_15_push_front_elem");
    TestSystem::start_test(test_16_push_front_elems, "TVector.test_16_push_front_elems");
    TestSystem::start_test(test_17_push_back_elem, "TVector.test_17_push_back_elem");
    TestSystem::start_test(test_18_push_back_elems, "TVector.test_18_push_back_elems");
    TestSystem::start_test(test_19_insert_elem, "TVector.test_19_insert_elem");
    TestSystem::start_test(test_20_insert_elems, "TVector.test_20_insert_elems");

    TestSystem::start_test(test_21_push_front_elem_in_empty_vec, "TVector.test_21_push_front_elem_in_empty_vec");
    TestSystem::start_test(test_22_push_front_elems_in_empty_vec, "TVector.test_22_push_front_elems_in_empty_vec");
    TestSystem::start_test(test_23_push_back_elem_in_empty_vec, "TVector.test_23_push_back_elem_in_empty_vec");
    TestSystem::start_test(test_24_push_back_elems_in_empty_vec, "TVector.test_24_push_back_elems_in_empty_vec");
    TestSystem::start_test(test_25_insert_elem_in_empty_vec, "TVector.test_25_insert_elem_in_empty_vec");
    TestSystem::start_test(test_26_insert_elems_in_empty_vec, "TVector.test_26_insert_elems_in_empty_vec");
    TestSystem::start_test(test_27_insert_elem_pos_out_of_range, "TVector.test_27_insert_elem_pos_out_of_range");
    TestSystem::start_test(test_28_insert_elems_pos_out_of_range, "TVector.test_28_insert_elems_pos_out_of_range");

    TestSystem::start_test(test_29_pop_front_elem, "TVector.test_29_pop_front_elem");
    TestSystem::start_test(test_30_pop_front_elems, "TVector.test_30_pop_front_elems");
    TestSystem::start_test(test_31_pop_front_elem_in_empty_vec, "TVector.test_31_pop_front_elem_in_empty_vec");
    TestSystem::start_test(test_32_pop_front_elems_in_empty_vec, "TVector.test_32_pop_front_elems_in_empty_vec");
    TestSystem::start_test(test_33_pop_front_elems_count_out_of_range, "TVector.test_33_pop_front_elems_count_out_of_range");

    TestSystem::start_test(test_34_pop_back_elem, "TVector.test_34_pop_back_elem");
    TestSystem::start_test(test_35_pop_back_elems, "TVector.test_35_pop_back_elems");
    TestSystem::start_test(test_36_pop_back_elem_in_empty_vec, "TVector.test_36_pop_back_elem_in_empty_vec");
    TestSystem::start_test(test_37_pop_back_elems_in_empty_vec, "TVector.test_37_pop_back_elems_in_empty_vec");
    TestSystem::start_test(test_38_pop_back_elems_count_out_of_range, "TVector.test_38_pop_back_elems_count_out_of_range");
    
    TestSystem::start_test(test_39_erase_elem, "TVector.test_39_erase_elem");
    TestSystem::start_test(test_40_erase_elems, "TVector.test_40_erase_elems");
    TestSystem::start_test(test_41_erase_elem_in_empty_vec, "TVector.test_41_erase_elem_in_empty_vec");
    TestSystem::start_test(test_42_erase_elems_in_empty_vec, "TVector.test_42_erase_elems_in_empty_vec");
    TestSystem::start_test(test_43_erase_elem_pos_out_of_range, "TVector.test_43_erase_elem_pos_out_of_range");
    TestSystem::start_test(test_44_erase_elems_pos_out_of_range, "TVector.test_44_erase_elems_pos_out_of_range");
    
    TestSystem::start_test(test_45_emplace, "TVector.test_45_emplace");
    TestSystem::start_test(test_46_emplace_in_empty_vec, "TVector.test_46_emplace_in_empty_vec");
    TestSystem::start_test(test_47_emplace_pos_out_of_range, "TVector.test_47_emplace_pos_out_of_range");
    TestSystem::start_test(test_48_emplace_not_busy, "TVector.test_48_emplace_not_busy");

    TestSystem::start_test(test_49_assign, "TVector.test_49_assign");
    TestSystem::start_test(test_50_assign_in_empty_vec, "TVector.test_50_assign_in_empty_vec");
    TestSystem::start_test(test_51_assign_with_null_size, "TVector.test_51_assign_with_null_size");
    TestSystem::start_test(test_52_assign_to_large_vec, "TVector.test_52_assign_to_large_vec");
    TestSystem::start_test(test_53_repeat_assign, "TVector.test_53_repeat_assign");

    TestSystem::start_test(test_54_at, "TVector.test_54_at");
    TestSystem::start_test(test_55_at_index_out_of_range, "TVector.test_55_at_index_out_of_range");
    TestSystem::start_test(test_56_at_with_an_invalid_elem, "TVector.test_56_at_with_an_invalid_elem");
    TestSystem::start_test(test_57_at_in_empty_vec, "TVector.test_57_at_in_empty_vec");

    TestSystem::start_test(test_58_clear, "TVector.test_58_clear");
    TestSystem::start_test(test_59_shrink_to_fit, "TVector.test_59_shrink_to_fit");
    TestSystem::start_test(test_60_shrink_to_fit_with_null_size, "TVector.test_60_shrink_to_fit_with_null_size");
    TestSystem::start_test(test_61_reserve, "TVector.test_61_reserve");
    TestSystem::start_test(test_62_reserve_return, "TVector.test_62_reserve_return");
    TestSystem::start_test(test_63_reserve_in_empty_vec, "TVector.test_63_reserve_in_empty_vec");

    TestSystem::start_test(test_64_resize_increasing_the_size, "TVector.test_64_resize_increasing_the_size");
    TestSystem::start_test(test_65_resize_reducing_the_size, "TVector.test_65_resize_reducing_the_size");
    TestSystem::start_test(test_66_resize_increasing_the_size_with_value, "TVector.test_66_resize_increasing_the_size_with_value");
    TestSystem::start_test(test_67_resize_in_empty_vec, "TVector.test_67_resize_in_empty_vec");
    TestSystem::start_test(test_68_resize_in_empty_vec_with_value, "TVector.test_68_resize_in_empty_vec_with_value");
    
    TestSystem::start_test(test_69_the_assignment_operator, "TVector.test_69_the_assignment_operator");
    TestSystem::start_test(test_70_the_assignment_operator_for_empty_vec, "TVector.test_70_the_assignment_operator_for_empty_vec");
    TestSystem::start_test(test_71_comparison_operator, "TVector.test_71_comparison_operator");
    TestSystem::start_test(test_72_comparison_operator_for_empty_vec, "TVector.test_72_comparison_operator_for_empty_vec");
    TestSystem::start_test(test_73_comparison_operator_after_erase, "TVector.test_73_comparison_operator_after_erase");
    TestSystem::start_test(test_74_indexing_operator, "TVector.test_74_indexing_operator");
    TestSystem::start_test(test_75_indexing_operator_modification, "TVector.test_75_indexing_operator_modification");
   
    TestSystem::start_test(test_76_find_first_elem, "TVector.test_76_find_first_elem");
    TestSystem::start_test(test_77_not_find_first_elem, "TVector.test_77_not_find_first_elem");
    TestSystem::start_test(test_78_find_last_elem, "TVector.test_78_find_last_elem");
    TestSystem::start_test(test_79_not_find_last_elem, "TVector.test_79_not_find_last_elem");
    TestSystem::start_test(test_80_find_elems, "TVector.test_80_find_elems");
    TestSystem::start_test(test_81_not_find_elems, "TVector.test_81_not_find_elems");
    TestSystem::start_test(test_82_find_first_elem_in_empty_vec, "TVector.test_82_find_first_elem_in_empty_vec");
    TestSystem::start_test(test_83_find_last_elem_in_empty_vec, "TVector.test_83_find_last_elem_in_empty_vec");
    TestSystem::start_test(test_84_find_elems_in_empty_vec, "TVector.test_84_find_elems_in_empty_vec");

    TestSystem::start_test(test_85_randomize, "TVector.test_85_randomize");
    TestSystem::start_test(test_86_randomize_in_empty_vec, "TVector.test_86_randomize_in_empty_vec");
    TestSystem::start_test(test_87_randomize_one_elem, "TVector.test_87_randomize_one_elem");

    TestSystem::start_test(test_88_pop_front_elem_after_push_front_elem, "TVector.test_88_pop_front_elem_after_push_front_elem");
    TestSystem::start_test(test_89_pop_back_elem_after_push_back_elem, "TVector.test_89_pop_back_elem_after_push_back_elem");
    TestSystem::start_test(test_90_erase_elem_after_insert_elem, "TVector.test_90_erase_elem_after_insert_elem");
    TestSystem::start_test(test_91_pop_front_elems_after_push_front_elems, "TVector.test_91_pop_front_elems_after_push_front_elems");
    TestSystem::start_test(test_92_pop_back_elems_after_push_back_elems, "TVector.test_92_pop_back_elems_after_push_back_elems");
    TestSystem::start_test(test_93_erase_elems_after_insert_elems, "TVector.test_93_erase_elems_after_insert_elems");
    TestSystem::start_test(test_94_pop_front_elem_after_emplace_elem, "TVector.test_94_pop_front_elem_after_emplace_elem");
    TestSystem::start_test(test_95_pop_back_elem_after_emplace_elem, "TVector.test_95_pop_back_elem_after_emplace_elem");
    TestSystem::start_test(test_96_erase_elem_after_emplace_elem, "TVector.test_96_erase_elem_after_emplace_elem");
    TestSystem::start_test(test_97_find_elems_after_pop_elem, "TVector.test_97_find_elems_after_pop_elem");
    TestSystem::start_test(test_98_emplace_after_erase_elem, "TVector.test_98_emplace_after_erase_elem");
    TestSystem::start_test(test_99_push_front_elem_after_pop_front_elem, "TVector.test_99_push_front_elem_after_pop_front_elem");
    TestSystem::start_test(test_100_find_elems_after_emplace, "TVector.test_100_find_elems_after_emplace");
    
    TestSystem::start_test(test_101_accessing_an_elem_using_an_iterator, "TVector.test_101_accessing_an_elem_using_an_iterator");
    TestSystem::start_test(test_102_hoara_sort, "TVector.test_102_hoara_sort");
    TestSystem::start_test(test_103_hoara_sort_after_erase_elem, "TVector.test_103_hoara_sort_after_erase_elem");

    TestSystem::print_final_info();

    /*TVector<int> vec111;
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

    std::cout << " address data " << vec333.begin() << "\n";
    std::cout << " address data " << vec333.end() << "\n";
    std::cout << " data mass " << vec333.data() << "\n";*/

    system("pause");
    return 0;
}