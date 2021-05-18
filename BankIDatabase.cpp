#include <iostream> // для ввода/вывода
#include <vector> //для создания массива
#include <string> // для создания строк
#include <Windows.h> // для изменения цвета
#include <iomanip> // для изменения параметров вывода 
#include <fstream> // для считывания/записи из/в файл(а)
#include <conio.h> // для меню (считывание символов напрямую из консоли без использования буфера и echo-вывода (getch(void))
#include <chrono>

#define COUNT_FIELDS_DEPOSITOR 6


void SetColor(int text, int bg) //Функция смены цвета, взятая из Интернета
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

struct Depositor
{
	std::string DepositorSurname;
	std::string DepositorName;
	std::string DepositorFatherName;
	int DepositorNumber = 0;
	int DepositorValue = 0;
	int DepositDuration = 0;
	friend bool operator== (const Depositor& firstDepos, const Depositor& secondDepos)
	{
		return ((firstDepos.DepositorSurname == secondDepos.DepositorSurname) &&
				(firstDepos.DepositorName == secondDepos.DepositorName) &&
				(firstDepos.DepositorFatherName == secondDepos.DepositorFatherName) &&
				(firstDepos.DepositorNumber == secondDepos.DepositorNumber) &&
				(firstDepos.DepositorValue == secondDepos.DepositorValue) &&
				(firstDepos.DepositDuration == secondDepos.DepositDuration));
	}
};

enum CheckOptions
{
	EQUAL,
	LESS,
	MORE,
};

struct TreeDepositors // структура для представления узлов дерева
{
	Depositor depositor;
	int Option;
	int size;
	TreeDepositors* left;
	TreeDepositors* right;
	TreeDepositors(int k, Depositor data) {depositor = data, Option = k; left = right = 0; size = 1; }

};

void menu_choice(std::vector<Depositor>& Depositors); // прототип меню
void menu(std::vector<Depositor>& Depositors); // прототип меню
void sub_menu_choice(std::vector<Depositor>& Depositors, std::string Option);
void SubMenu(std::vector<Depositor>& Depositors, std::string Option);
int m_count = 0;
int sub_m_count = 0;
std::vector<std::string> MenuOptions{ "Показать всех вкладчиков", "Отсортировать вкладчиков", "Поиск вкладчика", "Выход" };
std::vector<std::string> SubMenuOptions{ "По фамилии", "По имени","По отчеству","По номеру вклада","По размеру вклада","По длительности","Назад", };

const int NotUsed = system("color 70"); // изменения цвета консоли в серый 

int checkdigit()
{
	while (true)
	{
		int value;
		// вводим число, которое хотим представить в двоичной форме
		std::cin >> value; // число целое
		if (std::cin.fail()) // ecли предыдущее извелечение оказлось неудачным,
		{
			std::cin.clear(); // то возвращаем cin в обычный режим работы
			std::cin.ignore(32767, '\n'); // и удаляем из буфера значения предыдущего ввода 
			std::cout << "Недопустимое заданное число. Введите число правильно" << '\n';
		}
		else
		{
			std::cin.ignore(32767, '\n'); // удаляем из буфера значения предыдущего ввода 
			return value;
		}
	}
}

void FillDepositorsDataBase(std::vector<Depositor>& Depositors, std::ifstream& fin)
{
	Depositor TempDepositor;
	
	fin.open("DepositorsList.txt");
	if (fin.is_open()) // если файл удачно открыт
	{
		while (!fin.eof()) // пока не дошли до конца файла
		{
			fin >> TempDepositor.DepositorSurname >> TempDepositor.DepositorName >> TempDepositor.DepositorFatherName >> TempDepositor.DepositorNumber >> TempDepositor.DepositorValue >> TempDepositor.DepositDuration;
			fin.ignore(1); // игнорируем перенос строки
			Depositors.push_back(TempDepositor);
		}
	}
	else // если файл открыть не удалось
	{
		std::cout << "Файл не удалось открыть!" << std::endl;
	}
	fin.close(); // закрываем файл
}

