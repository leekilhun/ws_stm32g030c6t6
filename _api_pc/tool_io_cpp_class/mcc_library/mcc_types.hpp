#pragma once
#ifndef _MCC_TYPES_HPP
#define _MCC_TYPES_HPP

/*

lee kil hun
gns.lee2@samsung.com
2023.04

*/


#ifndef NOMINMAX
#define NOMINMAX
#endif


#ifdef _MSVC_LANG 

	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif

#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <type_traits>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <exception>
#include <variant>
#include <tuple>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include <any>
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include <iterator>
#include <execution>
#include <ratio>
#include <optional>
#include <string_view>
#include <memory_resource>

#include <cassert>
#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <bitset>

#ifdef _MSC_VER
    #define __FUNCTION_NAME__ __FUNCSIG__
#else
    #define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#endif

namespace mcc
{
    namespace literals
    {
        inline constexpr std::size_t operator""_size_t(unsigned long long value)
        {
            return static_cast<std::size_t>(value);
        }

        inline constexpr unsigned int operator""_unsigned(unsigned long long value)
        {
            return static_cast<unsigned int>(value);
        }

        inline constexpr short operator""_short(unsigned long long value)
        {
            return static_cast<short>(value);
        }

        inline constexpr unsigned short operator""_ushort(unsigned long long value)
        {
            return static_cast<unsigned short>(value);
        }

        inline constexpr char operator""_char(unsigned long long value)
        {
            return static_cast<char>(value);
        }

        inline constexpr char operator""_schar(unsigned long long value)
        {
            return static_cast<signed char>(value);
        }

        inline constexpr unsigned char operator""_uchar(unsigned long long value)
        {
            return static_cast<unsigned char>(value);
        }

        inline constexpr long double operator""_ldouble(long double value)
        {
            return value;
        }
    }
}



/**
 * @brief Root namespace for C++ Extension Library
 * 
 */
namespace mcc
{
    enum class direction_t{ left, right };

     /**
     * @brief Remove const volatile reference from Type.
     * 
     * @tparam Type for remove const volatile and reference.
     */
    template<typename Type>
    using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

    template<typename Type>
    constexpr bool is_const_v = std::is_const_v<std::remove_reference_t<Type>>;

    /**
     * @brief Decay, remove const, volatile, reference
     * 
     * @tparam Type 
     */
    template<typename Type>
    using decay_remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<std::decay_t<Type>>>;

    /**
     * @brief Decay, remove reference
     * 
     * @tparam Type 
     */
    template<typename Type>
    using decay_remove_ref_t = std::remove_reference_t<std::decay_t<Type>>;

    /**
     * @brief Decay, remove const, volatile, reference, then remove pointer and remove const and volatile
     * 
     * @tparam Type 
     */
    template<typename Type>
    using primitive_type_t = remove_cv_ref_t<std::remove_pointer_t<decay_remove_cv_ref_t<Type>>>;

    template<typename Type_1, typename Type_2>
    auto maximum(Type_1 a, Type_2 b)
    {
        return a >= b ? a : b;
    }

    template<typename Type_1, typename Type_2, typename... Types>
    auto maximum(Type_1 a, Type_2 b, Types... args)
    {
        if constexpr(sizeof...(args)==0)
            return maximum(a, b);
        else
            return maximum(maximum(a, b), maximum(b, args...));
    }

    template<typename Type_1, typename Type_2>
    auto minimum(Type_1 a, Type_2 b)
    {
        return b < a ? b : a;
    }

    template<typename Type_1, typename Type_2, typename... Types>
    auto minimum(Type_1 a, Type_2 b, Types... args)
    {
        if constexpr(sizeof...(args)==0)
            return minimum(a, b);
        else
            return minimum(minimum(a, b), minimum(b, args...));
    }

    /**
     * @brief Test if Type is reference to const object
     * 
     * @tparam Type 
     */
    template<typename Type>
    constexpr bool is_const_reference_v =
        std::is_reference_v<Type> && std::is_const_v<std::remove_reference_t<Type>>;
             
   /**
    * @brief This class implements all debugging requirements for C++ Library Extension
    * 
    */
    class debug_exception: public std::exception
    {
        private:
            std::string m_message;
            int m_lineno;
            std::string m_file_name;
            std::string m_what_msg;

        public:
            debug_exception(std::string message,
                int lineno, std::string file_name):
                m_lineno(lineno), m_message(message), m_file_name(file_name)
            { 
                std::ostringstream os;

                os << "debug_exception - file [" << this->m_file_name << "]\n";
                os << "thread id [" << std::this_thread::get_id() << "] - ";
                os << "line [" << this->m_lineno<<"]\n";
                os << "message: " << this->m_message;
                
                this->m_what_msg = os.str();
            }

            virtual const char* what() const noexcept override
            {
                return this->m_what_msg.c_str();
            }

    }; // end of class debug_exception

    template<typename Type>
    constexpr unsigned long long two_power_n(Type n)
    {
        unsigned long long two_power = 1;
        return (two_power << (unsigned long long)n);
    }

     /**
     * @brief Type to string name conversions are defined.
     * 
     */
    namespace types
    {
        template<typename Type>
        inline constexpr auto type_max_v = std::numeric_limits<Type>::max();

