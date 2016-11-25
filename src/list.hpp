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

        static ListPtr Cons(const T& head, const ListPtr tail=nullptr) {
            return ListPtr(new List<T>(head, tail));
        }

        ListPtr _drop(const unsigned amount) const {
            return amount? this->next->_drop(amount - 1) : this->next;
        }

        ListPtr _reverse(const ListPtr acc=nullptr) const {
            if (this->next) {
                return this->next->_reverse(List<T>::Cons(this->value, acc));
            } else {
                return List<T>::Cons(this->value, acc);
            }
        }

        ListPtr _insert(const T& value, const unsigned position) const {
            if (position == 0) {
                return this->insertFirst(value);
            } else if (position == this->size) {
                return this->append(value);
            } else {
                return this->insertMiddle(value, position);
            }
        }
        ListPtr insertFirst(const T& value) const {
            return List<T>::Cons(value, this->shared_from_this());
        }
        ListPtr insertMiddle(const T& value, const unsigned position) const {
            return this
                ->reverse()
                ->drop(this->size - position - 1)
                ->_reverse(List<T>::Cons(value, this->drop(position - 1)));
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
                return this->slice(0, position - 1)->concat(
                       this->slice(position + 1, -1));
            }
        }

        ListPtr tail() const {
            return this->next;
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
                return this->reverse()->drop(this->size - last - 2)->reverse();
            } else if (last >= this->size - 1) {
                return this->drop(first - 1);
            } else {
                return this->drop(first - 1)->reverse()
                           ->drop(this->size - last - 2)->reverse();
            }
        }

        ListPtr drop(const unsigned amount) const {
            if (amount >= this->size) {
                return nullptr;
            }
            return this->_drop(amount);
        }

        ListPtr append(const T& value) const {
            return this->concat(List<T>::Cons(value));
        }

        ListPtr concat(const ListPtr list) const {
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
