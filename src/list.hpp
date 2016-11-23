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

        ListPtr _tail(const unsigned position=0) const {
            return position? this->next->tail(position - 1) : this->next;
        }

        ListPtr _reverse(const ListPtr acc=nullptr) const {
            if (this->next) {
                return this->next->_reverse(
                    ListPtr(new List<T>(this->value, acc))
                );
            } else {
                return ListPtr(new List<T>(this->value, acc));
            }
        }

        ListPtr _insert(const T& value, const unsigned position) const {
            if (position == 0) {
                return this->insertFirst(value);
            } else if (position == this->size) {
                return this->insertLast(value);
            } else {
                return this->insertMiddle(value, position);
            }
        }
        ListPtr insertFirst(const T& value) const {
            return ListPtr(new List<T>(value, this->shared_from_this()));
        }
        ListPtr insertMiddle(const T& value, const unsigned position) const {
            const ListPtr newTail = ListPtr(
                new List<T>(value, this->tail(position - 1)));
            return this->reverse()->tail(this->size - position - 1)
                       ->_reverse(newTail);
        }
        ListPtr insertLast(const T& value) const {
            const ListPtr newTail = ListPtr(new List<T>(value, nullptr));
            return this->reverse()->_reverse(newTail);
        }

        ListPtr _remove(const unsigned position) const {
            if (position == 0) {
                return this->tail();
            } else if (size == 1 && position == 1) {
                return this->removeSecond();
            } else {
                return ListPtr(new List<T>(
                    this->value, this->next->_remove(position - 1)));
            }
        }
        ListPtr removeSecond() const {
            const ListPtr newTail = this->next? this->next->tail() : nullptr;
            return ListPtr(new List<T>(this->value, newTail));
        }

        List(const T* value, const unsigned size)
                : value(*value)
                , next(size > 1? new List<T>(value + 1, size - 1)
                               : nullptr)
                , size(size) {
        }
    public:
        List(const T& value, const ListPtr& next=nullptr)
                : value(value)
                , next(next)
                , size(next? next->size + 1 : 1) {
        }
        List(const std::initializer_list<T> value)
                : value(*value.begin())
                , next(value.size() > 1? new List<T>(value.begin() + 1,
                                                     value.size() - 1)
                                       : nullptr)
                , size(value.size()) {
            if (value.size() == 0) {
                throw std::invalid_argument("You can't create an empty list");
            }
        }
        List(const List<T>&) = delete;

        ListPtr insert(const T& value, const unsigned position=0) const
                throw (std::invalid_argument) {
            if (position > this->size) {
                throw std::invalid_argument(
                    "Position should not be greater than list size"
                );
            }
            return this->_insert(value, position);
        }

        ListPtr remove(const unsigned position=0) const
                throw (std::invalid_argument) {
            if (position >= this->size) {
                throw std::invalid_argument(
                    "Position should be less than list size"
                );
            }
            if (position == 0) {
                return this->tail();
            } else if (position == this->size - 1) {
                return this->reverse()->tail()->reverse();
            } else {
                return this->slice(0, position - 1)->concatenate(
                       this->slice(position + 1, -1));
            }
        }

        ListPtr tail(const unsigned position=0) const
                throw (std::invalid_argument) {
            if (position >= this->size) {
                throw std::invalid_argument(
                    "Position should be less than list size"
                );
            }
            return this->_tail(position);
        }
        ListPtr reverse() const {
            return this->_reverse();
        }

        ListPtr slice(const unsigned first, const unsigned last=-1) const
                throw (std::invalid_argument) {
            if (first > last) {
                throw std::invalid_argument(
                    "Slice first element index should not "
                    "be less than slice last element index"
                );
            } else if (first == 0 && last >= this->size) {
                throw std::invalid_argument(
                    "Slice should not contain all the list itself."
                );
            }

            if (first == 0) {
                return this->reverse()->tail(this->size - last - 2)->reverse();
            } else if (last >= this->size) {
                return this->tail(first - 1);
            } else {
                return this->tail(first - 1)->reverse()
                           ->tail(this->size - last - 2)->reverse();
            }
        }

        ListPtr concatenate(const ListPtr list) const {
            return this->reverse()->_reverse(list);
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
