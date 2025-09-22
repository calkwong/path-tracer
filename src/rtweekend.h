#pragma once

#include <cmath>
#include <limits>
#include <random>

#include <cstdlib>

const float infinity{ std::numeric_limits<float>::infinity() };
const float pi = 3.1415926535897932385;

inline float degreesToRadians(float degrees)
{
	return degrees * pi / 180.0f;
}

inline float randomfloat() 
{
    thread_local std::uniform_real_distribution<float> distribution; // [0.0, 1.0)
    thread_local std::mt19937 generator;
    return distribution(generator);
}

inline float randomfloat(float min, float max)
{
    return min + (max - min) * randomfloat();
}

inline float linearToGamma(float a)
{
    auto gamma{ 2.2f };
    return std::pow(a, 1.0f / gamma);
}

/*
template<typename T, size_t alignment = 64>
class aligned_vector {
private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;

public:
    aligned_vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

    aligned_vector(size_t size) 
        : m_size(size), m_capacity(size) 
    {
        size_t byteSize = m_size * sizeof(T);
        byteSize = (byteSize + alignment - 1) & ~(alignment - 1);

        void* ptr = _aligned_malloc(byteSize, alignment);
        if (!ptr) 
        {
            std::cout << "bad alloc\n";
            throw std::bad_alloc();
        }
        m_data = static_cast<T*>(ptr);

        // Construct elements
        for (size_t i = 0; i < m_size; ++i) 
        {
            new (m_data + i) T();
        }
    }

    aligned_vector(const aligned_vector& other)
        : m_size(other.m_size), m_capacity(other.m_capacity) {
        if (m_size == 0) 
        {
            m_data = nullptr;
            return;
        }

        size_t byteSize = m_capacity * sizeof(T);
        byteSize = (byteSize + alignment - 1) & ~(alignment - 1);

        void* ptr = _aligned_malloc(byteSize, alignment);
        if (!ptr) 
        {
            std::cout << "bad alloc\n";
            throw std::bad_alloc();
        }

        m_data = static_cast<T*>(ptr);

        // Copy construct elements
        for (size_t i = 0; i < m_size; ++i) {
            new (m_data + i) T(other.m_data[i]);
        }
    }

    aligned_vector& operator=(const aligned_vector& other) {
        if (this != &other) {
            this->~aligned_vector();
            new (this) aligned_vector(other);
        }
        return *this;
    }

    ~aligned_vector() 
    {
        if (m_data)
        {
            for (size_t i = 0; i < m_size; ++i)
            {
                m_data[i].~T();
            }

            _aligned_free(m_data);
        }
    }

    T& operator[](size_t idx) { return m_data[idx]; }
    const T& operator[](size_t idx) const { return m_data[idx]; }

    T* data() { return m_data; }
    const T* data() const { return m_data; }
    size_t size() const { return m_size; }

    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() { return m_data; }
    const_iterator begin() const { return m_data; }
    iterator end() { return m_data + m_size; }
    const_iterator end() const { return m_data + m_size; }

};
*/