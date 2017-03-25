#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <ctime>

using namespace std;

int main() {
	srand(time(0));

	int N; //Number of teams
	int M; //Number of problems
	cout << "Number of teams: " << endl; cin >> N;
	cout << "Number of problems: " << endl; cin >> M;

	ifstream fq("problems.txt");   //Questions in order
	ifstream fans("answers.txt");  //Modified answers to the question in order
	ifstream fkey("keywords.txt"); //Keywords for the next room
	
	int i, k, j;
	int room[M][N];
	for(k = 0; k < M; k++) {	//generates room numbers
		int used[N];
		memset(used, 0, sizeof(int)*N);
		for(i = 0; i < N; i++) {
			j = rand() % N;
			//cout << j << " ";
			while(used[j]) {
				j++;
				j = j % N;
			}
			used[j] = 1;
			room[k][i] = j;
			cout << j << "\t";
		}
		cout << endl;
	}
	int used[N][N];
	memset(used, 0, sizeof(int)*N*N);
	for(i = 0; i < M; i++) {	//checks for visiting the same room twice
		for(k = 0; k < N; k++) {
			if(used[k][room[i][k]]) {
				cout << i << " " << k << " " << room[i][k] << endl;
				for(j = k+1; j != k; j = (j+1) % N) {
					if(!used[k][room[i][j]] && !used[j][room[i][k]]) {
						int temp = room[i][j];
						room[i][j] = room[i][k];
						room[i][k] = temp;
						if(j < k) {
							used[j][room[i][j]] = 1;
							used[j][room[i][k]] = 0;
						}
						break;
					}
				}
			}
			used[k][room[i][k]] = 1;
		}
	}
	for(k = 0; k < M; k++) { for(i = 0; i < N; i++) {cout <<  room[k][i] << "\t";} cout << endl;}
	
	char keywords[M][N][20]; 		//stores all the keywords
	memset(keywords, '\0', sizeof(char)*20*M*N);
	for(i = 0; i < M; i++) {
		for(k = 0; k < N; k++) {
			fkey.getline(keywords[i][k], 20);
		}
	}

	ofstream out("all_problems.tex");
	out << "\\documentclass{article}" << endl;
	out << "\\usepackage[utf8]{inputenc}" << endl;
	out << "\\begin{document}" << endl;
	out << endl;
	for(i = 0; i < M; i++) {
		char question[6000];
		memset(question, '\0', sizeof(char)*6000);
		fq.getline(question, 6000);	//reads in question
		int ans;
		fans >> ans;		//reads in answer

		for(k = 0; k < N; k++) { 
			out << "\\newpage" << endl;
			out << "\\textbf{Team " << k+1;
			if(i != 0) {
				out << " -- ";
				for(j = 0; j < 20; j++) {
					out << keywords[i-1][k][j];
				}
			}
			out << "} \\\\" << endl;
			out << endl;
			if(i == 0) {
				out << "Cafeteria \\\\\\\\" << endl;
			} else {
				out << "Room " << room[i-1][k]+1 << "\\\\\\\\" << endl;
			}
			for(j = 0; j < 6000; j++) {
				out << question[j];
			}
			out << "\\\\\\\\" << endl;
			int temp = room[i][k]+1 - ans;
			if(i == M-1) {
				out << "Return to the Cafeteria with your answer." << endl;
			} else {
				if(temp < 0) {
					temp *= -1;
					out << "Take your answer and subtract " << temp;
				} else if(temp == 0) {
					out << "Leave your answer as it is";
				} else {
					out << "Take your answer and add " << temp;
				}
				out << ". Go to that room and say \"";
				for(j = 0; j < 20; j++) {
					out << keywords[i][k][j];
				}
				out << "\"" << endl;
			}
			
		}
	}
	out << "\\end{document}" << endl;
}
