#pragma once
#include <cstddef> // size_t
#include <stdexcept>

#define STEP_OF_CAPACITY 15

template<typename T> class TVector;
template<typename T> int find_first_elem(const TVector<T>& vec, const T& value);
template<typename T> int find_last_elem(const TVector<T>& vec, const T& value);
template<typename T> int* find_elems(const TVector<T>& vec, const T& value, int& size);

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

	void pop_front_elem();
	void pop_back_elem();
	void erase_elem(size_t);

	template<typename... Args>
	void emplace(size_t, Args&&... );

	friend int find_first_elem<T>(const TVector<T>& vec, const T& value);
	friend int find_last_elem<T>(const TVector<T>& vec, const T& value);
	friend int* find_elems<T>(const TVector<T>& vec, const T& value, int& size);

	void clear() noexcept;

	void assign(size_t, const T&);
	T& at(size_t); //возвращает ссылку на элемент с заданным индексом
	void shrink_to_fit();
	void reserve(size_t);
	void resize(size_t);

	void print_elems();
	void print_states();

private:
	inline bool is_full() const noexcept;
	void allocate_memory(size_t);
	void reallocate_memory(size_t);
	void reallocate_memory_for_delete(size_t);
	void free_memory() noexcept;
};

template<class T>
TVector<T>::TVector() {
	_data = nullptr;
	_capacity = 0;
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
	if (_size >= _capacity) {
		reallocate_memory(_capacity + 15);
	}
	for (size_t i = _size; i > 0; --i) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];
	}

	_data[0] = value;
	_states[0] = busy;

	_size++;
}

template<class T>
void TVector<T>::push_back_elem(const T& value) {
	if (_size >= _capacity) {
		reallocate_memory(_capacity + 15);
	}

	_data[_size] = value;
	_states[_size] = busy;
	_size++;
}

template<class T>
void TVector<T>::insert_elem(const T& value, size_t pos) { 
	if (_size >= _capacity) {
		reallocate_memory(_capacity + 15);
	}

	if (pos > _size) {
		throw std::out_of_range("Insert position out of range");
	}

	for (size_t i = _size; i > pos; i--) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];

	}

	_data[pos] = value;
	_states[pos] = busy;
	_size++;
}

// для удаления нужно перевыделить память при количестве удаленных(15%)
template<class T>
void TVector<T>::pop_front_elem() {
	if (_size == 0) {
		throw std::out_of_range("Can't pop from empty vector");
	}
	for (size_t i = 0; i < _size; i++) {
		if (_states[i] == busy) {
			_states[i] = deleted;
			break;
		}
	}
	_size--;
}

template<class T>
void TVector<T>::pop_back_elem() {
	if (_size == 0) {
		throw std::out_of_range("Can't pop from empty vector");
	}
	for (size_t i = _size - 1; i >= 0; i--) {
		if (_states[i] == busy) {
			_states[i] = empty;
			break;
		}
	}
	_size--;
}

template<class T>
void TVector<T>::erase_elem(size_t pos) {
	if (pos > _size) {
		throw std::out_of_range("Insert position out of range");
	}

	if (_size == 0) {
		throw std::out_of_range("Can't pop from empty vector");
	}
	/*if (_states[pos] != busy) {
	  throw std::logic_error();
	}*/
	int j = 0;
	for (size_t i = 0; i < _size; i++) {
		if (_states[i] == busy) {
			j++;

			if (j == pos) {
				_states[i] = deleted;
				_size--;
				break;
			}
		}
	}
}

template <typename T>
template <typename... Args>
void TVector<T>::emplace(size_t pos, Args&&... args) {
	if (pos >= _size) {
		throw std::out_of_range("Position out of range");
	}
	if (_states[pos] != busy) {
		throw std::logic_error("Can't replace not busy element");
	}

	if (_states[pos] == busy) {  //уничтожаем старый элемент, если он существует
		_data[pos].~T(); 
	}
	new (&_data[pos]) T(std::forward<Args>(args)...); //конструируем новый элемент на месте
	_states[pos] = busy;
}

