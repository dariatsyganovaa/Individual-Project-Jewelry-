#pragma once
#include <cstddef>
#include <stdexcept>

#define STEP_OF_CAPACITY 15
#define PERS_OF_DELETED 15

template<typename T> class TVector;
template<typename T> int find_first_elem(const TVector<T>&, const T&);
template<typename T> int find_last_elem(const TVector<T>&, const T&);
template<typename T> int* find_elems(const TVector<T>&, const T&, int&);
template<typename T> void swap(T*, T*);
template<typename T> void randomize(const TVector<T>&);
template<typename T> void hoara_sort_rec(const TVector<T>&, int, int);
template<typename T> void hoara_sort(const TVector<T>&);

enum State { empty, busy, deleted };

template<class T>
class TVector {
	T* _data; //массив объектов
	size_t _capacity; //выделенный размер
	size_t _size; //видимый размер
	size_t _deleted; //количество удаленных
	State* _states; //статус ячеек
public:
	//конструктор по умолчанию
	TVector();

	//конструктор с параметром для задания размера
	TVector(size_t);

	//конструктор, принимающий массив данных
	TVector(const T*, size_t);

	//конструктор копирования
	TVector(const TVector<T>&);

	//деструктор
	~TVector();

	size_t capacity() const noexcept;
	size_t size() const noexcept;

	inline bool is_empty() const noexcept;

	inline T& front() noexcept; //возвращает ссылку на первый элемент
	inline T& back() noexcept; //возвращает ссылку на последний элемент

	inline T* begin() noexcept; //возвращает указатель на первый элемент
	inline T* end() noexcept; //возвращает указатель на позицию после последнего элемента

	inline const T* data() const noexcept; 
	inline const State* states() const noexcept;

	void shift_elements(size_t, size_t);

	void push_front_elem(const T&);
	void push_front_elems(T*, size_t);
	void push_back_elem(const T&);
	void push_back_elems(T*, size_t);
	void insert_elem(const T&, size_t);
	void insert_elems(size_t, T*, size_t);

	void pop_front_elem();
	void pop_front_elems(size_t);
	void pop_back_elem();
	void pop_back_elems(size_t);
	void erase_elem(size_t);	
	void erase_elems(size_t, size_t);

	void emplace(size_t, T );

	void assign(const TVector<T>&);
	T& at(size_t); //функция обращения по индексу с проверкой допустимости индекса
	void clear() noexcept;

	void shrink_to_fit(); //для возврата дополнительной памяти в систему
	void reserve(size_t); //функция резервирования памяти заданного размера
	void resize(size_t); //функция изменения размера массива 
	void resize(size_t, const T&); //функция изменения размера массива с заполнителем

	TVector<T>& operator=(const TVector<T>&);
	bool operator==(const TVector<T>&) const;
	bool operator!=(const TVector<T>&) const;
	T& operator[](size_t);

	void print_elems();
	void print_states();

	friend int find_first_elem<T>(const TVector<T>&, const T&);
	friend int find_last_elem<T>(const TVector<T>&, const T&);
	friend int* find_elems<T>(const TVector<T>&, const T&, int&);
	friend void swap<T>(T*, T*);
	friend void randomize<T>(const TVector<T>&);
	friend void hoara_sort_rec<T>(const TVector<T>&, int, int);
	friend void hoara_sort<T>(const TVector<T>&);

private:
	inline bool is_full() const noexcept;
	void allocate_memory(size_t);
	void reallocate_memory(size_t);
	void reallocate_memory_for_delete();
	void free_memory() noexcept;
};

template<class T>
TVector<T>::TVector() {
	_data = nullptr;
	_size = 0;
	_capacity = 0;
	_states = nullptr;
	_deleted = 0;
}