        inline constexpr size_t InvalidIndex = type_max_v<size_t>;       
        inline constexpr size_t SelectAll = InvalidIndex;

        using big_integer_t = long long;
        using big_unsigned_t = unsigned long long;
        
        template<std::size_t KeyIndex, typename Type, auto Index, auto... Indices>
        constexpr auto get_nth_value(std::integer_sequence<Type, Index, Indices...>) noexcept
        {
            if constexpr(KeyIndex == 0)
                return Index;
            else if constexpr(KeyIndex>0 && sizeof...(Indices) != 0)
            {
                return get_nth_value<KeyIndex-1>(std::integer_sequence<Type, Indices...>{});
            }
            else
                return InvalidIndex;
        }

        template<auto Id, auto... Ids>
        using index_t = std::index_sequence<(std::size_t)Id, (std::size_t)Ids...>;

        template<auto N>
        using make_index_t = std::make_index_sequence<(std::size_t)N>;

        template<typename ...Types>
        using index_for = std::make_index_sequence<sizeof...(Types)>;

        template<typename Type>
        using remove_cvref_t = remove_cv_ref_t<Type>;

        template<typename Type, Type Tag = Type{}>
        class tag_type
        {
            public:

            using type = Type;
            static constexpr Type tag = Tag;

            protected:
                type value;
            
            public:
                    
            tag_type(Type v = Type{}) noexcept: value{ std::move(v) } { }

            template<typename ArgType, typename... ArgTypes>
            tag_type(ArgType arg, ArgTypes... args) noexcept: 
                value{ std::move(arg), std::move(args)... } { }

            tag_type(const tag_type&) noexcept = default;
            tag_type& operator=(const tag_type&) noexcept = default;

            tag_type(tag_type&&) noexcept= default;
            tag_type& operator=(tag_type&&) noexcept = default;
            
            const Type& get() const noexcept { return value; }
            Type& get() noexcept { return value; }

            operator const Type&() const noexcept{ return get(); }
            operator Type&() noexcept { return get(); }
        };

        namespace hidden
        {
            template<typename Type>
            struct st_is_tag_type_v
            {
                constexpr static bool value = false;
            };

            template<typename Type, int Tag>
            struct st_is_tag_type_v<tag_type<Type, Tag>>
            {
                constexpr static bool value = true;
            };

            template<typename Type>
            constexpr bool is_tag_type_v = st_is_tag_type_v<Type>::value;

            template<typename Type>
            struct st_is_numbers_type_v
            {
                constexpr static bool value = false;
            };

            template<template<auto...> class ContainerType, auto... Ints>
            struct st_is_numbers_type_v<ContainerType<Ints...>>
            {
                constexpr static bool value = true;
            };

        }
        // end of namespace hidden

        template<typename Type>
        constexpr bool is_tag_type_v = hidden::is_tag_type_v<remove_cvref_t<Type>>;

        template<typename Type>
        constexpr bool is_numbers_type_v = hidden::st_is_numbers_type_v<remove_cvref_t<Type>>::value;

        template<typename Type>
        decltype(auto) get_value(Type&& arg)
        {
            if constexpr(is_tag_type_v<Type>)
            {
                if constexpr(std::is_rvalue_reference_v<decltype(arg)>)
                    return std::move(arg.value);
                else
                    return arg.get();
            }
            else
                return std::forward<Type>(arg);
        }

        namespace hidden
        {
            // assume non-primitive type
            template<typename Type, auto Tag, bool = false>
            struct st_make_non_class_wrapper
            {
                using type = Type;
            };

            template<typename Type, auto Tag>
            struct st_make_non_class_wrapper<Type, Tag, true>
            {
                using type = tag_type<Type, Tag>;
            };

            template<typename Type, auto Tag>
            using non_class_wrapper_t = typename
                st_make_non_class_wrapper<Type, Tag, std::is_class_v<Type>>::type;

        }
        // end of namespace hidden

        template<typename Type, auto Tag>
            using non_class_wrapper_t = hidden::non_class_wrapper_t<Type, Tag>;
        		
		template <class TargetType, class _Ty>
		[[nodiscard]] constexpr std::enable_if_t<std::is_same_v<std::remove_reference_t<TargetType>,
			std::remove_reference_t<_Ty>>, _Ty&&>
			smart_forward(std::remove_reference_t<_Ty>& _Arg) noexcept 
		{  
			// std::cout <<"forwarding" << std::endl;
			return static_cast<_Ty&&>(_Arg);
		}

		template <class TargetType, class _Ty>
		[[nodiscard]] constexpr std::enable_if_t< !std::is_same_v<std::remove_reference_t<TargetType>,
			std::remove_reference_t<_Ty>>, TargetType>
			smart_forward(std::remove_reference_t<_Ty>& _Arg) noexcept 
		{  
			// std::cout <<"casting"<< std::endl;
			return static_cast<TargetType>(_Arg);
		}

		template <class TargetType, class _Ty>
		[[nodiscard]] constexpr std::enable_if_t< std::is_same_v<std::remove_reference_t<TargetType>,
			std::remove_reference_t<_Ty>>, _Ty&&>
		smart_forward(std::remove_reference_t<_Ty>&& _Arg) noexcept 
		{
			// std::cout <<"forwarding" << std::endl;
			static_assert(!std::is_lvalue_reference_v<_Ty>, "bad forward call");
			return static_cast<_Ty&&>(_Arg);
		}

