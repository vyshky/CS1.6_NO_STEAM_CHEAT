#include <iostream>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include "entity_player.h"

#define PROCESS_VM_RW PROCESS_VM_READ | PROCESS_VM_WRITE
int main()
{
	// Find process	
	HANDLE h_snapshot_proccess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	while (Process32Next(h_snapshot_proccess, &pe32)) {
		if (_tcscmp(pe32.szExeFile, _T("hl.exe")) == 0) {
			std::wcout << L"Process: " << pe32.szExeFile << L"\n";
			break;
		}
	}

	//Find hw.dll address
	HANDLE h_snapshot_module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pe32.th32ProcessID);
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32W);

	while (Module32Next(h_snapshot_module, &me32)) {
		if (_tcscmp(me32.szModule, _T("hw.dll")) == 0) {
			std::wcout << me32.szModule << std::endl;
			break;
		}
	}

	//Open process
	HANDLE h_open_process = OpenProcess(PROCESS_VM_RW, false, pe32.th32ProcessID);

	//Read process memory
	uintptr_t ptr_entity;
	ReadProcessMemory(h_open_process, me32.modBaseAddr + 0x1AFBFC, &ptr_entity, sizeof(ptr_entity), 0);

	while (true) {
		// coords считывать модели игроков и выводить адреса
		EntityPlayer players_offset = EntityPlayer((uintptr_t)me32.modBaseAddr);
		for (uintptr_t i = 0x0; i < players_offset.get_array_size(); i += players_offset.get_player_size()) {
			int* player_list = (int*)ptr_entity;
			uintptr_t ptr_x_coord = (uintptr_t)player_list + i + 0xB48;
			uintptr_t ptr_y_coord = (uintptr_t)player_list + i + 0xB4C;
			uintptr_t ptr_z_coord = (uintptr_t)player_list + i + 0xB50;
			float x_value;
			float y_value;
			float z_value;

			ReadProcessMemory(h_open_process, (int*)ptr_x_coord, &x_value, sizeof(float), 0);
			ReadProcessMemory(h_open_process, (int*)ptr_y_coord, &y_value, sizeof(float), 0);
			ReadProcessMemory(h_open_process, (int*)ptr_z_coord, &z_value, sizeof(float), 0);



			uintptr_t* ptr_model;
			uintptr_t model_entity_player_address = (uintptr_t)player_list + i + 0xB94;
			ReadProcessMemory(h_open_process, (int*)model_entity_player_address, &ptr_model, sizeof(ptr_model), 0);

			char buffer[255]{};
			ReadProcessMemory(h_open_process, ptr_model, buffer, sizeof(buffer), 0);
			if (buffer[0] != NULL) {
				std::cout << "Model: " << buffer << "   " << ptr_model;
				std::cout << " Player offset: " << player_list + i << " X_OFFSET: " << ptr_x_coord << " X: " << x_value << " Y: " << y_value << " Z: " << z_value << std::endl;
			}
		}
		Sleep(100);
		system("CLS");
	}
	CloseHandle(h_snapshot_proccess);
	CloseHandle(h_snapshot_module);
	CloseHandle(h_open_process);
}