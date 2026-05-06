#include <iostream>
#include <fstream>
#include <random>
#include <windows.h>
#include <new> 
using namespace std;


// Завдання 1.8: Клас VectorUShort
class VectorUShort {
private:
	unsigned short* ptr;
	int num; // Розмір вектора
	int State; // 0 - ОК, 1 - вихід за межі, 2 - мало пам'яті, 3 - ділення на 0

public:
	static int objectCount;


	// 1. Конструктор без параметрів
	VectorUShort() {
		num = 1;
		ptr = new (std::nothrow) unsigned short[1];
		if (!ptr) { State = 2; } // Код помилки: мало пам'яті
		else { ptr[0] = 0; State = 0; }
		objectCount++;
	}

	// 2. Конструктор з одним параметром (розмір)
	VectorUShort(int n) {
		if (n <= 0) n = 1;
		num = n;
		ptr = new (std::nothrow) unsigned short[num];
		if (!ptr) { State = 2; }
		else {
			for (int i = 0; i < num; i++) ptr[i] = 0;
			State = 0;
		}
		objectCount++;
	}

	// 3. Конструктор з двома параметрами (розмір та значення)
	VectorUShort(int n, unsigned short val) {
		if (n <= 0) n = 1;
		num = n;
		ptr = new (std::nothrow) unsigned short[num];
		if (!ptr) { State = 2; }
		else {
			for (int i = 0; i < num; i++) ptr[i] = val; // Заповнюємо всі елементи значенням val
			State = 0;
		}
		objectCount++;
	}

	// 4. Конструктор копіювання
	VectorUShort(const VectorUShort& other) {
		num = other.num;
		State = other.State;
		ptr = new (std::nothrow) unsigned short[num];
		if (ptr) {
			for (int i = 0; i < num; i++) ptr[i] = other.ptr[i];
		}
		else { State = 2; }
		objectCount++;
	}

	// 5. Деструктор
	~VectorUShort() {
		if (ptr) { delete[] ptr; }
		objectCount--;
	}

	// Унарні оператори

	// Префіксний ++ (++vec)
	VectorUShort& operator++() {
		if (ptr) for (int i = 0; i < num; i++) ptr[i]++;
		return *this;
	}

	// Постфіксний ++ (vec++) 
	VectorUShort operator++(int) {
		VectorUShort temp(*this);
		if (ptr) for (int i = 0; i < num; i++) ptr[i]++;
		return temp;
	}

	// Префіксний -- (--vec)
	VectorUShort& operator--() {
		if (ptr) for (int i = 0; i < num; i++) ptr[i]--;
		return *this;
	}

	// Постфіксний -- (vec--)
	VectorUShort operator--(int) {
		VectorUShort temp(*this);
		if (ptr) for (int i = 0; i < num; i++) ptr[i]--;
		return temp;
	}

	// Логічне ! (повертає true, якщо num не дорівнює нулю)
	bool operator!() const {
		return num != 0;
	}

	// Побітове ~ (заперечення всіх елементів)
	VectorUShort operator~() const {
		VectorUShort res(*this);
		for (int i = 0; i < num; i++) res.ptr[i] = ~res.ptr[i];
		return res;
	}

	// Арифметичний - (мінус)
	VectorUShort operator-() const {
		VectorUShort res(*this);
		// Для беззнакових чисел мінус спрацює за правилами wrap-around
		for (int i = 0; i < num; i++) res.ptr[i] = -res.ptr[i];
		return res;
	}

	// Операція присвоєння

	VectorUShort& operator=(const VectorUShort& other) {
		if (this != &other) {
			if (ptr) delete[] ptr;
			num = other.num;
			State = other.State;
			ptr = new (std::nothrow) unsigned short[num];
			if (ptr) {
				for (int i = 0; i < num; i++) ptr[i] = other.ptr[i];
			}
			else { State = 2; }
		}
		return *this;
	}

