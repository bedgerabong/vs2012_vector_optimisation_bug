// vs2012_vector_optimization_bug.cpp : Defines the entry point for the console application.
//
#include <stdlib.h> //size_t, malloc, free
#include <new> // bad_alloc, bad_array_new_length
#include <memory>
#include <iostream>
#include <vector>

// The simplest standards-conforming allocator
template<typename T>
class my_minimal_allocator
{	// generic my_allocator for type double
public:

    typedef T value_type;

    typedef value_type * pointer;
    typedef value_type const *const_pointer;

    my_minimal_allocator() noexcept
    {	// construct default my_allocator (do nothing)
    }


    __declspec(allocator) pointer allocate(size_t size) const
    {	// allocate array of _Count elements
        std::cout << __FUNCTION__ << "  " << size << std::endl;
        return static_cast<T*>(::operator new(size * sizeof(T)));
    }

    void deallocate(pointer p, size_t) const noexcept
    {	// deallocate object at _Ptr, ignore size
        ::operator delete(p);
    }

};

template<typename T>
class my_allocator
{	// generic my_allocator for type double
public:

    typedef T value_type;

    typedef value_type * pointer;
    typedef value_type const *const_pointer;

    my_allocator() noexcept
    {	// construct default my_allocator (do nothing)
    }

    __declspec(allocator) pointer allocate(size_t size) const
    {	// allocate array of _Count elements
        std::cout << __FUNCTION__ << "  " << size << std::endl;
        return static_cast<T*>(::operator new(size * sizeof(T)));
    }

    void deallocate(pointer p, size_t) const noexcept
    {	// deallocate object at _Ptr, ignore size
        ::operator delete(p);
    }

    template<class... _Types>
    void construct(value_type p, _Types&&... _Args)
    {	// construct _Objty(_Types...) at _Ptr
        std::cout << __FUNCSIG__ << std::endl;
        ::new (p) T(_STD forward<_Types>(_Args)...);
    }

    template<class T>
    void destroy(T * p)
    {	// destroy object at _Ptr
        p->~T();
    }
};


#if 1

namespace std {

#if _MSC_VER == 1600

#elif _MSC_VER == 1700

#elif _MSC_VER == 1900

    //
    template<>
    class allocator<double>
    {	// generic allocator for type double
    public:
//        typedef double _Not_user_specialized;

        typedef double value_type;

        typedef double *pointer;
        typedef const double *const_pointer;

        template<class _Other>
        struct rebind
        {	// convert this type to an allocator<_Other>
            typedef allocator<_Other> other;
        };

        allocator() noexcept
        {	// construct default allocator (do nothing)
        }

        allocator(const allocator<double>&) noexcept
        {	// construct by copying (do nothing)
        }

        template<class _Other>
        allocator(const allocator<_Other>&) noexcept
        {	// construct from related allocator (do nothing)
        }

        template<class _Other>
        allocator<double>& operator=(const allocator<_Other>&)
        {	// assign from a related allocator (do nothing)
            return (*this);
        }

        __declspec(allocator) pointer allocate(size_t size)
        {   // allocate array of _Count elements
            std::cout << __FUNCTION__  << "  " << size<< std::endl;
            return new double[size];
        }

        void deallocate(pointer p, size_t )
        {   // deallocate object at _Ptr, ignore size
            delete[] p;
        }

        static size_t max_size() noexcept
        {   // get maximum size
            return ( std::numeric_limits<size_t>::max()/sizeof(double) );
        }

        template<class... _Types>
            void construct(double *p, _Types&&... _Args)
        {   // construct _Objty(_Types...) at _Ptr
            std::cout << __FUNCSIG__ << std::endl;
            ::new (p) double(_STD forward<_Types>(_Args)...);
        }

        void destroy(double * p)
        {   // destroy object at _Ptr
        }


    };

#else 

    template <typename T> 
    class allocator {
    public: 
        typedef size_t size_type; 
        typedef ptrdiff_t difference_type; 
        typedef T* pointer; 
        typedef const T* const_pointer; 
        typedef T& reference; 
        typedef const T& const_reference; 
        typedef T value_type;
        template <typename U> struct rebind { typedef allocator<U> other; };
        pointer address(reference value) const noexcept; 
        const_pointer address(const_reference value) const noexcept;
        allocator() noexcept; 
        allocator(const allocator&) noexcept;
        template <typename U> 
        allocator(const allocator<U>&) noexcept; 
        ~allocator() {}
        size_type max_size() const noexcept;
        {
            return (std::numeric_limits<size_t>::max() / sizeof(T));
        }
        pointer allocate(size_type size, allocator<void>::const_pointer /*hint*/ = 0)
        {
            return ::operator new(size * sizeof(T));
        }
        void deallocate(pointer p, size_type num)
        {
            ::operator delete(p);
        }
        template <typename U, typename... Args> 
        void construct(U* p, Args&&... args)
        {
            std::cout << __FUNCSIG__ << std::endl;
            ::new (p) U(std::forward<Args>(args)...);
        }
        template <typename U> 
        void destroy(U* p)
        {
            p->~U();
        }
};


#endif

} // namespace std {

#endif


int main()
{
    std::vector<double> vec1{ 1,2 };
    std::vector<double, my_minimal_allocator<double>> vec2{ 1,2 };
    std::cout << std::endl;
    for (auto i : vec2)
    {
        std::cout << " " << i;
    }
    std::cout << std::endl << "Finished...." << std::endl;
    return 0;
}