template<typename T>
int find_first_elem(const TVector<T>& vec, const T& value) {
	int index = 0;
	for (int i = 0; i < vec._capacity; i++) {
		if (vec._states[i] == busy) {
			if (vec._data[i] == value) {
				return index;
			}
			index++;
		}
	}
	return -1;
}

template<typename T>
int find_last_elem(const TVector<T>& vec, const T& value) {
	int index = 0;
	for (int i = 0; i < vec._capacity; i++) {
		if (vec._states[i] == busy) {
			index++;
		}
	}
	for (int i = vec._size - 1; i >= 0; i--) {
		if (vec._states[i] == busy) {
			if (vec._data[i] == value) {
				return index - 1;
			}
			index--;
		}
	}
	return -1;
}

template<typename T>
int* find_elems(const TVector<T>& vec, const T& value, int& size) {
	size = 0;

	for (int i = 0; i < vec._capacity; i++) {
		if (vec._states[i] == busy && vec._data[i] == value) {
			size++;
		}
	}

	if (size == 0) {
		return nullptr;
	}

	int* result = new int[size];
	int index = 0; 
   
	for (int i = 0, j = 0; i < vec._capacity; i++) { //j - позиция в массиве result
		if (vec._states[i] == busy) {
			if (vec._data[i] == value) {
				result[j] = index; j++;
			}
			index++;
		}
	}

	return result;
}

template<class T>
void TVector<T>::clear() noexcept {
	delete[] _data;
	delete[] _states;
	_data = nullptr;
	_states = nullptr;
	_capacity = 0;
	_size = 0;
	_deleted = 0;
}

template<class T>
void TVector<T>::assign(size_t size, const T& value) {
	clear();
	reserve(size);
	for (size_t i = 0; i < size; i++) {
		push_back_elem(value);
	}
}

template<class T>
T& TVector<T>::at(size_t pos) {
	if (pos >= _size) {
		throw std::out_of_range("Index out of range");
	}
	if (_states[pos] != busy) {
		throw std::logic_error("Element at position is not valid");
	}
	return _data[pos];
}

template<class T>
void TVector<T>::shrink_to_fit() {
	if (_size == 0) {
		delete[] _data;
		delete[] _states;
		_data = nullptr;
		_states = nullptr;
		_capacity = 0;
		return;
	}

	T* new_data = new T[_size];
	State* new_states = new State[_size];

	size_t new_index = 0;
	for (size_t i = 0; i < _capacity; ++i) {
		if (_states[i] == busy) {
			new (new_data + new_index) T(std::move(_data[i]));
			new_states[new_index] = busy;
			++new_index;
			_data[i].~T();
		}
	}

	delete[] _data;
	delete[] _states;
	_data = new_data;
	_states = new_states;
	_capacity = _size;
	_deleted = 0;
}

template<class T>
void TVector<T>::reserve(size_t new_capacity) {
	if (new_capacity <= _capacity) {
		return;
	}

	T* new_data = new T[new_capacity];
	State* new_states = new State[new_capacity]();

	for (size_t i = 0; i < _capacity; ++i) {
		if (_states[i] == busy) {
			new (new_data + i) T(std::move(_data[i]));
			new_states[i] = busy;
			_data[i].~T();
		}
	}

	delete[] _data;
	delete[] _states;
	_data = new_data;
	_states = new_states;
	_capacity = new_capacity;
}

template<class T>
void TVector<T>::resize(size_t new_size) {
	if (new_size < _size) {
		for (size_t i = new_size; i < _size; ++i) {
			if (_states[i] == busy) {
				_data[i].~T();
				_states[i] = empty;
			}
		}
		_size = new_size;
	}
	else if (new_size > _size) {
		reserve(new_size);
		for (size_t i = _size; i < new_size; ++i) {
			new (_data + i) T();
			_states[i] = busy;
		}
		_size = new_size;
	}
}




