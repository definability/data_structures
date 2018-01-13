#ifndef LIST_HPP
#define LIST_HPP

#include <memory>

using std::invalid_argument;
using std::shared_ptr;
using std::initializer_list;

/**
 * \brief Immutable list implementation.
 */
template<typename T> class List {
private:
    /**
     * \brief Internal implementation of immutable list
     * which needs to be wrapped into public List.
     */
    class List_ : public std::enable_shared_from_this<List_> {
    private:
        /**
         * Handy alias for List pointer wrapped into shared pointer.
         */
        using ListPtr = const shared_ptr<const List_>;
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
                ? this->tail_->_reverse(ListPtr{new List_{this->value, acc}, List_::destroy})
                : ListPtr{new List_{this->value, acc}, List_::destroy};
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
            return ListPtr{new List_{value, this->shared_from_this()}, List_::destroy};
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
                ->_reverse(ListPtr{new List_{value, this->drop(position - 1)}, List_::destroy});
        }
        /** \brief Helper function for fill().
         * \param amount Size of list to be created.
         * \param tail Reference that holds pointer to list,
         * that should be appended as a tail to newly created list.
         * \param value Value that should appear in each node of new list.
         *
         * I need the `tail` to be an instance of ListPtr.
         * Separate List(const T&, const List_*) constructor
         * was created for this purpose.
         *
         * The `tail` is a reference to pointer,
         * because fill(), which called the fill_(),
         * should have an ability to free memory
         * in the case of fill_() fail
         * to avoid memory leak.
         */
        static const List_* fill_(unsigned amount, const List_*& tail,
                                  const T& value) {
            if (!amount) {
                const List_* result = tail;
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
         * First element of `begin` array
         * becomes a value of head of the list.
         * If there are more than one element in the array,
         * tail will be created recursively
         * from pointer to the second element.
         *
         * Should not be used with empty arrays.
         * Size should be specified correctly.
         */
        List_(const T* begin, const size_t size)
                : value{*begin}
                , tail_{size > 1? ListPtr{new List_(begin + 1, size - 1), List_::destroy}
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
        List_(const T& value, const List_* tail_)
            : value{value}
            , tail_{tail_, List_::destroy}
            , size_{tail_ ? tail_->size_ + 1 : 1} {
        }
        /**
         * Custom destruction function List_::destroy()
         * should be used instead of destructor for long lists
         * in order to avoid stack overflow
         * during recursive calls.
         */
        ~List_() = default;
    public:
        /**
         * \param value Value of the head.
         * \param tail_ Tail of the list.
         */
        List_(const T& value, ListPtr& tail_)
                : value{value}
                , tail_{tail_}
                , size_{tail_? tail_->size_ + 1 : 1} {
        }
        /** \brief Creates new instance of List from initializer list.
         * \param value Initializer list of values for the list.
         */
        explicit List_(const initializer_list<T> value)
                : value{*value.begin()}
                , tail_{value.size() > 1
                    ? ListPtr{new List_{value.begin() + 1, value.size() - 1}, List_::destroy}
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
        List_(const List_&) = delete;
        /**
         * We cannot copy, but sometimes we need an ability to move list.
         * For example, when some function returns a List_
         * and we store it in variable,
         * it needs to be either copied or moved.
         * We have prevented copying by deleting List_(const List_&),
         * so the copy constructor was explicitly introduced.
         */
        List_(List_&&) = default;
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
                return this->reverse()
                           ->drop(this->size_ - last - 2)
                           ->reverse();
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
            return this->concat(ListPtr{new List_{value}, List_::destroy});
        }
        /**
         * \brief Unite two lists together.
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
                || (this->tail_ != list.tail_
                    && *(this->tail_) != *(list.tail_));
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
         * \param amount Size of list to be created.
         * \param value Value that should appear in each node of new list.
         *
         * Public iterface for private List_::fill_() method.
         * It wraps List_::fill_() result into `shared_ptr`
         * with custom destruction function destroy().
         *
         * Also it creates a guard for tail,
         * that will destroy the tail if something will go wrong.
         * List::fill_() should store `tail`,
         * that is not yet wrapped into `shared_ptr`,
         * in the guard, and set it to `nullptr` in the end
         * in order to avoid destruction of successfully created list.
         */
        static ListPtr fill(size_t amount, const T& value) {
            if (amount == 0) {
                throw invalid_argument("You can't create an empty list");
            }

            struct TailGuard {
                const List_* ptr;
                ~TailGuard() {
                    List_::destroy(this->ptr);
                }
            } guard{};
            const List_* result = fill_(amount, guard.ptr, value);
            return amount
                ? ListPtr{result, List_::destroy}
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
    };
    /** \brief Pointer to wrapped list.
     */
    const shared_ptr<const List_> list;
    /** \brief Construct list wrapper.
     * \param list List_ to be wrapped.
     *
     * This constructor uses private class List_,
     * so it's not for a public usage.
     * The constructor only copies pointer to `list`
     * and wraps it.
     */
    explicit List(const shared_ptr<const List_>& list) : list{list} {
    };
public:
    /** \brief Create a list with a single element.
     * \param value Value of the head.
     */
    explicit List(const T& value)
        : list{new List_{value}, List_::destroy} {
    }
    /**
     * \brief Create a list with head and tail.
     * \param value Value of the head.
     * \param tail Tail of the list.
     */
    List(const T& value, const List<T>& tail)
        : list{new List_{value, tail.list}, List_::destroy} {
    }
    /** \brief Create new instance of List from initializer list.
     * \param value Initializer list of values for the list.
     */
    explicit List(const initializer_list<T> value)
        : list{new List_{value}, List_::destroy} {
    }
    /**
     * List instances can be copied,
     * because it's a wrapper
     * and will not copy all elements.
     */
    List(const List&) = default;
    /**
     * When we should store returned value from some function in variable,
     * it's better to have an ability to move it than copy and destroy.
     */
    List(List&& list) = default;
    /**
     * \brief Destructor is default.
     */
    ~List() = default;
    /**
     * @copydoc List_::size
     */
    size_t size() const {
        return this->list->size();
    }
    /**
     * @copydoc List_::insert
     */
    const List insert(const T& value, const size_t position = 0) const {
        return List{this->list->insert(value, position)};
    }
    /**
     * @copydoc List_::remove
     */
    const List remove(const size_t position = 0) const {
        return List{this->list->remove(position)};
    }
    /**
     * @copydoc List_::tail
     */
    const List tail() const {
        return List{this->list->tail()};
    }
    /**
     * @copydoc List_::reverse
     */
    const List reverse() const {
        return List{this->list->reverse()};
    }
    /**
     * @copydoc List_::slice
     */
    const List slice(const size_t first, const size_t last = -1) const {
        return List{this->list->slice(first, last)};
    }
    /**
     * @copydoc List_::drop
     */
    const List drop(const size_t amount) const {
        return List{this->list->drop(amount)};
    }
    /**
     * @copydoc List_::append
     */
    const List append(const T& value) const {
        return List{this->list->append(value)};
    }
    /**
     * @copydoc List_::concat
     */
    const List concat(const List& list) const {
        return List{this->list->concat(list.list)};
    }
    /**
     * @copydoc List_::operator!=
     */
    bool operator!=(const List& list) const {
        return *this->list != *list.list;
    }
    /**
     * @copydoc List_::operator==
     */
    bool operator==(const List& list) const {
        return *this->list == *list.list;
    }
    /**
     * \brief Create new List of specific length with specific values.
     * \param amount Size of list to be created.
     * \param value Value that should appear in each node of new list.
     *
     * See List_::fill for implementation details.
     */
    static const List fill(size_t amount, const T& value) {
        return List{List_::fill(amount, value)};
    }
};

#endif
