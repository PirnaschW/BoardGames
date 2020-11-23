#include <utility>

template<typename T>
Pimpl<T>::Pimpl() : p_{ new T{} } {}

template<typename T>
template<typename ...Args>
Pimpl<T>::Pimpl(Args&& ...args) : p_{ new T{std::forward<Args>(args)...} } {}

template<typename T>
Pimpl<T>::~Pimpl() {}

template<typename T>
T* Pimpl<T>::operator->() { return p_.get(); }

template<typename T>
T& Pimpl<T>::operator*() { return *p_.get(); }
