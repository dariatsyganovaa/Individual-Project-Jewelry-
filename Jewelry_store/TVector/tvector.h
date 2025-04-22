#pragma once
#include <cstddef> // size_t
#include <stdexcept>

#define STEP_OF_CAPACITY 15

enum State { empty, busy, deleted };
template<class T>
class TVector {
	T* _data; //массив объектов
	size_t _capacity; //выделенный размер
	size_t _size; //видимый размер
	size_t _deleted; //количество удаленных
	State* _states; //статус ячеек
public:
	// Конструктор по умолчанию
	TVector();

	// Конструктор с параметром для задания размера
	TVector(size_t);

	// Конструктор, принимающий массив данных
	TVector(const T*, size_t);

	// Конструктор копирования
	TVector(const TVector<T>&);

	// Деструктор
	~TVector();

	size_t capacity() const noexcept;
	size_t size() const noexcept;

	inline bool is_empty() const noexcept;

	inline T& front() noexcept;
	inline T& back() noexcept;

	inline T* begin() noexcept;
	inline T* end() noexcept;

	//inline T* data() noexcept; // сеттер
	inline const T* data() const noexcept; // геттер
	/* ... */

	void push_front_elem(const T&);
	void push_back_elem(const T&);
	void insert_elem(const T&, size_t);
private:
	inline bool is_full() const noexcept;
	/* ... */
};

template<class T>
TVector<T>::TVector() {
	_data = nullptr;
	_capacity = STEP_OF_CAPACITY;
	_size = 0; 
	_deleted = 0; 
	_states = nullptr;

}

template<class T>
TVector<T>::TVector(size_t size) {
	_data = nullptr;
	_size = size;
	_capacity = (size / STEP_OF_CAPACITY + 1) * STEP_OF_CAPACITY;
	_deleted = 0;
	_states = nullptr;
	

	if (_capacity > 0) {
		_data = new T[_capacity];   
		_states = new State[_capacity];  
		for (size_t i = 0; i < _capacity; i++) {
			_states[i] = empty;
		}
	}
}

template<class T>
TVector<T>::TVector(const T* arr, size_t size) {
	_capacity = (size / STEP_OF_CAPACITY + 1) * STEP_OF_CAPACITY;
	_data = new T[_capacity];
	_size = size;

	for (size_t i = 0; i < _capacity; ++i) {
		_data[i] = arr[i];
	}
	_deleted = 0;
	_states = nullptr;

	if (_capacity > 0) {
		_states = new State[_capacity];
		for (size_t i = 0; i < _size; i++) {
			_states[i] = busy;
		}
		for (size_t i = _size; i < _capacity; i++) {
			_states[i] = empty;
		}
	}
}

template<class T>
TVector<T>::TVector(const TVector<T>& other) {
	_data = nullptr;
	_capacity = (other._size / STEP_OF_CAPACITY + 1) * STEP_OF_CAPACITY;
	_size = other._size;
	_deleted = other._deleted;
	_states = nullptr;

	if (_capacity > 0) {
		_data = new T[_capacity];
		_states = new State[_capacity];
		for (size_t i = 0; i < _capacity; i++) {
			_data[i] = other._data[i];
			_states[i] = other._states[i];
		}
	}
}

template<class T>
TVector<T>::~TVector() {
	delete[] _data;
	delete[] _states;
}

//template<class T>
//inline T* TVector<T>::data() noexcept { return _data; }

template<class T>
inline const T* TVector<T>::data() const noexcept {	return _data; }

template<class T>
size_t TVector<T>::capacity() const noexcept { return _capacity; }

template<class T>
size_t TVector<T>::size() const noexcept { return _size; }

template<class T>
inline T& TVector<T>::front() noexcept {
	return _data[0];
}

template<class T>
inline T& TVector<T>::back() noexcept {
	for (size_t i = _size; i > 0; --i) {
		if (_states[i - 1] == busy) {
			return _data[i - 1];
		}
	}
}

template<class T>
inline T* TVector<T>::begin() noexcept {
	// Возвращает указатель на первый элемент
	// Если вектор пуст (_data == nullptr или _size == 0), вернет указатель "на конец"
	return _data;
}

template<class T>
inline T* TVector<T>::end() noexcept {
	// Возвращает указатель на позицию ПОСЛЕ последнего элемента
	// Если вектор пуст, вернет то же значение, что и begin()
	return _data + _size;
}

template<class T>
inline bool TVector<T>::is_empty() const noexcept {
	if (_size == 0) {
		return 1;
	}
	else if (_size > 0) {
		for (size_t i = 0; i < _capacity; i++) {
			if (_states[i] == busy) {
				return 0;
			}
		}
	}
	return 1;
}

//вставка 
template<class T>
void TVector<T>::push_front_elem(const T& value) {
	for (size_t i = _size; i > 0; --i) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];
	}

	_data[0] = value;
	_states[0] = busy;

	++_size;
}

template<class T>
void TVector<T>::push_back_elem(const T& value) {
	if (_size > 0 && _states[_size - 1] != busy) {
		throw std::logic_error("Cannot push_back: last element must be in 'busy' state!");
	}
	_data[_size] = value;
	_states[_size] = busy;

	++_size;
}

template<class T>
void TVector<T>::insert_elem(const T& value, size_t pos) {  
	//нужна проверка состояний
	for (size_t i = _size - 1; i > pos; i--) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];
	}
	_data[pos] = value;
	_states[pos] = busy;

	++_size;
}