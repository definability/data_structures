#ifndef LIST_HPP
#define LIST_HPP

#include <memory>

using std::invalid_argument;
using std::shared_ptr;
using std::initializer_list;

/**
 * \brief Immutable list implementation.
 */
template<typename T> class List
        : public std::enable_shared_from_this< List<T> >
        {
    /**
     * Handy alias for List template with T parameter.
     * Allows to avoid ugly code like `shared_ptr<List<T>>`.
     */
    using List_ = List<T>;
    /**
     * Handy alias for List pointer wrapped into shared pointer.
     */
    using ListPtr = const shared_ptr<const List_>;
    private:
        /**
         * value that the list stores.
         */
        const T value;
        /**
         * Tail of the list.
         */
        ListPtr tail_;
        /**
         * Size of the list.
         */
        const size_t size_;
        /**
         * \param amount Number of nodes to be removed.
         * \return List without `amount` first elements.
         */
        ListPtr _drop(const size_t amount) const {
            return amount
                ? this->tail_->_drop(amount - 1)
                : this->tail_;
        }
        /**
         * \param acc List that will be appended to end of resulting list.
         * \return Reversed list with `acc` appended.
         */
        ListPtr _reverse(ListPtr acc=nullptr) const {
            return this->tail_
                ? this->tail_->_reverse(List_::Cons(this->value, acc))
                : List_::Cons(this->value, acc);
        }
        /**
         * \param value Value to be inserted.
         * \param position Index of the inserted element in resulting list.
         */
        ListPtr _insert(const T& value, const size_t position) const {
            if (position == 0) {
                return this->insertFirst(value);
            } else if (position == this->size_) {
                return this->append(value);
            } else {
                return this->insertMiddle(value, position);
            }
        }
        /**
         * \param value Value to be inserted.
         * \return List with `value` in head and `this` in tail.
         */
        ListPtr insertFirst(const T& value) const {
            return List_::Cons(value, this->shared_from_this());
        }
        /**
         * \param value Value to be inserted.
         * \param position Index that the `value` should have in new List.
         * \return List with `value` inserted.
         *
         * Should be used only for cases when `position`
         * is more than `0` and less than `this` list size.
         */
        ListPtr insertMiddle(const T& value, const size_t position) const {
            return this
                ->reverse()
                ->drop(this->size_ - position - 1)
                ->_reverse(List_::Cons(value, this->drop(position - 1)));
        }
        /** \brief Helper function for fill().
         * \param amount Size of list to be created.
         * \param tail Reference that holds pointer to list,
         * that should be appended as a tail to newly created list.
         * \param value Value that should appear in each node of new list.
         *
         * I need the `tail` to be an instance of `shared_ptr<List<T>>`.
         * Separate List(const T&, const List_*) constructor
         * was created for this purpose.
         *
         * The `tail` is a reference to pointer,
         * because fill(), which called the fill_(),
         * should have an ability to free memory
         * in the case of fill_() fail
         * to avoid memory leak.
         */
        static const List_* fill_(unsigned amount, const List*& tail,
                                  const T& value) {
            if (!amount) {
                const List* result = tail;
                tail = nullptr;
                return result;
            }
            tail = new List_{value, tail};
            return fill_(amount - 1, tail, value);
        }
        /** \brief Helper constructor for initializer list arguments.
         * \param begin Beginning of values array.
         * \param size Size of the `begin` array.
         *
         * Used in List(const initializer_list<T>) constructor.
         * First element of `begin` array becomes a value of head of the list.
         * If there are more than one element in the array,
         * tail will be created recursively from pointer to the second element.
         *
         * Should not be used with empty arrays.
         * Size should be specified correctly.
         */
        List(const T* begin, const size_t size)
                : value{*begin}
                , tail_{size > 1? new List_(begin + 1, size - 1)
                               : nullptr}
                , size_{size} {
        }
        /**
         * \brief Take a regular pointer and wrap it into `shared_ptr`.
         * \param value Value of the head.
         * \param tail_ Pointer to tail.
         *
         * Wraps `tail_` into `shared_ptr`
         * and adds custom destruction strategy destroy().
         */
        List(const T& value, const List_* tail_)
            : value{value}
            , tail_{tail_, List::destroy}
            , size_{tail_ ? tail_->size_ + 1 : 1} {
        }
    public:
        /**
         * \param value Value of the head.
         * \param tail_ Tail of the list.
         */
        List(const T& value, ListPtr& tail_=nullptr)
                : value{value}
                , tail_{tail_}
                , size_{tail_? tail_->size_ + 1 : 1} {
        }
        /** \brief Creates new instance of List from initializer list.
         * \param value Initializer list of values for the list.
         */
        explicit List(const initializer_list<T> value)
                : value{*value.begin()}
                , tail_{value.size() > 1
                    ? new List_{value.begin() + 1, value.size() - 1}
                    : nullptr}
                , size_{value.size()} {
            if (value.size() == 0) {
                throw invalid_argument("You can't create an empty list");
            }
        }
        /**
         * Copy constructor is not needed,
         * because all fields of List are `const`,
         * so cannot be changed if you don't hack something.
         *
         * This means that instead of copying
         * you can freely use existent instance of the List.
         */
        List(const List_&) = delete;
        /**
         * Get size of the list in OOP way.
         */
        size_t size() const {
            return this->size_;
        }
        /**
         * \param value Value to be inserted.
         * \param position Index of the inserted element in resulting list.
         * \return List with new element.
         *
         * When `position` equals `0`, the element is a new head.
         * When `position` is equal to size, append() will be called.
         * \return List with new element inserted.
         */
        ListPtr insert(const T& value, const size_t position = 0) const {
            if (position > this->size_) {
                throw invalid_argument(
                    "Position should not be greater than list size"
                );
            }
            return this->_insert(value, position);
        }
        /**
         * \param position Position of element to be removed.
         * \return List without element that was had `position` index.
         */
        ListPtr remove(const size_t position = 0) const {
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
        /**
         * Get tail of the list in OOP way.
         */
        ListPtr tail() const {
            return this->tail_;
        }
        /**
         * \return List with elements in reversed order.
         */
        ListPtr reverse() const {
            return this->_reverse();
        }
        /**
         * \param first Amount of elements to be dropped
         * from the beginning of the list.
         * \param last Index of the last element that should appear
         * in new list.
         * \return List that contains elements
         * that had indices from `first` to `last` (including)
         * from original list.
         */
        ListPtr slice(const size_t first, const size_t last = -1) const {
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
        /**
         * \param amount Number of elements to remove.
         * \return List without first `amount` elements
         * of current List.
         */
        ListPtr drop(const size_t amount) const {
            return (amount >= this->size_)
                ? nullptr
                : this->_drop(amount);
        }
        /**
         * \param value Value to append.
         * \return List with the last node with `value`
         * appended to current list.
         */
        ListPtr append(const T& value) const {
            return this->concat(List_::Cons(value));
        }
        /**
         * \param list List to be concatenated.
         * \return Concatenation of current list with `list`.
         */
        ListPtr concat(ListPtr list) const {
            return this->reverse()->_reverse(list);
        }
        /**
         * \brief Check whether lists are not equal.
         * Equality means same number of nodes with same values.
         * \param list List to compare with.
         * \return `false` if lists are equal,
         * `true` otherwise.
         */
        bool operator!=(const List_& list) const {
            return false
                || ((!this->tail_) ^ (!list.tail_))
                || (this->value != list.value)
                || (this->tail_ != list.tail_ && *(this->tail_) != *(list.tail_));
        }
        /**
         * \brief Check whether lists are equal.
         * Equality means same number of nodes with same values.
         * \param list List to compare with.
         * \return `true` if lists are equal,
         * `false` otherwise.
         */
        bool operator==(const List_& list) const {
            return !(*this != list);
        }
        /**
         * \brief Creates `shared_ptr` to List
         * with custom destruction strategy destroy().
         * \param head Head for new list.
         * \param tail Tail for new list.
         * \return List with specified `head` and `tail`.
         */
        static ListPtr Cons(const T& head, ListPtr& tail = nullptr) {
            return ListPtr{new List_(head, tail), List_::destroy};
        }
        /**
         * \param amount Size of list to be created.
         * \param value Value that should appear in each node of new list.
         *
         * Public iterface for private fill_() method.
         * It wraps insertBulk_() result into `shared_ptr`
         * with custom destruction function destroy().
         *
         * Also it creates a guard for tail,
         * that will destroy the tail if something will go wrong.
         * fill_() should store `tail`,
         * that is not yet wrapped into `shared_ptr`,
         * in the guard, and set it to `nullptr` in the end
         * in order to avoid destruction of successfully created list.
         */
        static ListPtr fill(size_t amount, const T& value) {
            if (amount == 0) {
                throw invalid_argument("You can't create an empty list");
            }

            struct TailGuard {
                const List* ptr;
                ~TailGuard() {
                    List::destroy(this->ptr);
                }
            } guard{};
            const List* result = fill_(amount, guard.ptr, value);
            return amount
                ? ListPtr{result, List::destroy}
                : nullptr;
        }
        /** \brief Custom destruction strategy,
         * which should be called in order to delete a list.
         * \param list Pointer to list to destroy.
         *
         * Watching references count allows us to stop,
         * when we reached the node,
         * that is used by another list.
         *
         * Also this prevents long loop of construction and destruction,
         * because destruction calls this function `destroy` again
         * and it will create a lot of redundant entities
         * without `tail.use_count() == 1` condition.
         */
        static void destroy(const List_* list) {
            if (!list) return;

            shared_ptr<const List_> tail = list->tail_;
            delete list;

            for (; tail && tail.use_count() == 1; tail = tail->tail_);
        }
        /**
         * Destructor cannot be hidden yet,
         * but it should be,
         * because custom destruction function destroy()
         * should be used instead for long lists
         * in order to avoid stack overflow
         * during recursive calls.
         */
        ~List() {}
};

#endif
