#include "String.h"
#include <iostream>
#include <fstream>
using namespace std;
void String::expandCapacity(int newCapacity)
{
    if (newCapacity <= capacity) 
        return;
    char *newArr = new char[newCapacity];
    for (int i = 0; i < length; i++)
        newArr[i] = arr[i];
    delete[] arr;
    arr = newArr;
    capacity = newCapacity;
}
String::String()
{
    length = 0;
    capacity = 1;
    arr = new char[capacity];
    arr[0] = '\0';
}
String::String(const char *s)
{
    length = 0;
    while (s[length] != '\0')
        length++;
    capacity = length + 1;
    arr = new char[capacity];
    for (int i = 0; i < length; i++)
        arr[i] = s[i];
    arr[length] = '\0';
}
String::String(int size, char ch)
{
    if (size < 0)
        size = 0;
    length = size;
    capacity = length + 1;
    arr = new char[capacity];
    for (int i = 0; i < length; i++)
        arr[i] = ch;
    arr[length] = '\0';
}
String::String(const String &other)
{
    length = other.length;
    capacity = length + 1;
    arr = new char[capacity];
    for (int i = 0; i < length; i++)
        arr[i] = other.arr[i];
    arr[length] = '\0';
}
String::~String()
{
    delete[] arr;
}
ostream &operator<<(ostream &out, const String &s)
{
    if (s.length > 0)
        for (int i = 0; i < s.length; i++)
            out << s.arr[i];
    return out;
}
istream &operator>>(istream &in, String &s)
{
    char buffer[1000];
    in >> buffer;
    int newLength = 0;
    while (buffer[newLength] != '\0')
        newLength++;
    if (newLength >= s.capacity)
        s.expandCapacity(newLength + 1);
    for (int i = 0; i < newLength; i++)
        s.arr[i] = buffer[i];
    s.arr[newLength] = '\0';
    s.length = newLength;
    return in;
}
String &String::operator=(const String &other)
{
    if (this != &other)
    {
        delete[] arr;
        length = other.length;
        capacity = length + 1;
        arr = new char[capacity];
        for (int i = 0; i < length; i++)
            arr[i] = other.arr[i];
        arr[length] = '\0';
    }
    return *this;
}
String operator+(const String& str1, const String& str2)
{
    int new_size = str1.length + str2.length;
    String temp;
    temp.expandCapacity(new_size + 1);
    for (int i = 0; i < str1.length; i++)
        temp.arr[i] = str1.arr[i];
    for (int i = 0; i < str2.length; i++)
        temp.arr[str1.length + i] = str2.arr[i];
    temp.length = new_size;
    temp.arr[new_size] = '\0';
    return temp;
}
String &String::operator+=(const String &other)
{
    int newLength = length + other.length;
    if (newLength >= capacity)
        expandCapacity(newLength + 1);
    for (int i = 0; i < other.length; i++)
        arr[length + i] = other.arr[i];
    length = newLength;
    arr[length] = '\0';
    return *this;
}
bool String::operator==(const String &s) const
{
    if (length != s.length)
        return false;
    for (int i = 0; i < length; i++)
        if (arr[i] != s.arr[i])
            return false;
    return true;
}
bool String::operator!=(const String &s) const
{
    if (length != s.length)
        return true;
    for (int i = 0; i < length; i++)
        if (arr[i] != s.arr[i])
            return true;
    return false;
}
bool String::operator<(const String &s) const
{
    int minLength = (length < s.length) ? length : s.length;
    for (int i = 0; i < minLength; i++)
    {
        if (arr[i] < s.arr[i])
            return true;
        else if (arr[i] > s.arr[i])
            return false;
    }
    return length < s.length;
}
bool String::operator<=(const String &s) const
{
    int minLength = (length < s.length) ? length : s.length;
    for (int i = 0; i < minLength; i++)
    {
        if (arr[i] < s.arr[i])
            return true;
        else if (arr[i] > s.arr[i])
            return false;
    }
    return length <= s.length;
}
bool String::operator>(const String &s) const
{
    int minLength = (length < s.length) ? length : s.length;
    for (int i = 0; i < minLength; i++)
    {
        if (arr[i] > s.arr[i])
            return true;
        else if (arr[i] < s.arr[i])
            return false;
    }
    return length > s.length;
}
bool String::operator>=(const String &s) const
{
    int minLength = (length < s.length) ? length : s.length;
    for (int i = 0; i < minLength; i++)
    {
        if (arr[i] > s.arr[i])
            return true;
        else if (arr[i] < s.arr[i])
            return false;
    }
    return length >= s.length;
}
char &String::operator[](int index)
{
    if (index < 0 || index >= length)
    {
        static char dummy = '\0';
        return dummy;
    }
    return arr[index];
}
const char &String::operator[](int index) const
{
    if (index < 0 || index >= length)
    {
        static char dummy = '\0';
        return dummy;
    }
    return arr[index];
}
int String::size() const
{
    return length;
}
int String::capacity_size() const  
{  
    return capacity;  
}
bool String::empty() const
{
    return length == 0;
}
void String::clear()
{
    length = 0;
    if (arr != nullptr)
        arr[0] = '\0';
}
char String::at(int index) const
{
    if (index < 0 || index >= length)
        return '\0';
    return arr[index];
}
char String::front() const
{
    if (length == 0)
        return '\0';
    return arr[0];
}
char String::back() const
{
    if (length == 0)
        return '\0';
    return arr[length - 1];
}
void String::push_back(char ch)
{
    if (length + 1 >= capacity)
        expandCapacity((capacity == 0) ? 2 : capacity * 2);
    arr[length] = ch;
    length++;
    arr[length] = '\0';
}
void String::pop_back()
{
    if (length > 0)
        length--;
        arr[length] = '\0';
}
void String::insert(int index, const void *data, bool isStringObject)
{
    if (!data || index < 0 || index > length)
        return;
    const char *charData;
    int insertLength = 0;
    if (isStringObject)
    {
        const String *strObj = static_cast<const String *>(data);
        charData = strObj->arr;
        insertLength = strObj->length;
    }
    else
    {
        charData = static_cast<const char *>(data);
        while (charData[insertLength] != '\0')
        {
            insertLength++;
        }
    }
    int newLength = length + insertLength;
    if (newLength >= capacity)
        expandCapacity(newLength + 1);
    for (int i = length - 1; i >= index; i--)
        arr[i + insertLength] = arr[i];
    for (int i = 0; i < insertLength; i++)
        arr[index + i] = charData[i];
    length = newLength;
    arr[length] = '\0';
}
void String::erase(int pos, int count)
{
    if (pos < 0 || pos >= length || count <= 0)
        return;
    int newLength = (pos + count > length) ? pos : length - count;
    for (int i = pos; i < newLength; i++)
        arr[i] = arr[i + count];
    length = newLength;
    arr[length] = '\0';
}
void String::replace(int index, const void *data, bool isStringObject)
{
    if (!data || index < 0 || index >= length)
        return;
    const char *charData;
    int replaceLength = 0;
    if (isStringObject)
    {
        const String *strObj = static_cast<const String *>(data);
        charData = strObj->arr;
        replaceLength = strObj->length;
    }
    else
    {
        charData = static_cast<const char *>(data);
        while (charData[replaceLength] != '\0')
        {
            replaceLength++;
        }
    }
    int remainingLength = length - index;
    int newLength = (replaceLength > remainingLength) ? index + replaceLength : length;
    if (newLength >= capacity)
        expandCapacity(newLength + 1);
    for (int i = 0; i < replaceLength; i++)
        arr[index + i] = charData[i];
    arr[newLength] = '\0';
    length = newLength;
}
String* String::split(const char* delimiter, int& count) const
{
    if (delimiter == nullptr || delimiter[0] == '\0')
    {
        count = 0;
        return nullptr;
    }
    int delim_length = 0;
    while (delimiter[delim_length] != '\0')
        delim_length++;
    count = 1;
    for (int i = 0; i <= length - delim_length; i++)
    {
        bool match = true;
        for (int j = 0; j < delim_length; j++)
        {
            if (arr[i + j] != delimiter[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            count++;
            i += delim_length - 1;
        }
    }
    String* result = new String[count];
    int start = 0, idx = 0;
    for (int i = 0; i <= length - delim_length; i++)
    {
        bool match = true;
        for (int j = 0; j < delim_length; j++)
        {
            if (arr[i + j] != delimiter[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            int sub_length = i - start;
            if (idx < count)
            {
                char* substring = new char[sub_length + 1];
                for (int k = 0; k < sub_length; k++)
                    substring[k] = arr[start + k];
                substring[sub_length] = '\0';
                result[idx++] = String(substring);
                delete[] substring;
            }
            start = i + delim_length;
            i += delim_length - 1;
        }
    }
    if (start < length && idx < count)
    {
        char* substring = new char[length - start + 1];
        for (int i = start; i < length; i++)
            substring[i - start] = arr[i];
        substring[length - start] = '\0';
        result[idx] = String(substring);
        delete[] substring;
    }
    return result;
}
String* String::tokenize(const char* delim, int& count) const
{
    if (delim == nullptr || delim[0] == '\0')
    {
        count = 1;
        return new String[1]{ *this };
    }
    int delimCount = 0;
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; delim[j] != '\0'; j++)
        {
            if (arr[i] == delim[j])
            {
                delimCount++;
                break;
            }
        }
    }
    count = delimCount + 1;
    String* tokens = new String[count];
    int start = 0, tokenIndex = 0;
    for (int i = 0; i <= length; i++)
    {
        bool isDelimiter = false;
        for (int j = 0; delim[j] != '\0'; j++)
        {
            if (arr[i] == delim[j] || arr[i] == '\0')
            {
                isDelimiter = true;
                break;
            }
        }
        if (isDelimiter || i == length)
        {
            int tokenLength = i - start;
            if (tokenLength > 0 && tokenIndex < count)
            {
                char* temp = new char[tokenLength + 1];
                for (int j = 0; j < tokenLength; j++)
                {
                    temp[j] = arr[start + j];
                }
                temp[tokenLength] = '\0';
                tokens[tokenIndex++] = String(temp);
                delete[] temp;
            }
            start = i + 1;
        }
    }
    return tokens;
}

String String::join(const char* delimiter, String* arr, int count)
{
    if (delimiter == nullptr || delimiter[0] == '\0' || arr == nullptr || count <= 0)
        return String("");
    int delim_length = 0;
    while (delimiter[delim_length] != '\0')
        delim_length++;
    int total_length = 0;
    for (int i = 0; i < count; i++)
    {
        int arr_length = arr[i].length;
        total_length += arr_length;
    }
    total_length += (count - 1) * delim_length;
    char* joined = new char[total_length + 1];
    int pos = 0;
    for (int i = 0; i < count; i++)
    {
        int arr_length = arr[i].length;
        for (int j = 0; j < arr_length && pos < total_length; j++)
            joined[pos++] = arr[i].arr[j];
        if (i < count - 1)
        {
            for (int j = 0; j < delim_length && pos < total_length; j++)
                joined[pos++] = delimiter[j];
        }
    }
    if (pos <= total_length)
        joined[pos] = '\0';
    String result(joined);
    delete[] joined;
    return result;
}
int String::count(const char *s) const
{
    int s_len = 0;
    while (s[s_len] != '\0')
        s_len++;
    if (s_len == 0 || length < s_len)
        return 0;
    int count = 0;
    for (int i = 0; i <= length - s_len; i++)
    {
        bool match = true;
        for (int j = 0; j < s_len; j++)
        {
            if (arr[i + j] != s[j])
                match = false;
                break;
        }
        if (match)
        {
            count++;
            i += s_len - 1;
        }
    }
    return count;
}
int String::find(const char *s, int pos) const
{
    if (s == nullptr || pos < 0 || pos >= length)
        return -1;
    int subLen = 0;
    while (s[subLen] != '\0')
        subLen++;
    if (subLen == 0 || subLen > length)
        return -1;
    for (int i = pos; i <= length - subLen; i++)
    {
        int j = 0;
        while (j < subLen && arr[i + j] == s[j])
            j++;
        if (j == subLen)
            return i;
    }
    return -1;
}
int String::rfind(const char *s, int pos) const
{
    if (s == nullptr)
        return -1;
    int subLen = 0;
    while (s[subLen] != '\0')
        subLen++;
    if (subLen == 0 || subLen > length)
        return -1;
    if (pos < 0 || pos >= length)
        pos = length - 1;
    for (int i = pos; i >= 0; i--)
    {
        int j = 0;
        while (j < subLen && arr[i + j] == s[j])
            j++;
        if (j == subLen)
            return i;
    }
    return -1;
}
int String::find(const String &str, int pos) const
{
	for (int i = pos; i <= length - str.length; i++)
	{
		bool found = true;
		for (int j = 0; j < str.length; j++)
		{
			if (arr[i + j] != str.arr[j])
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return i;
		}
	}
	return -1;
}
int String::rfind(const String &str, int pos) const
{
	if (pos == -1 || pos > length - str.length)
	{
		pos = length - str.length;
	}

	for (int i = pos; i >= 0; i--)
	{
		bool found = true;
		for (int j = 0; j < str.length; j++)
		{
			if (arr[i + j] != str.arr[j])
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return i;
		}
	}
	return -1;
}
int String::find_first_of(const String &str, int pos) const
{
	for (int i = pos; i < length; i++)
	{
		for (int j = 0; j < str.length; j++)
		{
			if (arr[i] == str.arr[j])
			{
				return i;
			}
		}
	}
	return -1;
}
int String::find_last_of(const String &str, int pos) const
{
	if (pos == -1 || pos >= length)
	{
		pos = length - 1;
	}
	for (int i = pos; i >= 0; i--)
	{
		for (int j = 0; j < str.length; j++)
		{
			if (arr[i] == str.arr[j])
			{
				return i;
			}
		}
	}
	return -1;
}
int String::find_first_not_of(const String &str, int pos) const
{
	for (int i = pos; i < length; i++)
	{
		bool found = false;
		for (int j = 0; j < str.length; j++)
		{
			if (arr[i] == str.arr[j])
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return i;
		}
	}
	return -1;
}
int String::find_last_not_of(const String &str, int pos) const
{
	if (pos == -1 || pos >= length)
	{
		pos = length - 1;
	}
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		for (int j = 0; j < str.length; j++)
		{
			if (arr[i] == str.arr[j])
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return i;
		}
	}
	return -1;
}

String String::substr(int pos, int count) const
{
    if (pos < 0 || pos >= length || count <= 0)
        return String();
    if (pos + count > length)
        count = length - pos; 
    char *newArr = new char[count + 1];
    for (int i = 0; i < count; i++)
        newArr[i] = arr[pos + i];
    newArr[count] = '\0';
    String subStr(newArr);
    delete[] newArr;
    return subStr;
}
void String::trim()
{
    if (!arr || length == 0)
        return;
    int start = 0;
    while (start < length && (arr[start] == ' ' || arr[start] == '\t' || arr[start] == '\n'))
        start++;
    int end = length - 1;
    while (end >= start && (arr[end] == ' ' || arr[end] == '\t' || arr[end] == '\n'))
        end--;
    int newLength = end - start + 1;
    if (newLength <= 0)
    {
        length = 0;
        arr[0] = '\0';
        return;
    }
    for (int i = 0; i < newLength; i++)
        arr[i] = arr[start + i];
    length = newLength;
    arr[length] = '\0';
}
void String::swap(String &other)
{
    char *tempArr = arr;
    arr = other.arr;
    other.arr = tempArr;
    int tempLength = length;
    length = other.length;
    other.length = tempLength;
    int tempCapacity = capacity;
    capacity = other.capacity;
    other.capacity = tempCapacity;
}
void String::resize(int n, char ch)
{
    if (n < 0)
        return;
    if (n > capacity)
        expandCapacity(n);
    if (n > length)
        for (int i = length; i < n; i++)
            arr[i] = ch;
    length = n;
    arr[length] = '\0';
}
void String::shrink_to_fit()
{
    if (capacity > length)
    {
        char *newArr = new char[length + 1];
        for (int i = 0; i < length; i++)
            newArr[i] = arr[i];
        newArr[length] = '\0';
        delete[] arr;
        arr = newArr;
        capacity = length;
    }
}
bool String::starts_with(const char *s) const
{
    if (arr == nullptr || s == nullptr) return false;
    int s_len = 0;
    while (s[s_len] != '\0') s_len++;
    if (s_len > length) return false;
    for (int i = 0; i < s_len; i++)
        if (arr[i] != s[i]) return false;
    return true;
}
bool String::ends_with(const char *s) const
{
    if (arr == nullptr || s == nullptr) return false;
    int s_len = 0;
    while (s[s_len] != '\0') s_len++;
    if (s_len > length) return false;
    for (int i = 0; i < s_len; i++)
        if (arr[length - s_len + i] != s[i]) return false;
    return true;
}
void String::reverse()
{
    if (arr == nullptr || length < 2) return;
    int left = 0, right = length - 1;
    while (left < right)
    {
        char temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}
void String::rotate_left(int n)
{
    if (arr == nullptr || length < 2 || n <= 0) return;
    n = n % length;
    if (n == 0) return;
    char *temp = new char[length];
    for (int i = 0; i < length; i++)
        temp[i] = arr[(i + n) % length];
    for (int i = 0; i < length; i++)
        arr[i] = temp[i];
    delete[] temp;
}
void String::rotate_right(int n)
{
    if (arr == nullptr || length < 2 || n <= 0) return;
    n = n % length;
    if (n == 0) return;
    char *temp = new char[length];
    for (int i = 0; i < length; i++)
        temp[i] = arr[(i - n + length) % length];
    for (int i = 0; i < length; i++)
        arr[i] = temp[i];
    delete[] temp;
}
void String::toupper()
{
    if (!arr)
        return;
    for (int i = 0; i < length; i++)
        if (arr[i] >= 'a' && arr[i] <= 'z')
            arr[i] -= 32;
}
void String::tolower()
{
    if (!arr)
        return;
    for (int i = 0; i < length; i++)
        if (arr[i] >= 'A' && arr[i] <= 'Z')
            arr[i] += 32;
}
void String::capitalize()
{
    if (!arr || length == 0)
        return;
    if (arr[0] >= 'a' && arr[0] <= 'z')
        arr[0] -= 32;
    for (int i = 1; i < length; i++)
        if (arr[i - 1] == ' ' && arr[i] >= 'a' && arr[i] <= 'z')
            arr[i] -= 32;
        else if (arr[i - 1] != ' ' && arr[i] >= 'A' && arr[i] <= 'Z')
            arr[i] += 32;
}
void String::swapcase()
{
    if (!arr)
        return;
    for (int i = 0; i < length; i++)
        if (arr[i] >= 'a' && arr[i] <= 'z')
            arr[i] -= 32;
        else if (arr[i] >= 'A' && arr[i] <= 'Z')
            arr[i] += 32;
}
bool String::is_palindrome() const
{
    if (arr == nullptr || length == 0)
        return false;
    int left = 0;
    int right = length - 1;
    while (left < right)
    {
        if (arr[left] != arr[right])
            return false;
        left++;
        right--;
    }
    return true;
}
void String::remove_char(char ch)
{
    int newLength = 0;
    for (int i = 0; i < length; i++)
        if (arr[i] != ch)
            arr[newLength++] = arr[i];
    length = newLength;
    arr[length] = '\0';
}
bool String::write_to_file(const char *filename) const
{
    ofstream file(filename);
    if (!file)
        return false;
    if (arr != nullptr)
        file << arr;
    file.close();
    return true;
}
bool String::read_from_file(const char *filename)
{
    ifstream file(filename);
    if (!file)
        return false;
    char buffer[1000];
    file.getline(buffer, 1000);
    int newLength = 0;
    while (buffer[newLength] != '\0')
        newLength++;
    delete[] arr;
    arr = new char[newLength + 1];
    for (int i = 0; i < newLength; i++)
        arr[i] = buffer[i];
    arr[newLength] = '\0';
    length = newLength;
    file.close();
    return true;
}
int String::itos(int num)
{
    int temp = num, count = 0;
    while (temp)
        temp /= 10;
        count++;
    arr = new char[count + 1];
    arr[count] = '\0';
    for (int i = count - 1; i >= 0; i--)
        arr[i] = (num % 10) + '0';
        num /= 10;
    length = count;
    return length;
}
int String::stoi(const char *s)
{
    int num = 0, i = 0;
    while (s[i] >= '0' && s[i] <= '9')
    {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    return num;
}
long String::stol(const char *s)
{
    long num = 0, i = 0;
    while (s[i] >= '0' && s[i] <= '9')
    {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    return num;
}
long long String::stoll(const char *s)
{
    long long num = 0, i = 0;
    while (s[i] >= '0' && s[i] <= '9')
    {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    return num;
}
float String::stof(const char *s)
{
    float num = 0, factor = 1;
    int i = 0;
    bool decimalFound = false;
    while (s[i])
    {
        if (s[i] == '.')
        {
            decimalFound = true;
            i++;
            continue;
        }
        num = num * 10 + (s[i] - '0');
        if (decimalFound)
            factor *= 10;
        i++;
    }
    return num / factor;
}
double String::stod(const char *s)
{
    double num = 0, factor = 1;
    int i = 0;
    bool decimalFound = false;
    while (s[i])
    {
        if (s[i] == '.')
        {
            decimalFound = true;
            i++;
            continue;
        }
        num = num * 10 + (s[i] - '0');
        if (decimalFound)
            factor *= 10;
        i++;
    }
    return num / factor;
}

long double String::stold(const char *s)
{
    long double num = 0, factor = 1;
    int i = 0;
    bool decimalFound = false;
    while (s[i])
    {
        if (s[i] == '.')
        {
            decimalFound = true;
            i++;
            continue;
        }
        num = num * 10 + (s[i] - '0');
        if (decimalFound)
            factor *= 10;
        i++;
    }
    return num / factor;
}