		template <class TargetType, class _Ty>
		[[nodiscard]] constexpr std::enable_if_t< !std::is_same_v<std::remove_reference_t<TargetType>,
			std::remove_reference_t<_Ty>>, TargetType>
		smart_forward(std::remove_reference_t<_Ty>&& _Arg) noexcept 
		{   
			// std::cout <<"casting"<< std::endl;
			return static_cast<TargetType>(_Arg);
		}

        template <class TargetType, class _Ty>
        [[nodiscard]] constexpr std::enable_if_t< std::is_same_v<std::remove_reference_t<TargetType>,
		std::remove_reference_t<_Ty>>, std::remove_reference_t<_Ty>&&>
        smart_move(_Ty&& _Arg) noexcept
        {   
            // forward _Arg as movable
            return static_cast<std::remove_reference_t<_Ty>&&>(_Arg);
        }

        template <class TargetType, class _Ty>
        [[nodiscard]] constexpr std::enable_if_t< !std::is_same_v<std::remove_reference_t<TargetType>,
			std::remove_reference_t<_Ty>>, TargetType>
        smart_move(_Ty&& _Arg) noexcept
        {   
            // forward _Arg as movable
            return static_cast<TargetType>(_Arg);
        }

		template<auto N, typename T, typename Deleter>
		inline auto& cast_ref(std::unique_ptr<T[], Deleter>& uptr) noexcept
		{
			return reinterpret_cast<T(&)[N]>(uptr[0]);
		}

        template<auto N, typename T, typename Deleter>
		inline auto& cast_ref(std::index_sequence<N>, std::unique_ptr<T[], Deleter>& uptr) noexcept
		{
			return reinterpret_cast<T(&)[N]>(uptr[0]);
		}

		template<auto N, typename T, typename Deleter>
		inline auto& cast_ref(std::unique_ptr<T[], Deleter> const& uptr) noexcept
		{
			return reinterpret_cast<const T(&)[N]>(uptr[0]);
		}

        template<auto N, typename T, typename Deleter>
		inline auto& cast_ref(std::index_sequence<N>, std::unique_ptr<T[], Deleter> const& uptr) noexcept
		{
			return reinterpret_cast<const T(&)[N]>(uptr[0]);
		}

        ////////////////////////////////////////////////
        template<auto N1, auto N2, typename T, typename Deleter>
		inline auto& cast_ref(std::unique_ptr<T[], Deleter>& uptr) noexcept
		{
			return reinterpret_cast<T(&)[N1][N2]>(uptr[0]);
		}

        template<auto N1, auto N2, typename T, typename Deleter>
		inline auto& cast_ref(std::index_sequence<N1, N2>, std::unique_ptr<T[], Deleter>& uptr) noexcept
		{
			return reinterpret_cast<T(&)[N1][N2]>(uptr[0]);
		}

		template<auto N1, auto N2, typename T, typename Deleter>
		inline auto& cast_ref(std::unique_ptr<T[], Deleter> const& uptr) noexcept
		{
			return reinterpret_cast<const T(&)[N1][N2]>(uptr[0]);
		}

        template<auto N1, auto N2, typename T, typename Deleter>
		inline auto& cast_ref(std::index_sequence<N1, N2>, std::unique_ptr<T[], Deleter> const& uptr) noexcept
		{
			return reinterpret_cast<const T(&)[N1][N2]>(uptr[0]);
		}
        ///////////////////////////////////////////////
        template<auto N1, auto N2, auto N3, typename T, typename Deleter>
		inline auto& cast_ref(std::unique_ptr<T[], Deleter>& uptr) noexcept
		{
			return reinterpret_cast<T(&)[N1][N2][N3]>(uptr[0]);
		}

        template<auto N1, auto N2, auto N3, typename T, typename Deleter>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, std::unique_ptr<T[], Deleter>& uptr) noexcept
		{
			return reinterpret_cast<T(&)[N1][N2][N3]>(uptr[0]);
		}

		template<auto N1, auto N2, auto N3, typename T, typename Deleter>
		inline auto& cast_ref(std::unique_ptr<T[], Deleter> const& uptr) noexcept
		{
			return reinterpret_cast<const T(&)[N1][N2][N3]>(uptr[0]);
		}

