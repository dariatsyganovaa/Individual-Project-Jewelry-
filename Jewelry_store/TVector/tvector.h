#pragma once
#include <cstddef> // size_t
#include <stdexcept>

#define STEP_OF_CAPACITY 15

enum State { empty, busy, deleted };
template<class T>
class TVector {
	T* _data; //������ ��������
	size_t _capacity; //���������� ������
	size_t _size; //������� ������
	size_t _deleted; //���������� ���������
	State* _states; //������ �����
public:
	// ����������� �� ���������
	TVector();

	// ����������� � ���������� ��� ������� �������
	TVector(size_t);

	// �����������, ����������� ������ ������
	TVector(const T*, size_t);

	// ����������� �����������
	TVector(const TVector<T>&);

	// ����������
	~TVector();

	size_t capacity() const noexcept;
	size_t size() const noexcept;

	inline bool is_empty() const noexcept;

	inline T& front() noexcept;
	inline T& back() noexcept;

	inline T* begin() noexcept;
	inline T* end() noexcept;

	//inline T* data() noexcept; // ������
	inline const T* data() const noexcept; // ������
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
	// ���������� ��������� �� ������ �������
	// ���� ������ ���� (_data == nullptr ��� _size == 0), ������ ��������� "�� �����"
	return _data;
}

template<class T>
inline T* TVector<T>::end() noexcept {
	// ���������� ��������� �� ������� ����� ���������� ��������
	// ���� ������ ����, ������ �� �� ��������, ��� � begin()
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

//������� 
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
	//����� �������� ���������
	for (size_t i = _size - 1; i > pos; i--) {
		_data[i] = _data[i - 1];
		_states[i] = _states[i - 1];
	}
	_data[pos] = value;
	_states[pos] = busy;

	++_size;
}