void PrintTableHeader()
{
	SetColor(1, 7);
	std::cout << std::setw(15) << std::left << "Фамилия" << "\t";
	std::cout << std::setw(15) << std::left << "Имя" << "\t";
	std::cout << std::setw(15) << std::left << "Отчество" << "\t";
	std::cout << std::setw(10) << std::left << "Номер вкладчика" << "\t";
	std::cout << std::setw(10) << std::left << "Размер вклада" << "\t";
	std::cout << std::setw(5) << std::left << "Длительность вклада";
	std::cout << std::endl;
	SetColor(0, 7);
}

void PrintDepositorsList(std::vector<Depositor>& Depositors)
{
	system("cls");
	PrintTableHeader();
	for (int i = 0; i < Depositors.size(); i++)
	{
		std::cout << std::setw(15) << std::left << Depositors[i].DepositorSurname << "\t";
		std::cout << std::setw(15) << std::left << Depositors[i].DepositorName << "\t";
		std::cout << std::setw(15) << std::left << Depositors[i].DepositorFatherName << "\t";
		std::cout << std::setw(10) << std::left << Depositors[i].DepositorNumber << "\t";
		std::cout << std::setw(10) << std::left << Depositors[i].DepositorValue << "\t";
		std::cout << std::setw(5) << std::left << Depositors[i].DepositDuration;
		std::cout << std::endl;
	}
}

void PrintDepositor(const Depositor& Depos)
{
	std::cout << std::setw(15) << std::left << Depos.DepositorSurname << "\t";
	std::cout << std::setw(15) << std::left << Depos.DepositorName << "\t";
	std::cout << std::setw(15) << std::left << Depos.DepositorFatherName << "\t";
	std::cout << std::setw(10) << std::left << Depos.DepositorNumber << "\t";
	std::cout << std::setw(10) << std::left << Depos.DepositorValue << "\t";
	std::cout << std::setw(5) << std::left << Depos.DepositDuration;
	std::cout << std::endl;
}

CheckOptions CheckEqualValues(Depositor& FirstDepositor, Depositor& SecondDepositor, int SwitchOption, int SubOption, int MainOption)
{
	switch (SwitchOption)
	{
	case 1:
		if (FirstDepositor.DepositorSurname < SecondDepositor.DepositorSurname)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorSurname > SecondDepositor.DepositorSurname)
			return CheckOptions::MORE;

		if (SubOption == MainOption - 1)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 2, SubOption + 2, MainOption);
		else if (SubOption <= COUNT_FIELDS_DEPOSITOR)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 1, SubOption + 1, MainOption);
		else
			return CheckOptions::EQUAL;

	case 2:
		if (FirstDepositor.DepositorName < SecondDepositor.DepositorName)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorName > SecondDepositor.DepositorName)
			return CheckOptions::MORE;

		if (SubOption == MainOption - 1)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 2, SubOption + 2, MainOption);
		else if (SubOption <= COUNT_FIELDS_DEPOSITOR)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 1, SubOption + 1, MainOption);
		else
			return CheckOptions::EQUAL;
	case 3:
		if (FirstDepositor.DepositorFatherName < SecondDepositor.DepositorFatherName)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorFatherName > SecondDepositor.DepositorFatherName)
			return CheckOptions::MORE;

		if (SubOption == MainOption - 1)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 2, SubOption + 2, MainOption);
		else if (SubOption <= COUNT_FIELDS_DEPOSITOR)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 1, SubOption + 1, MainOption);
		else
			return CheckOptions::EQUAL;
	case 4:
		if (FirstDepositor.DepositorNumber < SecondDepositor.DepositorNumber)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorNumber > SecondDepositor.DepositorNumber)
			return CheckOptions::MORE;

		if (SubOption == MainOption - 1)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 2, SubOption + 2, MainOption);
		else if (SubOption <= COUNT_FIELDS_DEPOSITOR)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 1, SubOption + 1, MainOption);
		else
			return CheckOptions::EQUAL;
	case 5:
		if (FirstDepositor.DepositorValue < SecondDepositor.DepositorValue)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorValue > SecondDepositor.DepositorValue)
			return CheckOptions::MORE;

		if (SubOption == MainOption - 1)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 2, SubOption + 2, MainOption);
		else if (SubOption <= COUNT_FIELDS_DEPOSITOR)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 1, SubOption + 1, MainOption);
		else
			return CheckOptions::EQUAL;
	case 6:
		if (FirstDepositor.DepositDuration < SecondDepositor.DepositDuration)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositDuration > SecondDepositor.DepositDuration)
			return CheckOptions::MORE;

		if (SubOption == MainOption - 1)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 2, SubOption + 2, MainOption);
		else if (SubOption <= COUNT_FIELDS_DEPOSITOR)
			return CheckEqualValues(FirstDepositor, SecondDepositor, SubOption + 1, SubOption + 1, MainOption);
		else
			return CheckOptions::EQUAL;
	}
}

