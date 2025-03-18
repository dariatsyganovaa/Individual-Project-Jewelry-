#pragma once
class FIO {
	char* _first_name;
	char* _second_name;
	char* _last_name;

public:
	//������������
	FIO();
	FIO(const char*, const char*, const char*);
	FIO(char*, char*, char*);
	FIO(const FIO&);

	//����������
	~FIO();

	//�������
	void set_first_name(const char*);
	void set_second_name(const char*);
	void set_last_name(const char*);

	//�������
	const char* get_first_name();
	const char* get_second_name();
	const char* get_last_name();

private:
	void check_corrections(const char* name);
};