template<class T>
TVector<T>::TVector(size_t size) {
	_data = nullptr;
	_size = size;
	_capacity = 0;
	_states = nullptr;
	_deleted = 0;

	if (_size > 0) { 
		_capacity = (size / STEP_OF_CAPACITY + 1) * STEP_OF_CAPACITY; 
		_data = new T[_capacity];
		_states = new State[_capacity];
		for (size_t i = 0; i < _capacity; i++) {
			_states[i] = empty;
		}
	}
}
template<class T>
TVector<T>::TVector(const T* arr, size_t size) {
	_size = size;
	_capacity = 0;
	_data = nullptr;
	_states = nullptr;
	_deleted = 0;
	
	if (_size > 0) {
		_capacity = (size / STEP_OF_CAPACITY + 1) * STEP_OF_CAPACITY;
		_data = new T[_capacity];

		for (size_t i = 0; i < _size; i++) {
			_data[i] = arr[i];
		}

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
	_states = nullptr;
	_size = other._size;
	_capacity = 0;
	_deleted = other._deleted;

	if (_size > 0) {
		_capacity = (_size / STEP_OF_CAPACITY + 1) * STEP_OF_CAPACITY;
		_data = new T[_capacity];
		_states = new State[_capacity];

		for (size_t i = 0; i < _size; i++) {
			_data[i] = other._data[i];
			_states[i] = other._states[i];
		}

		for (size_t i = _size; i < _capacity; i++) {
			_states[i] = empty;
		}
	}
}

template<class T>
TVector<T>::~TVector() { 
	if (_data != nullptr) {
		delete[] _data;
		delete[] _states;
	}
}

template<class T>
size_t TVector<T>::size() const noexcept { return _size; }

template<class T>
size_t TVector<T>::capacity() const noexcept { return _capacity; } 

template<class T>
inline const T* TVector<T>::data() const noexcept {	return _data; }

template<class T>
inline const State* TVector<T>::states() const noexcept { return _states; }

template<class T>
inline T& TVector<T>::front() noexcept {
	return _data[0];
}
 
template<class T>
inline T& TVector<T>::back() noexcept {
	for (size_t i = _size; i > 0; i--) {
		if (_states[i - 1] == busy) {
			return _data[i - 1];
		}
	}
}

template<class T>
inline T* TVector<T>::begin() noexcept {
	//если вектор пуст (_data == nullptr или _size == 0), вернет указатель "на конец"
	return _data;
}

template<class T>
inline T* TVector<T>::end() noexcept {
	//если вектор пуст, вернет то же значение, что и begin()
	return _data + _size;
}

template<class T>
inline bool TVector<T>::is_empty() const noexcept {
	if (_size == 0) {
		return true;
	}
	else if (_size > 0) {
		for (size_t i = 0; i < _size + _deleted; i++) {
			if (_states[i] == busy) {
				return false;
			}
		}
	}
	return true;
}
template<class T>
void TVector<T>::shift_elements(size_t count, size_t pos) {
	if (count == 0 || pos >= _size) return;

	for (size_t i = _size; i > pos + count - 1; i--) {
		_data[i] = _data[i - count];   
		_states[i] = _states[i - count];  
	}

}
template<class T>
void TVector<T>::push_front_elem(const T& value) {
	if (_size >= _capacity) {
		reallocate_memory(_capacity + STEP_OF_CAPACITY);
	}
	if (_size > 0) {
		shift_elements(1, 0);
	}

	_data[0] = value;
	_states[0] = busy;

	_size++;
}

template<class T>
void TVector<T>::push_front_elems(T* values, size_t count) {
	if (values == nullptr) {
		throw std::invalid_argument("TVector::push_front_elems: values is nullptr");
	}
	if (count == 0) {
		throw std::invalid_argument("TVector::push_front_elems: count cannot be zero");
	}

	_size += count;
	if (_size >= _capacity) {
		reallocate_memory(_capacity + STEP_OF_CAPACITY); 
	}
	if (_size > 0) { 
		shift_elements(count, 0);
	}

	for (size_t i = 0; i < count; i++) {
		_data[i] = values[i];
		_states[i] = busy;
	}
}

template<class T>
void TVector<T>::push_back_elem(const T& value) {
	if (_size >= _capacity) {
		reallocate_memory(_capacity + STEP_OF_CAPACITY);
	}

	_data[_size] = value;
	_states[_size] = busy;
	_size++;
}

template<class T>
void TVector<T>::push_back_elems(T* values, size_t count) {
	if (_size + count >= _capacity) {
		reallocate_memory(_capacity + STEP_OF_CAPACITY);
	}
	for (size_t i = _size; i <= _size + count; i++) {
		_data[i] = values[i - _size];
		_states[i] = busy;
	}
	_size += count;
}

template<class T>
void TVector<T>::insert_elem(const T& value, size_t pos) {
	if (pos > _size || pos < 0) {
		throw std::out_of_range("TVector::insert_elem: insert position out of range!");
	}

	if (_size >= _capacity) {
		reallocate_memory(_capacity + STEP_OF_CAPACITY);
	}
	if (_size > 0) {
		shift_elements(1, pos);
	}

	_data[pos - 1] = value;
	_states[pos - 1] = busy;

	_size++;
}

template<class T>
void TVector<T>::insert_elems(size_t pos, T* values, size_t count) {
	if (pos > _size || pos < 0) {
		throw std::out_of_range("TVector::insert_elems: insert position out of range");
	}
	_size += count;
	if (_size >= _capacity) {
		reallocate_memory(_capacity + STEP_OF_CAPACITY);
	}
	if (_size > 0) { //else??
		shift_elements(count, pos);
	}
	for (size_t i = 0; i < count; i++) {
		_data[i + pos - 1] = values[i];
		_states[i + pos - 1] = busy;
	}
}

template<class T>
void TVector<T>::pop_front_elem() {
	if (_size == 0) {
		throw std::out_of_range("TVector::pop_front_elem: cannot be deleted from an empty vector");
	}
	for (size_t i = 0; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			_states[i] = deleted;
			break;
		}
	}
	_deleted += 1;

	if (_deleted * 100 > _capacity * PERS_OF_DELETED) {
		reallocate_memory_for_delete();
	}
	_size--;
}