CheckOptions CheckEqualTreeValues(Depositor& FirstDepositor, Depositor& SecondDepositor, int SwitchOption)
{
	switch (SwitchOption)
	{
	case 0:
		if (FirstDepositor.DepositorSurname < SecondDepositor.DepositorSurname)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorSurname > SecondDepositor.DepositorSurname)
			return CheckOptions::MORE;
		else
			return CheckOptions::EQUAL;

	case 1:
		if (FirstDepositor.DepositorName < SecondDepositor.DepositorName)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorName > SecondDepositor.DepositorName)
			return CheckOptions::MORE;
		else
			return CheckOptions::EQUAL;
	case 2:
		if (FirstDepositor.DepositorFatherName < SecondDepositor.DepositorFatherName)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorFatherName > SecondDepositor.DepositorFatherName)
			return CheckOptions::MORE;
		else
			return CheckOptions::EQUAL;
	case 3:
		if (FirstDepositor.DepositorNumber < SecondDepositor.DepositorNumber)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorNumber > SecondDepositor.DepositorNumber)
			return CheckOptions::MORE;
		else
			return CheckOptions::EQUAL;
	case 4:
		if (FirstDepositor.DepositorValue < SecondDepositor.DepositorValue)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositorValue > SecondDepositor.DepositorValue)
			return CheckOptions::MORE;
		else
			return CheckOptions::EQUAL;
	case 5:
		if (FirstDepositor.DepositDuration < SecondDepositor.DepositDuration)
			return CheckOptions::LESS;
		else if (FirstDepositor.DepositDuration > SecondDepositor.DepositDuration)
			return CheckOptions::MORE;
		else
			return CheckOptions::EQUAL;
	}
}

void quickSort(std::vector<Depositor>& Depositors, int left, int right, int key)
{
	Depositor pivot; // разрешающий элемент
	int l_hold = left; //левая граница
	int r_hold = right; // правая граница
	pivot = Depositors[left];
	CheckOptions checkOptionValue;
	while (left < right) // пока границы не сомкнутся
	{
		checkOptionValue = CheckEqualValues(Depositors[right], pivot, key, NULL, key);
		while ((checkOptionValue == EQUAL || checkOptionValue  == MORE) && (left < right))
		{
			right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
			checkOptionValue = CheckEqualValues(Depositors[right], pivot, key, NULL, key);
		}

		if (left != right) // если границы не сомкнулись
		{
			Depositors[left] = Depositors[right]; // перемещаем элемент [right] на место разрешающего
			left++; // сдвигаем левую границу вправо
		}

		checkOptionValue = CheckEqualValues(Depositors[left], pivot, key, NULL, key);
		while ((checkOptionValue == EQUAL || checkOptionValue == LESS) && (left < right))
		{
			left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
			checkOptionValue = CheckEqualValues(Depositors[left], pivot, key, NULL, key);
		}

		if (left != right) // если границы не сомкнулись
		{
			Depositors[right] = Depositors[left]; // перемещаем элемент [left] на место [right]
			right--; // сдвигаем правую границу вправо
		}
	}
	Depositors[left] = pivot; // ставим разрешающий элемент на место
	pivot = Depositors[left];
	int tempLeft = left;
	left = l_hold;
	right = r_hold;

	checkOptionValue = CheckEqualValues(Depositors[left], pivot, key, NULL, key);
	if (checkOptionValue == LESS) // Рекурсивно вызываем сортировку для левой и правой части массива
	{
		quickSort(Depositors, left, tempLeft - 1, key);
	}

	checkOptionValue = CheckEqualValues(Depositors[right], pivot, key, NULL, key);
	if (checkOptionValue == MORE)
	{
		quickSort(Depositors, tempLeft + 1, right, key);
	}
}

