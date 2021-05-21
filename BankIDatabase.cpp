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
	Depositor DepositorData;
	int Option;
	int DepthNodes;
	TreeDepositors* left;
	TreeDepositors* right;
	TreeDepositors(int CurrentOption, Depositor Data) {DepositorData = Data, Option = CurrentOption; left = right = 0; DepthNodes = 1; }

};

void Menu_choice(std::vector<Depositor>& Depositors); // прототип меню
void Menu(std::vector<Depositor>& Depositors); // прототип меню
void Sub_menu_choice(std::vector<Depositor>& Depositors, std::string Option);
void SubMenu(std::vector<Depositor>& Depositors, std::string Option);
void PrintDepositor(const Depositor& Depos);
int m_count = 0;
int sub_m_count = 0;
std::vector<std::string> MenuOptions{ "Показать всех вкладчиков", "Отсортировать вкладчиков", "Поиск вкладчика", "Выход" };
std::vector<std::string> SubMenuOptions{ "По фамилии", "По имени","По отчеству","По номеру вклада","По размеру вклада","По длительности","Назад", };

const int NotUsed = system("color 70"); // изменения цвета консоли в серый 

bool IsDepositorValidData(Depositor CurrentDepositor)
{
	return (CurrentDepositor.DepositorNumber > 0) && (CurrentDepositor.DepositorValue > 0) && (CurrentDepositor.DepositDuration > 0);
}

