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
	// ����������� �� ���������
	TVector(size_t size = 0);

	// ����������� � ���������� ��� ������� �������
	TVector(size_t);

	// �����������, ����������� ������ ������
	TVector(const T*, size_t);

	// ����������� �����������
	TVector(const TVector<T>&);

	// ����������
	~TVector();
	
	inline bool is_empty() const noexcept;

	inline T& front() noexcept;
	inline T& back() noexcept;

	inline T* begin() noexcept;
	inline T* end() noexcept;

	inline T* data() noexcept; // ������
	inline const T* data() const noexcept; // ������
	/* ... */
private:
	inline bool is_full() const noexcept;
	/* ... */
};

