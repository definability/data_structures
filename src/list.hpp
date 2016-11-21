#ifndef LIST_HPP
#define LIST_HPP

#include <memory>

template<typename T> class List
        : public std::enable_shared_from_this< List<T> > {
    using ListPtr = std::shared_ptr< const List<T> >;
    private:
        const T value;
        const ListPtr next;
        const unsigned size;

        ListPtr _insert(const T& value, const unsigned position) const {
            if (position == 0) {
                return this->insertFirst(value);
            } else if (size == 1 && position == 1) {
                return this->insertSecond(value);
            } else {
                return ListPtr(new List<T>(
                    this->value, this->next->_insert(value, position - 1)));
            }
        }
        ListPtr insertFirst(const T& value) const {
            return ListPtr(new List<T>(value, this->shared_from_this()));
        }
        ListPtr insertSecond(const T& value) const {
            const ListPtr newTail = ListPtr(new List<T>(value, this->next));
            return ListPtr(new List<T>(this->value, newTail));
        }
    public:
        List(const T& value, const ListPtr& next=nullptr)
                : value(value), next(next), size(next? next->size + 1 : 1) {
        }
        List(const List<T>&) = delete;
        List(const T&, const List<T>*) = delete;

        ListPtr insert(const T& value, const unsigned position=0) const
                throw (std::invalid_argument) {
            if (position > size) {
                throw std::invalid_argument(
                    "Position should not be greater than list size"
                );
            }
            return this->_insert(value, position);
        }

        bool operator!=(const List<T>& list) const {
            return false
                || (!this->next ^ !list.next)
                || (this->value != list.value)
                || (this->next != list.next && *(this->next) != *(list.next));
        }
        bool operator==(const List<T>& list) const {
            return !(*this != list);
        }

        ~List() {}
};

#endif