bool IsEqualDepositors(std::vector<Depositor>& DepositorsArray, Depositor& FindDepositor)
{
	bool bIsFind = false;
	int i = 0;
	while (i < DepositorsArray.size())
	{
		if (DepositorsArray[i] == FindDepositor)
		{
			bIsFind = true;
			break;
		}
		else
		{
			i++;
		}
	}
	return bIsFind;
}


void findDepositorsByValue(TreeDepositors* p, Depositor k, std::vector<Depositor>& FinderDepositors) // поиск ключа k в дереве p
{
	if (!p) return; // в пустом дереве можно не искать

	CheckOptions checkValue = CheckEqualTreeValues(k, p->depositor, p->Option);
	if (checkValue == EQUAL)
	{
		if (!IsEqualDepositors(FinderDepositors, p->depositor))
		{
			FinderDepositors.push_back(p->depositor);
			findDepositorsByValue(p->left, k, FinderDepositors);
		}
	}

	if (checkValue == LESS)
		findDepositorsByValue(p->left, k, FinderDepositors);
	else
		findDepositorsByValue(p->right, k, FinderDepositors);
}

TreeDepositors* find(TreeDepositors* p, Depositor k, std::vector<Depositor>& FinderDepositors) // поиск ключа k в дереве p
{
	if (!p) return 0; // в пустом дереве можно не искать
	CheckOptions checkValue = CheckEqualTreeValues(k, p->depositor, p->Option);
	if (checkValue == EQUAL)
	{
		if (!IsEqualDepositors(FinderDepositors, p->depositor))
			return p;
	}

	if (checkValue == LESS)
		return find(p->left, k, FinderDepositors);
	else
		return find(p->right, k, FinderDepositors);
}

int getsize(TreeDepositors* p) // обертка для поля size, работает с пустыми деревьями (t=NULL)
{
	if (!p) return 0;
	return p->size;
}

void fixsize(TreeDepositors* p) // установление корректного размера дерева
{
	p->size = getsize(p->left) + getsize(p->right) + 1;
}

TreeDepositors* rotateright(TreeDepositors* p) // правый поворот вокруг узла p
{
	TreeDepositors* q = p->left;
	if (!q) return p;
	p->left = q->right;
	q->right = p;
	q->size = p->size;
	fixsize(p);
	return q;
}

TreeDepositors* rotateleft(TreeDepositors* q) // левый поворот вокруг узла q
{
	TreeDepositors* p = q->right;
	if (!p) return q;
	q->right = p->left;
	p->left = q;
	p->size = q->size;
	fixsize(q);
	return p;
}

TreeDepositors* insertroot(TreeDepositors* p, Depositor k, int Option) // вставка нового узла с ключом k в корень дерева p 
{
	if (!p) return new TreeDepositors(Option, k);

	CheckOptions checkValue = CheckEqualTreeValues(k, p->depositor, p->Option);
	if (checkValue == LESS)
	{
		p->left = insertroot(p->left, k, Option);
		return rotateright(p);
	}
	else
	{
		p->right = insertroot(p->right, k, Option);
		return rotateleft(p);
	}
}