	VectorUShort& operator+=(const VectorUShort& other) { // Додавання поелементно (з урахуванням різних розмірів)
		int minNum = (num < other.num) ? num : other.num;
		for (int i = 0; i < minNum; i++) ptr[i] += other.ptr[i];
		return *this;
	}

	VectorUShort& operator-=(const VectorUShort& other) { // Віднімання поелементно (з урахуванням різних розмірів)
		int minNum = (num < other.num) ? num : other.num;
		for (int i = 0; i < minNum; i++) ptr[i] -= other.ptr[i];
		return *this;
	}

	VectorUShort& operator*=(unsigned short val) { // Множення на скаляр
		for (int i = 0; i < num; i++) ptr[i] *= val;
		return *this;
	}

	VectorUShort& operator/=(unsigned short val) {
		if (val == 0) { State = 3; return *this; } // Захист від ділення на 0
		for (int i = 0; i < num; i++) ptr[i] /= val;
		return *this;
	}

	VectorUShort& operator%=(unsigned short val) { // Операція взяття залишку від ділення
		if (val == 0) { State = 3; return *this; }
		for (int i = 0; i < num; i++) ptr[i] %= val;
		return *this;
	}

	VectorUShort& operator|=(const VectorUShort& other) { // Побітове OR
		int minNum = (num < other.num) ? num : other.num;
		for (int i = 0; i < minNum; i++) ptr[i] |= other.ptr[i];
		return *this;
	}

	VectorUShort& operator^=(const VectorUShort& other) { // Побітове XOR
		int minNum = (num < other.num) ? num : other.num;
		for (int i = 0; i < minNum; i++) ptr[i] ^= other.ptr[i];
		return *this;
	}

	VectorUShort& operator&=(const VectorUShort& other) { // Побітове AND
		int minNum = (num < other.num) ? num : other.num;
		for (int i = 0; i < minNum; i++) ptr[i] &= other.ptr[i];
		return *this;
	}

	// Побітові та арифметичні оператори, які повертають новий об'єкт (використовують оператори присвоєння всередині)

	VectorUShort operator+(const VectorUShort& other) const {
		VectorUShort res(*this); res += other; return res;
	}

	VectorUShort operator-(const VectorUShort& other) const {
		VectorUShort res(*this); res -= other; return res;
	}

	VectorUShort operator*(unsigned short val) const {
		VectorUShort res(*this); res *= val; return res;
	}

	VectorUShort operator/(unsigned short val) const {
		VectorUShort res(*this); res /= val; return res;
	}

	VectorUShort operator%(unsigned short val) const {
		VectorUShort res(*this); res %= val; return res;
	}

	VectorUShort operator|(const VectorUShort& other) const {
		VectorUShort res(*this); res |= other; return res;
	}

	VectorUShort operator^(const VectorUShort& other) const {
		VectorUShort res(*this); res ^= other; return res;
	}

	VectorUShort operator&(const VectorUShort& other) const {
		VectorUShort res(*this); res &= other; return res;
	}

	// Логічні оператори (порівняння за сумою елементів)

	// Рівність (перевіряємо поелементно)
	bool operator==(const VectorUShort& other) const {
		if (num != other.num) return false;
		for (int i = 0; i < num; i++) {
			if (ptr[i] != other.ptr[i]) return false;
		}
		return true;
	}

	bool operator!=(const VectorUShort& other) const {
		return !(*this == other);
	}

	// Індексація [] (з поверненням останнього елемента при помилці)
	unsigned short& operator[](int index) {
		if (index < 0 || index >= num) {
			State = 1; // Код помилки: вихід за межі
			return ptr[num - 1];
		}
		return ptr[index];
	}

	// Розподіл пам'яті (перевантаження new/delete для класу)
	void* operator new(size_t size) {
		cout << " [new] Виділено пам'ять для об'єкта VectorUShort\n";
		return ::operator new(size);
	}

	void operator delete(void* p) {
		cout << " [delete] Пам'ять об'єкта VectorUShort звільнено\n";
		::operator delete(p);
	}

