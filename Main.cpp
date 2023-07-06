#include <fstream>
#include <iostream>
#include <queue>

struct HeaderData
{
	char groupName[50];
	int themesCount = 0;
};

struct ThemeData
{
	char songName[50];
	char artistName[50];
	char albumName[50];
	char genre[50];
	char url[100];
	float duration;
	int year;
};

enum Ops
{
	Quit,
	Add,
	Read,
	Save,
	Load,
	Menu
};

ThemeData promptSong();
void saveList(std::queue<ThemeData> list, HeaderData headerData);
void menu(Ops& ops);
void read(std::queue<ThemeData>& list, HeaderData& headerData);
void loadList(std::queue<ThemeData>& list, HeaderData& headerData);

int main()
{
	Ops options = Menu;
	std::queue<ThemeData> list;
	HeaderData headerData{};

	std::cout << "Enter the group's name: ";
	std::cin.getline(headerData.groupName, 50);

	do
	{
		switch (options)
		{
		case Quit: break;

		case Add:
			list.push(promptSong());
			headerData.themesCount++;
			options = Menu;
			break;

		case Read:
			read(list, headerData);
			options = Menu;
			break;

		case Menu:
			menu(options);
			break;

		case Save:
			saveList(list, headerData);
			break;

		case Load:
			loadList(list, headerData);
			break;

		default:;
		}
	} while (options != Quit);

	saveList(list, headerData);
}

ThemeData promptSong()
{
	ThemeData themeData{};
	std::cin.ignore();
	std::cout << "Enter the song name: ";
	std::cin.getline(themeData.songName, 50);
	std::cout << "Enter the artist name: ";
	std::cin.getline(themeData.artistName, 50);
	std::cout << "Enter the album name: ";
	std::cin.getline(themeData.albumName, 50);
	std::cout << "Enter the genre: ";
	std::cin.getline(themeData.genre, 50);
	std::cout << "Enter the URL: ";
	std::cin.getline(themeData.url, 100);
	std::cout << "Enter the duration: ";
	std::cin >> themeData.duration;
	std::cout << "Enter the year: ";
	std::cin >> themeData.year;
	std::cin.ignore();

	return themeData;
}

void saveList(std::queue<ThemeData> list, HeaderData headerData)
{
	std::ofstream outFile;
	outFile.open("songs.bin", std::ios::out | std::ios::binary);

	if (outFile.is_open())
	{
		while (!list.empty())
		{
			ThemeData data = list.front();
			outFile.write(reinterpret_cast<char*>(&data), sizeof(ThemeData));
			list.pop();
		}

		outFile.close();
		std::cout << "Data saved to file." << std::endl;
	}
	else
	{
		std::cout << "Error opening file." << std::endl;
	}

	outFile.open("header.bin", std::ios::out | std::ios::binary);
	outFile.write(reinterpret_cast<char*>(&headerData), sizeof(HeaderData));
}

void menu(Ops& ops)
{
	system("cls");
	std::cout << "0: Quit" << std::endl
		<< "1: Add song" << std::endl
		<< "2: Read playlist" << std::endl
		<< "3: Save playlist" << std::endl
		<< "4: Load playlist" << std::endl;

	char key = 0;

	do
	{
		std::cin >> key;
	} while (key < '0' || key > '4');

	switch (key)
	{
	case '0':
		ops = Quit;
		break;

	case '1':
		ops = Add;
		break;

	case '2':
		ops = Read;
		break;

	case '3':
		ops = Save;
		break;

	case '4':
		ops = Load;
		break;

	default:
		ops = Menu;
		break;
	}
}

void read(std::queue<ThemeData>& list, HeaderData& headerData)
{
	loadList(list, headerData);

	for (int i = 0; i < headerData.themesCount; i++)
	{
		ThemeData theme = list.front();
		list.pop();
		list.push(theme);

		std::cout << i + 1 << ")\n" << "Song: " << theme.songName << std::endl
			<< "Artist: " << theme.artistName << std::endl
			<< "Album: " << theme.albumName << std::endl
			<< "Genre: " << theme.genre << std::endl
			<< "URL: " << theme.url << std::endl
			<< "Duration: " << theme.duration << std::endl
			<< "Year: " << theme.year << std::endl << std::endl;

		std::cin.ignore();
		std::cin.get();
	}
}

void loadList(std::queue<ThemeData>& list, HeaderData& headerData)
{
	std::ifstream input;
	ThemeData themes[50];

	input.open("header.bin", std::ios::in | std::ios::binary);
	input.read(reinterpret_cast<char*>(&headerData), sizeof(HeaderData));
	input.close();

	input.open("songs.bin", std::ios::in | std::ios::binary);
	input.read(reinterpret_cast<char*>(&themes), sizeof(ThemeData));
	input.close();

	for (int i = 0; i < headerData.themesCount; i++)
		list.push(themes[i]);

	std::cout << "Data loaded";
}