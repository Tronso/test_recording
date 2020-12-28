#include "SQLiteManager.h"
#include<conio.h>
#include<iostream>
#include <fstream>
#include <memory>
#include "Simulation.h"
#include "DataProcessor.h"
#include "TestVehicle.h"
#include "SituationGraph.h"
#include <time.h>
#include "Winuser.h"

using std::string;
using std::endl;
using std::cout;
using std::cin;
using std::make_unique;
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::move;

static const int second = 10;
static const int minute = 60*second;
static const int hour = 60*minute;

int main() {
	//Use block to delete sim and dp when they are not needed anymore
	string scenario;
	string dbFile;
	string pdfFile;
	string csvFilePerformance;
	string csvFileData;
	bool cont = true;

	double mp_10; //duration of simulation[h]
	double mp_20; //simulated time[h]
	int mp_30; //number of vehicles simulated
	double mp_40; //ratio of simulated time to real time, mp_20/mp_10

	int mo_10 = 2.7317e+87; //overall number of reasonable situations
	int mo_20; //number of situations recorded overall
	int mo_30; //number of situations recorded in the last session
	int mo_40; //highest number of vehicles per situation
	int mo_50; //highest number of continuous situations
	double mo_60; //percentage of possible situations recorded, mo_20/mo_10

	int last_mo_20 = 0;

	cout << "Name of the scenario to load (without file extension): ";
	cin >> scenario;
	dbFile = scenario + ".db";
	pdfFile = scenario + ".pdf";
	csvFilePerformance = scenario + "_perf.csv";
	csvFileData = scenario + "_data.csv";
	cout << "Number of Hours to be simulated: ";
	cin >> mp_20;

	//find last_mo_10
	std::fstream data(csvFileData);
	if (!data) {
		std::cout << "Error opening CSV file " << csvFileData << "." << endl;
		std::cout << "Press any button to exit." << endl;
		_getch();
		return -1;
	}
	//read lines
	std::vector<std::string> lines;
	for (string line; std::getline(data, line);) {
		lines.push_back(line);
	}
	data.close();
	//go backwards through lines to find the last correct line
	for (int i = lines.size() - 1; i >= 0; ++i) {
		std::vector<string> entries;
		std::istringstream entryStream(lines[i]);
		for (string entry; std::getline(entryStream, entry, ',');) {
			entries.push_back(entry);
		}
		if (entries.size() == 6) {
			last_mo_20 = std::stoi(entries[1]);
			break;
		}
	}


	while (cont) {
		time_t start, end;
		{
			Simulation sim("", scenario);
			unique_ptr<DataManager> dm = make_unique<SQLiteManager>(dbFile);
			DataProcessor dp(move(dm));
			sim.initialize();
			shared_ptr<TestVehicle> vut = sim.add_vehicle(100, 1, 1, 0, 100);
			time(&start);
			sim.runAndRecord(dp, move(vut), mp_20*hour);
			mp_30 = sim.getNumberOfVehicles();
		}
		time(&end);
		double seconds_taken = difftime(end, start);
		mp_10 = seconds_taken / 3600.0;
		mp_40 = mp_20 / mp_10;


		SQLiteManager dm2(dbFile);
		unique_ptr<SituationGraph> sg = move(dm2.retrieveSituationGraph());
		
		mo_20 = dm2.getNumberOfSituations();
		mo_30 = mo_20 - last_mo_20;
		mo_40 = dm2.getMaxNumberOfObjects();
		mo_50 = sg->getLongestSequence();
		//mo_10 too large, approximate mo_60 by 0.0
		//mo_60 = mo_20 / mo_10;
		mo_60 = 0.0;
		last_mo_20 = mo_10;

		std::fstream performance(csvFilePerformance, std::ios::app);
		if (!performance) {
			std::cout << "Error opening CSV file " << csvFilePerformance << "." << endl;
			std::cout << "Press any button to exit." << endl;
			_getch();
			return -1;
		}
		performance << mp_10 << "," << mp_20 << "," << mp_30 << "," << mp_40 << endl;
		performance.close();

		std::fstream data(csvFileData, std::ios::app);
		if (!data) {
			std::cout << "Error opening CSV file " << csvFileData << "." << endl;
			std::cout << "Press any button to exit." << endl;
			_getch();
			return -1;
		}
		data << mo_10 << "," << mo_20 << "," << mo_30 << "," << mo_40 << "," << mo_50 << "," << mo_60 << endl;
		data.close();

		cout << "mp_10 = " << mp_10 << ", mp_20 = " << mp_20 << ", mp_30 = " << mp_30 << ", mp_40 = " << mp_40 << endl;
		cout << "mo_10 = " << mo_10 << ", mo_20 = " << mo_20 << ", mo_30 = " << mo_30 << ", mo_40 = " << mo_40 << ", mo_50 = " << mo_50 << ", mo_60 = " << mo_60 << endl;

		sg->saveAsPDF(pdfFile);
		//set window flashing
		FLASHWINFO fi;
		fi.cbSize = sizeof(FLASHWINFO);
		fi.hwnd = GetConsoleWindow();
		fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fi.uCount = 0;
		fi.dwTimeout = 0;
		FlashWindowEx(&fi);

		cout << "Done. Do you want to start another run (1 for yes, 0 for no)? ";
		cin >> cont;
	}
}