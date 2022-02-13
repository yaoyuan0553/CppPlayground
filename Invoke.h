//
// Created by yuan on 2/3/22.
//

#pragma once
#ifndef CPPPLAYGROUND_INVOKE_H
#define CPPPLAYGROUND_INVOKE_H

#include <type_traits>
#include <any>
#include <memory>

namespace details
{

struct Nat {
    Nat() = delete;
    Nat(const Nat&) = delete;
    Nat& operator=(const Nat&) = delete;
    ~Nat() = delete;
};

template <typename T>
struct IsReferenceWrapperImpl : public std::false_type { };

template <typename T>
struct IsReferenceWrapperImpl<std::reference_wrapper<T>> : public std::true_type { };

template <typename T>
struct IsReferenceWrapper : public IsReferenceWrapperImpl<std::remove_cv_t<T>> { };

template <typename T>
inline constexpr bool IsReferenceWrapper_v = IsReferenceWrapper<T>::value;

template <typename Fp>
struct MemberFunctionClass { };

template <typename Cls, typename Fp>
struct MemberFunctionClass<Fp Cls::*> {
    using type = Cls;
};

template <typename Fp>
using MemberFunctionClass_t = typename MemberFunctionClass<Fp>::type;

template <typename T>
struct IsSmartPtr : public std::false_type { };

template <template<typename...> typename R, typename... Ts>
struct IsSmartPtr<R<Ts...>> : public std::conditional_t
    <
        std::is_same_v<R<Ts...>, std::unique_ptr<Ts...> > || std::is_same_v<R<Ts...>, std::shared_ptr<Ts...>>,
        std::true_type,
        std::false_type
    > { };

template <typename T>
inline constexpr bool IsSmartPtr_v = IsSmartPtr<T>::value;

template <typename T>
struct IsPointerOrSmartPtr : public std::conditional_t
    <
        std::is_pointer_v<std::remove_cv<T>> || IsSmartPtr_v<std::remove_cv<T>>,
        std::true_type,
        std::false_type
    > { };

template <typename T>
inline constexpr bool IsPointerOrSmartPtr_v = IsPointerOrSmartPtr<T>::value;

#define INVOKE_RETURN(...) \
    noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) \
    { return __VA_ARGS__; }

template <typename... Args>
auto Invoke(std::any, Args&&...) -> Nat;

// member function/object pointer with obj reference
template <
    typename Fp,
    typename Obj,
    template <typename> typename FuncOrObj,
    typename DecayFp = std::decay_t<Fp>,
    typename DecayObj = std::decay_t<Obj>,
    typename FpClass = MemberFunctionClass_t<DecayFp>
>
using EnableIfIsMemberWithObjReference_t = std::enable_if_t
    <
        FuncOrObj<DecayFp>::value
        && std::is_base_of_v<FpClass, DecayObj>
    >;

// member function/object pointer with reference wrapper as obj
template <
    typename Fp,
    typename Obj,
    template <typename> typename FuncOrObj,
    typename DecayFp = std::decay_t<Fp>,
    typename DecayObj = std::decay_t<Obj> >
using EnableIfIsMemberWithReferenceWrapper_t = std::enable_if_t
    <
        FuncOrObj<DecayFp>::value
        && IsReferenceWrapper_v<DecayObj>
    >;

// member function/object pointer with (smart) pointer as obj
template <
    typename Fp,
    typename Obj,
    template <typename> typename FuncOrObj,
    typename DecayFp = std::decay_t<Fp>,
    typename DecayObj = std::decay_t<Obj>,
    typename FpClass = MemberFunctionClass_t<DecayFp>
>
using EnableIfIsMemberWithPointer_t = std::enable_if_t
    <
        FuncOrObj<DecayFp>::value
        && !std::is_base_of_v<FpClass, DecayObj>
        && !IsReferenceWrapper_v<DecayObj>
    >;

template <typename Fp, typename Obj, typename... Args,
    typename = EnableIfIsMemberWithObjReference_t<Fp, Obj, std::is_member_function_pointer>>
constexpr auto Invoke(Fp&& fp, Obj&& obj, Args&&... args)
INVOKE_RETURN((std::forward<Obj>(obj).*fp)(std::forward<Args>(args)...))

template <typename Fp, typename Obj, typename... Args,
    typename = EnableIfIsMemberWithReferenceWrapper_t<Fp, Obj, std::is_member_function_pointer>>
constexpr auto Invoke(Fp&& fp, Obj&& obj, Args&&... args)
INVOKE_RETURN((std::forward<Obj>(obj).get().*fp)(std::forward<Args>(args)...))

template <typename Fp, typename Obj, typename... Args,
    typename = EnableIfIsMemberWithPointer_t<Fp, Obj, std::is_member_function_pointer>>
constexpr auto Invoke(Fp&& fp, Obj&& obj, Args&&... args)
INVOKE_RETURN(((*std::forward<Obj>(obj)).*fp)(std::forward<Args>(args)...))

template <typename Fp, typename Obj,
    typename = EnableIfIsMemberWithObjReference_t<Fp, Obj, std::is_member_object_pointer>>
constexpr auto Invoke(Fp&& fp, Obj&& obj)
INVOKE_RETURN(std::forward<Obj>(obj).*fp)

template <typename Fp, typename Obj,
    typename = EnableIfIsMemberWithReferenceWrapper_t<Fp, Obj, std::is_member_object_pointer>>
constexpr auto Invoke(Fp&& fp, Obj&& obj)
INVOKE_RETURN(std::forward<Obj>(obj).get().*fp)

template <typename Fp, typename Obj,
    typename = EnableIfIsMemberWithPointer_t<Fp, Obj, std::is_member_object_pointer>>
constexpr auto Invoke(Fp&& fp, Obj&& obj)
INVOKE_RETURN((*std::forward<Obj>(obj)).*fp)

}

#endif //CPPPLAYGROUND_INVOKE_H
