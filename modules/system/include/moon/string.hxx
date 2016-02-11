#ifndef MOON_STRING_H_
#define MOON_STRING_H_

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <wchar.h>

static inline wchar_t*
chars_to_utf8(const char* str)
{
#ifdef _WIN32
  // get length of converted string in characters
  const size_t length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, sizeof(str), NULL, 0);
  wchar_t *name = new wchar_t[length];
  MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, sizeof(str), name, length);
  return name;
#else
  const size_t length = mbstowcs(0, str, 0);
  wchar_t *text = new wchar_t[length+1];
  mbstowcs(text, str, length+1);
  return text;
#endif
};

struct Utf8ToChars {
  size_t length;
  char* str;
};

static inline Utf8ToChars
utf8_to_chars(const wchar_t* str)
{
#ifdef _WIN32
#else
  Utf8ToChars data;
  const size_t length = wcslen(str);
  data.length = (length * 4) + 1;
  data.str = new char[data.length];
  wcstombs(data.str, str, data.length);
  return data;
#endif
}

namespace Moon {
  /* String
   * Glorified wchar_t wrapper.
   */
  class WString {
  public:
    WString(wchar_t *str) {
      m_length = wcslen(str);
      m_str = new wchar_t[m_length];
      memcpy(m_str, str, m_length * sizeof(wchar_t));
    }

    WString(char *str) {
      m_str = chars_to_utf8(str);
      m_length = wcslen(m_str);
    }

    ~WString() {
      delete[] m_str;
    }

    wchar_t operator[](size_t index) const {
      return m_str[index];
    }

    size_t length() const {
      return m_length;
    }

    const wchar_t* c_str() const {
      return m_str;
    }
  private:
    wchar_t *m_str;
    size_t m_length;
  };

  class String {
  public:
    String(wchar_t *str) {
      const Utf8ToChars data = utf8_to_chars(str);
      m_str = data.str;
      m_length = data.length;
    }

    String(char *str) {
      /* Definetly not sure about this, so best if we don't use it */
      m_length = strlen(str);
      m_str = new char[m_length];
      memcpy(m_str, str, m_length * sizeof(char));
    }

    ~String() {
      delete[] m_str;
    }

    char* get_cptr(size_t index) const {
      return &(m_str[index]);
    }

    char operator[](size_t index) const {
      return m_str[index];
    }

    size_t length() const {
      return m_length;
    }

    const char* c_str() const {
      return m_str;
    }
  private:
    char *m_str;
    size_t m_length;
  };
}

#endif
