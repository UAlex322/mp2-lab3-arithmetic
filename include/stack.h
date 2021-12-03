// объявление и реализация шаблонного стека
// стек поддерживает операции: 
// - вставка элемента, 
// - извлечение элемента, 
// - просмотр верхнего элемента (без удаления)
// - проверка на пустоту, 
// - получение количества элементов в стеке
// - очистка стека
// при вставке в полный стек должна перевыделяться память

template <typename ValType>
class MyStack {
public:
	MyStack(const size_t cap = 0): capacity(cap) {
		data = new ValType[cap];
	}

	MyStack(const MyStack &copy): _size(copy._size), capacity(copy.capacity) {
		data = new ValType[capacity];
		for (size_t i = 0; i < _size; ++i)
			data[i] = copy.data[i];
	}

	~MyStack() {
		delete[] data;
	}

	void push(const ValType &elem) {
		if (capacity == 0) {
			data = new ValType[1];
			capacity = 1;
		}

		if (_size == capacity) {
			ValType *new_data = new ValType[capacity*2];
			for (size_t i = 0; i < capacity; ++i)
				new_data[i] = data[i];
			delete[] data;

			data = new_data;
			capacity *= 2;
		}
		data[_size++] = elem;
	}

	void pop() {
		if (_size == 0)
			throw "MyStack::pop(): stack is empty";

		--(_size);
	}

	void clear() {
		delete[] data;
		data = new ValType[capacity];
		_size = 0;
	}

	ValType top() {
		if (_size == 0)
			throw "MyStack::top(): stack is empty";

		return data[_size-1];
	}

	inline size_t size() {
		return this->_size;
	}

	inline bool empty() {
		return _size == 0;
	}

private:
	ValType *data;
	size_t capacity;
	size_t _size = 0;
};