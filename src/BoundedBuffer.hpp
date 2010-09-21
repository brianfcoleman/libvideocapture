#ifndef VIDEO_CAPTURE_BOUNDED_BUFFER_H
#define VIDEO_CAPTURE_BOUNDED_BUFFER_H

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <boost/utility.hpp>

namespace VideoCapture {

/**
 * <p>A bounded buffer based on boost circular buffer.</p>
 * <p>Taken from a bounded buffer example
 *    given with the circular buffer documentation.</p>
 * <p>Modified so that pop_back returns the last element
 *    from the buffer by value</p>
 */
template <class T> class bounded_buffer : private boost::noncopyable {
 public:
  typedef boost::circular_buffer<T> container_type;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::value_type value_type;
  typedef typename container_type::const_reference const_reference;

  explicit bounded_buffer(size_type capacity)
      : m_unread(0),
        m_container(capacity) {
  }

  void push_front(const_reference item) {
    // param_type represents the "best" way
    // to pass a parameter of type value_type to a method
    boost::mutex::scoped_lock lock(m_mutex);
    m_not_full.wait(
        lock,
        boost::bind(&bounded_buffer<value_type>::is_not_full, this));
    m_container.push_front(item);
    ++m_unread;
    lock.unlock();
    m_not_empty.notify_one();
  }

  value_type pop_back() {
    boost::mutex::scoped_lock lock(m_mutex);
    m_not_empty.wait(
        lock,
        boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
    value_type item(m_container[--m_unread]);
    lock.unlock();
    m_not_full.notify_one();
    return item;
  }

  bool isInitialized() const {
    return true;
  }

 private:
   bool is_not_empty() const {
     return m_unread > 0;
   }

   bool is_not_full() const {
     return m_unread < m_container.capacity();
   }

   size_type m_unread;
   container_type m_container;
   boost::mutex m_mutex;
   boost::condition m_not_empty;
   boost::condition m_not_full;
};

} // VideoCapture

#endif // VIDEO_CAPTURE_BOUNDED_BUFFER_H