int FillDepositorsDataBase(std::vector<Depositor>& Depositors, std::ifstream& fin)
{
	Depositor TempDepositor;
	std::cout << "....Заполнение записей...." << std::endl;
	fin.open("DepositorsList.txt");
	if (fin.is_open()) // если файл удачно открыт
	{
		while (!fin.eof()) // пока не дошли до конца файла
		{
			fin >> TempDepositor.DepositorSurname >> TempDepositor.DepositorName >> TempDepositor.DepositorFatherName >> TempDepositor.DepositorNumber >> TempDepositor.DepositorValue >> TempDepositor.DepositDuration;
			fin.ignore(1); // игнорируем перенос строки
			if (IsDepositorValidData(TempDepositor))
			{
				Depositors.push_back(TempDepositor);
			}
			else
			{
				std::cout << "Некорректные значения у вкладчика - значения номера, размера и длительности должны быть больше нуля!" << std::endl;
				PrintDepositor(TempDepositor);
				return 1;
			}
		}
	}
	else // если файл открыть не удалось
	{
		std::cout << "Файл не удалось открыть!" << std::endl;
	}
	fin.close(); // закрываем файл
	return 0;
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

void QuickSort(std::vector<Depositor>& Depositors, int LeftValueBorder, int RightValueBorder, int Key)
{
	Depositor Pivot; // разрешающий элемент
	int l_hold = LeftValueBorder; //левая граница
	int r_hold = RightValueBorder; // правая граница
	Pivot = Depositors[LeftValueBorder];
	CheckOptions checkOptionValue;
	while (LeftValueBorder< RightValueBorder) // пока границы не сомкнутся
	{
		checkOptionValue = CheckEqualValues(Depositors[RightValueBorder], Pivot, Key, NULL, Key);
		while ((checkOptionValue == EQUAL || checkOptionValue  == MORE) && (LeftValueBorder< RightValueBorder))
		{
			RightValueBorder--; // сдвигаем правую границу пока элемент [RightValueBorder] больше [Pivot]
			checkOptionValue = CheckEqualValues(Depositors[RightValueBorder], Pivot, Key, NULL, Key);
		}

		if (LeftValueBorder!= RightValueBorder) // если границы не сомкнулись
		{
			Depositors[LeftValueBorder] = Depositors[RightValueBorder]; // перемещаем элемент [RightValueBorder] на место разрешающего
			LeftValueBorder++; // сдвигаем левую границу вправо
		}

		checkOptionValue = CheckEqualValues(Depositors[LeftValueBorder], Pivot, Key, NULL, Key);
		while ((checkOptionValue == EQUAL || checkOptionValue == LESS) && (LeftValueBorder< RightValueBorder))
		{
			LeftValueBorder++; // сдвигаем левую границу пока элемент [LeftValueBorder] меньше [Pivot]
			checkOptionValue = CheckEqualValues(Depositors[LeftValueBorder], Pivot, Key, NULL, Key);
		}

		if (LeftValueBorder!= RightValueBorder) // если границы не сомкнулись
		{
			Depositors[RightValueBorder] = Depositors[LeftValueBorder]; // перемещаем элемент [LeftValueBorder] на место [RightValueBorder]
			RightValueBorder--; // сдвигаем правую границу вправо
		}
	}
	Depositors[LeftValueBorder] = Pivot; // ставим разрешающий элемент на место
	Pivot = Depositors[LeftValueBorder];
	int tempLeftValueBorder= LeftValueBorder;
	LeftValueBorder= l_hold;
	RightValueBorder = r_hold;

	checkOptionValue = CheckEqualValues(Depositors[LeftValueBorder], Pivot, Key, NULL, Key);
	if (checkOptionValue == LESS) // Рекурсивно вызываем сортировку для левой и правой части массива
	{
		QuickSort(Depositors, LeftValueBorder, tempLeftValueBorder- 1, Key);
	}

	checkOptionValue = CheckEqualValues(Depositors[RightValueBorder], Pivot, Key, NULL, Key);
	if (checkOptionValue == MORE)
	{
		QuickSort(Depositors, tempLeftValueBorder+ 1, RightValueBorder, Key);
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


void FindDepositorsByValue(TreeDepositors* TreeNode, Depositor Key, std::vector<Depositor>& FinderDepositors) // поиск ключа k в дереве p
{
	if (!TreeNode) return; // в пустом дереве можно не искать

	CheckOptions checkValue = CheckEqualTreeValues(Key, TreeNode->DepositorData, TreeNode->Option);
	if (checkValue == EQUAL)
	{
		if (!IsEqualDepositors(FinderDepositors, TreeNode->DepositorData))
		{
			FinderDepositors.push_back(TreeNode->DepositorData);
			FindDepositorsByValue(TreeNode->left, Key, FinderDepositors);
		}
	}

	if (checkValue == LESS)
		FindDepositorsByValue(TreeNode->left, Key, FinderDepositors);
	else
		FindDepositorsByValue(TreeNode->right, Key, FinderDepositors);
}

TreeDepositors* find(TreeDepositors* TreeNode, Depositor Key, std::vector<Depositor>& FinderDepositors) // поиск ключа k в дереве p
{
	if (!TreeNode) return 0; // в пустом дереве можно не искать

	CheckOptions checkValue = CheckEqualTreeValues(Key, TreeNode->DepositorData, TreeNode->Option);
	if (checkValue == EQUAL)
	{
		if (!IsEqualDepositors(FinderDepositors, TreeNode->DepositorData))
			return TreeNode;
	}

	if (checkValue == LESS)
		return find(TreeNode->left, Key, FinderDepositors);
	else
		return find(TreeNode->right, Key, FinderDepositors);
}

int GetSize(TreeDepositors* TreeNode) // обертка для поля size, работает с пустыми деревьями (t=NULL)
{
	if (!TreeNode) return 0;
	return TreeNode->DepthNodes;
}

void FixSizeNode(TreeDepositors* TreeNode) // установление корректного размера дерева
{
	TreeNode->DepthNodes = GetSize(TreeNode->left) + GetSize(TreeNode->right) + 1;
}

TreeDepositors* RotateRightTrees(TreeDepositors* TreeNode) // правый поворот вокруг узла p
{
	TreeDepositors* q = TreeNode->left;

	if (!q) return TreeNode;
	TreeNode->left = q->right;
	q->right = TreeNode;
	q->DepthNodes = TreeNode->DepthNodes;
	FixSizeNode(TreeNode);

	return q;
}

TreeDepositors* RotateLeftTrees(TreeDepositors* q) // левый поворот вокруг узла q
{
	TreeDepositors* p = q->right;

	if (!p) return q;
	q->right = p->left;
	p->left = q;
	p->DepthNodes = q->DepthNodes;
	FixSizeNode(q);

	return p;
}

TreeDepositors* InsertRoot(TreeDepositors* TreeNode, Depositor Key, int Option) // вставка нового узла с ключом k в корень дерева p 
{
	if (!TreeNode) return new TreeDepositors(Option, Key);

	CheckOptions checkValue = CheckEqualTreeValues(Key, TreeNode->DepositorData, TreeNode->Option);
	if (checkValue == LESS)
	{
		TreeNode->left = InsertRoot(TreeNode->left, Key, Option);
		return RotateRightTrees(TreeNode);
	}
	else
	{
		TreeNode->right = InsertRoot(TreeNode->right, Key, Option);
		return RotateLeftTrees(TreeNode);
	}
}

TreeDepositors* InsertNodeRandom(TreeDepositors* TreeNode, Depositor Key, int Option) // рандомизированная вставка нового узла с ключом k в дерево p
{
	if (!TreeNode) return new TreeDepositors(Option, Key);
	if (rand() % (TreeNode->DepthNodes + 1) == 0)
		return InsertRoot(TreeNode, Key, Option);
	CheckOptions checkValue = CheckEqualTreeValues(TreeNode->DepositorData, Key, TreeNode->Option);
	if (checkValue == MORE)
		TreeNode->left = InsertNodeRandom(TreeNode->left, Key, Option);
	else
		TreeNode->right = InsertNodeRandom(TreeNode->right, Key, Option);
	FixSizeNode(TreeNode);
	return TreeNode;
}

void DeleteTree(TreeDepositors* TreeNode)
{
	if (TreeNode)
	{
		DeleteTree(TreeNode->left);
		DeleteTree(TreeNode->right);
		delete TreeNode->left;
		delete TreeNode->right;
	}
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
		Tree = InsertNodeRandom(Tree, Depositors[i], sub_m_count);
	}

	auto start = sc.now();     // устанавливаем начало отсчета времени события
	FindDepositorsByValue(Tree, TempDepos, FinderDepositors);
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
	DeleteTree(Tree);
	delete Tree;
	Menu(Depositors);
}

void SortDepositorsList(std::vector<Depositor>& Depositors)
{
	std::chrono::steady_clock sc;
	auto start = sc.now();     // устанавливаем начало отсчета времени события
	QuickSort(Depositors, NULL, Depositors.size() - 1, sub_m_count + 1);
	auto end = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие

	PrintDepositorsList(Depositors);
	std::cout << "Время, затраченное на сортировку: " << time_span.count() << " секунд" << std::endl << std::endl;
	system("pause");
	Menu(Depositors);

}

void Sub_conf_val(std::vector<Depositor>& Depositors, std::string Option) // выбор после подтверждения в зависимости от значения текущего
{
	if (Option == "Sort")
	{
		if (sub_m_count != SubMenuOptions.size() - 1)
		{
			SortDepositorsList(Depositors);
		}
		else
		{
			Menu(Depositors);
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
			Menu(Depositors);
		}
	}
}

void Sub_menu_choice(std::vector<Depositor>& Depositors, std::string Option) // в зависимости от стрелок изменяем меню
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
			Sub_conf_val(Depositors, Option);
			break;
		default:
			Sub_menu_choice(Depositors, Option);
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
		Sub_conf_val(Depositors, Option);
		break;
	default:
		Sub_menu_choice(Depositors, Option);
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
	Sub_menu_choice(Depositors, Option);
}

void Conf_val(std::vector<Depositor>& Depositors) // выбор после подтверждения в зависимости от значения текущего
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

	Menu(Depositors);
}