TreeDepositors* insert(TreeDepositors* p, Depositor k, int Option) // рандомизированная вставка нового узла с ключом k в дерево p
{
	if (!p) return new TreeDepositors(Option, k);
	if (rand() % (p->size + 1) == 0)
		return insertroot(p, k, Option);
	CheckOptions checkValue = CheckEqualTreeValues(p->depositor, k, p->Option);
	if (checkValue == MORE)
		p->left = insert(p->left, k, Option);
	else
		p->right = insert(p->right, k, Option);
	fixsize(p);
	return p;
}



void SearchDepositorByField(std::vector<Depositor>& Depositors)
{
	system("cls");

	int FindIntValue;
	std::string FindStringText;
	std::vector<Depositor> FinderDepositors;
	bool bIsFind = false;
	std::chrono::steady_clock sc;
	TreeDepositors* Tree = nullptr;
	TreeDepositors* FindNode;
	Depositor TempDepos;

	std::cout << "Введите значение: ";
	std::getline(std::cin, FindStringText);


	switch (sub_m_count)
	{
	case 0:
		TempDepos.DepositorSurname = FindStringText;
		break;
	case 1:
		TempDepos.DepositorName = FindStringText;
		break;
	case 2:
		TempDepos.DepositorFatherName = FindStringText;
		break;
	case 3:
		FindIntValue = std::stoi(FindStringText);
		TempDepos.DepositorNumber = FindIntValue;
		break;
	case 4:
		FindIntValue = std::stoi(FindStringText);
		TempDepos.DepositorValue = FindIntValue;
		break;
	case 5:
		FindIntValue = std::stoi(FindStringText);
		TempDepos.DepositDuration = FindIntValue;
		break;
	default:
		break;
	}
	for (int i = 0; i < Depositors.size(); i++)
	{
		Tree = insert(Tree, Depositors[i], sub_m_count);
	}

	auto start = sc.now();     // устанавливаем начало отсчета времени события
	findDepositorsByValue(Tree, TempDepos, FinderDepositors);
	auto end = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие
	if (FinderDepositors.size() != 0)
	{
		system("cls");
		PrintTableHeader();
		for (const auto& depositor : FinderDepositors)
		{
			PrintDepositor(depositor);
		}
		std::cout << "Время, затраченное на поиск: " << time_span.count() << " секунд" << std::endl << std::endl;
	}
	else
	{
		SetColor(4, 7);
		std::cout << "NOT FOUND" << std::endl;
		SetColor(0, 7);
	}
	system("pause");
	menu(Depositors);
}

void SortDepositorsList(std::vector<Depositor>& Depositors)
{
	std::chrono::steady_clock sc;
	auto start = sc.now();     // устанавливаем начало отсчета времени события
	quickSort(Depositors, NULL, Depositors.size() - 1, sub_m_count + 1);
	auto end = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие

	PrintDepositorsList(Depositors);
	std::cout << "Время, затраченное на сортировку: " << time_span.count() << " секунд" << std::endl << std::endl;
	system("pause");
	menu(Depositors);

}

void sub_conf_val(std::vector<Depositor>& Depositors, std::string Option) // выбор после подтверждения в зависимости от значения текущего
{
	if (Option == "Sort")
	{
		if (sub_m_count != SubMenuOptions.size() - 1)
		{
			SortDepositorsList(Depositors);
		}
		else
		{
			menu(Depositors);
		}
	}
	else if (Option == "Search")
	{
		if (sub_m_count != SubMenuOptions.size() - 1)
		{
			//search
			SearchDepositorByField(Depositors);
		}
		else
		{
			menu(Depositors);
		}
	}
}

