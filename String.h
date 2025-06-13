#ifndef STRING_H
#define STRING_H
#include <iostream>
using namespace std;
class String {
private:
	char *arr;
	int length;
	int capacity;
	void expandCapacity(int newCapacity);

public:
	String();
	String(const char *s);
	String(int size, char ch);
	String(const String &other);
	~String();

	friend ostream &operator<<(ostream &out, const String &s);  // Output operator
	friend istream &operator>>(istream &in, String &s);  // Input operator
	String &operator=(const String &other);  // Assignment operator
	friend String operator+(const String &str1, const String &str2);  // Concatenation operator
	String &operator+=(const String &other);    // Append operator
	bool operator==(const String &s) const;  // Equality check
	bool operator!=(const String &s) const;  // Inequality check
	bool operator<(const String &s) const;  // Less than
	bool operator<=(const String &s) const;  // Less than or equal
	bool operator>(const String &s) const;  // Greater than
	bool operator>=(const String &s) const;  // Greater than or equal
	char &operator[](int index);  // Mutable index
	const char &operator[](int index) const;  // Read-only index

	int size() const;
	int capacity_size() const;
	bool empty() const;
	void clear();

	char at(int index) const;
	char front() const;
	char back() const;

	void push_back(char ch);
	void pop_back();

	void insert(int index, const void *data, bool isStringObject);
	void erase(int pos, int count);
	void replace(int index, const void *data, bool isStringObject);

	String *split(const char *delimiter, int &count) const;
	String *tokenize(const char *delim, int &count) const;
	static String join(const char *delimiter, String *arr, int count);

	int count(const char *s) const;
	int find(const char *s, int pos = 0) const;
	int rfind(const char *s, int pos = -1) const;
    
	int find(const String &str, int pos = 0) const;
	int rfind(const String &str, int pos = -1) const;
	int find_first_of(const String &str, int pos = 0) const;
	int find_last_of(const String &str, int pos = -1) const;
	int find_first_not_of(const String &str, int pos = 0) const;
	int find_last_not_of(const String &str, int pos = -1) const;

	String substr(int pos, int count) const;

	void trim();

	int compare(const char *s) const;
	void swap(String &other);

	void resize(int n, char ch = '\0');
	void shrink_to_fit();

	bool starts_with(const char *s) const;
	bool ends_with(const char *s) const;

	void reverse();
	void rotate_left(int n);
	void rotate_right(int n);

	void toupper();
	void tolower();
	void capitalize();
	void swapcase();

	bool is_palindrome() const;
	void remove_char(char ch);

	bool write_to_file(const char *filename) const;
	bool read_from_file(const char *filename);

	int itos(int num);
	int stoi(const char *s);
	long stol(const char *s);
	long long stoll(const char *s);
	float stof(const char *s);
	double stod(const char *s);
	long double stold(const char *s);
};
#endif