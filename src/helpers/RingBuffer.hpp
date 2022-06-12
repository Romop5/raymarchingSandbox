#ifndef RAYMARCHER_RINGBUFFER_HPP
#define RAYMARCHER_RINGBUFFER_HPP

namespace raymarcher {

/**
 * @brief A generic ring buffer with fixed size SIZE
 */
template<size_t SIZE, typename T = size_t>
class RingBuffer
{
  using StorageType = std::array<T, SIZE>;

public:
  RingBuffer()
  {
    for (auto& period : this->buffer) {
      period = 0;
    }
  }
  auto Add(T period) -> void
  {
    this->sum += period - this->buffer[this->position];
    this->buffer[this->position++] = period;
    this->position = this->position % SIZE;
  }
  auto GetAverage() const -> double { return this->sum / double(SIZE); }
  auto GetArray() const -> const StorageType& { return buffer; }

private:
  StorageType buffer;
  size_t position = 0;
  T sum = 0;
};

}
#endif