void sub_menu_choice(std::vector<Depositor>& Depositors, std::string Option) // в зависимости от стрелок изменяем меню
{
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) // если стрелки
	{
		switch (k1)
		{
		case 0x48: // стрелка вверх
			sub_m_count--;
			if (sub_m_count < 0) sub_m_count = 0;
			SubMenu(Depositors, Option);
			break;

		case 0x50: // стрелка вниз
			sub_m_count++;
			if (sub_m_count > SubMenuOptions.size() - 1) sub_m_count = SubMenuOptions.size() - 1;
			SubMenu(Depositors, Option);
			break;
		case 0xD: // подтвердить
			sub_conf_val(Depositors, Option);
			break;
		default:
			sub_menu_choice(Depositors, Option);
		}
	}
	switch (k1)
	{
	case 0x48: // стрелка вверх
		sub_m_count--;
		if (sub_m_count < 0) sub_m_count = 0;
		SubMenu(Depositors, Option);
		break;

	case 0x50: // стрелка вниз
		sub_m_count++;
		if (sub_m_count > SubMenuOptions.size() - 1) sub_m_count = SubMenuOptions.size() - 1;
		SubMenu(Depositors, Option);
		break;
	case 0xD: // подтвердить
		sub_conf_val(Depositors, Option);
		break;
	default:
		sub_menu_choice(Depositors, Option);
	}
}

void SubMenu(std::vector<Depositor>& Depositors, std::string Option)
{
	system("cls"); // очищаем консоль
	if (Option == "Sort")
		std::cout << "-----Сортировка записей-----" << std::endl;
	else
		std::cout << "Поиск записи" << std::endl;

	for (int i = 0; i < SubMenuOptions.size(); i++)
	{
		if (i == sub_m_count)
			SetColor(5, 7);

		std::cout << SubMenuOptions[i] << std::endl;
		SetColor(0, 7);
	}
	sub_menu_choice(Depositors, Option);
}

void conf_val(std::vector<Depositor>& Depositors) // выбор после подтверждения в зависимости от значения текущего
{
	switch (m_count)
	{
	case 0:
		PrintDepositorsList(Depositors);
		system("pause");
		break;
	case 1:
		SubMenu(Depositors, "Sort");
		break;
	case 2:
		SubMenu(Depositors, "Search");
		break;
	case 3:
		SetColor(7, 7);
		exit(0);
		break;
	default:
		break;
	}

	menu(Depositors);
}

void menu(std::vector<Depositor>& Depositors) // меню
{
	system("cls"); // очищаем консоль
	std::cout << "-----Основное меню-----" << std::endl;
	for (int i = 0; i < MenuOptions.size(); i++)
	{
		if (i == m_count)
			SetColor(5, 7);

		std::cout << MenuOptions[i] << std::endl;
		SetColor(0, 7);
	}
	menu_choice(Depositors);
}

void menu_choice(std::vector<Depositor>& Depositors) // в зависимости от стрелок изменяем меню
{
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) // если стрелки
	{
		switch (k1)
		{
		case 0x48: // стрелка вверх
			m_count--;
			if (m_count < 0) m_count = 0;
			menu(Depositors);
			break;

		case 0x50: // стрелка вниз
			m_count++;
			if (m_count > MenuOptions.size() - 1) m_count = MenuOptions.size() - 1;
			menu(Depositors);
			break;
		case 0xD: // подтвердить
			conf_val(Depositors);
			break;
		default:
			menu_choice(Depositors);
		}
	}
	switch (k1)
	{
	case 0x48: // стрелка вверх
		m_count--;
		if (m_count < 0) m_count = 0;
		menu(Depositors);
		break;

	case 0x50: // стрелка вниз
		m_count++;
		if (m_count > MenuOptions.size() - 1) m_count = MenuOptions.size() - 1;
		menu(Depositors);
		break;
	case 0xD: // подтвердить
		conf_val(Depositors);
		break;
	default:
		menu_choice(Depositors);
	}
}

int main()
{
	std::ifstream fin;
	std::vector<Depositor> Depositors;

	HANDLE  hout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD  size;
	size.X = 200;     // кол-во символов на строку
	size.Y = 1001;  // увеличиваем до 1000 - строк, можно хоть 2000
	SetConsoleScreenBufferSize(hout, size);
	setlocale(0, ""); // локализация
	SetConsoleCP(1251); // локализация ввода строки
	SetConsoleOutputCP(1251); // локализация вывода строки

	FillDepositorsDataBase(Depositors, fin);

	menu(Depositors);

	system("pause");
	return 0;
}