	// Виклик функції () — повертає суму елементів
	unsigned int operator()() const {
		unsigned int sum = 0;
		for (int i = 0; i < num; i++) sum += ptr[i];
		return sum;
	}

	// Порівняння (за сумою елементів)
	bool operator>(const VectorUShort& other) const { return this->operator()() > other.operator()(); }
	bool operator>=(const VectorUShort& other) const { return this->operator()() >= other.operator()(); }
	bool operator<(const VectorUShort& other) const { return this->operator()() < other.operator()(); }
	bool operator<=(const VectorUShort& other) const { return this->operator()() <= other.operator()(); }

	// 

	int getState() const { return State; } // Метод для очищення стану (наприклад, після обробки помилки)
	void clearState() { State = 0; } // Метод для виведення вектора та його стану

	void Print() const { // Виводимо вектор та його стан
		cout << "[ ";
		for (int i = 0; i < num; i++) cout << ptr[i] << " ";
		cout << "] (Розмір: " << num << ", Стан: " << State << ")\n";
	}

	void setValuesFromInput() { // Метод для заповнення вектора з клавіатури
		for (int i = 0; i < num; i++) {
			cout << " Введіть елемент [" << i << "]: ";
			cin >> ptr[i];
		}
	}
};

int VectorUShort::objectCount = 0;

// Меню для тестування класу VectorUShort
void Task_VectorUShort_Menu() {
	int subChoice;
	VectorUShort v1(3, 1);
	VectorUShort v2(3, 4);

	do {
		cout << "\n======================================\n";
		cout << ">>> ТЕСТУВАННЯ VectorUShort <<<\n";
		cout << " Вектор v1: "; v1.Print();
		cout << " Вектор v2: "; v2.Print();
		cout << "--------------------------------------\n";
		cout << "1. Заповнити v1 з клавіатури\n";
		cout << "2. Заповнити v2 випадковими числами\n";
		cout << "3. Заповнити v1 з файлу (vector_in.txt)\n";
		cout << "4. Тест унарних (++v1, --v2, ~v1, -v1)\n";
		cout << "5. Тест арифметики (v1 + v2, v1 * 2, v1 / 0)\n";
		cout << "6. Тест побітових операцій (v1 | v2, v1 & v2)\n";
		cout << "7. Тест індексації [] (спробувати вийти за межі)\n";
		cout << "8. Тест порівняння (v1 > v2, v1 == v2)\n";
		cout << "0. Повернутися до Головного меню\n";
		cout << "Ваш вибір: ";

		cin >> subChoice;
		if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); subChoice = -1; }

		switch (subChoice) {
		case 1: {
			int size; cout << "Введіть новий розмір для v1: "; cin >> size;
			v1 = VectorUShort(size); v1.setValuesFromInput(); break;
		}
		case 2: {
			random_device rd; mt19937 gen(rd()); uniform_int_distribution<unsigned short> dist(1, 100);
			int size; cout << "Введіть новий розмір для v2: "; cin >> size;
			v2 = VectorUShort(size);
			for (int i = 0; i < size; i++) v2[i] = dist(gen);
			cout << "v2 заповнено випадковими числами!\n"; break;
		}
		case 3: {
			ifstream ifs("vector_in.txt");
			if (!ifs) {
				ofstream ofs("vector_in.txt"); ofs << "4\n11 22 33 44\n"; ofs.close(); ifs.open("vector_in.txt");
			}
			int size; ifs >> size; v1 = VectorUShort(size);
			for (int i = 0; i < size; i++) ifs >> v1[i];
			ifs.close(); cout << "Дані для v1 зчитано з файлу!\n"; break;
		}
		case 4: {
			cout << "\n Оригінал v1: "; v1.Print();
			cout << " Після ++v1: "; (++v1).Print();
			cout << " Після ~v1:  "; (~v1).Print();
			cout << " Після -v1:  "; (-v1).Print();
			cout << "\n Оригінал v2: "; v2.Print();
			cout << " Після --v2: "; (v2--).Print();
			break;
		}
		case 5: {
			VectorUShort sum = v1 + v2; cout << "\n v1 + v2 = "; sum.Print();
			VectorUShort mul = v1 * 2; cout << " v1 * 2 =  "; mul.Print();
			VectorUShort div = v1 / 0; cout << " v1 / 0 =  "; div.Print();
			if (div.getState() == 3) cout << "[Увага] Зафіксовано ділення на нуль!\n";
			break;
		}
		case 6: {
			VectorUShort orVec = v1 | v2; cout << "\n v1 | v2 = "; orVec.Print();
			VectorUShort andVec = v1 & v2; cout << " v1 & v2 = "; andVec.Print();
			break;
		}
		case 7: {
			cout << "\nНамагаємося звернутися до елемента v1[1000]...\n";
			unsigned short val = v1[1000]; cout << "Повернуте значення: " << val << "\n";
			if (v1.getState() == 1) cout << "[Увага] Вихід за межі масиву! Стан змінено.\n";
			v1.clearState(); break;
		}
		case 8: {
			cout << "\nСума елементів v1: " << v1() << "\n"; cout << "Сума елементів v2: " << v2() << "\n";
			if (v1 == v2) cout << "Вектори АБСОЛЮТНО РІВНІ (поелементно).\n";
			else if (v1 > v2) cout << "Вектор v1 БІЛЬШИЙ за v2 (за сумою елементів).\n";
			else cout << "Вектор v1 МЕНШИЙ за v2 (за сумою елементів).\n";
			break;
		}
		case 0: cout << "Повернення до Головного меню...\n"; break;
		default: cout << "Невірний вибір!\n";
		}
	} while (subChoice != 0);
}

