#pragma once

#ifdef DLLCONTAINER_EXPORTS
#define DLLCONTAINER_API __declspec(dllexport)
#else 
#define DLLCONTAINER_API __declspec(dllimport)
#endif

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

	template <typename T> struct DLLCONTAINER_API NODE
	{
		T* prev_pos{ nullptr };
		T data{};
		T* next_pos{ nullptr };
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
				NODE<T*> next{ mPtr };

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
				NODE<T*> source{ other.mPtr };
				NODE<T>* destination{ mPtr };
			
				while (source != nullptr)
				{
					NODE<T>* current{ new NODE<T*>{} };

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

		
	};





}