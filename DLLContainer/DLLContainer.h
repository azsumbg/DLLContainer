#pragma once

#ifdef DLLCONTAINER_EXPORTS
#define DLLCONTAINER_API __declspec(dllexport)
#else 
#define DLLCONTAINER_API __declspec(dllimport)
#endif

#include <iterator>

constexpr int BAD_PTR{ 5001 };
constexpr int BAD_INDEX{ 5002 };
constexpr int BAD_PARAM{ 5003 };
constexpr int BAD_ERR{ 5004 };


namespace cont
{
	class DLLCONTAINER_API EXCEPTION
	{
	private:
		int what{ 0 };

	public:
		EXCEPTION(int what_happened);

		const wchar_t* eGet()const;
	};

	template <typename T> struct NODE
	{
		NODE* prev_pos{ nullptr };
		T data{};
		NODE* next_pos{ nullptr };
	};

	template <typename T> class BAG
	{
	private:
		NODE<T>* mPtr{ nullptr };

		size_t container_size{ 0 };
	
	public:
		BAG() {};
		BAG(T element)
		{
			NODE<T>* temp{ new NODE<T>{} };

			temp->data = element;

			mPtr = temp;

			++container_size;
		}
		BAG(BAG<T>& other)
		{
			if (other.mPtr)
			{
				if (other.max_size == 1)
				{
					NODE<T>* temp{ new NODE<T>{} };
					temp->data = other.mPtr->data;

					++container_size;
				}
				else
				{
					if (container_size == 0)
					{
						mPtr = new NODE<T>{};

						mPtr->data = other.mPtr->data;

						++container_size;
					}
					else
					{
						NODE<T>* source{ other.mPtr->next_pos };
						NODE<T>* current_node{ mPtr };
						
						while (source != nullptr)
						{
							NODE<T>* destination{ new NODE<T>*{} };

							destination->prev_pos = current_node;
							destination->data = source->data;
							current_node->next_pos = destination;

							current_node = destination;
							source = source->next_pos;
						
							++container_size;
						}
					}
				}
			}
		}
		BAG(BAG<T>&& other)
		{
			mPtr = other.mPtr;

			container_size = other.container_size;

			other.mPtr = nullptr;
		}

		~BAG()
		{
			if (mPtr)
			{
				NODE<T>* current{ mPtr };
				NODE<T>* next{ mPtr };

				while (next != nullptr)
				{
					next = current->next_pos;
					
					delete current;

					current = next;
				}
			}
		}

		BAG<T>& operator = (BAG<T>& other)
		{
			if (mPtr != nullptr && mPtr == other.mPtr)throw EXCEPTION(BAD_PARAM);

			if (mPtr)
			{
				NODE<T>* current{ mPtr };
				NODE<T>* next{ mPtr };

				while (next != nullptr)
				{
					next = current->next_pos;
					delete current;
					current = next;
				}
			}

			mPtr = nullptr;
			container_size = 0;

			if (other.mPtr)
			{
				NODE<T>* source{ other.mPtr };
				NODE<T>* destination{ mPtr };
			
				while (source != nullptr)
				{
					NODE<T>* current{ new NODE<T>{} };

					current->data = source->data;
					
					if (destination == mPtr)
					{
						destination = current;
						++container_size;
					}
					else
					{
						current->prev_pos = destination;
						destination->next_pos = current;

						current = destination;
						source = source->next_pos;

						++container_size;
					}
				}
			}

			return (*this);
		}
		BAG<T>& operator = (BAG<T>&& other)
		{
			if (mPtr != nullptr && mPtr == other.mPtr)throw EXCEPTION(BAD_PARAM);

			if (mPtr)
			{
				NODE<T>* current{ mPtr };
				NODE<T>* next{ mPtr };

				while (next != nullptr)
				{
					next = current->next_pos;
					delete current;
					current = next;
				}
			}

			mPtr = nullptr;
			container_size = 0;

			if (other.mPtr)
			{
				mPtr = other.mPtr;
				container_size = other.container_size;

				other.mPtr = nullptr;
			}

			return (*this);
		}

		T& operator [] (size_t index)
		{
			if (index >= container_size || index < 0)throw EXCEPTION(BAD_INDEX);

			NODE<T>* temp{ mPtr };

			for (size_t i = 0; i < index; ++i)
			{
				temp = temp->next_pos;

				if (temp == nullptr)throw EXCEPTION(BAD_PTR);
			}

			return temp->data;
		}

		bool empty() const
		{
			return (!mPtr);
		}

		size_t size()const 
		{
			return container_size;
		}

		void clear()
		{
			if (mPtr)
			{
				NODE<T>* current{ mPtr };
				NODE<T>* next{ mPtr };

				while (next != nullptr)
				{
					next = current->next_pos;
					delete current;
					current = next;
				}
			}

			mPtr = nullptr;
			container_size = 0;
		}

		void push_back(T element)
		{
			if (!mPtr)
			{
				mPtr = new NODE<T>{};

				mPtr->data = element;

				++container_size;
			}
			else
			{
				NODE<T>* counter{ mPtr };
				NODE<T>* temp{ new NODE<T>{} };

				while (counter->next_pos != nullptr)counter = counter->next_pos;

				temp->data = element;
				temp->prev_pos = counter;
				counter->next_pos = temp;

				++container_size;
			}
		}
		void push_back(T* element)
		{
			if (!mPtr)
			{
				mPtr = new NODE<T>{};

				mPtr->data = *element;

				++container_size;
			}
			else
			{
				NODE<T>* counter{ mPtr };
				NODE<T>* temp{ new NODE<T>{} };

				while (counter->next_pos != nullptr)counter = counter->next_pos;

				temp->data = *element;
				temp->prev_pos = counter;
				counter->next_pos = temp;

				++container_size;
			}
		}
		
		void push_front(T element)
		{
			if (!mPtr)
			{
				mPtr = new NODE<T>;
				mPtr->data = element;
				++container_size;
			}
			else
			{
				NODE<T>* temp = new NODE<T>;

				temp->data = element;
				temp->next_pos = mPtr;
				mPtr = temp;

				++container_size;
			}
		}
		void push_front(T* element)
		{
			if (!mPtr)
			{
				mPtr = new NODE<T>;
				mPtr->data = *element;
				++container_size;
			}
			else
			{
				NODE<T>* temp = new NODE<T>;

				temp->data = *element;
				temp->next_pos = mPtr;
				mPtr = temp;

				++container_size;
			}
		}

		void erase(size_t index)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);

