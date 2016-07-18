// vs2012_vector_optimization_bug.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>

namespace std {

    // CLASS allocator<void>
    template<>
    class allocator<double>
    {	// generic allocator for type double
    public:
        typedef double _Not_user_specialized;

        typedef double value_type;

        typedef double *pointer;
        typedef const double *const_pointer;

        template<class _Other>
        struct rebind
        {	// convert this type to an allocator<_Other>
            typedef allocator<_Other> other;
        };

        allocator() _THROW0()
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
        {	// allocate array of _Count elements
            std::cout << __FUNCTION__  << "  " << size<< std::endl;
            return new double[size];
        }


        void deallocate(pointer p, size_t )
        {	// deallocate object at _Ptr, ignore size
            delete[] p;
        }

        static size_t max_size() noexcept
        {	// get maximum size
            return (1000000);
        }

/*
        template<class _Objty,
            class... _Types>
            void construct(_Objty *p, _Types&&... _Args)
        {	// construct _Objty(_Types...) at _Ptr
            std::cout << __FUNCSIG__ << std::endl;
            ::new (p) _Objty(_STD forward<_Types>(_Args)...);
        }
    */

        template<class... _Types>
            void construct(double *p, _Types&&... _Args)
        {	// construct _Objty(_Types...) at _Ptr
            std::cout << __FUNCSIG__ << std::endl;
            ::new (p) double(_STD forward<_Types>(_Args)...);
        }

        template<class _Uty>
        void destroy(_Uty *_Ptr)
        {	// destroy object at _Ptr
            _Ptr->~_Uty();
        }


    };

}




int main()
{
    std::vector<double> vec1{ 1,2 };
    std::vector<double> vec2(vec1);
    std::cout << std::endl;
    for (auto i : vec2)
    {
        std::cout << " " << i;
    }
    std::cout << std::endl << "Finished...." << std::endl;
    return 0;
}

