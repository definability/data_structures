#ifndef LIST_HPP
#define LIST_HPP

#include <memory>

template<typename T> class List {
    typedef std::shared_ptr< const List<T> > ListPtr;
    private:
        T value;
        ListPtr next;
    public:
        List(const T& value, const ListPtr& next=nullptr)
                : value(value), next(next) {
        }
        List(const List<T>&) = delete;
        List(const T&, const List<T>*) = delete;

        bool operator==(const List<T>& list) const {
            return !(!this->next ^ !list.next)
                && (this->value == list.value)
                && (this->next == list.next || *(this->next) == *(list.next));
        }
        bool operator!=(const List<T>& list) const {
            return !(*this == list);
        }

        ~List() {}
};

#endif
