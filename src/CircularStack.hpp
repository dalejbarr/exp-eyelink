#include <cstddef>
#include <memory>

template <typename T> class CircularStack {
protected:
  std::unique_ptr<T[]> m_buff;
  std::size_t m_size = 0;
  std::size_t m_top = 0;
  std::size_t m_nElements = 0;

public:
  CircularStack(std::size_t size) :
    m_buff(std::make_unique<T[]>(size)),
    m_size(size) {
  }

  std::size_t get_size() {
    return m_size;
  }
  
  std::size_t get_nElements() {
    return m_nElements;
  }

  bool is_full() {
    return m_nElements == m_size;
  }
  
  void reset() {
    m_top = 0;
    m_nElements = 0;
  }
  
  void push(const T& x) {
    m_buff[m_top] = x;
    m_top = (m_top + 1) % m_size;
    if (m_nElements < m_size) {
      m_nElements += 1;
    }
  }

  T pop() {
    m_top = (m_top - 1 + m_size) % m_size;
    m_nElements -= 1;
    return m_buff[m_top];
  }

  T get_at(std::size_t i) {
    return m_buff[(m_top - 1 + m_size - i) % m_size];
  }
};
