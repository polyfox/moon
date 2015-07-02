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
  int length;

  // get length of converted string in characters
  length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, sizeof(str), NULL, 0);

  wchar_t *name = new wchar_t[length];
  MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, sizeof(str), name, length);
  return name;
#else
  int length = mbstowcs(0, str, 0);
  wchar_t *text = new wchar_t[length+1];
  mbstowcs(text, str, length+1);
  return text;
#endif
};

namespace Moon {
  /* String
   * Glorified wchar_t wrapper.
   */
  class String {
  public:
    String(wchar_t *str) {
      m_length = wcslen(str);
      m_str = new wchar_t[m_length];
      memcpy(m_str, str, m_length * sizeof(wchar_t));
    }

    String(char *str) {
      m_str = chars_to_utf8(str);
      m_length = wcslen(m_str);
    }

    ~String() {
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
}

#endif
