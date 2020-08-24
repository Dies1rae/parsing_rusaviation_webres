#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <windows.h>
namespace fs = std::filesystem;
using namespace std;


int main() {
	setlocale(LC_ALL, "russian_russia.1251");
	std::vector <std::string> flyght_base;
	std::string base_dir = "Z:\\hhh\\flyplan\\";
	std::string mask_files = ".html";
	vector<string> file_paths;

	// собираем файлы
	for (auto& ptrF : fs::directory_iterator(base_dir)) {
		string tmp_file = ptrF.path().string();
		if (tmp_file.find(mask_files) != string::npos) {
			file_paths.push_back(tmp_file);
		}
	}
	//сортируем их в порядке убывания, начать писать в ЦСВ необходимо со старейшего файла
	sort(file_paths.begin(), file_paths.end(), [](const string& A, const string& B) {
		int a = stoi(A.substr(A.find_last_of("\\")+1, A.find_last_of(".") - A.find_last_of("\\")-1));
		int b = stoi(B.substr(B.find_last_of("\\") + 1, B.find_last_of(".") - B.find_last_of("\\") - 1));
		if (a > b) {
			return 1;
		}
		else {
			return 0;
		}
		});
		

	for (auto& ptrF : file_paths) {
		ifstream src_html(ptrF, std::ios::binary | std::ios::in);
		string tmp_buff;
		cout << ptrF << endl;
		while (getline(src_html, tmp_buff)) {

			string::size_type ptr_pos_fpl = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-type_tlg\"");
			string::size_type ptr_pos_filling_data = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-filing_datetime\"");
			string::size_type ptr_pos_aircraft = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-aircraft_ident\"");
			string::size_type ptr_pos_departure_datetime = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-departure_datetime\"");
			string::size_type ptr_pos_departure_aerodrome = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-departure_aerodrome\"");
			string::size_type ptr_pos_arrival_datetime = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-arrival_datetime\"");
			string::size_type ptr_pos_arrival_aerodrome = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-arrival_aerodrome\"");
			string::size_type ptr_pos_status_datetime = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-status_datetime\"");
			string::size_type ptr_pos_comments = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-operator_rmk\"");
			string::size_type ptr_pos_status = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-status\"");


			while (ptr_pos_fpl != string::npos && ptr_pos_filling_data != string::npos && ptr_pos_aircraft != string::npos && ptr_pos_departure_datetime != string::npos
			&& ptr_pos_departure_aerodrome != string::npos && ptr_pos_arrival_datetime != string::npos && ptr_pos_arrival_aerodrome != string::npos && ptr_pos_status != string::npos) {

				string fpl_type = tmp_buff.substr(ptr_pos_fpl + 72, 3);
				//cout << "TLG: " << fpl_type << " | ";

				string filling_datetime = tmp_buff.substr(ptr_pos_filling_data + 76, 19);
				//cout << "FILLING DATE: " << filling_datetime << " | ";

				string aircraft_ident = tmp_buff.substr(ptr_pos_aircraft + 78, 7);
				//cout << "AIRCRAFT NUMBER: " << aircraft_ident << " | ";

				string departure_datetime = tmp_buff.substr(ptr_pos_departure_datetime + 79, 16);
				//cout << "DEPARTURE DATE: " << departure_datetime << " | ";

				string departure_aerodrome = tmp_buff.substr(ptr_pos_departure_aerodrome + 82, 5);
				//cout << "DEPARTURE AERODROME: " << departure_aerodrome << " | ";

				string arrival_datetime = tmp_buff.substr(ptr_pos_arrival_datetime + 77, 16);
				//cout << "ARRIVAL DATE: " << arrival_datetime << " | ";

				string arrival_aerodrome = tmp_buff.substr(ptr_pos_arrival_aerodrome + 80, 5);
				//cout << "ARRIVAL AERODROME: " << arrival_aerodrome << " | ";

				string comments = tmp_buff.substr(ptr_pos_comments + 73, 5);

				string status_datetime;
				if (comments == "&nbsp") {
					status_datetime = tmp_buff.substr(ptr_pos_status_datetime + 76, 17);
					//cout << "STATUS DATE: " << status_datetime << " | ";
				}
				else {
					status_datetime = "CANCELLED";
				}

				string status = tmp_buff.substr(ptr_pos_status + 94, 8);
				
				string final = fpl_type + "," + filling_datetime + "," + aircraft_ident + "," + departure_datetime + "," + departure_aerodrome + "," + arrival_datetime + "," + arrival_aerodrome + "," + status_datetime + "," + status;
				
				cout << final << endl;
				//system("PAUSE");
				flyght_base.push_back(final);
				final.clear();


				ptr_pos_status = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-status\"", ptr_pos_status + 1);
				ptr_pos_comments = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-operator_rmk\"", ptr_pos_comments + 1);
				ptr_pos_status_datetime = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-status_datetime\"", ptr_pos_status_datetime + 1);
				ptr_pos_arrival_aerodrome = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-arrival_aerodrome\"", ptr_pos_arrival_aerodrome + 1);
				ptr_pos_arrival_datetime = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-arrival_datetime\"", ptr_pos_arrival_datetime + 1);
				ptr_pos_departure_aerodrome = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-departure_aerodrome\"", ptr_pos_departure_aerodrome + 1);
				ptr_pos_departure_datetime = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-departure_datetime\"", ptr_pos_departure_datetime + 1);
				ptr_pos_aircraft = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-aircraft_ident\"", ptr_pos_aircraft + 1);
				ptr_pos_filling_data = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-filing_datetime\"", ptr_pos_filling_data + 1);
				ptr_pos_fpl = tmp_buff.find("\"x-grid3-cell-inner x-grid3-col-grid_fpl-type_tlg\"", ptr_pos_fpl + 1);
			}
		}
		src_html.close();
		tmp_buff.clear();
		sort(flyght_base.rbegin(), flyght_base.rend());

		std::ofstream csvbase(base_dir + "main_base.csv", std::ios_base::app | std::ios_base::out);
		for (const auto& ptr : flyght_base) {
			csvbase << ptr << '\n';
		}
		csvbase.close();
		flyght_base.clear();
	}
			
	return 0;
}


//x-grid3-cell-inner x-grid3-col-grid_fpl-status
//x-grid3-cell-inner x-grid3-col-grid_fpl-operator_rmk
//x-grid3-cell-inner x-grid3-col-grid_fpl-status_datetime
//x-grid3-cell-inner x-grid3-col-grid_fpl-arrival_aerodrome
//x-grid3-cell-inner x-grid3-col-grid_fpl-arrival_datetime
//x-grid3-cell-inner x-grid3-col-grid_fpl-departure_aerodrome
//x-grid3-cell-inner x-grid3-col-grid_fpl-departure_datetime
//x-grid3-cell-inner x-grid3-col-grid_fpl-aircraft_ident