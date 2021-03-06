#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

class result {
private:
	FILE* bur;

public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };

	result(bool read = false) {
		if (read) 
			bur = fopen("result.txt", "r"); 
		else 
			bur = fopen("result.txt", "w");
	}

	~result() { 
		fclose(bur); 
	}

	void write_type(ext_cls t) { 
		fwrite(&t, sizeof(t), 1, bur); 
	}

	void write_verdict(verdict v) { 
		write_type(ext_cls::VERDICT); 
		fwrite(&v, sizeof(v), 1, bur); 
	}

	void write_message(string str) { 
		write_type(ext_cls::MESSAGE); 
		int l = str.size(); 
		fwrite(&l, sizeof(l), 1, bur); 
		fwrite(&str[0], sizeof(str[0]), l, bur); 
	}

	void write_time(long long x) { 
		write_type(ext_cls::TIME); 
		fwrite(&x, sizeof(x), 1, bur); 
	}

	void write_memory(unsigned long long x) { 
		write_type(ext_cls::MEMORY); 
		fwrite(&x, sizeof(x), 1, bur); 
	}
} checker_result;

int main(int argc, char * argv[])
{
	if (argc != 2 && argc != 3) {
		std::cout << "Use 'checker [output file] [answer file]'" << std::endl;
		return 1;
	}

	FILE * buo = fopen(argv[1], "rb"); 
	FILE * bua = nullptr;

	bool compare_files = false;
	if (argc == 3) {
		compare_files = true;
		bua = fopen(argv[2], "rb");
	}

	int res_n = 0, ans_n = 0;
	double res_time, ans_time;

	fread(&res_time, sizeof(res_time), 1, buo);
	fread(&res_n, sizeof(res_n), 1, buo);

	if (compare_files == true) {
		fread(&ans_time, sizeof(ans_time), 1, bua);
		fread(&ans_n, sizeof(ans_n), 1, bua);
	}

	bool error = false;

	if (compare_files == true && ans_n != res_n) {
		checker_result.write_message("PE. Numbers of elements in the arrays are different.");
		checker_result.write_verdict(verdict::PE);
	}
	else {
		int prev_elem = 0, cur_elem = 0, ans_part;
		fread(&prev_elem, sizeof(prev_elem), 1, buo);
		if (compare_files == true) {
			fread(&ans_part, sizeof(ans_part), 1, bua);
			if (prev_elem != ans_part) error = true;
		}
		if (error == false) {
			for (int i = 1; i < res_n; ++i) {
				fread(&cur_elem, sizeof(cur_elem), 1, buo);
				if (compare_files == true) {
					fread(&ans_part, sizeof(ans_part), 1, bua);
					if (cur_elem != ans_part) {
						error = true;
						break;
					}
				}
				if (cur_elem < prev_elem) {
					error = true;
					break;
				}
				prev_elem = cur_elem;
			}
		}
		
		if (error == false) {
			checker_result.write_message("AC. Array is sorted correctly.");
			checker_result.write_verdict(verdict::AC);
		}
		else {
			checker_result.write_message("WA. Array sorting has an error.");
			checker_result.write_verdict(verdict::WA);
		}
		checker_result.write_time(static_cast<long long>(res_time * 1e7));
	}
	
	fclose(buo);
	if(compare_files == true) 
		fclose(bua);
	return 0;
}