void Menu(std::vector<Depositor>& Depositors) // меню
{
	system("cls"); // очищаем консоль
	std::cout << "-----Основное меню-----" << "                 Количество записей: " << Depositors.size() << std::endl;
	for (int i = 0; i < MenuOptions.size(); i++)
	{
		if (i == m_count)
			SetColor(5, 7);

		std::cout << MenuOptions[i] << std::endl;
		SetColor(0, 7);
	}
	Menu_choice(Depositors);
}

void Menu_choice(std::vector<Depositor>& Depositors) // в зависимости от стрелок изменяем меню
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
			Menu(Depositors);
			break;

		case 0x50: // стрелка вниз
			m_count++;
			if (m_count > MenuOptions.size() - 1) m_count = MenuOptions.size() - 1;
			Menu(Depositors);
			break;
		case 0xD: // подтвердить
			Conf_val(Depositors);
			break;
		default:
			Menu_choice(Depositors);
		}
	}
	switch (k1)
	{
	case 0x48: // стрелка вверх
		m_count--;
		if (m_count < 0) m_count = 0;
		Menu(Depositors);
		break;

	case 0x50: // стрелка вниз
		m_count++;
		if (m_count > MenuOptions.size() - 1) m_count = MenuOptions.size() - 1;
		Menu(Depositors);
		break;
	case 0xD: // подтвердить
		Conf_val(Depositors);
		break;
	default:
		Menu_choice(Depositors);
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

	if (FillDepositorsDataBase(Depositors, fin) == 0)
	{
		Menu(Depositors);
	}
	return 0;
}