// Завдання 2.8: Клас BusSchedule (Асоціативний масив)
// Структура, яка зберігає пару "Номер рейсу - Час"
struct BusRoute {
	string flightNumber; // Номер рейсу (ключ)
	string departureTime; // Час відправлення
};

// Клас, який містить динамічний масив структур BusRoute та реалізує необхідні функції та оператори
class BusSchedule {
private:
	BusRoute* routes; 
	int count;
	int CodeError;
	string dummy;

public:
	// Конструктор
	BusSchedule() : count(0), CodeError(0), routes(nullptr), dummy("Помилка: Рейс не знайдено") {}

	// Деструктор (звільняємо пам'ять)
	~BusSchedule() {
		if (routes) delete[] routes;
	}

	// 1. Функція створення набору асоціативних сутностей (додавання)
	void addRoute(const string& fNum, const string& dTime) {
		// Виділяємо нову пам'ять, більшу на 1
		BusRoute* temp = new BusRoute[count + 1];

		// Копіюємо старі дані
		for (int i = 0; i < count; i++) {
			temp[i] = routes[i];
		}

		// Записуємо нові дані
		temp[count].flightNumber = fNum;
		temp[count].departureTime = dTime;

		// Видаляємо старий масив і перемикаємо вказівник
		if (routes) delete[] routes;
		routes = temp;
		count++;
	}

	// 2. Перевантаження операції індексації []
	string& operator[](const string& keyFlight) {
		CodeError = 0; // Скидаємо помилку перед пошуком

		// Шукаємо рейс за ключем
		for (int i = 0; i < count; i++) {
			if (routes[i].flightNumber == keyFlight) {
				return routes[i].departureTime; // Повертаємо асоціативну сутність (час)
			}
		}

		// Якщо цикл завершився, значить рейсу немає
		CodeError = 1;
		return dummy; // Повертаємо безпечний рядок з помилкою
	}

	// 3. Альтернативне звернення через перевантаження ()
	string& operator()(const string& keyFlight) {
		return this->operator[](keyFlight); // Просто викликаємо вже написану логіку []
	}

	// Гетери для помилок
	int getCodeError() const { return CodeError; }
	void clearError() { CodeError = 0; }