template<class T>
void TVector<T>::pop_front_elems(size_t count) {
	if (_size == 0) {
		throw std::out_of_range("TVector::pop_front_elems: cannot be deleted from an empty vector");
	}

	if (count > _size) {
		throw std::invalid_argument("TVector::pop_front_elems: the counter cannot be larger than the size");
	}
	size_t j = 0;
	for (size_t i = 0; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			_states[i] = deleted;
			j++;
		}
		if (j == count) {
			break;
		}
	}
	_deleted += count;
	_size = _size - count;

	if (_deleted * 100 > _capacity * PERS_OF_DELETED) {
		reallocate_memory_for_delete();
	}
}

template<class T>
void TVector<T>::pop_back_elem() {
	if (_size == 0) {
		throw std::out_of_range("TVector::pop_back_elem: cannot be deleted from an empty vector");
	}
	for (size_t i = _size - 1; i >= 0; i--) {
		if (_states[i] == busy) {
			_states[i] = empty;
			break;
		}
	}
	_deleted += 1;
	_size--;

	if (_deleted * 100 > _capacity * PERS_OF_DELETED) {
		reallocate_memory_for_delete();
	}
}

template<class T>
void TVector<T>::pop_back_elems(size_t count) {
	if (_size == 0) {
		throw std::out_of_range("TVector::pop_back_elems: cannot be deleted from an empty vector");
	}

	if (count > _size) {
		throw std::invalid_argument("TVector::pop_back_elems: the counter cannot be larger than the size");
	}
	size_t j = 0;
	for (size_t i = _size + _deleted - 1; i >= 0; i--) {//тест
		if (_states[i] == busy) {
			_states[i] = empty;
			j++;
		}

		if (_states[i] == deleted) {
			_states[i] = empty;
		}

		if (j == count) {
			break;
		}
	}
	_deleted += count;
	_size = _size - count;

	if (_deleted * 100 > _capacity * PERS_OF_DELETED) {
		reallocate_memory_for_delete();
	}
}

template<class T>
void TVector<T>::erase_elem(size_t pos) {
	if (pos > _size || pos < 0) {
		throw std::out_of_range("TVector::erase_elem: erase position out of range");
	}

	if (_size == 0) {
		throw std::out_of_range("TVector::erase_elem: cannot be deleted from an empty vector");
	}

	int busy_count = 0;
	for (size_t i = 0; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			busy_count++;
			if (busy_count == pos) {
				_states[i] = deleted;
				_size--;
				break;
			}
		}
	}
	_deleted += 1;
	if (_deleted * 100 > _capacity * PERS_OF_DELETED) {
		reallocate_memory_for_delete();
	}
}
template<class T>
void TVector<T>::erase_elems(size_t pos, size_t count) {
	if (_size == 0) {
		throw std::out_of_range("TVector::erase_elems: cannot be deleted from an empty vector");
	}

	if (count > _size) {
		throw std::invalid_argument("TVector::erase_elems: the counter cannot be larger than the size");
	}

	if (pos > _size || pos < 0) {
		throw std::out_of_range("TVector::erase_elems: erase position out of range");
	}
	size_t j = 0;
	for (size_t i = pos - 1; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			_states[i] = deleted;
			j++;
		}
		if (j == count) {
			break;
		}
	}
	_deleted += count;
	_size = _size - count;

	if (_deleted * 100 > _capacity * PERS_OF_DELETED) {
		reallocate_memory_for_delete();
	}
}

