#include "header.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF8");

    size_t memory_value = 0;
    LPVOID next_free_ptr = nullptr;

    MessageBoxW(NULL, L"Введи объем памяти (в гигабайтах), которую хочешь предоставить сети (лучше закрыть все процессы, так подсчеты будут точнее)", L"Testnet", MB_ICONINFORMATION);

    std::cout << "Ввод ---> ";
    std::cin >> memory_value;


    std::cout << "Считаем Ваш ANOR...\n";

    std::pair<d32, LPVOID>* info = calculateAnor(memory_value * 1024 * 1024 * 1024);

    if (info) {
        std::wstring message = L"Успех. Ваш узел оценивается примерно в : " + std::to_wstring(info->first) + L" ANOR.\n";
        MessageBoxW(NULL, message.c_str(), L"Testnet", MB_ICONINFORMATION);

        next_free_ptr = info->second;
    } else {
        MessageBoxW(NULL, L"Ваша ОС не способна выделить столько памяти.", L"Testnet", MB_ICONINFORMATION);
        VirtualFree(info->second, 0, MEM_RELEASE);
        delete info;

        return 1;
    }

    VirtualFree(info->second, 0, MEM_RELEASE);
    delete info;

    return 0;
}