	// 4. Перевантаження ДРУЖНІХ операцій виведення (<<)
	friend ostream& operator<<(ostream& os, const BusSchedule& schedule) {
		if (schedule.count == 0) {
			os << "[ Розклад порожній ]\n";
			return os;
		}
		os << "\n--- ТАБЛО ВІДПРАВЛЕННЯ АВТОБУСІВ ---\n";
		for (int i = 0; i < schedule.count; i++) {
			os << " Рейс: " << schedule.routes[i].flightNumber
				<< " \t| Час відправлення: " << schedule.routes[i].departureTime << "\n";
		}
		os << "------------------------------------\n";
		return os;
	}

	// 5. Перевантаження ДРУЖНІХ операцій введення (>>)
	friend istream& operator>>(istream& is, BusSchedule& schedule) {
		string fNum, dTime;
		cout << " Введіть номер рейсу (напр. 101-А): ";
		is >> fNum;
		cout << " Введіть час відправлення (напр. 14:30): ";
		is >> dTime;

		schedule.addRoute(fNum, dTime); // Викликаємо функцію додавання
		return is;
	}
};

// Меню для тестування класу BusSchedule
void Task_BusSchedule_Menu() {
	int subChoice;
	BusSchedule station;

	// Додаємо базові рейси для перевірки
	station.addRoute("101-A", "08:15");
	station.addRoute("205-B", "12:45");

	do {
		cout << "\n======================================\n";
		cout << ">>> ТЕСТУВАННЯ АСОЦІАТИВНОГО МАСИВУ <<<\n";
		cout << "1. Додати рейс через клавіатуру (тест >>)\n";
		cout << "2. Вивести весь розклад (тест <<)\n";
		cout << "3. Знайти час за номером рейсу (тест [])\n";
		cout << "4. Знайти час за номером рейсу (тест ())\n";
		cout << "0. Повернутися до Головного меню\n";
		cout << "Ваш вибір: ";

		cin >> subChoice;
		if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); subChoice = -1; }

		switch (subChoice) {
		case 1: {
			cout << "\n--- Додавання рейсу ---\n";
			cin >> station;
			cout << "Рейс успішно додано!\n"; break;
		}
		case 2: {
			cout << station; break;
		}
		case 3: {
			string searchKey; cout << "\n Введіть номер рейсу для пошуку: "; cin >> searchKey;
			string result = station[searchKey];
			if (station.getCodeError() == 1) cout << " [ПОМИЛКА] CodeError = 1. Такого рейсу не існує!\n";
			else cout << " Знайдено! Час відправлення рейсу " << searchKey << ": " << result << "\n";
			break;
		}
		case 4: {
			string searchKey; cout << "\n Введіть номер рейсу для пошуку: "; cin >> searchKey;
			string result = station(searchKey);
			if (station.getCodeError() == 1) cout << " [ПОМИЛКА] CodeError = 1. Такого рейсу не існує!\n";
			else cout << " Знайдено (через дужки)! Час: " << result << "\n";
			break;
		}
		case 0: cout << "Повернення до Головного меню...\n"; break;
		default: cout << "Невірний вибір!\n";
		}
	} while (subChoice != 0);
}


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int mainChoice;

	do {
		cout << "\n================ ГОЛОВНЕ МЕНЮ =================\n";
		cout << "1. Тестування класу VectorUShort\n";
		cout << "2. Тестування класу BusSchedule (Асоціативний масив)\n";
		cout << "0. Вихід з програми\n";
		cout << "===============================================\n";
		cout << "Ваш вибір: ";

		cin >> mainChoice;

		if (cin.fail()) {
			cin.clear(); cin.ignore(32767, '\n');
			mainChoice = -1;
		}

		switch (mainChoice) {
		case 1:
			Task_VectorUShort_Menu();
			break;
		case 2:
			Task_BusSchedule_Menu();
			break;
		case 0:
			cout << "Роботу завершено. До побачення!\n";
			break;
		default:
			cout << "Невірний вибір. Спробуйте ще раз.\n";
		}
	} while (mainChoice != 0);

	return 0;
}