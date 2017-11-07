#ifndef LIST_HPP
#define LIST_HPP

#include <memory>

using std::invalid_argument;

template<typename T> class List
        : public std::enable_shared_from_this< List<T> >
        {
    using ListPtr = const std::shared_ptr< const List<T> >;
    using List_ = List<T>;
    private:
        const T value;
        ListPtr next;
        const size_t size_;

        ListPtr _drop(const size_t amount) const {
            return amount
                ? this->next->_drop(amount - 1)
                : this->next;
        }

        ListPtr _reverse(ListPtr acc=nullptr) const {
            return this->next
                ? this->next->_reverse(List_::Cons(this->value, acc))
                : List_::Cons(this->value, acc);
        }

        ListPtr _insert(const T& value, const size_t position) const {
            if (position == 0) {
                return this->insertFirst(value);
            } else if (position == this->size_) {
                return this->append(value);
            } else {
                return this->insertMiddle(value, position);
            }
        }
        ListPtr insertFirst(const T& value) const {
            return List_::Cons(value, this->shared_from_this());
        }
        ListPtr insertMiddle(const T& value, const size_t position) const {
            return this
                ->reverse()
                ->drop(this->size_ - position - 1)
                ->_reverse(List_::Cons(value, this->drop(position - 1)));
        }

        List(const T* value, const size_t size)
                : value(*value)
                , next(size > 1? new List_(value + 1, size - 1)
                               : nullptr)
                , size(size) {
        }
    public:
        List(const T& value, ListPtr& next=nullptr)
                : value{value}
                , next{next}
                , size_{next? next->size_ + 1 : 1} {
        }
        List(const std::initializer_list<T> value)
                : value{*value.begin()}
                , next{value.size() > 1
                    ? new List_{value.begin() + 1, value.size() - 1}
                    : nullptr}
                , size_{value.size()} {
            if (value.size() == 0) {
                throw invalid_argument("You can't create an empty list");
            }
        }

        List(const List_&) = delete;

        size_t size() const {
            return this->size_;
        }

        ListPtr insert(const T& value, const size_t position=0) const
                throw (invalid_argument) {
            if (position > this->size_) {
                throw invalid_argument(
                    "Position should not be greater than list size"
                );
            }
            return this->_insert(value, position);
        }

        ListPtr remove(const size_t position=0) const
                throw (invalid_argument) {
            if (position >= this->size_) {
                throw invalid_argument(
                    "Position should be less than list size"
                );
            }
            if (position == 0) {
                return this->tail();
            } else if (position == this->size_ - 1) {
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

        ListPtr slice(const size_t first, const size_t last=-1) const
                throw (invalid_argument) {
            if (first > last) {
                throw invalid_argument(
                    "Slice first element index should not "
                    "be less than slice last element index"
                );
            } else if (first == 0 && last >= this->size_) {
                throw invalid_argument(
                    "Slice should not contain all the list itself."
                );
            }

            if (first == 0) {
                return this->reverse()->drop(this->size_ - last - 2)->reverse();
            } else if (last >= this->size_ - 1) {
                return this->drop(first - 1);
            } else {
                return this->drop(first - 1)->reverse()
                           ->drop(this->size_ - last - 2)->reverse();
            }
        }

        ListPtr drop(const size_t amount) const {
            return (amount >= this->size_)
                ? nullptr
                : this->_drop(amount);
        }

        ListPtr append(const T& value) const {
            return this->concat(List_::Cons(value));
        }

        ListPtr concat(ListPtr list) const {
            return this->reverse()->_reverse(list);
        }

        bool operator!=(const List_& list) const {
            return false
                || (!this->next ^ !list.next)
                || (this->value != list.value)
                || (this->next != list.next && *(this->next) != *(list.next));
        }
        bool operator==(const List_& list) const {
            return !(*this != list);
        }

        static ListPtr Cons(const T& head, ListPtr& tail=nullptr) {
            return ListPtr{new List_(head, tail), List_::destroy};
        }

        /**
         * Custom destruction strategy,
         * which should be called in order to delete a list.
         */
        static void destroy(const List_* list) {
            if (!list) return;

            std::shared_ptr<const List_> tail = list->next;
            delete list;

            /**
             * Watching references count allows us to stop,
             * when we reached the node,
             * which is used by another list.
             *
             * Also this prevents long loop of construction and destruction,
             * because destruction calls this function `destroy` again
             * and it will create a lot of redundant entities
             * without `tail.use_count() == 1` condition.
             */
            for (; tail && tail.use_count() == 1; tail = tail->next);
        }

        ~List() {}
};

#endif