template<class T>
void TVector<T>::print_elems() {
	int entrance = 0;
	for (size_t i = 0; entrance < _size; i++) {
		if (_states[i] == busy) {
			std::cout << _data[i] << " ";
			entrance++;
		}
	}
	std::cout << std::endl;
	/*for (size_t i = 0; i < _capacity; i++) {
		std::cout << _data[i] << " ";
	}
	std::cout << std::endl;*/
}
template<class T>
void TVector<T>::print_states() {
	for (size_t i = 0; i < _capacity; i++) {
		std::cout << _states[i] << " ";
	}
	std::cout << std::endl;
}

template<class T>
void TVector<T>::allocate_memory(size_t new_capacity) {
	T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
	State* new_states = new State[new_capacity];

	// Инициализация состояний
	for (size_t i = 0; i < new_capacity; ++i) {
		new_states[i] = empty;
	}

	_data = new_data;
	_states = new_states;
	_capacity = new_capacity;
}

template<class T>
void TVector<T>::reallocate_memory_for_delete(size_t new_capacity) {
	// 1. Выделяем новую память
	T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
	State* new_states = new State[new_capacity];

	// 2. Инициализируем все новые ячейки как empty
	for (size_t i = 0; i < new_capacity; ++i) {
		new_states[i] = empty;
	}

	// 3. Переносим только busy элементы
	size_t new_size = 0;
	for (size_t i = 0; i < _size; ++i) {
		if (_states[i] == busy) {
			// Конструируем объект в новой памяти
			new (&new_data[new_size]) T(std::move(_data[i]));
			new_states[new_size] = busy;
			++new_size;

			// Уничтожаем старый объект
			_data[i].~T();
		}
		// DELETED элементы не переносим
	}

	// 4. Освобождаем старую память
	operator delete[](_data);
	delete[] _states;

	// 5. Обновляем состояние
	_data = new_data;
	_states = new_states;
	_size = new_size;
	_capacity = new_capacity;
	_deleted = 0;  // Все удаленные элементы исключены
}
template<class T>
void TVector<T>::reallocate_memory(size_t new_capacity) {
	// Вычисляем минимально необходимую емкость
	size_t required_capacity = _size - _deleted;
	if (new_capacity < required_capacity) {
		new_capacity = required_capacity;
	}

	T* new_data = static_cast<T*>(operator new(new_capacity * sizeof(T)));
	State* new_states = new State[new_capacity]();  // Инициализация empty

	size_t new_size = 0;
	try {
		// Переносим только busy элементы
		for (size_t i = 0; i < _size; ++i) {
			if (_states[i] == busy) {
				new (&new_data[new_size]) T(std::move(_data[i]));
				new_states[new_size] = busy;
				++new_size;
			}
			_data[i].~T();  // Уничтожаем старый объект в любом случае
		}
	}
	catch (...) {
		// Откат изменений при исключении
		for (size_t i = 0; i < new_size; ++i) {
			new_data[i].~T();
		}
		operator delete(new_data);
		delete[] new_states;
		throw;
	}

	// Освобождаем старую память
	operator delete[](_data);
	delete[] _states;

	// Обновляем состояние
	_data = new_data;
	_states = new_states;
	_size = new_size;
	_capacity = new_capacity;
	_deleted = 0;
}
template<class T>
void TVector<T>::free_memory() noexcept {
	// Уничтожаем активные объекты
	for (size_t i = 0; i < _size; ++i) {
		if (_states[i] == busy) {
			_data[i].~T();
		}
	}

	// Освобождаем память
	operator delete[](_data);
	delete[] _states;

	// Сбрасываем состояние
	_data = nullptr;
	_states = nullptr;
	_size = 0;
	_capacity = 0;
	_deleted = 0;
}