template <typename T>
void TVector<T>::emplace(size_t pos, T val) { 
	if (_size == 0) {
		throw std::out_of_range("TVector::emplace: cannot be emplaced from an empty vector");
	}
	if (pos > _size || pos < 0) {
		throw std::out_of_range("TVector::emplace: emplace position out of range");
	}
	int busy_count = 0;
	for (size_t i = 0; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			if (busy_count == pos) {
				_data[i] = val;
				break;
			}
			busy_count++;
		}
	}
}

template<class T>
void TVector<T>::assign(const TVector<T>& other) {
	if (this == &other) {
		return;
	}

	_size = 0;
	_deleted = 0;

	if (other._size > _capacity) {
		reallocate_memory(other._size);
	}

	for (size_t i = 0; i < other._size; i++) {
		_data[i] = other._data[i];       
		_states[i] = other._states[i]; 
	}

	_size = other._size;
	_deleted = other._deleted;
}

template<class T>
T& TVector<T>::at(size_t index) {
	if (index >= _size) {
		throw std::out_of_range("TVector::at: index out of range");
	}
	if (_states[index] != busy) {
		throw std::logic_error("TVector::at: element at index is not valid");
	}
	size_t j = 0;
	for (size_t i = 0; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			if (j == index) {
				return _data[i];
			}
			j++;
		}
	}
	return _data[index];
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
void TVector<T>::shrink_to_fit() {
	if (_size == 0) {
		clear();
		return;
	}

	T* new_data = new T[_size];
	State* new_states = new State[_size];
	size_t j = 0;
	for (size_t i = 0; i < _capacity; i++) {
		if (_states[i] == busy) {
			new_data[j] = _data[i]; 
			new_states[j] = busy;
			j++;
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
	size_t j = 0;
	for (size_t i = 0; i < _capacity; i++) {//!!
		if (_states[i] == busy) {
			new_data[j] = _data[i];
			new_states[j] = busy;
			j++;
		}
	}

	delete[] _data;
	delete[] _states;
	_data = new_data;
	_states = new_states;
	_capacity = new_capacity;
}
//массив 100, удаление разным образом, без перевыделений
template<class T>
void TVector<T>::resize(size_t new_size) {// удаленные чередуются с заполненными
	if (new_size < _size) {
		for (size_t i = new_size; i < _size + _deleted; i++) {
			if (_states[i] == busy) {
				_states[i] = empty;
			}
			if (_states[i] == deleted) {
				_states[i] = empty;
			}
		}
		_size = new_size;
	}
	else if (new_size > _size) {
		reserve(new_size);
		for (size_t i = _size; i < new_size; i++) {
			_data[i] = 0; 
			_states[i] = busy;
		}
		_size = new_size;
	}
}

template <typename T>
void TVector<T>::resize(size_t new_size, const T& value) {//!!!
	if (new_size < _size) {
		for (size_t i = new_size; i < _size; i++) {
			if (_states[i] == busy) {
				_states[i] = empty;
			}
		}
		_size = new_size;
	}
	else if (new_size > _size) {
		reserve(new_size);
		for (size_t i = _size; i < new_size; i++) {
			_data[i] = value;
			_states[i] = busy;
		}
		_size = new_size;
	}
}

template <typename T>
TVector<T>& TVector<T>::operator=(const TVector<T>& other) {	
	if (this != &other) {
		if (_capacity != other._capacity) {
			clear();
			reserve(other._capacity);
		}
		else {
			for (size_t i = other._size + _deleted; i < _capacity; i++) {
				_states[i] = empty;
			}
		}
		
		for (size_t i = 0; i < other._size + _deleted; i++) {
			_data[i] = other._data[i];
			_states[i] = busy;
		}
		_size = other._size;
		_deleted = other._deleted;
	}
	return *this;
}
template<class T>
bool TVector<T>::operator==(const TVector<T>& other) const {
	if (this == &other) return true;
	if (_size != other._size) return false;
	for (size_t i = 0; i < _size + _deleted; i++) {
		bool matched = false;
		for (size_t j = 0; j < _size + _deleted; j++) {
			if (_states[i] == other._states[j]) {
				if (_states[i] != busy || _data[i] == other._data[j]) {
					matched = true;
					break;
				}
			}
		}
		if (!matched) {
			return false; 
		}
	}
	return true;
}
template <typename T>
bool TVector<T>::operator!=(const TVector<T>& other) const {
	return !(*this == other);
}
template <typename T>
T& TVector<T>::operator[](size_t pos) {
	size_t j = 0;
	for (size_t i = 0; i < _size + _deleted; i++) {
		if (_states[i] == busy) {
			if (j == pos) {
				return _data[i];
			}
			j++;
		}
	}
	return _data[pos];
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

template<typename T>
int find_first_elem(const TVector<T>& vec, const T& value) {
	if (vec.size() == 0) {
		throw std::invalid_argument("TVector::find_first_elem: cannot find from empty vector");
	}
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
	if (vec.size() == 0) {
		throw std::invalid_argument("TVector::find_last_elem: cannot find from empty vector");
	}
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
	if (vec.size() == 0) {
		throw std::invalid_argument("TVector::find_elems: cannot find from empty vector");
	}
	size = 0;

	for (size_t i = 0; i < vec._capacity; i++) {
		if (vec._states[i] == busy && vec._data[i] == value) {
			size++;
		}
	}

	if (size == 0) {
		return nullptr;
	}

	int* result = new int[size];
	int index = 0;

	for (size_t i = 0, j = 0; i < vec._capacity; i++) { //j - позиция в массиве result
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
inline bool TVector<T>::is_full() const noexcept { return _size >= _capacity; }

template<class T>
void TVector<T>::allocate_memory(size_t new_capacity) {
	T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
	State* new_states = new State[new_capacity];

	for (size_t i = 0; i < new_capacity; ++i) {
		new_states[i] = empty;
	}

	_data = new_data;
	_states = new_states;
	_capacity = new_capacity;
}

template<class T>
void TVector<T>::reallocate_memory_for_delete() {
	T* new_data = new T[_size + STEP_OF_CAPACITY];
	State* new_states = new State[_size + STEP_OF_CAPACITY];

	size_t new_index = 0;
	for (size_t i = 0; i < _capacity; ++i) {
		if (_states[i] == busy) {
			new_data[new_index] = _data[i];
			new_states[new_index] = busy;
			new_index++;
		}
	}

	delete[] _data;
	delete[] _states;

	_data = new_data;
	_states = new_states;
	_capacity = _size + STEP_OF_CAPACITY;
	_deleted = 0;

	for (size_t i = _size; i < _capacity; ++i) {
		_states[i] = empty;
	}
}

template<class T>
void TVector<T>::reallocate_memory(size_t new_capacity) {
	if (_capacity == 0) {
		allocate_memory(STEP_OF_CAPACITY);
	}
	T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
	State* new_states = new State[new_capacity];

	for (size_t i = 0; i < new_capacity; ++i) {
		new_states[i] = empty;
	}

	size_t new_size = 0;
	for (size_t i = 0; i < _size; ++i) {
		if (_states[i] == busy) {
			new (&new_data[new_size]) T(std::move(_data[i]));
			new_states[new_size] = busy;
			++new_size;
		}
	}
	delete[] _data;
	delete[] _states;
	
	_data = new_data;
	_states = new_states;
	_capacity = new_capacity;
	_deleted = 0;
}
template<class T>
void TVector<T>::free_memory() noexcept {
	delete[] _data;
	delete[] _states;

	_data = nullptr;
	_states = nullptr;
	_size = 0;
	_capacity = 0;
	_deleted = 0;
}
template <typename T>
void randomize(const TVector<T>& vec) {
	srand(time(NULL));

	for (int i = vec._size; i > 0; i--) {
		if (vec._states[i] == busy) {
			int j = rand() % (i + 1);
			swap(&vec._data[i], &vec._data[j]);
			swap(&vec._states[i], &vec._states[j]);
		}
	}
}

template <typename T>
void swap(T* a, T* b) {
	T temp = *a;
	*a = *b;
	*b = temp;
}

template <typename T>
void hoara_sort_rec(const TVector<T>& vec, int left, int right) {
	if (left < right) {
		int l = left, r = right;
		int base_pos = (left + right) / 2;
		T base_value = vec._data[base_pos];

		while (l <= r) {
			while (vec._data[l] < base_value) {
				l++;
			}
			while (vec._data[r] > base_value) {
				r--;
			}
			if (l <= r) {
				if (l < r) {
					swap(&vec._data[l], &vec._data[r]);
					swap(&vec._states[l], &vec._states[r]);
				}
				l++;
				r--;
			}
		}
		hoara_sort_rec(vec, left, r);
		hoara_sort_rec(vec, l, right);
	}
}

template <typename T>
void hoara_sort(const TVector<T>& vec) {
	hoara_sort_rec(vec, 0, vec._size);
}