			if (index < 0 || index >= container_size)throw EXCEPTION(BAD_INDEX);

			if (container_size == 1)
			{
				delete mPtr;
				mPtr = nullptr;
			}
			else
			{
				if (index == 0)
				{
					NODE<T>* temp = mPtr->next_pos;
					delete mPtr;
					mPtr = temp;
				}
				else
				{
					NODE<T>* to_delete{ mPtr };
					NODE<T>* previous{ nullptr };

					for (size_t i = 0; i < index; ++i)to_delete = to_delete->next_pos;

					previous = to_delete->prev_pos;
					previous->next_pos = to_delete->next_pos;

					delete to_delete;
				}
			}
			
			--container_size;
		}

		void insert(T element, size_t index)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);

			if (index < 0 || index >= container_size)throw EXCEPTION(BAD_INDEX);

			if (index == 0)push_front(element);
			else
			{
				NODE<T>* old_node{ mPtr };
				NODE<T>* new_node{ new NODE<T>{} };
			
				for (size_t i = 0; i < index -1; ++i)old_node = old_node->next_pos;
			
				new_node->prev_pos = old_node;
				new_node->data = element;
				new_node->next_pos = old_node->next_pos;
				
				NODE<T>* atemp = old_node->next_pos;

				old_node->next_pos = new_node;
				atemp->prev_pos = new_node;
			
				++container_size;
			}
		}
		void insert(T* element, size_t index)
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);

			if (index < 0 || index >= container_size)throw EXCEPTION(BAD_INDEX);

			if (index == 0)push_front(*element);
			else
			{
				NODE<T>* old_node{ mPtr };
				NODE<T>* new_node{ new NODE<T>{} };

				for (size_t i = 0; i < index - 1; ++i)old_node = old_node->next_pos;

				new_node->prev_pos = old_node;
				new_node->data = *element;
				new_node->next_pos = old_node->next_pos;
				
				NODE<T>* atemp = old_node->next_pos;

				old_node->next_pos = new_node;
				atemp->prev_pos = new_node;
				
				++container_size;
			}
		}

		T& front()
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);
			
			return mPtr->data;
		}
		T& back()
		{
			if (!mPtr)throw EXCEPTION(BAD_PTR);

			NODE<T>* temp{ mPtr };

			while (temp->next_pos != nullptr)temp = temp->next_pos;

			return temp->data;
		}

		class iterator
		{
		private:
			NODE<T>* it_ptr{ nullptr };
			BAG* container_ptr{ nullptr };
			
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class BAG<T>;

			iterator(BAG<T>* current_bag, NODE<T>* init_node)
			{
				it_ptr = init_node;
				container_ptr = current_bag;
			}

			pointer operator -> ()
			{
				return it_ptr;
			}
			reference operator * ()
			{
				return it_ptr->data;
			}

			iterator& operator ++()
			{
				it_ptr = it_ptr->next_pos;

				return (*this);
			}
			iterator operator ++(int)
			{
				iterator temp{ (*this) };

				it_ptr = it_ptr->next_pos;

				return temp;
			}

			iterator& operator --()
			{
				if (it_ptr == nullptr)
				{
					NODE<T>* temp{ container_ptr->mPtr };

					while (temp->next_pos != nullptr)temp = temp->next_pos;

					it_ptr = temp;
				}
				else it_ptr = it_ptr->prev_pos;

				return (*this);
			}
			iterator operator --(int)
			{
				iterator temp{ (*this) };

				it_ptr = it_ptr->prev_pos;

				return temp;
			}

			iterator& operator + (size_t step)
			{
				for (size_t i = 0; i < step; ++i)it_ptr = it_ptr->next_pos;

				return (*this);
			}
			iterator& operator - (size_t step)
			{
				if (it_ptr == nullptr)
				{
					NODE<T>* temp{ container_ptr->mPtr };

					while (temp->next_pos != nullptr)temp = temp->next_pos;
					for (size_t i = 0; i < step; ++i)temp = temp->prev_pos;
					it_ptr = temp;
				}
				else
					for (size_t i = 0; i < step; ++i)it_ptr = it_ptr->prev_pos;

				return (*this);
			}

			friend bool operator == (const iterator& current, const iterator& other)
			{
				if (!other.it_ptr) return false;

				return(current.it_ptr == other.it_ptr);
			}
			friend bool operator != (const iterator& current, const iterator& other)
			{
				if (!other.it_ptr) return false;

				return(current.it_ptr != other.it_ptr);
			}

			bool operator > (const iterator& other)
			{
				NODE<T>* temp{ container_ptr->mPtr };

				size_t counter_current{ 0 };
				size_t counter_other{ 0 };

				while (temp != it_ptr)
				{
					temp = temp->next_pos;
					++counter_current;
				}

				temp = container_ptr->mPtr;
				while (temp != other.it_ptr)
					{
						temp = temp->next_pos;
						++counter_other;
					}
				
				return (counter_current > counter_other);
			}
			bool operator < (const iterator& other)
			{
				NODE<T>* temp{ container_ptr->mPtr };

				size_t counter_current{ 0 };
				size_t counter_other{ 0 };

				while (temp != it_ptr)
				{
					temp = temp->next_pos;
					++counter_current;
				}

				temp = container_ptr->mPtr;
				while (temp != other.it_ptr)
					{
						temp = temp->next_pos;
						++counter_other;
					}
				
				return (counter_current < counter_other);
			}

			bool operator >= (const iterator& other)
			{
				NODE<T>* temp{ container_ptr->mPtr };

				size_t counter_current{ 0 };
				size_t counter_other{ 0 };

				while (temp != it_ptr)
				{

					temp = temp->next_pos;
					++counter_current;
				}

				temp = container_ptr->mPtr;
				while (temp != other.it_ptr)
				{
					temp = temp->next_pos;
					++counter_other;
				}

				return (counter_current >= counter_other);
			}
			bool operator <= (const iterator& other)
			{
				NODE<T>* temp{ container_ptr->mPtr };

				size_t counter_current{ 0 };
				size_t counter_other{ 0 };

				while (temp != it_ptr)
				{
					temp = temp->next_pos;
					++counter_current;
				}

				temp = container_ptr->mPtr;
				while (temp != other.it_ptr)
				{
					temp = temp->next_pos;
					++counter_other;
				}

				return (counter_current <= counter_other);
			}

		};

		iterator begin()
		{
			return iterator(this, mPtr);
		}
		iterator end()
		{
			return iterator(this, nullptr);
		}

		void erase(iterator what)
		{
			NODE<T>* temp{ mPtr };
			size_t count = 0;

			while (temp != what.it_ptr)
			{
				temp = temp->next_pos;
				++count;
			}

			erase(count);
		}
	};





}