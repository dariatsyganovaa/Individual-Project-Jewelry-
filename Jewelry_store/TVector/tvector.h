#pragma once
#include <cstddef> // size_t
#include <stdexcept>

enum State { empty, busy, deleted };
template<class T>
class TVector {
	T* _data;
	size_t _capacity;
	size_t _size;
	size_t _deleted;
	State* _states;
public:
	// Конструктор по умолчанию
	TVector(size_t size = 0);

	// Конструктор с параметром для задания размера
	TVector(size_t);

	// Конструктор, принимающий массив данных
	TVector(const T*, size_t);

	// Конструктор копирования
	TVector(const TVector<T>&);

	// Деструктор
	~TVector();
	
	inline bool is_empty() const noexcept;

	inline T& front() noexcept;
	inline T& back() noexcept;

	inline T* begin() noexcept;
	inline T* end() noexcept;

	inline T* data() noexcept; // сеттер
	inline const T* data() const noexcept; // геттер
	/* ... */
private:
	inline bool is_full() const noexcept;
	/* ... */
};

