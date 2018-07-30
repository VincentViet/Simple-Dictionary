#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <jansson/jansson.h>
#include <string>

#define URL "./data/AnhViet.txt"

json_error_t error;
json_t* en_vi_dict = nullptr;
json_t* vi_en_dict = nullptr;

void translate_en_vi(const char* word)
{
	const auto meaning = json_object_get(en_vi_dict, word);
	if (meaning)
	{
		printf("Azor dictionary:\n\n");
		std::cout << std::setw(7) << std::left << "STT";
		std::cout << std::setw(30) << std::left << "English";
		std::cout << std::setw(30) << std::left << "Vietnamese";
		std::cout << std::endl;

		std::cout << std::setfill('-');
		std::cout << std::setw(67) << "-" << std::endl;
		std::cout << std::setfill(' ');

		std::cout << std::setw(7) << std::left << "1.";
		std::cout << std::setw(30) << std::left << word;
		std::cout << std::setw(30) << std::left << json_string_value(meaning);
		std::cout << std::endl;
	}
	else { printf("Not found this word. You should add this as new word.\n"); }
}

void translate_vi_en(const char* word)
{
	const auto meaning = json_object_get(vi_en_dict, word);
	if (meaning)
	{
		printf("Azor dictionary:\n\n");
		std::cout << std::setw(7) << std::left << "STT";
		std::cout << std::setw(30) << std::left << "Vietnamese";
		std::cout << std::setw(30) << std::left << "English";
		std::cout << std::endl;

		std::cout << std::setfill('-');
		std::cout << std::setw(67) << "-" << std::endl;
		std::cout << std::setfill(' ');

		std::cout << std::setw(7) << std::left << "1.";
		std::cout << std::setw(30) << std::left << word;
		std::cout << std::setw(30) << std::left << json_string_value(meaning);
		std::cout << std::endl;
	}
	else { printf("Not found this word. You should add this as new word.\n"); }
}

void add_word_en(const char* word, const char* value)
{
	const auto meaning = json_object_get(en_vi_dict, word);
	if (meaning) // updating
	{
		std::string meaning_value = json_string_value(meaning);
		json_object_del(vi_en_dict, meaning_value.c_str());
		json_object_set(vi_en_dict, value, json_string(word));
	}
	else { json_object_set(vi_en_dict, value, json_string(word)); } // adding new word

	json_object_set(en_vi_dict, word, json_string(value));
}

void add_word_vi(const char* word, const char* value)
{
	const auto meaning = json_object_get(vi_en_dict, word);
	if (meaning) // updating
	{
		std::string meaning_value = json_string_value(meaning);
		json_object_del(en_vi_dict, meaning_value.c_str());
		json_object_set(en_vi_dict, value, json_string(word));
	}
	else { json_object_set(en_vi_dict, value, json_string(word)); } // adding new word

	json_object_set(vi_en_dict, word, json_string(value));
}

void list_all()
{
	const char *key;
	json_t *value;
	unsigned stt = 1;

  //table-header
	printf("Azor dictionary:\n\n");
	std::cout << std::setw(7) << std::left << "STT";
	std::cout << std::setw(30) << std::left << "English";
	std::cout << std::setw(30) << std::left << "Vietnamese";
	std::cout << std::endl;

	std::cout << std::setfill('-');
	std::cout << std::setw(67) << "-" << std::endl;
	std::cout << std::setfill(' ');

  //table-body
	json_object_foreach(en_vi_dict, key, value) {
		/* block of code that uses key and value */

		std::cout << std::setw(7) << std::left << stt;
		std::cout << std::setw(30) << std::left << key;
		std::cout << std::setw(30) << std::left << json_string_value(value);
		std::cout << std::endl;
		stt++;
	}
}

void print_help()
{
	printf(
		"Usage: trans [option] [arg1] [arg2] [arg3]\n"
		"\tOption:\n"
		"\t\tNo args || -h || --help: \t\t\t\tDisplay help.\n"
		"\t\t-a || --add [en || vi] [\"word\"] [\"meaning\"]: \t\tAdd new word.If this word is exist, it will be replaced\n"
		"\t\t-l || --list-all: \t\t\t\t\tShow all dictionary data in alphabetical order.\n"
		"\t\t-en || --en-vi [\"word\"]: \t\t\t\tTranslate English word to Vietnamese.\n"
		"\t\t-vi || --vi-en [\"word\"]: \t\t\t\tTranslate Vietnamese word to English.\n"
	);
}

int main(const int argc, char* argv[])
{
	const auto json = json_load_file(URL, 0, &error);

	if (!json_unpack(json, "{s:o, s:o}", "en-vi", &en_vi_dict, "vi-en", &vi_en_dict))
	{
		switch (argc)
		{
		case 2:

			if (!strcmp(argv[1], "-l") || !strcmp(argv[1], "--list-all"))
			{
				list_all();
			}
			else { print_help(); }
			break;

		case 3:
			if (!strcmp(argv[1], "-en") || !strcmp(argv[1], "--en-vi"))
			{
				translate_en_vi(argv[2]);
			}
			else if (!strcmp(argv[1], "-vi") || !strcmp(argv[1], "--vi-en"))
			{
				translate_vi_en(argv[2]);
			}
			else { print_help(); }
			break;

		case 5:
			if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--add"))
			{
				if (!strcmp(argv[2], "en"))
				{
					add_word_en(argv[3], argv[4]);
				}
				else if (!strcmp(argv[2], "vi"))
				{
					add_word_vi(argv[3], argv[4]);
				}
				else { print_help(); }
			}
			else { print_help(); }
			break;

		default:
			print_help();
			break;
		}

		json_dump_file(json, URL, JSON_SORT_KEYS | JSON_COMPACT | JSON_INDENT(1));

	}
	else
	{
		printf("\nerror: %s\nat line: %d, column: %d\n\n", error.text, error.line, error.column);
	} // print error

	return 0;
}