        template<auto N1, auto N2, auto N3, typename T, typename Deleter>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, std::unique_ptr<T[], Deleter> const& uptr) noexcept
		{
			return reinterpret_cast<const T(&)[N1][N2][N3]>(uptr[0]);
		}

        //////////////////////////////////////////////

        template<auto N1, auto N2, typename T, auto N,
		    typename = std::enable_if_t<N1* N2 == N>>
		inline auto& cast_ref(T(&array)[N]) noexcept
		{
			using array2_t = T[N1][N2];
			return reinterpret_cast<array2_t&>(array[0]);
		}

        template<auto N1, auto N2, typename T, auto N,
		    typename = std::enable_if_t<N1* N2 == N>>
		inline auto& cast_ref(std::index_sequence<N1, N2>, T(&array)[N]) noexcept
		{
			using array2_t = T[N1][N2];
			return reinterpret_cast<array2_t&>(array[0]);
		}

		template<auto N1, auto N2, typename T, auto N = N1 * N2>
		inline auto& cast_ref(T(&array)[N1][N2])  noexcept
		{
			using array_t = T[N];
			return reinterpret_cast<array_t&>(array[0][0]);
		}

        template<auto N1, auto N2, typename T, auto N = N1 * N2>
		inline auto& cast_ref(std::index_sequence<N1, N2>, T(&array)[N1][N2])  noexcept
		{
			using array_t = T[N];
			return reinterpret_cast<array_t&>(array[0][0]);
		}

		template<auto N1, auto N2, auto N3, typename T, auto N,
			typename = std::enable_if_t<N1* N2* N3 == N>>
		inline auto& cast_ref(T(&array)[N])  noexcept
		{
			using array3_t = T[N1][N2][N3];
			return reinterpret_cast<array3_t&>(array[0]);
		}

        template<auto N1, auto N2, auto N3, typename T, auto N,
			typename = std::enable_if_t<N1* N2* N3 == N>>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, T(&array)[N]) noexcept
		{
			using array3_t = T[N1][N2][N3];
			return reinterpret_cast<array3_t&>(array[0]);
		}

		template<auto N1, auto N2, auto N3, typename T, auto N = N1 * N2* N3>
		inline auto& cast_ref(T(&array)[N1][N2][N3])  noexcept
		{
			using array_t = T[N];
			return reinterpret_cast<array_t&>(array[0][0][0]);
		}

        template<auto N1, auto N2, auto N3, typename T, auto N = N1 * N2* N3>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, T(&array)[N1][N2][N3])  noexcept
		{
			using array_t = T[N];
			return reinterpret_cast<array_t&>(array[0][0][0]);
		}
        
        template<auto N, typename T,
            typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline auto& cast_ref(T array) noexcept
		{
            using ele_t = std::remove_pointer_t<T>;

			using array_t = ele_t[N];
			return reinterpret_cast<array_t&>(array[0]);
		}

        template<auto N1, auto N2, typename T,
            typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline auto& cast_ref(T array) noexcept
		{
            using ele_t = std::remove_pointer_t<T>;

			using array_t = ele_t[N1][N2];
			return reinterpret_cast<array_t&>(array[0]);
		}
        
		template<auto N1, auto N2, auto N3, typename T,
            typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline auto& cast_ref(T array)  noexcept
		{
            using ele_t = std::remove_pointer_t<T>;

			using array_t = ele_t[N1][N2][N3];
			return reinterpret_cast<array_t&>(array[0]);
		}

        template<auto N, typename T,
            typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline auto& cast_ref(std::index_sequence<N>, T array) noexcept
		{
            using ele_t = std::remove_pointer_t<T>;
			using array_t = ele_t[N];
			return reinterpret_cast<array_t&>(array[0]);
		}

        template<auto N1, auto N2, typename T,
            typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline auto& cast_ref(std::index_sequence<N1, N2>, T array) noexcept
		{
            using ele_t = std::remove_pointer_t<T>;
			using array_t = ele_t[N1][N2];
			return reinterpret_cast<array_t&>(array[0]);
		}
        
		template<auto N1, auto N2, auto N3, typename T,
            typename = std::enable_if_t<std::is_pointer_v<T>>>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, T array)  noexcept
		{
            using ele_t = std::remove_pointer_t<T>;

			using array_t = ele_t[N1][N2][N3];
			return reinterpret_cast<array_t&>(array[0]);
		}

		template<std::size_t N1, typename T, std::size_t N,
            typename = std::enable_if_t<N1 == N>>
		inline auto& cast_ref(std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, typename T, std::size_t N,
            typename = std::enable_if_t<N1 == N>>
		inline auto& cast_ref(std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

		template<std::size_t N1, std::size_t N2, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline auto& cast_ref(std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1][N2];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, std::size_t N2, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline auto& cast_ref(std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N1][N2];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, typename T, std::size_t N,
			typename = std::enable_if_t< N1 == N>>
		inline auto& cast_ref(std::index_sequence<N1>, std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, typename T, std::size_t N,
			typename = std::enable_if_t< N1 == N>>
		inline auto& cast_ref(std::index_sequence<N1>, std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N1];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, std::size_t N2, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline auto& cast_ref(std::index_sequence<N1, N2>, std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1][N2];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, std::size_t N2, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline auto& cast_ref(std::index_sequence<N1, N2>, std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N1][N2];
			return reinterpret_cast<c_array_t&>(array[0]);
		}
		
		template<std::size_t N1, std::size_t N2, std::size_t N3, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
		inline auto& cast_ref(std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, std::size_t N2, std::size_t N3, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
		inline auto& cast_ref(std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, std::size_t N2, std::size_t N3, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        template<std::size_t N1, std::size_t N2, std::size_t N3, typename T, std::size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(array[0]);
		}

        //////////////////////////////////////////////////////
        
        template<std::size_t N, typename T>
		inline auto& cast_ref(std::vector<T>& vctr) noexcept
		{
            assert(N == vctr.size());

			using c_array_t = T[N];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N, typename T>
		inline auto& cast_ref(std::vector<T> const& vctr) noexcept
		{
            assert(N == vctr.size());

			using c_array_t = const T[N];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N, typename T>
		inline auto& cast_ref(std::index_sequence<N>, std::vector<T>& vctr) noexcept
		{
            assert(N == vctr.size());

			using c_array_t = T[N];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N, typename T>
		inline auto& cast_ref(std::index_sequence<N>, std::vector<T> const& vctr) noexcept
		{
            assert(N == vctr.size());

			using c_array_t = const T[N];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

		template<std::size_t N1, std::size_t N2, typename T>
		inline auto& cast_ref(std::vector<T>& vctr) noexcept
		{
            assert(N1 * N2 == vctr.size());

			using c_array_t = T[N1][N2];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N1, std::size_t N2, typename T>
		inline auto& cast_ref(std::vector<T> const& vctr) noexcept
		{
            assert(N1 * N2 == vctr.size());

			using c_array_t = const T[N1][N2];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N1, std::size_t N2, typename T>
		inline auto& cast_ref(std::index_sequence<N1, N2>, std::vector<T>& vctr) noexcept
		{
            assert(N1 * N2 == vctr.size());

			using c_array_t = T[N1][N2];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N1, std::size_t N2, typename T>
		inline auto& cast_ref(std::index_sequence<N1, N2>, std::vector<T> const& vctr) noexcept
		{
            assert(N1 * N2 == vctr.size());

			using c_array_t = const T[N1][N2];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}
		
		template<std::size_t N1, std::size_t N2, std::size_t N3, typename T>
		inline auto& cast_ref(std::vector<T>& vctr) noexcept
		{
            assert(N1 * N2 * N3 == vctr.size());

			using c_array_t = T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N1, std::size_t N2, std::size_t N3, typename T>
		inline auto& cast_ref(std::vector<T> const& vctr) noexcept
		{
            assert(N1 * N2 * N3 == vctr.size());

			using c_array_t = const T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N1, std::size_t N2, std::size_t N3, typename T>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, std::vector<T>& vctr) noexcept
		{
            assert(N1 * N2 * N3 == vctr.size());

			using c_array_t = T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

        template<std::size_t N1, std::size_t N2, std::size_t N3, typename T>
		inline auto& cast_ref(std::index_sequence<N1, N2, N3>, std::vector<T> const& vctr) noexcept
		{
            assert(N1 * N2 * N3 == vctr.size());

			using c_array_t = const T[N1][N2][N3];
			return reinterpret_cast<c_array_t&>(vctr[0]);
		}

		template<size_t N1, size_t N2, typename T, size_t N,
			typename = std::enable_if_t<N1* N2 == N>>
		inline auto cast_array(T(&array)[N]) noexcept
		{
			using array_t = T[N1][N2];
			return *reinterpret_cast<array_t*>(array);
		}

		template<size_t N1, size_t N2, typename T, size_t N = N1 * N2>
		inline auto cast_array(T(&array)[N1][N2]) noexcept
		{
			using array_t = T[N];
			return *reinterpret_cast<array_t*>(array);
		}

		template<size_t N1, size_t N2, size_t N3, typename T, size_t N,
			typename = std::enable_if_t<N1* N2* N3 == N>>
		inline auto cast_array(T(&array)[N]) noexcept
		{
			using array_t = T[N1][N2][N3];
			return *reinterpret_cast<array_t*>(array);
		}

		template<size_t N1, size_t N2, size_t N3, typename T, size_t N = N1 * N2* N3>
		inline auto cast_array(T(&array)[N1][N2][N3]) noexcept
		{
			using array_t = T[N];
			return *reinterpret_cast<array_t*>(array);
		}

		template<typename T, size_t N>
		inline auto cast_array(std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N];
			return *reinterpret_cast<c_array_t*>(array.data());
		}

        template<typename T, size_t N>
		inline auto cast_array(std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N];
			return *reinterpret_cast<c_array_t*>(array.data());
		}

		template<size_t N1, size_t N2, typename T, size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline auto cast_array(std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1][N2];
			return *reinterpret_cast<c_array_t*>(array.data());
		}

        template<size_t N1, size_t N2, typename T, size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline auto cast_array(std::array<T, N> const& array) noexcept
		{
			using c_array_t = const T[N1][N2];
			return *reinterpret_cast<c_array_t*>(array.data());
		}
		
		template<size_t N1, size_t N2, size_t N3, typename T, size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
		inline auto cast_array(std::array<T, N>& array) noexcept
		{
			using c_array_t = T[N1][N2][N3];
			return *reinterpret_cast<c_array_t*>(array.data());
		}

        template<size_t N1, size_t N2, size_t N3, typename T, size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
		inline auto cast_array(std::array<T, N> const& array) noexcept
		{
			using c_array_t = T[N1][N2][N3];
			return *reinterpret_cast<c_array_t*>(array.data());
		}

        // always returns lvalue reference
        template<auto Index, typename... ArgTypes,
        typename = std::enable_if_t<(Index < sizeof...(ArgTypes))>>
        decltype(auto) get_nth_argument(ArgTypes&&... args)
        {
            auto arguments = std::forward_as_tuple( std::forward<ArgTypes>(args)... );

            using element_t = decltype(std::get<Index>(arguments));

            return std::forward<element_t>(std::get<Index>(arguments));
        }

        /**
         * @brief This type is used to manipulate type list.
         * 
         * @tparam Types for ZERO or more variadic template parameters.
         */
        template<typename... Types> struct type_list_t{};
        template<auto... Ints> struct numbers_t{};

        template<typename T, size_t Size>
        auto convert_to_tuple(const std::array<T, Size>& array);
        
        template<typename Type, auto Size>
        using create_tuple_t = decltype(types::convert_to_tuple(std::array<Type, Size>{}));
        
        namespace hidden
        {
            template<typename Type>
            struct st_is_vector_v
            {
                static constexpr bool value = false;
            };

            template<typename Type, typename... Types>
            struct st_is_vector_v<std::vector<Type, Types...>>
            {
                static constexpr bool value = true;
            };
           
            template<typename Type>
            struct st_is_integer_sequence
            {
                static constexpr bool value = false;
            };
            
            template<typename Type, auto... Indices>
            struct st_is_integer_sequence<std::integer_sequence<Type, Indices...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr bool is_integer_sequence_v = st_is_integer_sequence<Type>::value;

            template<typename T>
            struct st_is_type_list
            {
                static constexpr bool value = false;
            };

            template<typename... Ts>
            struct st_is_type_list<type_list_t<Ts...>>
            {
                static constexpr bool value = true;
            };

            template<typename T>
            constexpr bool is_type_list_v = st_is_type_list<remove_cv_ref_t<T>>::value;

            template<typename T>
            struct st_recursive_type_list
            {
                using type = T;
            };

            template<template<typename...> class TemplateType, typename... Types>
            struct st_recursive_type_list<TemplateType<Types...>>
            {
                using type = type_list_t<typename st_recursive_type_list<Types>::type...>;
            };

            template<typename T>
            using to_recursive_type_list_t = typename st_recursive_type_list<T>::type;

        }
        // end of namespace hidden        

        template<typename Type>
        constexpr bool is_vector_v = hidden::st_is_vector_v<remove_cvref_t<Type>>::value;

        template<typename Type>
        constexpr bool is_integer_sequence_v = hidden::is_integer_sequence_v<remove_cvref_t<Type>>;

        template<typename T>
        constexpr bool is_type_list_v = hidden::is_type_list_v<remove_cv_ref_t<T>>;

        template<typename T>
        using to_recursive_type_list_t = hidden::to_recursive_type_list_t<remove_cv_ref_t<T>>;

        /**
         * @brief Create type_list_t by removing const, volatile, reference
         * 
         * @tparam Types 
         */
        template<typename... Types>
        using remove_cv_ref_type_list_t =  type_list_t<mcc::remove_cv_ref_t<Types>...>;

        /**
         * @brief Create type_list_t by decaying and removing const, volatile, reference
         * 
         * @tparam Types 
         */
        template<typename... Types>
        using decay_remove_cv_ref_type_list_t =  type_list_t<mcc::decay_remove_cv_ref_t<Types>...>;

        /**
         * @brief Create type_list_t by decaying and removing reference
         * 
         * @tparam Types 
         */
        template<typename... Types>
        using decay_remove_ref_type_list_t =  type_list_t<mcc::decay_remove_ref_t<Types>...>;

        enum class TemplateCategory { Unknown, NonTemplate = Unknown, Type, Type_Value, Value, Value_Type };

        template<template<typename...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Type; }

        template<template<typename T, auto...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Type_Value; }

        template<template<auto...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Value; }

        template<template<auto, typename...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Value_Type; }

        template<typename> 
        constexpr auto template_category(...) { return TemplateCategory::Unknown; }

        namespace hidden
        {
            template<typename Type>
            struct st_is_template_type
            {
                static constexpr bool value = false;
            };

            template<template<typename...> class TemplateType, typename... Types>
            struct st_is_template_type<TemplateType<Types...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename T, auto...> class TemplateType, typename T, T... args>
            struct st_is_template_type<TemplateType<T, args...>>
            {
                static constexpr bool value = true;
            };

            template<template<auto...> class TemplateType, auto... args>
            struct st_is_template_type<TemplateType<args...>>
            {
                static constexpr bool value = true;
            };

            template<template<auto, typename...> class TemplateType, auto arg, typename... Types>
            struct st_is_template_type<TemplateType<arg, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr bool is_template_type_v = st_is_template_type<Type>::value;
        }
        // end of namespace hidden

        template<typename Type>
            constexpr bool is_template_type_v = hidden::is_template_type_v<remove_cvref_t<Type>>;

        namespace hidden
        {
            template<typename... Types>
            struct st_type_list_to_tuple_of_vectors
            {
                using type = std::tuple< std::vector<Types>... >;
            };

            template<typename... Types>
            struct st_type_list_to_tuple_of_vectors<type_list_t<Types...>>
            {
                using type = std::tuple<std::vector<Types>... >;
            };

            template<template<typename...> class CntrType, typename... Types>
            struct st_type_list_to_tuple_of_vectors<CntrType<Types...>>
            {
                using type = std::tuple< std::vector<Types>... >;
            };

            template<typename TupleType>
            struct st_tuple_to_type_list;

            template<typename... Types>
            struct st_tuple_to_type_list<std::tuple<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename TupleType>
            using tuple_to_type_list_t = typename st_tuple_to_type_list<TupleType>::type; 

            template<typename TypeList>
            struct st_type_list_to_tuple;

            template<typename... Types>
            struct st_type_list_to_tuple<type_list_t<Types...>>
            {
                using type = std::tuple<Types...>;
            };

            template<typename TypeList>
            using type_list_to_tuple_t = typename st_type_list_to_tuple<TypeList>::type; 

            template<typename TupleType>
            struct st_variant_to_type_list;

            template<typename... Types>
            struct st_variant_to_type_list<std::variant<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename VarType>
            using variant_to_type_list_t = typename st_variant_to_type_list<VarType>::type; 

        }
        // end of namespace hidden

        template<typename TupleType>
        using tuple_to_type_list_t = hidden::tuple_to_type_list_t<remove_cvref_t<TupleType>>; 

        template<typename TypeList>
        using type_list_to_tuple_t = hidden::type_list_to_tuple_t<remove_cvref_t<TypeList>>;

        template<typename VarType>
        using variant_to_type_list_t = hidden::variant_to_type_list_t<remove_cvref_t<VarType>>; 

        template<typename... Types>
        using tuple_of_vectors_t = typename hidden::st_type_list_to_tuple_of_vectors<Types...>::type;

        namespace hidden
        {
            template<std::size_t ...I>
            struct st_vector
            {
                using type = std::vector<std::size_t>;
            };

            template<typename Type> struct st_index_tuple_vector;
            
            template<std::size_t ...Is>
            struct st_index_tuple_vector<std::index_sequence<Is...>>
            {
                using type = std::tuple<typename st_vector<Is>::type...>;
            };
        
        }
        // end of namespace hidden
        
        template<std::size_t N>
        using index_tuple_vector_t = typename hidden::st_index_tuple_vector<std::make_index_sequence<N>>::type;

        template<typename PointerType, typename SizeType>
        struct array_wrapper_t: public std::pair<PointerType, SizeType>
        {
            public:
                using base_type = std::pair<PointerType, SizeType>;
               
                template<typename ElementType, size_t ElementCount>
                array_wrapper_t(ElementType(&array)[ElementCount]) noexcept:
                    std::pair<ElementType*, size_t>{array, ElementCount} { }
        };

        template<typename ElementType, size_t ElementCount>
        array_wrapper_t(ElementType(&)[ElementCount]) -> array_wrapper_t<ElementType*, size_t>;

        template<typename ElementType, size_t ElementSize>
        auto array_wrapper(ElementType(&array)[ElementSize]) noexcept
        {
            return array_wrapper_t<ElementType*, size_t>{array};
        }

        template<typename Type>
        auto to_ref(const Type& value) { return std::ref(const_cast<Type&>(value)); }
        
        template<typename Type>
        constexpr bool is_array_v = std::is_array_v<remove_cv_ref_t<Type>>;      

        template<typename ElementType, size_t ElementSize>
        constexpr size_t array_size(ElementType(&array)[ElementSize]) noexcept { return (size_t) ElementSize; }
              
        // forward declaration
        template<typename ContainerType> class reverse_st;
        template<typename ContainerType> auto reverse(ContainerType&& container);
        template<typename ElementType, size_t ElementCount> auto reverse(ElementType(&array)[ElementCount]);

        template<typename Type, typename... Types> auto reverse(Type&& arg, Types&&... args);
        template<typename Type, typename... Types> auto make_vector(Type&& arg, Types&&... args);
        template<typename Type, typename... Types> auto make_container(Type&& arg, Types&&... args);
        
        template< template<typename, std::size_t> class ContainerType,
            typename ArgType, typename... ArgTypes>
        auto make_container(ArgType&& arg, ArgTypes&& ... args);

        template< template<typename, std::size_t> class ContainerType,
            typename ArgType, typename... ArgTypes>
        auto make_container(ArgType&& arg, ArgTypes&& ... args);

        template<typename ContainerType, typename IndexType>
        decltype(auto) get_element(ContainerType container, IndexType index);

        #if !defined(__clang_major__)
            template<typename Type, typename... Types> auto make_variants(Type&& arg, Types&&... args);
        #endif

        template<typename Type>
        decltype(auto) decay(Type&& arg) 
        { 
            using type = std::remove_reference_t<Type>;

            if constexpr(std::is_array_v<type> || std::is_function_v<type>)
                return decay_remove_cv_ref_t<type>(arg);
            else
                return std::forward<Type>(arg);
        }

        namespace hidden
        {
            template<typename Type>
            struct st_dummy_type_container{ };
        }

        /**
         * @brief Returns Type's string name.
         * 
         * @tparam Type &mdash; Constraints: NONE
         * @return std::string &mdash; if fails, DOES NOT FAIL
         * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
         */
        template<typename Type>
        std::string type_to_string()
        {
            #ifdef __NVCC__

                {   std::string fname = typeid(hidden::st_dummy_type_container<Type>).name();

                    const char* to_str = "struct mcc::types::hidden::st_dummy_type_container<";

                    std::size_t len = strlen(to_str);

                    auto pos = fname.find(to_str);

                    if(pos != fname.npos) // we found substring 
                        fname = fname.substr(pos + len);
                    
                    // remove trailing "> "
                    fname.pop_back();

                    //////////////////////////////////////////////
                    
                    to_str = " __ptr64"; len = strlen(to_str);
                    while( (pos = fname.find(to_str)) != fname.npos)
                        fname.erase(pos, len);
                        
                    to_str = " __ptr32"; len = strlen(to_str);
                    while( (pos = fname.find(to_str)) != fname.npos)
                        fname.erase(pos, len);

                    to_str = "enum class"; len = strlen(to_str);
                    while( (pos = fname.find(to_str)) != fname.npos)
                        fname.erase(pos, len);

                    to_str = "enum"; len = strlen(to_str);
                    while( (pos = fname.find(to_str)) != fname.npos)
                        fname.erase(pos, len);

                    to_str = "class"; len = strlen(to_str);
                    while( (pos = fname.find(to_str)) != fname.npos)
                        fname.erase(pos, len);

                    to_str = "struct"; len = strlen(to_str);
                    while( (pos = fname.find(to_str)) != fname.npos)
                        fname.erase(pos, len);
                   
                    while(fname.back() == ' ') fname.pop_back();
                    
                    return fname;
                }

            #elif defined(__FUNCSIG__)
                std::string fname(__FUNCSIG__);
                const char* to_str = "to_string<";
                size_t len = strlen(to_str);
                auto pos = fname.find("to_string<");
                fname = fname.substr(pos+len);
                fname = fname.substr(0, fname.find_last_of('>'));

                //////////////////////////////////////////////
                    
                to_str = " __ptr64"; len = strlen(to_str);
                while( (pos = fname.find(to_str)) != fname.npos)
                    fname.erase(pos, len);
                    
                to_str = " __ptr32"; len = strlen(to_str);
                while( (pos = fname.find(to_str)) != fname.npos)
                    fname.erase(pos, len);

                to_str = "enum class"; len = strlen(to_str);
                while( (pos = fname.find(to_str)) != fname.npos)
                    fname.erase(pos, len);

                to_str = "enum"; len = strlen(to_str);
                while( (pos = fname.find(to_str)) != fname.npos)
                    fname.erase(pos, len);

                to_str = "class"; len = strlen(to_str);
                while( (pos = fname.find(to_str)) != fname.npos)
                    fname.erase(pos, len);

                to_str = "struct"; len = strlen(to_str);
                while( (pos = fname.find(to_str)) != fname.npos)
                    fname.erase(pos, len);
                
                while(fname.back() == ' ') fname.pop_back();
                
                return fname;
            #else
                
                std::string fname(__PRETTY_FUNCTION__);
                
                #ifdef __clang_major__
                    const char* ftext = "[Type = ";
                    auto pos = fname.find(ftext) + strlen(ftext);
                    fname = fname.substr(pos);
                    fname.pop_back();
                    return fname;

                #elif defined(__ICL)
                    const char* ftext = "type_to_string<";
                    auto pos = fname.find(ftext) + strlen(ftext);
                    fname = fname.substr(pos);
                    pos = fname.find_last_of('>');
                    return fname.substr(0, pos);
                #else
                    const char* ftext = "[with Type = ";
                    auto pos = fname.find(ftext) + strlen(ftext);
                    fname = fname.substr(pos);
                    pos = fname.find_first_of(';');
                    return fname.substr(0, pos);
                 #endif
             #endif

        } // end of type_to_string()

    } // end of namespace types



namespace types
{
        template <typename Type, typename... Types>
        struct is_same_st;

        template <typename Type>
        struct is_same_st<Type>
        {
                static constexpr bool value = true;
        };

        template <typename Type1, typename Type2>
        struct is_same_st<Type1, Type2>
        {
                static constexpr bool value = std::is_same_v<Type1, Type2>;
        };

        template <typename Type1, typename Type2, typename... Types>
        struct is_same_st<Type1, Type2, Types...>
        {
                static constexpr bool value = is_same_st<Type1, Type2>::value &&
                                              is_same_st<Type2, Types...>::value;
        };

        template <typename Type>
        struct is_same_st<type_list_t<Type>>
        {
                static constexpr bool value = true;
        };

        template <typename Type1, typename Type2>
        struct is_same_st<type_list_t<Type1, Type2>>
        {
                static constexpr bool value = is_same_st<Type1, Type2>::value;
        };

        template <typename Type1, typename Type2, typename... Types>
        struct is_same_st<type_list_t<Type1, Type2, Types...>>
        {
                static constexpr bool value =
                    is_same_st<Type1, Type2, Types...>::value;
        };

        template <typename Type, typename... Types>
        constexpr auto is_same_v = is_same_st<Type, Types...>::value;

}
// end of namespace

} // end of namespace mmc

#define Mcc_DebugException(debug_message) mcc::debug_exception{debug_message, __LINE__, __FILE__}


/**
 * @brief Throw a debug_exception with \a message as argument
 * 
 */
#define Mcc_ThrowDebugException(debug_message) throw mcc::debug_exception{debug_message, __LINE__, __FILE__}

 /**
  * @brief A macro that returns type_arg's string name
  * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
  */
#define Mcc_GetTypeName(type_arg) mcc::types::type_to_string<type_arg>()
 
 /**
  * @brief A macro that returns instance_arg's type category string name 
  * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
  */
#define Mcc_GetTypeCategory(instance_arg) Mcc_GetTypeName(decltype(instance_arg))
 
/**
 * @brief A macro that returns instance_arg's value category string name 
 * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
 */
#define Mcc_GetValueCategory(instance_arg) Mcc_GetTypeName(decltype((instance_arg)))




#endif 
// end of file _MCC_TYPES_HPP
